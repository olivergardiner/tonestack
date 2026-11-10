#include "simulationcontroller.h"
#include "assetpaths.h"
#include "spiceinput.h"
#include "ngspice/sharedspice.h"

#include <cmath>
#include <QFile>
#include <QProcess>
#include <QFileInfo>
#include <QDir>


namespace {
SimulationController *active = nullptr;
struct Engine {
    QStringList errors;
    bool exited = false;
    bool initialized = false;
    bool needsReload = false;
    static int text(char *text, int, void *data) {
        auto &engine = *static_cast<Engine*>(data);
        const auto message = QString::fromLocal8Bit(text);
        if (message.startsWith("stderr", Qt::CaseInsensitive) && (message.contains("error", Qt::CaseInsensitive) ||
            message.contains("failed", Qt::CaseInsensitive) ||
            message.contains("no such file", Qt::CaseInsensitive)))
            engine.errors.append(message);
        return 0;
    }
    static int exit(int, bool, bool, int, void *data) {
        static_cast<Engine*>(data)->exited = true;
        return 0;
    }
    int command(const QString &text) {
        auto bytes = text.toLocal8Bit();
        return ngSpice_Command(bytes.data());
    }
    int sourceFile(const QString &path) {
        QString error;
        const int result=loadSpiceFile(path,&error);
        if (!error.isEmpty()) errors.append(error);
        return result;
    }
    QVector<double> run(const QStringList &commands) {
        errors.clear();
        if (!initialized) {
            initialized = ngSpice_Init(text, nullptr, exit, nullptr, nullptr, nullptr, this) == 0;
            if (!initialized) errors.append("Could not initialize ngspice.");
        }
        if (exited) errors.append("ngspice exited; restart Tonestack to recover.");
        const bool reload = !commands.isEmpty() && commands.first().startsWith("source ", Qt::CaseInsensitive);
        if (needsReload && !reload) errors.append("Load a circuit again after the failed simulator command.");
        if (reload) needsReload = false;
        for (const auto &commandText : commands) {
            if (!errors.isEmpty() || exited) break;
            // Missing source files can trigger a controlled exit in the bundled
            // DLL. Reject those before entering it, preserving a usable session.
            const auto parts = QProcess::splitCommand(commandText);
            if (parts.size() == 2 && parts[0].compare("source", Qt::CaseInsensitive) == 0) {
                QFile source(parts[1]);
                if (!source.open(QIODevice::ReadOnly)) {
                    errors.append("Cannot read netlist: " + parts[1] + ": " + source.errorString());
                    break;
                }
            }
            if ((parts.size()==2 && parts[0].compare("source",Qt::CaseInsensitive)==0 ? sourceFile(parts[1]) : command(commandText)) != 0)
                errors.append("ngspice command failed: " + commandText);
        }
        if (exited) errors.append("ngspice exited; restart Tonestack to recover.");
        if (!errors.isEmpty()) { needsReload = true; return {}; }
        command("destroy all");
        if (command("run") != 0 || exited || !errors.isEmpty()) {
            if (errors.isEmpty()) errors.append("ngspice analysis failed.");
            return {};
        }
        auto info = ngGet_Vec_Info(const_cast<char*>("v(3)"));
        if (!info || info->v_length < 2 || !info->v_compdata) {
            errors.append("The simulation returned no complex V(3) response.");
            return {};
        }
        QVector<double> values;
        values.reserve(info->v_length);
        for (int i=0; i<info->v_length; ++i) {
            const auto value = info->v_compdata[i];
            const double magnitude = std::hypot(value.cx_real, value.cx_imag);
            if (!std::isfinite(magnitude)) {
                errors.append("The simulation returned a non-finite response.");
                return {};
            }
            values.append(magnitude);
        }
        return values;
    }
};
}
int dispatchSpiceCommand(const char *command)
{
    if (active) {
        active->enqueue(QString::fromLocal8Bit(command));
        return 0;
    }
    const auto path=AssetPaths::sourcePath(QString::fromLocal8Bit(command));
    if (!path.isEmpty()) return loadSpiceFile(path);
    return ngSpice_Command(const_cast<char*>(command));
}

SimulationController::SimulationController(QObject *parent) : QObject(parent), worker(new QObject)
{
    Q_ASSERT(!active); // The shared ngspice library has one process-wide circuit.
    active = this;
    worker->moveToThread(&simulationThread);
    connect(&simulationThread, &QThread::finished, worker, &QObject::deleteLater);
    debounce.setSingleShot(true);
    debounce.setInterval(15);
    connect(&debounce, &QTimer::timeout, this, &SimulationController::start);
    simulationThread.start();
}
SimulationController::~SimulationController()
{
    debounce.stop();
    active = nullptr;
    // Never destroy callbacks or DLL-owned vectors while analysis is in flight.
    simulationThread.quit();
    simulationThread.wait();
}
void SimulationController::enqueue(const QString &command)
{
    Q_ASSERT(QThread::currentThread() == this->thread());
    // A circuit load supersedes all unapplied alterations for the old circuit.
    if (command.startsWith("source ", Qt::CaseInsensitive))
        pending.clear();
    if (command.startsWith("alter ", Qt::CaseInsensitive)) {
        const auto key = command.section('=', 0, 0).trimmed();
        for (int i=pending.size()-1; i>=0; --i)
            if (pending[i].section('=', 0, 0).trimmed().compare(key, Qt::CaseInsensitive) == 0)
                pending.removeAt(i);
    }
    pending.append(command);
}
void SimulationController::request()
{
    ++revision;
    requested = true;
    emit busyChanged(true);
    if (!running) debounce.start();
}
void SimulationController::start()
{
    if (running || !requested) return;
    running = true;
    requested = false;
    const auto commands = pending;
    pending.clear();
    const auto generation = revision;
    // Callback state belongs to the worker and survives successive jobs.
    QMetaObject::invokeMethod(worker, [this, commands, generation] {
        // All ngspice entry points and callbacks execute on this one worker.
        static Engine engine; // ngspice retains callback context between controller owners.
        const auto values = engine.run(commands);
        const auto error = engine.errors.join('\n');
        QMetaObject::invokeMethod(this, [this, values, error, generation] {
            running = false;
            if (generation == revision) {
                if (error.isEmpty()) emit ready(values);
                else emit failed(error);
            }
            if (requested) debounce.start();
            else emit busyChanged(false);
        }, Qt::QueuedConnection);
    }, Qt::QueuedConnection);
}
