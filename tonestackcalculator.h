#ifndef TONESTACKCALCULATOR_H
#define TONESTACKCALCULATOR_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QFileDialog>
#include <QColorDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPrinter>
#include <QPrintDialog>

#include "ngspice/sharedspice.h"

#include "potentiometer.h"
#include "resistor.h"
#include "capacitor.h"
#include "inductor.h"
#include "circuit.h"
#include "namedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ToneStackCalculator; }
QT_END_NAMESPACE

class ToneStackCalculator : public QMainWindow
{
    Q_OBJECT

public:
    ToneStackCalculator(QWidget *parent = nullptr);
    ~ToneStackCalculator();

private slots:
    void on_actionAbout_triggered();

    void on_actionQuit_triggered();

    void on_stackSelection_currentIndexChanged(int index);

    void on_pot1Position_valueChanged(int value);

    void on_pot2Position_valueChanged(int value);

    void on_pot3Position_valueChanged(int value);

    void on_pot1Type_currentIndexChanged(int index);

    void on_pot2Type_currentIndexChanged(int index);

    void on_pot3Type_currentIndexChanged(int index);

    void on_r1Value_editingFinished();

    void on_r2Value_editingFinished();

    void on_r3Value_editingFinished();

    void on_r4Value_editingFinished();

    void on_r5Value_editingFinished();

    void on_c1Value_editingFinished();

    void on_c2Value_editingFinished();

    void on_c3Value_editingFinished();

    void on_c4Value_editingFinished();

    void on_c5Value_editingFinished();

    void on_l1Value_editingFinished();

    void on_l2Value_editingFinished();

    void on_l3Value_editingFinished();

    void on_l4Value_editingFinished();

    void on_l5Value_editingFinished();

    void on_pot1Value_editingFinished();

    void on_pot2Value_editingFinished();

    void on_pot3Value_editingFinished();

    void on_rSValue_editingFinished();

    void on_rLValue_editingFinished();

    void on_resetButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_actionPrint_triggered();

    void on_actionTone_Stack_Mode_triggered();

    void on_actionBasic_Electronics_Mode_triggered();

    void on_actionActive_Circuits_Mode_triggered();

private:
    Ui::ToneStackCalculator *ui;

    QGraphicsScene scene;
    QGraphicsScene circuitScene;

    QColor plotColour;
    QGraphicsItemGroup *plot = nullptr;
    QGraphicsItemGroup *savedPlot[256];
    int savedPlots = 0;

    QGraphicsItemGroup *range[8];

    int currentRange = 0;

    QImage *circuitImage = nullptr;

    qreal decade;

    circuitUiMap uiMap;

    QJsonArray circuits;
    int currentStack;

    Circuit *circuit;

    QString filename;

    void buildFrequencyResponseScene();

    void buildCircuitSelection();

    void createPlot();

    QGraphicsItemGroup *createRange(int start);

    void showRange(int r);

    bool waitSimulationEnd();

    void setStack(int stack);

    char *toString(QString source);

    void readConfig(QString configFile);
};

int ng_getchar(char* outputreturn, int ident, void* userdata);

int ng_getstat(char* outputreturn, int ident, void* userdata);

int ng_exit(int, bool, bool, int ident, void*);

int ng_thread_runs(bool noruns, int ident, void* userdata);

int ng_initdata(pvecinfoall intdata, int ident, void* userdata);

int ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata);

int cieq(char* p, char* s);

int ciprefix(const char* p, const char* s);

#endif // TONESTACKCALCULATOR_H
