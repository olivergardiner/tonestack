#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QStringList>
#include <QVector>

// Adapters retain their existing command API. Without a controller (e.g.
// numerical tests), commands go directly to ngspice.
int dispatchSpiceCommand(const char *command);

class SimulationController : public QObject
{
    Q_OBJECT
public:
    explicit SimulationController(QObject *parent = nullptr);
    ~SimulationController();
    void enqueue(const QString &command);
    void request();
signals:
    void ready(const QVector<double> &magnitudes);
    void failed(const QString &message);
    void busyChanged(bool busy);
private:
    void start();
    QThread simulationThread;
    QObject *worker;
    QTimer debounce;
    QStringList pending;
    quint64 revision = 0;
    bool running = false;
    bool requested = false;
};
#endif
