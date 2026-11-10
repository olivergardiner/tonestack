#include <QtTest>
#include <QSignalSpy>
#include <QSemaphore>
#include <QMutex>
#include <atomic>
#include <limits>
#include <QTemporaryDir>
#include <QFile>
#include <QDir>
#include "simulationcontroller.h"
#include "ngspice/sharedspice.h"

namespace {
QMutex lock;
QStringList history;
std::atomic<int> runs{0}, invalidResult{0};
std::atomic<bool> hold{false}, fail{false};
QSemaphore entered, releaseRun;
double current = 1;
ngcomplex_t data[2];
vector_info vector{};
}
extern "C" int ngSpice_Init(SendChar*, SendStat*, ControlledExit*, SendData*, SendInitData*, BGThreadRunning*, void*)
{ return 0; }
extern "C" int ngSpice_Command(char *command)
{
    const QString text = QString::fromLatin1(command);
    { QMutexLocker guard(&lock); history.append(text); }
    if (text.startsWith("alter r1=")) current = text.section('=',1).toDouble();
    if (text == "run") {
        ++runs;
        if (hold.exchange(false)) {
            entered.release();
            if (!releaseRun.tryAcquire(1,5000)) return 1;
        }
        if (fail.exchange(false)) return 1;
        data[0] = {current,0}; data[1] = {current*0.5,0};
    }
    return 0;
}
extern "C" int ngSpice_Circ(char **lines)
{
    QMutexLocker guard(&lock);
    history.append(QString::fromLatin1(lines[0]));
    return 0;
}
extern "C" pvector_info ngGet_Vec_Info(char*)
{
    if (invalidResult == 1) return nullptr;
    if (invalidResult == 2) data[0].cx_real = std::numeric_limits<double>::quiet_NaN();
    vector.v_length=2; vector.v_compdata=data;
    return &vector;
}
class ControllerTest : public QObject
{
    Q_OBJECT
    QTemporaryDir directory;
    QString originalDirectory;
private slots:
    void initTestCase()
    {
        originalDirectory = QDir::currentPath();
        QVERIFY(directory.isValid());
        QVERIFY(QDir::setCurrent(directory.path()));
        for (const auto name : {"old","new"}) {
            QFile file(name);
            QVERIFY(file.open(QIODevice::WriteOnly));
            file.write(QByteArray("source ") + name + "\n.end\n");
        }
    }
    void cleanupTestCase() { QDir::setCurrent(originalDirectory); }
    void init()
    {
        QMutexLocker guard(&lock);
        history.clear(); runs=0; current=1; hold=false; fail=false; invalidResult=0;
    }
    void combinesRapidEdits()
    {
        SimulationController controller;
        QSignalSpy ready(&controller,&SimulationController::ready);
        for (int i=1; i<=50; ++i) {
            controller.enqueue(QString("alter r1=%1").arg(i));
            controller.request();
        }
        QCOMPARE(runs.load(),0); // Scheduling does not run or wait in the caller.
        QTRY_COMPARE_WITH_TIMEOUT(ready.count(),1,2000);
        QCOMPARE(runs.load(),1);
        QCOMPARE(qvariant_cast<QVector<double>>(ready.at(0).at(0)).at(0),50.0);
    }
    void supersedesInFlightAndSwitchesCircuit()
    {
        SimulationController controller;
        QSignalSpy ready(&controller,&SimulationController::ready);
        hold=true;
        controller.enqueue("source old");
        controller.enqueue("alter r1=2");
        controller.request();
        QTRY_VERIFY_WITH_TIMEOUT(entered.available()>0,2000);
        entered.acquire();
        controller.enqueue("alter r1=3");
        controller.request();
        controller.enqueue("source new");
        controller.enqueue("alter r1=9");
        controller.request();
        // No source/alter commands can reach the running simulator.
        {
            QMutexLocker guard(&lock);
            QVERIFY(!history.contains("source new"));
            QVERIFY(!history.contains("alter r1=3"));
        }
        releaseRun.release();
        QTRY_COMPARE_WITH_TIMEOUT(ready.count(),1,2000);
        QCOMPARE(runs.load(),2);
        QCOMPARE(qvariant_cast<QVector<double>>(ready.at(0).at(0)).at(0),9.0);
        QMutexLocker guard(&lock);
        QVERIFY(!history.contains("alter r1=3"));
        QVERIFY(history.indexOf("source new")>history.indexOf("run"));
    }
    void reportsFailureAndRecovers()
    {
        SimulationController controller;
        QSignalSpy errors(&controller,&SimulationController::failed);
        QSignalSpy ready(&controller,&SimulationController::ready);
        fail=true;
        controller.request();
        QTRY_COMPARE_WITH_TIMEOUT(errors.count(),1,2000);
        QCOMPARE(ready.count(),0);
        controller.enqueue("alter r1=7");
        controller.request();
        QTRY_COMPARE_WITH_TIMEOUT(ready.count(),1,2000);
        QCOMPARE(qvariant_cast<QVector<double>>(ready.at(0).at(0)).at(0),7.0);
    }
    void invalidResults_data()
    {
        QTest::addColumn<int>("mode");
        QTest::newRow("missing-vector") << 1;
        QTest::newRow("nonfinite-vector") << 2;
    }
    void invalidResults()
    {
        QFETCH(int, mode);
        SimulationController controller;
        QSignalSpy errors(&controller,&SimulationController::failed);
        QSignalSpy ready(&controller,&SimulationController::ready);
        invalidResult=mode;
        controller.request();
        QTRY_COMPARE_WITH_TIMEOUT(errors.count(),1,2000);
        QVERIFY(ready.isEmpty());
        invalidResult=0;
        controller.request();
        QTRY_COMPARE_WITH_TIMEOUT(ready.count(),1,2000);
    }
    void shutdownWithRunInFlight()
    {
        auto controller = new SimulationController;
        hold=true;
        controller->request();
        QTRY_VERIFY_WITH_TIMEOUT(entered.available()>0,2000);
        entered.acquire();
        // Completion can be queued while the GUI owner is being destroyed.
        releaseRun.release();
        delete controller;
        QCoreApplication::processEvents();
        QCOMPARE(runs.load(),1);
        // A fresh owner must initialize its own callback context.
        SimulationController next;
        QSignalSpy ready(&next,&SimulationController::ready);
        next.request();
        QTRY_COMPARE_WITH_TIMEOUT(ready.count(),1,2000);
    }
};
QTEST_GUILESS_MAIN(ControllerTest)
#include "tst_controller.moc"
