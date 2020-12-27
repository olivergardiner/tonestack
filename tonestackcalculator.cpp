#include "tonestackcalculator.h"
#include "ui_tonestackcalculator.h"
#include <tchar.h>
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <windows.h>
#endif

#include "circuitdefinitions.h"

bool no_bg = true;
int vecgetnumber = 0;
double v2dat;
bool has_break = false;
bool errorflag = false;

#define spice(X) (ngSpice_Command(const_cast <char *>(X)))
#define magnitude(X) (sqrt(X.cx_real * X.cx_real + X.cx_imag * X.cx_imag))

ToneStackCalculator::ToneStackCalculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ToneStackCalculator)
{
    ngSpice_Init(ng_getchar, ng_getstat, ng_exit, ng_data, ng_initdata, ng_thread_runs, NULL);

    ui->setupUi(this);

    pot1 = new Potentiometer(ui->pot1Position, ui->pot1Value, ui->pot1Type, ui->pot1Label, 1);
    pot2 = new Potentiometer(ui->pot2Position, ui->pot2Value, ui->pot2Type, ui->pot2Label, 2);
    pot3 = new Potentiometer(ui->pot3Position, ui->pot3Value, ui->pot3Type, ui->pot3Label, 3);
    pot4 = nullptr; // Not yet supported by the UI

    resistors[0] = new Resistor(ui->r1Label, ui->r1Value, 1);
    resistors[1] = new Resistor(ui->r2Label, ui->r2Value, 2);
    resistors[2] = new Resistor(ui->r3Label, ui->r3Value, 3);
    resistors[3] = new Resistor(ui->r4Label, ui->r4Value, 4);
    resistors[4] = new Resistor(ui->r5Label, ui->r5Value, 5);

    capacitors[0] = new Capacitor(ui->c1Label, ui->c1Value, 1);
    capacitors[1] = new Capacitor(ui->c2Label, ui->c2Value, 2);
    capacitors[2] = new Capacitor(ui->c3Label, ui->c3Value, 3);
    capacitors[3] = new Capacitor(ui->c4Label, ui->c4Value, 4);
    capacitors[4] = new Capacitor(ui->c5Label, ui->c5Value, 5);

    inductors[0] = new Inductor(ui->l1Label, ui->l1Value, 1);
    inductors[1] = new Inductor(ui->l2Label, ui->l2Value, 2);
    inductors[2] = new Inductor(ui->l3Label, ui->l3Value, 3);
    inductors[3] = new Inductor(ui->l4Label, ui->l4Value, 4);
    inductors[4] = new Inductor(ui->l5Label, ui->l5Value, 5);

    rS = new Resistor(ui->rSLabel, ui->rSValue, 98);
    rL = new Resistor(ui->rLLabel, ui->rLValue, 99);

    circuit = new Circuit(definitions, pot1, pot2, pot3, pot4, resistors, capacitors, inductors, rS, rL);

    buildFrequencyResponseScene();
    ui->frequencyResponse->setScene(&scene);

    buildCircuitSelection();
}

ToneStackCalculator::~ToneStackCalculator()
{
    delete ui;
}

void ToneStackCalculator::buildFrequencyResponseScene()
{
    scene.addLine(0, 0, 0, 400); // Y axis
    scene.addLine(0, 0, 400, 0); // X axis

    QPen dbPen;
    dbPen.setColor(QColor(0, 127, 127, 255));
    QPen dbDecPen;
    dbDecPen.setColor(QColor(0, 63, 63, 255));

    QGraphicsTextItem *text;
    text = scene.addText("10Hz");
    text->setPos(0.0 - 12, -30);
    text = scene.addText("100Hz");
    text->setPos(100.0 - 15, -30);
    text = scene.addText("1kHz");
    text->setPos(200.0 - 12, -30);
    text = scene.addText("10kHz");
    text->setPos(300.0 - 15, -30);
    text = scene.addText("100kHz");
    text->setPos(400.0 - 20, -30);

    text = scene.addText("0dB");
    text->setPos(-40, 0.0 - 10);
    text = scene.addText("-10dB");
    text->setPos(-50, 100.0 - 10);
    text = scene.addText("-20dB");
    text->setPos(-50, 200.0 - 10);
    text = scene.addText("-30dB");
    text->setPos(-50, 300.0 - 10);
    text = scene.addText("-40dB");
    text->setPos(-50, 400.0 - 10);


    for (int i=1; i < 41; i++) {
        if ((i % 10) == 0) {
            scene.addLine(0, i*10, 400, i*10, dbDecPen);
        } else {
            scene.addLine(0, i*10, 400, i*10, dbPen);
        }
    }

    decade = 400/4;

    QPen freqPen;
    freqPen.setColor(QColor(127, 127, 40, 255));

    for (int i=0; i < 4; i++) {
        for (int j=0; j < 10; j++) {
            qreal x = (log10(1.0 + j) + i) * decade;

            if (x < 400 && x > 0) {
                scene.addLine(x, 0, x, 400, freqPen);
            }
        }
    }
}

void ToneStackCalculator::buildCircuitSelection()
{
    for (int i=0; i < CIR_UNDEFINED; i++) {
        ui->stackSelection->addItem(definitions[i].circuitLabel);
    }

    ui->stackSelection->setCurrentIndex(0);
}

void ToneStackCalculator::setStack(int stack)
{
    if (!waitSimulationEnd()) {
        spice("bg_halt");
        spice("reset");
    }

    circuit->setCurrentCircuit(stack);

    QPixmap circuitImage(circuit->getImage());
    circuitImage = circuitImage.scaled(339, 267, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->circuitDisplay->setPixmap(circuitImage);
}

bool ToneStackCalculator::waitSimulationEnd()
{
    bool completed = false;
    for (int i=0;i < 20;i++) {
#if defined(__MINGW32__) || defined(_MSC_VER)
        Sleep (100);
#else
        usleep (100000);
#endif
        QApplication::processEvents(QEventLoop::AllEvents);

        if (no_bg) {
            completed = true;
            break;
        }
    }

    return (completed);
}

void ToneStackCalculator::createPlot()
{
    spice("bg_run");

    if (!waitSimulationEnd()) {
        // The simulation did not complete inside 2s
        return;
    }

    pvector_info myvec = ngGet_Vec_Info(const_cast<char *>("V(3)"));
    if (!myvec) {
        return;
    }

    int veclength = myvec->v_length;

     if (plot) {
        scene.removeItem(plot);
     }

    QList<QGraphicsItem *> segments;

    QPen plotPen;
    plotPen.setColor(QColor(191, 40, 40, 255));

    qreal interval = decade / 10;
    qreal y = log10(magnitude(myvec->v_compdata[0]) + 1e-9) * -200;

    for (int i=1;i < veclength; i++) {
        qreal newy = log10(magnitude(myvec->v_compdata[i]) + 1e-9) * -200;
        if (newy < 400) {
           segments.append(scene.addLine(interval * (i-1), y, interval * i, newy, plotPen));
        }
        y = newy;
    }

    plot = scene.createItemGroup(segments);
}

/* Callback function called from bg thread in ngspice to transfer
any string created by printf or puts. Output to stdout in ngspice is
preceded by token stdout, same with stderr.*/
int ng_getchar(char* outputreturn, int ident, void* userdata)
{
    printf("%s\n", outputreturn);
    /* setting a flag if an error message occurred */
    if (ciprefix("stderr Error:", outputreturn))
        errorflag = true;
    return 0;
}


int ng_getstat(char* outputreturn, int ident, void* userdata)
{
    printf("%s\n", outputreturn);
    return 0;
}

int ng_thread_runs(bool noruns, int ident, void* userdata)
{
    no_bg = noruns;
    if (noruns)
        printf("bg not running\n");
    else
        printf("bg running\n");

    return 0;
}

/* Callback function called from bg thread in ngspice once per accepted data point */
int ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata)
{
    int* ret;

    v2dat = vdata->vecsa[vecgetnumber]->creal;
    if (!has_break && (v2dat > 0.5)) {
        //        printf("Data V(2) value: %f\n", v2dat);
            /* using signal SIGTERM by sending to main thread, alterp() then is run from the main thread,
              (not on Windows though!)  */
#ifndef _MSC_VER
        if (testnumber == 4)
            pthread_kill(mainthread, SIGTERM);
#endif
        has_break = true;
        printf("Pause requested, setpoint reached\n");
        /* leave bg thread for a while to allow halting it from main */
#if defined(__MINGW32__) || defined(_MSC_VER)
        Sleep(100);
#else
        usleep(100000);
#endif
        //        ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_halt");
    }
    return 0;
}


/* Callback function called from bg thread in ngspice once upon intialization
   of the simulation vectors)*/
int ng_initdata(pvecinfoall intdata, int ident, void* userdata)
{
    int i;
    int vn = intdata->veccount;
    for (i = 0; i < vn; i++) {
        printf("Vector: %s\n", intdata->vecs[i]->vecname);
        /* find the location of V(2) */
        if (cieq(intdata->vecs[i]->vecname, const_cast <char*>("V(2)")))
            vecgetnumber = i;
    }
    return 0;
}


/* Callback function called from bg thread in ngspice if fcn controlled_exit()
   is hit. Do not exit, but unload ngspice. */
int ng_exit(int exitstatus, bool immediate, bool quitexit, int ident, void* userdata)
{
    if (quitexit) {
        printf("DNote: Returned form quit with exit status %d\n", exitstatus);
        exit(exitstatus);
    }

    if (immediate) {
        printf("DNote: Unloading ngspice inmmediately is not possible\n");
        printf("DNote: Can we recover?\n");
    } else {
        printf("DNote: Unloading ngspice is not possible\n");
        printf("DNote: Can we recover? Send 'quit' command to ngspice.\n");
        errorflag = true;
        ngSpice_Command(const_cast <char*>("quit 5"));
        //        raise(SIGINT);
    }

    return exitstatus;
}

/* Case insensitive str eq.
   Like strcasecmp( ) XXX */
int cieq(char* p, char* s)
{
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) !=
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (*s ? false : true);
}

/* Case insensitive prefix. */
int ciprefix(const char* p, const char* s)
{
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) !=
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (true);
}

char *ToneStackCalculator::toString(QString source)
{
    char text[64];

    int i = 0;
    for (i = 0; i < source.length(); i++) {
        text[i] = source.at(i).toLatin1();
    }
    text[i] = 0;

    return _strdup(text);
}

void ToneStackCalculator::on_actionAbout_triggered()
{

}

void ToneStackCalculator::on_actionQuit_triggered()
{
    QCoreApplication::quit();
}

void ToneStackCalculator::on_stackSelection_currentIndexChanged(int index)
{
    setStack(index);

    createPlot();
}

void ToneStackCalculator::on_pot1Position_valueChanged(int value)
{
    pot1->setPosition(value);

    createPlot();
}

void ToneStackCalculator::on_pot2Position_valueChanged(int value)
{
    pot2->setPosition(value);

    createPlot();
}

void ToneStackCalculator::on_pot3Position_valueChanged(int value)
{
    pot3->setPosition(value);

    createPlot();
}

void ToneStackCalculator::on_pot1Type_currentIndexChanged(int index)
{
    pot1->setType(index);

    createPlot();
}

void ToneStackCalculator::on_pot2Type_currentIndexChanged(int index)
{
    pot2->setType(index);

    createPlot();
}

void ToneStackCalculator::on_pot3Type_currentIndexChanged(int index)
{
    pot3->setType(index);

    createPlot();
}

void ToneStackCalculator::on_pot1Value_editingFinished()
{
    pot1->setTextValue(toString(ui->pot1Value->text()));

    createPlot();
}

void ToneStackCalculator::on_pot2Value_editingFinished()
{
    pot2->setTextValue(toString(ui->pot2Value->text()));

    createPlot();
}

void ToneStackCalculator::on_pot3Value_editingFinished()
{
    pot3->setTextValue(toString(ui->pot3Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r1Value_editingFinished()
{
    resistors[0]->setTextValue(toString(ui->r1Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r2Value_editingFinished()
{
    resistors[1]->setTextValue(toString(ui->r2Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r3Value_editingFinished()
{
    resistors[2]->setTextValue(toString(ui->r3Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r4Value_editingFinished()
{
    resistors[3]->setTextValue(toString(ui->r4Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r5Value_editingFinished()
{
    resistors[4]->setTextValue(toString(ui->r5Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c1Value_editingFinished()
{
    capacitors[0]->setTextValue(toString(ui->c1Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c2Value_editingFinished()
{
    capacitors[1]->setTextValue(toString(ui->c2Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c3Value_editingFinished()
{
    capacitors[2]->setTextValue(toString(ui->c3Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c4Value_editingFinished()
{
    capacitors[3]->setTextValue(toString(ui->c4Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c5Value_editingFinished()
{
    capacitors[4]->setTextValue(toString(ui->c5Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l1Value_editingFinished()
{
    inductors[0]->setTextValue(toString(ui->l1Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l2Value_editingFinished()
{
    inductors[1]->setTextValue(toString(ui->l2Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l3Value_editingFinished()
{
    inductors[2]->setTextValue(toString(ui->l3Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l4Value_editingFinished()
{
    inductors[3]->setTextValue(toString(ui->l4Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l5Value_editingFinished()
{
    inductors[4]->setTextValue(toString(ui->l5Value->text()));

    createPlot();
}

void ToneStackCalculator::on_rSValue_editingFinished()
{
    rS->setTextValue(toString(ui->rSValue->text()));

    createPlot();
}

void ToneStackCalculator::on_rLValue_editingFinished()
{
    rL->setTextValue(toString(ui->rLValue->text()));

    createPlot();
}

void ToneStackCalculator::on_resetButton_clicked()
{
    setStack(circuit->getCurrentCircuit());

    createPlot();
}
