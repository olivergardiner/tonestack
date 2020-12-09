#ifndef TONESTACKCALCULATOR_H
#define TONESTACKCALCULATOR_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

#include "ngspice/sharedspice.h"

#include "potentiometer.h"
#include "fender.h"
#include "marshall.h"
#include "vox.h"
#include "marshall18.h"
#include "moonlight.h"

typedef enum e_potFunction {
    POT_BASS,
    POT_MIDDLE,
    POT_TREBLE,
    POT_ALL
} potFunction;

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

    void on_bassPosition_valueChanged(int value);

    void on_midPosition_valueChanged(int value);

    void on_treblePosition_valueChanged(int value);

    void on_bassValue_textEdited(const QString &arg1);

    void on_midValue_textEdited(const QString &arg1);

    void on_trebleValue_textEdited(const QString &arg1);

    void on_bassType_currentIndexChanged(int index);

    void on_midType_currentIndexChanged(int index);

    void on_trebleType_currentIndexChanged(int index);

    void on_stackSelection_currentIndexChanged(int index);

private:
    Ui::ToneStackCalculator *ui;

    QGraphicsScene scene;

    QGraphicsItemGroup *plot = nullptr;

    qreal decade;

    Potentiometer *bass;
    Potentiometer *mid;
    Potentiometer *treble;

    Fender *fender;
    Marshall *marshall;
    Vox *vox;
    Marshall18 *marshall18;
    Moonlight *moonlight;

    void buildFrequencyResponseScene();

    void buildCircuitSelection();

    void createPlot();

    bool waitSimulationEnd();

    void setStack(int stack);
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
