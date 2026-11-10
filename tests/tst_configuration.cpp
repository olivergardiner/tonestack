#include <QtTest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QComboBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QGraphicsLineItem>
#include "circuit.h"
#include "tonestackcalculator.h"
#include <QTemporaryDir>
#include "settingsdialog.h"

// Capture the real adapters' command output without loading ngspice.
static QStringList commands;
extern "C" int ngSpice_Command(char *command)
{
    commands.append(QString::fromLatin1(command));
    return 0;
}

extern "C" int ngSpice_Init(SendChar*, SendStat*, ControlledExit*, SendData*, SendInitData*, BGThreadRunning*, void*)
{
    return 0;
}
extern "C" int ngSpice_Circ(char**) { return 0; }
extern "C" pvector_info ngGet_Vec_Info(char*) { return nullptr; }

class ConfigurationTest : public QObject
{
    Q_OBJECT
private slots:
    void potRoundTrip()
    {
        QDial dial; dial.setRange(0, 99);
        QLineEdit text; QLabel label; QComboBox types;
        types.addItems({"Linear", "Log", "Log A", "Reverse", "S"});
        Potentiometer pot(&dial, &text, &types, &label, 1);
        pot.read({{"value", 100000}, {"type", 0}, {"ganged", true}, {"function", "Bass"}, {"position", 17}});
        QJsonObject saved;
        pot.write(saved);
        pot.setPosition(88);
        commands.clear();
        pot.read(saved);
        QCOMPARE(pot.getPosition(), 17);
        QCOMPARE(dial.value(), 17);
        QCOMPARE(pot.getValue(), 100000);
        QVERIFY(pot.isGanged());
        QVERIFY(commands.contains("alter rp1lo=17171"));
        QVERIFY(commands.contains("alter rp1lo2=17171"));
        QJsonObject again;
        pot.write(again);
        QCOMPARE(again, saved);
        saved.remove("position");
        pot.read(saved);
        QCOMPARE(pot.getPosition(), 49);
        saved["position"] = 900;
        pot.read(saved);
        QCOMPARE(pot.getPosition(), 99);
    }

    void taperRoundTrip()
    {
        SettingsDialog settings;
        settings.read({{"algorithm", 0}, {"log", 0.18}, {"logA", 0.32}, {"reverseLog", 0.81}});
        const auto saved = settings.write();
        const double response = settings.taperFunction(POT_LOG, 0.37);
        settings.read({{"algorithm", 1}, {"log", 0.11}});
        settings.read(saved);
        QCOMPARE(settings.write(), saved);
        QCOMPARE(settings.taperFunction(POT_LOG, 0.37), response);
        settings.read({});
        QCOMPARE(settings.write(), saved);
        settings.read({{"algorithm", 1}, {"log", 0.18}, {"logA", 0.32}, {"reverseLog", 0.81}});
        SettingsDialog restored;
        restored.read(settings.write());
        QCOMPARE(restored.taperFunction(POT_RLOG, 0.65), settings.taperFunction(POT_RLOG, 0.65));
    }

    void circuitNameAndValues()
    {
        QLabel image, label;
        QLineEdit text;
        Resistor source(&label, &text, 98), load(&label, &text, 99), resistor(&label, &text, 1);
        Capacitor capacitor(&label, &text, 1);
        Inductor inductor(&label, &text, 1);
        QDial dial; dial.setRange(0, 99);
        QComboBox types; types.addItems({"Linear", "Log", "Log A", "Reverse", "S"});
        Potentiometer p1(&dial, &text, &types, &label, 1), p2(&dial, &text, &types, &label, 2), p3(&dial, &text, &types, &label, 3);
        circuitUiMap map{};
        map.circuitDisplay = &image;
        map.rS = &source; map.rL = &load;
        map.pot1 = &p1; map.pot2 = &p2; map.pot3 = &p3;
        for (int i = 0; i < 5; ++i) {
            map.resistors[i] = &resistor;
            map.capacitors[i] = &capacitor;
            map.inductors[i] = &inductor;
        }
        Circuit circuit(&map);
        QJsonObject entry{{"circuitname", "Saved test"}, {"range", 1},
            {"rS", QJsonObject{{"value", 1200}}}, {"rL", QJsonObject{{"value", 470000}}},
            {"resistors", QJsonArray{QJsonObject{{"value", 33000}}}},
            {"capacitors", QJsonArray{QJsonObject{{"value", 47000}}}},
            {"inductors", QJsonArray{QJsonObject{{"value", 2200}}}},
            {"pot1", QJsonObject{{"value", 250000}, {"type", 0}, {"position", 73}}}};
        circuit.read(entry);
        QJsonObject root;
        circuit.write(root);
        const auto bytes = QJsonDocument(root).toJson();
        circuit.read(QJsonDocument::fromJson(bytes).object()["circuits"].toArray().first().toObject());
        QJsonObject again;
        circuit.write(again);
        QCOMPARE(again, root);
        QCOMPARE(circuit.getCircuitName(), QString("Saved test"));
        QCOMPARE(source.getValue(), 1200);
        QCOMPARE(load.getValue(), 470000);
        QCOMPARE(capacitor.getValue(), 47000);
        QCOMPARE(inductor.getValue(), 2200);
        QCOMPARE(p1.getPosition(), 73);
    }

    void namedComparisons()
    {
        ToneStackCalculator window;
        QVERIFY(!window.addComparison("Pending"));
        const auto requested=window.requestedConfiguration;
        window.settingsDialog.read({{"log",0.2}}); // A preview must not change captured request metadata.
        window.renderPlot(QVector<double>(161,0.5));
        QVERIFY(window.addComparison("Clean"));
        const auto settings=window.comparisons[0].configuration;
        QCOMPARE(settings,requested);
        const auto samples=window.comparisons[0].magnitudes;
        auto list=window.findChild<QTreeWidget*>("comparisonsList");
        QVERIFY(list);
        QCOMPARE(list->topLevelItemCount(),1);
        list->topLevelItem(0)->setText(0,"Renamed");
        QCOMPARE(window.comparisons[0].name,QString("Renamed"));
        list->topLevelItem(0)->setCheckState(0,Qt::Unchecked);
        QVERIFY(!window.comparisons[0].trace->isVisible());
        list->topLevelItem(0)->setCheckState(0,Qt::Checked);
        QVERIFY(window.comparisons[0].trace->isVisible());
        const auto y=window.comparisons[0].trace->childrenBoundingRect().top();
        window.showRange(1);
        QVERIFY(std::abs(window.comparisons[0].trace->childrenBoundingRect().top()-y-200)<1e-8);
        QCOMPARE(window.comparisons[0].magnitudes,samples);
        window.restoreComparisonAt(0);
        QCOMPARE(window.uiMap.pot1->getPosition(),settings["pot1"].toObject()["position"].toInt());
        QVERIFY(!window.responseCurrent);
        QVERIFY(!window.addComparison("Stale"));
        window.renderPlot(samples);
        const auto live=window.plot;
        window.on_pushButton_3_clicked();
        QVERIFY(window.comparisons.isEmpty());
        QCOMPARE(list->topLevelItemCount(),0);
        QCOMPARE(window.plot,live);
    }
    void settingsCommitAndCancel()
    {
        ToneStackCalculator window;
        const auto original=window.settingsDialog.write();
        QTimer::singleShot(0,&window.settingsDialog,[&window] {
            window.settingsDialog.read({{"log",0.2}});
            window.settingsDialog.reject();
        });
        window.on_actionSettings_triggered();
        QCOMPARE(window.settingsDialog.write(),original);
        window.uiMap.pot1->setValue(1234567);
        QTimer::singleShot(0,&window.settingsDialog,[&window] {
            window.settingsDialog.read({{"log",0.19}});
            window.settingsDialog.accept();
        });
        window.on_actionSettings_triggered();
        QCOMPARE(window.uiMap.pot1->getValue(),1234567);
        QCOMPARE(window.settingsDialog.getPotFactor(POT_LOG),0.19);
    }
    void centrePotsKeepsConfiguration()
    {
        ToneStackCalculator window;
        window.uiMap.pot1->setPosition(10);
        window.uiMap.pot2->setPosition(23);
        window.uiMap.pot3->setPosition(90);
        window.uiMap.resistors[0]->setValue(47000);
        window.uiMap.pot1->setValue(1234567);
        QJsonObject before;
        window.circuit->write(before);
        auto entry=before["circuits"].toArray().first().toObject();
        for(const auto key : {"pot1","pot2","pot3"}) {
            auto pot=entry[key].toObject();
            pot["position"]=49;
            entry[key]=pot;
        }
        before["circuits"]=QJsonArray{entry};
        const auto taper=window.settingsDialog.write();
        window.filename="saved-configuration.tsc";
        const auto button=window.findChild<QPushButton*>("centrePotsButton");
        QVERIFY(button);
        QTest::mouseClick(button,Qt::LeftButton);
        QJsonObject after;
        window.circuit->write(after);
        QCOMPARE(after,before);
        QCOMPARE(window.settingsDialog.write(),taper);
        QCOMPARE(window.filename,QString("saved-configuration.tsc"));
        QVERIFY(!window.responseCurrent);
        window.uiMap.pot2->setPosition(23);
        window.on_actionActive_Circuits_Mode_triggered(); // Pearl has pots 1 and 3 only.
        window.uiMap.pot1->setPosition(8);
        window.uiMap.pot3->setPosition(88);
        QTest::mouseClick(button,Qt::LeftButton);
        QCOMPARE(window.uiMap.pot1->getPosition(),49);
        QCOMPARE(window.uiMap.pot3->getPosition(),49);
        QCOMPARE(window.uiMap.pot2->getPosition(),23);
        window.on_actionBasic_Electronics_Mode_triggered(); // First circuit has no pots.
        QTest::mouseClick(button,Qt::LeftButton);
        QCOMPARE(window.uiMap.pot2->getPosition(),23);
    }
    void comparisonLimitAndDelete()
    {
        ToneStackCalculator window;
        window.renderPlot(QVector<double>(2,0.1));
        for(int i=0;i<256;++i) QVERIFY(window.addComparison(QString::number(i)));
        QVERIFY(!window.addComparison("Overflow"));
        QCOMPARE(window.comparisons.size(),256);
        window.deleteComparisonAt(17);
        QCOMPARE(window.comparisons.size(),255);
        QVERIFY(window.addComparison("Replacement"));
        window.on_pushButton_3_clicked();
        QVERIFY(window.comparisons.isEmpty());
    }
    void savedFileRestoresResponseConfiguration()
    {
        ToneStackCalculator window;
        QVERIFY(!window.circuits.isEmpty());
        window.uiMap.pot1->setPosition(23);
        window.uiMap.pot2->setPosition(84);
        window.settingsDialog.read({{"algorithm", 1}, {"log", 0.19}, {"logA", 0.34}, {"reverseLog", 0.79}});
        window.circuit->setCircuitName("My configuration");
        const auto settings = window.settingsDialog.write();
        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        const auto path = directory.filePath("test.tsc");
        QVERIFY(window.saveToFile(path));
        QFile file(path);
        QVERIFY(file.open(QIODevice::ReadOnly));
        const auto root = QJsonDocument::fromJson(file.readAll()).object();
        QCOMPARE(root["formatVersion"].toInt(), 2);
        const auto entry = root["circuits"].toArray().first().toObject();
        window.uiMap.pot1->setPosition(99);
        window.uiMap.pot2->setPosition(0);
        window.settingsDialog.read({{"algorithm", 0}});
        window.circuits[window.currentStack] = entry;
        window.setStack(window.currentStack);
        QCOMPARE(window.circuit->getCircuitName(), QString("My configuration"));
        QCOMPARE(window.uiMap.pot1->getPosition(), 23);
        QCOMPARE(window.uiMap.pot2->getPosition(), 84);
        QCOMPARE(window.settingsDialog.write(), settings);
        window.filename = path;
        window.on_resetButton_clicked();
        QCOMPARE(window.filename, path);
        QCOMPARE(window.uiMap.pot1->getPosition(), 23);
    }

    void longValueText()
    {
        QLabel label; QLineEdit text;
        Resistor resistor(&label, &text, 1);
        const QByteArray value = QString("12K").append(QString(4096, ' ')).toLatin1();
        resistor.setTextValue(value.constData());
        QCOMPARE(resistor.getValue(), 12000);
        QCOMPARE(text.text(), QString("12K"));
    }
};
QTEST_MAIN(ConfigurationTest)
#include "tst_configuration.moc"
