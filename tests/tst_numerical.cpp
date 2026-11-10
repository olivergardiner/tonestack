#include <QtTest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QFontDatabase>
#include <QTemporaryDir>
#include <complex>
#include <vector>
#include <memory>
#include <algorithm>
#include "circuit.h"
#include "assetpaths.h"
#include "tonestackcalculator.h"
#include "settingsdialog.h"

using Complex = std::complex<double>;
static constexpr double pi = 3.14159265358979323846;
static QStringList messages;
static bool exited = false;
static int output(char *text, int, void*) { messages.append(QString::fromLocal8Bit(text)); return 0; }
static int controlledExit(int, bool, bool, int, void*) { exited = true; return 0; }
static int command(const QString &text)
{
    auto bytes = text.toLocal8Bit();
    return ngSpice_Command(bytes.data());
}

// Independent nodal-analysis reference. The op-amp is its finite-gain,
// single-pole Thevenin equivalent, including input and output resistances.
class Network
{
    std::vector<std::vector<Complex>> a;
    std::vector<Complex> b;
public:
    Network() : a(32, std::vector<Complex>(32)), b(32) {}
    void admittance(int p, int n, Complex y)
    {
        a[p][p] += y; a[n][n] += y;
        a[p][n] -= y; a[n][p] -= y;
    }
    void resistor(int p, int n, double r) { admittance(p, n, 1.0 / r); }
    void capacitor(int p, int n, double c, double f) { admittance(p, n, Complex(0, 2*pi*f*c)); }
    void opamp(int p, int n, int out, double f)
    {
        resistor(p, n, 1e7);
        const Complex gain = 1e5 / Complex(1, 2*pi*f*1000*1.5915e-6);
        a[out][out] += 0.1;
        a[out][p] -= gain * 0.1;
        a[out][n] += gain * 0.1;
    }
    Complex solve()
    {
        // VIN 0 1 AC 1: node 1 is -1 V, not +1 V.
        for (int fixed : {0, 1}) {
            std::fill(a[fixed].begin(), a[fixed].end(), Complex{});
            a[fixed][fixed] = 1;
            b[fixed] = fixed == 1 ? -1 : 0;
        }
        // Unused node numbers need a harmless constraint.
        for (int i=2; i<32; ++i) {
            bool used = false;
            for (auto x : a[i]) used |= std::abs(x) > 0;
            if (!used) a[i][i] = 1;
        }
        for (int i=0; i<32; ++i) {
            int pivot = i;
            for (int j=i+1; j<32; ++j)
                if (std::abs(a[j][i]) > std::abs(a[pivot][i])) pivot = j;
            if (std::abs(a[pivot][i]) < 1e-18) return Complex(NAN, NAN);
            std::swap(a[i], a[pivot]); std::swap(b[i], b[pivot]);
            const auto divisor = a[i][i];
            for (int k=i; k<32; ++k) a[i][k] /= divisor;
            b[i] /= divisor;
            for (int j=0; j<32; ++j) if (j != i) {
                const auto scale = a[j][i];
                for (int k=i; k<32; ++k) a[j][k] -= scale*a[i][k];
                b[j] -= scale*b[i];
            }
        }
        return b[3];
    }
};

struct Controls
{
    QLabel label, image;
    QLineEdit text;
    QDial dial;
    QComboBox types;
    SettingsDialog settings;
    circuitUiMap map{};
    std::unique_ptr<Potentiometer> pots[3];
    std::unique_ptr<Resistor> resistors[5];
    std::unique_ptr<Capacitor> capacitors[5];
    std::unique_ptr<Inductor> inductors[5];
    Resistor source{&label, &text, 98}, load{&label, &text, 99};
    Circuit circuit{&map};
    Controls()
    {
        dial.setRange(0, 99);
        types.addItems({"Linear", "Log", "Log A", "Reverse", "S"});
        for (int i=0; i<3; ++i) {
            pots[i].reset(new Potentiometer(&dial, &text, &types, &label, i+1));
            pots[i]->setSweeper(&settings);
        }
        map.pot1=pots[0].get(); map.pot2=pots[1].get(); map.pot3=pots[2].get();
        map.circuitDisplay=&image; map.rS=&source; map.rL=&load;
        for (int i=0; i<5; ++i) {
            resistors[i].reset(new Resistor(&label, &text, i+1));
            capacitors[i].reset(new Capacitor(&label, &text, i+1));
            inductors[i].reset(new Inductor(&label, &text, i+1));
            map.resistors[i]=resistors[i].get();
            map.capacitors[i]=capacitors[i].get();
            map.inductors[i]=inductors[i].get();
        }
    }
    void read(const QJsonObject &entry)
    {
        settings.read(entry.value("taperSettings").toObject());
        circuit.read(entry);
    }
};

class NumericalTest : public QObject
{
    Q_OBJECT
    QJsonObject catalogue(const QString &path, int index)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) return {};
        const auto entries = QJsonDocument::fromJson(file.readAll()).object()["circuits"].toArray();
        return index >= 0 && index < entries.size() ? entries.at(index).toObject() : QJsonObject{};
    }
    struct Sample { double frequency; Complex voltage; };
    QVector<Sample> simulate()
    {

        if (command("destroy all") != 0 || command("run") != 0 || exited)
            return {};
        auto f = ngGet_Vec_Info(const_cast<char*>("frequency"));
        if (!f || (!f->v_realdata && !f->v_compdata)) return {};
        const vector_info frequency = *f; // ngGet_Vec_Info reuses its descriptor.
        auto v = ngGet_Vec_Info(const_cast<char*>("v(3)"));
        if (!f || !v || !v->v_compdata || frequency.v_length != v->v_length) return {};
        QVector<Sample> result;
        for (int i=0; i<v->v_length; ++i) {
            const double hz = frequency.v_realdata ? frequency.v_realdata[i] : frequency.v_compdata[i].cx_real;
            result.append(Sample{hz, Complex(v->v_compdata[i].cx_real, v->v_compdata[i].cx_imag)});
        }
        return result;
    }
    void checkSweep(const QVector<Sample> &samples)
    {
        QVERIFY2(!exited, qPrintable(messages.join('\n')));
        for (const auto &message : messages)
            QVERIFY2(!message.contains("stderr Error:", Qt::CaseInsensitive), qPrintable(messages.join('\n')));
        QCOMPARE(samples.size(), 161);
        for (int i=0; i<samples.size(); ++i) {
            QVERIFY(std::isfinite(samples[i].voltage.real()));
            QVERIFY(std::isfinite(samples[i].voltage.imag()));
            const double expected = 10*std::pow(10.0, i/40.0);
            QVERIFY(std::abs(samples[i].frequency/expected-1) < 1e-10);
        }
    }
    void compare(Complex actual, Complex expected, double f)
    {
        const double error = std::abs(actual-expected);
        QVERIFY2(error < 1e-8 + 2e-5*std::abs(expected),
            qPrintable(QString("f=%1 Hz actual=(%2,%3), reference=(%4,%5), error=%6\n%7")
                .arg(f).arg(actual.real(),0,'g',14).arg(actual.imag(),0,'g',14)
                .arg(expected.real(),0,'g',14).arg(expected.imag(),0,'g',14).arg(error)
                .arg(messages.join('\n'))));
    }
private slots:
    void initTestCase()
    {
        QCOMPARE(ngSpice_Init(output, output, controlledExit, nullptr, nullptr, nullptr, nullptr), 0);
        QCOMPARE(command("set noaskquit"), 0);
    }
    void init() { messages.clear(); }
    void rcLowPass()
    {
        Controls controls;
        const auto entry = catalogue("../circuits/basic/basic.tsc", 2);
        QVERIFY(!entry.isEmpty());
        controls.read(entry);
        const auto samples = simulate();
        checkSweep(samples);
        // Loaded RC network: Rseries=R98+R1, parallel R99/C1, VIN=-1.
        const double r=160001, load=1e7, c=1e-9;
        for (const auto &sample : samples)
            compare(sample.voltage, -1.0 / Complex(1+r/load, 2*pi*sample.frequency*r*c), sample.frequency);
        // Changing the production capacitor adapter must move the pole.
        controls.capacitors[0]->setValue(2200);
        const auto changed = simulate();
        checkSweep(changed);
        QCOMPARE(changed.size(), 161);
        for (const auto &sample : changed)
            compare(sample.voltage, -1.0 / Complex(1+r/load, 2*pi*sample.frequency*r*2.2e-9), sample.frequency);
    }
    void toneStacks_data()
    {
        QTest::addColumn<bool>("active");
        QTest::addColumn<int>("position");
        for (bool active : {false,true})
            for (int position : {10,49,90})
                QTest::newRow(qPrintable(QString("%1-%2").arg(active ? "Pearl-ganged" : "Fender").arg(position))) << active << position;
    }
    void toneStacks()
    {
        QFETCH(bool, active);
        QFETCH(int, position);
        Controls controls;
        auto entry = catalogue(active ? "../circuits/active/active.tsc" : "../circuits/tonestacks.tsc", 0);
        QVERIFY(!entry.isEmpty());
        // Use linear tapers here to isolate deck wiring and integer pot splitting.
        for (const auto key : {"pot1","pot2","pot3"}) if (entry.contains(key)) {
            auto pot = entry[key].toObject(); pot["type"]=0; pot["position"]=position; entry[key]=pot;
        }
        controls.read(entry);
        const auto samples = simulate();
        checkSweep(samples);
        const auto low = [position](int total) { return int(total*(position/99.0)); };
        for (const auto &sample : samples) {
            const double f = sample.frequency;
            Network n;
            if (!active) {
                n.resistor(1,2,38000); n.resistor(3,0,500000);
                n.resistor(2,10,100000);
                n.capacitor(2,11,270e-12,f); n.capacitor(10,12,100e-9,f); n.capacitor(10,13,47e-9,f);
                n.resistor(12,13,low(250000));
                n.resistor(0,13,low(10000));
                n.resistor(12,3,low(250000)); n.resistor(3,11,250000-low(250000));
            } else {
                n.resistor(1,2,1); n.resistor(3,0,10000);
                n.resistor(12,13,2200); n.resistor(17,18,2200);
                n.capacitor(12,18,4.7e-9,f); n.capacitor(15,0,68e-9,f);
                n.resistor(3,10,low(10000)); n.resistor(10,2,10000-low(10000));
                n.resistor(15,13,100000-low(100000));
                n.resistor(15,17,100000-low(100000));
                n.opamp(10,12,18,f);
                n.resistor(18,21,22000); n.resistor(10,20,22000);
                n.resistor(21,0,22000); n.resistor(20,22,22000); n.opamp(21,20,22,f);
                n.resistor(2,30,10000); n.resistor(22,30,10000); n.resistor(30,3,10000); n.opamp(0,30,3,f);
            }
            compare(sample.voltage, n.solve(), f);
        }
    }
    void restoredResponse_data()
    {
        QTest::addColumn<int>("algorithm");
        QTest::newRow("midpoint-tapers") << 0;
        QTest::newRow("legacy-tapers") << 1;
    }
    void restoredResponse()
    {
        QFETCH(int, algorithm);
        Controls original;
        auto entry = catalogue("../circuits/active/active.tsc",0);
        QVERIFY(!entry.isEmpty());
        original.read(entry);
        original.settings.read({{"algorithm",algorithm},{"log",0.19},{"logA",0.34},{"reverseLog",0.79}});
        original.pots[0]->setType(POT_LOG);
        original.pots[0]->setPosition(23);
        original.pots[2]->setType(POT_RLOG);
        original.pots[2]->setPosition(84);
        const auto before = simulate();
        checkSweep(before);
        QCOMPARE(before.size(), 161);
        QJsonObject root;
        original.circuit.write(root);
        auto saved = root["circuits"].toArray().first().toObject();
        saved["taperSettings"]=original.settings.write();
        root["circuits"]=QJsonArray{saved};
        root["formatVersion"]=2;
        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        QFile file(directory.filePath("response.tsc"));
        QVERIFY(file.open(QIODevice::WriteOnly));
        const auto bytes=QJsonDocument(root).toJson();
        QCOMPARE(file.write(bytes), bytes.size()); file.close();
        // Perturb both the loaded circuit and taper settings before restoration.
        original.pots[0]->setPosition(70);
        original.pots[2]->setPosition(20);
        const auto changed = simulate();
        checkSweep(changed);
        QCOMPARE(changed.size(), 161);
        QVERIFY(std::abs(changed[80].voltage-before[80].voltage)>1e-3);
        QVERIFY(file.open(QIODevice::ReadOnly));
        Controls restored;
        restored.read(QJsonDocument::fromJson(file.readAll()).object()["circuits"].toArray().first().toObject());
        const auto after = simulate();
        checkSweep(after);
        QCOMPARE(after.size(),before.size());
        for (int i=0; i<after.size(); ++i)
            compare(after[i].voltage,before[i].voltage,after[i].frequency);
    }
    void controllerUsesRealEngine()
    {
        SimulationController controller;
        Controls controls;
        QSignalSpy ready(&controller, &SimulationController::ready);
        QSignalSpy errors(&controller, &SimulationController::failed);
        controls.read(catalogue("../circuits/basic/basic.tsc",2));
        controller.request();
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY2(errors.isEmpty(), errors.isEmpty() ? "" : qPrintable(errors.first().first().toString()));
        QCOMPARE(ready.count(),1);
        const auto first = qvariant_cast<QVector<double>>(ready.takeFirst().first());
        QCOMPARE(first.size(),161);
        for (int value : {1200,1800,2200}) {
            controls.capacitors[0]->setValue(value);
            controller.request();
        }
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY2(errors.isEmpty(), errors.isEmpty() ? "" : qPrintable(errors.first().first().toString()));
        QCOMPARE(ready.count(),1);
        const auto values = qvariant_cast<QVector<double>>(ready.first().first());
        QCOMPARE(values.size(),161);
        for (int i=0; i<values.size(); ++i) {
            const double f = 10*std::pow(10.0,i/40.0);
            const double expected = std::abs(-1.0/Complex(1+160001/1e7,2*pi*f*160001*2.2e-9));
            QVERIFY(std::abs(values[i]-expected)<1e-8);
        }
        QVERIFY(std::abs(first[80]-values[80])>0.1);
    }
    void realWindowUpdatesAndRecovers()
    {
        ToneStackCalculator window;
        QSignalSpy ready(&window.simulation,&SimulationController::ready);
        QSignalSpy errors(&window.simulation,&SimulationController::failed);
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY2(errors.isEmpty(), errors.isEmpty() ? "" : qPrintable(errors.first().first().toString()));
        QVERIFY(window.plot);
        ready.clear();
        for (int p=10; p<90; ++p) window.on_pot1Position_valueChanged(p);
        window.on_actionActive_Circuits_Mode_triggered();
        window.on_pot3Position_valueChanged(71);
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY2(errors.isEmpty(), errors.isEmpty() ? "" : qPrintable(errors.first().first().toString()));
        QCOMPARE(ready.count(),1);
        QCOMPARE(window.uiMap.pot3->getPosition(),71);
        QCOMPARE(window.currentRange,1);
        QVERIFY(window.plot);
        window.simulation.enqueue("source __tonestack_missing_test_file__.cir");
        window.createPlot();
        QTRY_COMPARE_WITH_TIMEOUT(errors.count(),1,5000);
        errors.clear(); ready.clear();
        window.on_actionBasic_Electronics_Mode_triggered();
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY2(errors.isEmpty(), errors.isEmpty() ? "" : qPrintable(errors.first().first().toString()));
        QCOMPARE(ready.count(),1);
        QCOMPARE(window.currentRange,0);
        QVERIFY(window.plot);
    }
    void pathsWithSpacesAndUnrelatedCwd()
    {
        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        const auto original = QDir::currentPath();
        struct RestoreDirectory {
            QString path;
            ~RestoreDirectory() { QDir::setCurrent(path); }
        } restore{original};
        QVERIFY(QDir(directory.path()).mkpath("custom library with spaces"));
        const auto base = directory.filePath("custom library with spaces");
        {
            QFile model(QDir(base).filePath("parts.inc"));
            QVERIFY(model.open(QIODevice::WriteOnly));
            model.write("R1 1 3 160001\nC1 3 0 1n\nR99 3 0 10meg\n");
            QFile deck(QDir(base).filePath("test filter.cir"));
            QVERIFY(deck.open(QIODevice::WriteOnly));
            deck.write("Custom RC\nVIN 0 1 AC 1\n.include parts.inc\n.ac dec 40 10 100k\n.end\n");
        }
        QVERIFY(QDir::setCurrent(directory.path()));
        SimulationController controller;
        QSignalSpy ready(&controller,&SimulationController::ready);
        QSignalSpy errors(&controller,&SimulationController::failed);
        controller.enqueue(AssetPaths::sourceCommand(QDir(AssetPaths::circuitsDirectory()).filePath("fender.cir")));
        controller.request();
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY2(errors.isEmpty(), errors.isEmpty() ? "" : qPrintable(errors.first().first().toString()));
        ready.clear();
        controller.enqueue(AssetPaths::sourceCommand(QDir(base).filePath("test filter.cir")));
        controller.request();
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY2(errors.isEmpty(), errors.isEmpty() ? "" : qPrintable(errors.first().first().toString()));
        const auto values=qvariant_cast<QVector<double>>(ready.first().first());
        QCOMPARE(values.size(),161);
        for (int i=0; i<values.size(); ++i) {
            const double f=10*std::pow(10.0,i/40.0);
            QVERIFY(std::abs(values[i]-std::abs(-1.0/Complex(1+160001/1e7,2*pi*f*160001*1e-9)))<1e-8);
        }
        auto resolved=AssetPaths::resolved({{"command","source \"test filter.cir\""},{"image","diagram.png"}},base);
        QCOMPARE(AssetPaths::sourcePath(resolved["command"].toString()),QDir(base).filePath("test filter.cir"));
        const auto saved=AssetPaths::forSave(resolved,directory.path());
        const auto reopened=AssetPaths::resolved(saved,directory.path());
        QCOMPARE(reopened,resolved);
    }
    void windowFromUnrelatedDirectory()
    {
        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        const auto previous=QDir::currentPath();
        QVERIFY(QDir::setCurrent(directory.path()));
        {
            ToneStackCalculator window;
            QSignalSpy ready(&window.simulation,&SimulationController::ready);
            QSignalSpy errors(&window.simulation,&SimulationController::failed);
            QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
            QVERIFY2(errors.isEmpty(), errors.isEmpty() ? "" : qPrintable(errors.first().first().toString()));
            QVERIFY(window.plot);
            QHelpEngineCore help(QDir(QCoreApplication::applicationDirPath()).filePath("help/tonestack.qhc"));
            help.setReadOnly(true);
            QVERIFY2(help.setupData(),qPrintable(help.error()));
            const auto helpFile=help.documentationFileName("whitecottage.tonestack.help");
            QCOMPARE(QFileInfo(helpFile).canonicalFilePath(),QFileInfo(QDir(QCoreApplication::applicationDirPath()).filePath("help/tonestack.qch")).canonicalFilePath());
            QVERIFY(!help.fileData(QUrl("qthelp://whitecottage.tonestack.help/doc/index.html")).isEmpty());
            QVERIFY(!window.circuits.isEmpty());
            const auto entry=window.circuits.first().toObject();
            QVERIFY(QFileInfo::exists(entry["image"].toString()));
        }
        QVERIFY(QDir::setCurrent(previous));
    }
    void namedComparisonRestoresResponse()
    {
        if(qEnvironmentVariableIsSet("TONESTACK_PREVIEW")) {
            QFontDatabase::addApplicationFont(QDir(qEnvironmentVariable("SystemRoot")).filePath("Fonts/segoeui.ttf"));
            QApplication::setFont(QFont("Segoe UI",9));
        }
        ToneStackCalculator window;
        QSignalSpy ready(&window.simulation,&SimulationController::ready);
        QSignalSpy errors(&window.simulation,&SimulationController::failed);
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY(errors.isEmpty());
        ready.clear();
        window.settingsDialog.read({{"algorithm",0},{"log",0.19},{"logA",0.34},{"reverseLog",0.79}});
        window.on_pot1Position_valueChanged(23);
        window.on_pot2Position_valueChanged(84);
        window.on_pot3Position_valueChanged(window.uiMap.pot3->getPosition());
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY(errors.isEmpty());
        QVERIFY(window.addComparison("Reference"));
        if(qEnvironmentVariableIsSet("TONESTACK_PREVIEW")) {
            QFontDatabase::addApplicationFont(QDir(qEnvironmentVariable("SystemRoot")).filePath("Fonts/segoeui.ttf"));
            window.show();
            QCoreApplication::processEvents();
            QVERIFY(window.grab().save(qEnvironmentVariable("TONESTACK_PREVIEW")));
        }
        const auto original=window.currentResponse;
        const auto settings=window.settingsDialog.write();
        ready.clear();
        window.on_actionActive_Circuits_Mode_triggered();
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY(errors.isEmpty());
        QCOMPARE(window.currentRange,1);
        ready.clear();
        QTimer::singleShot(0,&window.settingsDialog,[&window] {
            window.settingsDialog.read({{"log",0.21}});
            window.settingsDialog.accept();
        });
        window.on_actionSettings_triggered();
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY(errors.isEmpty()); // Pearl has no pot2: do not alter absent deck elements.
        ready.clear();
        window.restoreComparisonAt(0);
        QTRY_VERIFY_WITH_TIMEOUT(ready.count()+errors.count()>0,5000);
        QVERIFY(errors.isEmpty());
        QCOMPARE(window.currentRange,0);
        QCOMPARE(window.uiMap.pot1->getPosition(),23);
        QCOMPARE(window.uiMap.pot2->getPosition(),84);
        QCOMPARE(window.settingsDialog.write(),settings);
        QCOMPARE(window.currentResponse.size(),original.size());
        for(int i=0;i<original.size();++i)
            QVERIFY(std::abs(window.currentResponse[i]-original[i])<1e-10);
        QCOMPARE(window.comparisons.size(),1);
        window.deleteComparisonAt(0);
        QVERIFY(window.comparisons.isEmpty());
    }
};
QTEST_MAIN(NumericalTest)
#include "tst_numerical.moc"
