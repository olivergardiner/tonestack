#include "tonestackcalculator.h"
#include "ui_tonestackcalculator.h"
#include <tchar.h>
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <windows.h>
#endif

bool no_bg = true;
int vecgetnumber = 0;
double v2dat;
bool has_break = false;
bool errorflag = false;

#define spice(X) (ngSpice_Command(const_cast <char *>(X)))
#define magnitude(X) (sqrt(X.cx_real * X.cx_real + X.cx_imag * X.cx_imag))

int ranges = 2;
int rangeStart[2] = {0, 20};

ToneStackCalculator::ToneStackCalculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ToneStackCalculator)
{
    ngSpice_Init(ng_getchar, ng_getstat, ng_exit, ng_data, ng_initdata, ng_thread_runs, NULL);

    readConfig(tr("tonestacks.tsc"));

    filename = tr("");

    plotColour = QColor(191, 40, 40, 255);

    ui->setupUi(this);

    uiMap.pot1 = new Potentiometer(ui->pot1Position, ui->pot1Value, ui->pot1Type, ui->pot1Label, 1);
    uiMap.pot2 = new Potentiometer(ui->pot2Position, ui->pot2Value, ui->pot2Type, ui->pot2Label, 2);
    uiMap.pot3 = new Potentiometer(ui->pot3Position, ui->pot3Value, ui->pot3Type, ui->pot3Label, 3);
    uiMap.pot4 = nullptr; // Not yet supported by the UI

    uiMap.resistors[0] = new Resistor(ui->r1Label, ui->r1Value, 1);
    uiMap.resistors[1] = new Resistor(ui->r2Label, ui->r2Value, 2);
    uiMap.resistors[2] = new Resistor(ui->r3Label, ui->r3Value, 3);
    uiMap.resistors[3] = new Resistor(ui->r4Label, ui->r4Value, 4);
    uiMap.resistors[4] = new Resistor(ui->r5Label, ui->r5Value, 5);

    uiMap.capacitors[0] = new Capacitor(ui->c1Label, ui->c1Value, 1);
    uiMap.capacitors[1] = new Capacitor(ui->c2Label, ui->c2Value, 2);
    uiMap.capacitors[2] = new Capacitor(ui->c3Label, ui->c3Value, 3);
    uiMap.capacitors[3] = new Capacitor(ui->c4Label, ui->c4Value, 4);
    uiMap.capacitors[4] = new Capacitor(ui->c5Label, ui->c5Value, 5);

    uiMap.inductors[0] = new Inductor(ui->l1Label, ui->l1Value, 1);
    uiMap.inductors[1] = new Inductor(ui->l2Label, ui->l2Value, 2);
    uiMap.inductors[2] = new Inductor(ui->l3Label, ui->l3Value, 3);
    uiMap.inductors[3] = new Inductor(ui->l4Label, ui->l4Value, 4);
    uiMap.inductors[4] = new Inductor(ui->l5Label, ui->l5Value, 5);

    uiMap.rS = new Resistor(ui->rSLabel, ui->rSValue, 98);
    uiMap.rL = new Resistor(ui->rLLabel, ui->rLValue, 99);

    uiMap.circuitDisplay = ui->circuitDisplay;
    uiMap.stackSelection = ui->stackSelection;

    circuit = new Circuit(&uiMap);

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

    for (int r=0; r < ranges; r++) {
        range[r] = createRange(rangeStart[r]);

        if (r > 0) {
            range[r]->setVisible(false);
        }
    }

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

QGraphicsItemGroup *ToneStackCalculator::createRange(int start)
{
    QList<QGraphicsItem *> labels;

    for (int i=0; i < 5; i++) {
        QGraphicsTextItem *text;
        char labelText[16];
        sprintf(labelText, "%s%ddB", (start > 0) ? "+" : "", start);
        text = scene.addText(labelText);
        double x = (start == 0) ? -40.0 : -50.0;
        text->setPos(x, 100.0 * i - 10);
        labels.append(text);

        start -= 10;
    }

    return scene.createItemGroup(labels);
}

void ToneStackCalculator::showRange(int r)
{
    for (int i=0; i < ranges; i++) {
        range[i]->setVisible(i == r);
    }

    currentRange = r;
}

void ToneStackCalculator::buildCircuitSelection()
{
    ui->stackSelection->clear();

    for (int i=0; i < circuits.count(); i++) {
        QJsonValue currentCircuit = circuits.at(i);
        if (currentCircuit.isObject()) {
            QJsonObject circuitObject = currentCircuit.toObject();
            if (circuitObject.contains("circuitname") && circuitObject["circuitname"].isString()) {
                ui->stackSelection->addItem(circuitObject["circuitname"].toString());
            } else {
                ui->stackSelection->addItem("<Undefined>");
            }
        }
    }

    ui->stackSelection->setCurrentIndex(0);
}

void ToneStackCalculator::setStack(int stack)
{
    if (!waitSimulationEnd()) {
        spice("bg_halt");
        spice("reset");
    }

    //circuit->setCurrentCircuit(stack);
    QJsonValue currentCircuit = circuits.at(stack);
    if (currentCircuit.isObject()) {
        circuit->read(currentCircuit.toObject());
    }

    showRange(circuit->getRange());
    currentStack = stack;
    filename = tr("");
    ui->actionSave->setEnabled(false);
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
    plotPen.setColor(plotColour);

    qreal offset = 10.0 * rangeStart[currentRange];
    qreal interval = decade / 40;
    qreal y = log10(magnitude(myvec->v_compdata[0]) + 1e-9) * -200 + offset;
    if (y < 0) {
        y = 0;
    } else if (y > 400) {
        y = 400;
    }

    for (int i=1;i < veclength; i++) {
        qreal newy = log10(magnitude(myvec->v_compdata[i]) + 1e-9) * -200 + offset;
        if (newy < 0) {
            newy = 0;
        }

        if (newy < 400) {
           segments.append(scene.addLine(interval * (i-1), y, interval * i, newy, plotPen));
        }
        y = newy;
    }

    plot = scene.createItemGroup(segments);
}

char *ToneStackCalculator::toString(QString source)
{
    char text[64];

    int i = 0;
    for (i = 0; i < source.length(); i++) {
        text[i] = source.at(i).toLatin1();
    }
    text[i] = 0;

    fprintf(stderr, "Value = %s\n", text);

    return _strdup(text);
}

void ToneStackCalculator::readConfig(QString filename)
{
    QFile configFile(tr("../circuits/").append(filename));

    if (!configFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open config file.");
    } else {
        QByteArray configData = configFile.readAll();

        QJsonDocument configDoc(QJsonDocument::fromJson(configData));

        if (configDoc.isObject()) {
            QJsonObject config = configDoc.object();

            if (config.contains("circuits") && config["circuits"].isArray()) {
                circuits = config["circuits"].toArray();
            }
        }
    }
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
    uiMap.pot1->setPosition(value);

    createPlot();
}

void ToneStackCalculator::on_pot2Position_valueChanged(int value)
{
    uiMap.pot2->setPosition(value);

    createPlot();
}

void ToneStackCalculator::on_pot3Position_valueChanged(int value)
{
    uiMap.pot3->setPosition(value);

    createPlot();
}

void ToneStackCalculator::on_pot1Type_currentIndexChanged(int index)
{
    uiMap.pot1->setType(index);

    createPlot();
}

void ToneStackCalculator::on_pot2Type_currentIndexChanged(int index)
{
    uiMap.pot2->setType(index);

    createPlot();
}

void ToneStackCalculator::on_pot3Type_currentIndexChanged(int index)
{
    uiMap.pot3->setType(index);

    createPlot();
}

void ToneStackCalculator::on_pot1Value_editingFinished()
{
    uiMap.pot1->setTextValue(toString(ui->pot1Value->text()));

    createPlot();
}

void ToneStackCalculator::on_pot2Value_editingFinished()
{
    uiMap.pot2->setTextValue(toString(ui->pot2Value->text()));

    createPlot();
}

void ToneStackCalculator::on_pot3Value_editingFinished()
{
    uiMap.pot3->setTextValue(toString(ui->pot3Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r1Value_editingFinished()
{
    uiMap.resistors[0]->setTextValue(toString(ui->r1Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r2Value_editingFinished()
{
    uiMap.resistors[1]->setTextValue(toString(ui->r2Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r3Value_editingFinished()
{
    uiMap.resistors[2]->setTextValue(toString(ui->r3Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r4Value_editingFinished()
{
    uiMap.resistors[3]->setTextValue(toString(ui->r4Value->text()));

    createPlot();
}

void ToneStackCalculator::on_r5Value_editingFinished()
{
    uiMap.resistors[4]->setTextValue(toString(ui->r5Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c1Value_editingFinished()
{
    uiMap.capacitors[0]->setTextValue(toString(ui->c1Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c2Value_editingFinished()
{
    uiMap.capacitors[1]->setTextValue(toString(ui->c2Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c3Value_editingFinished()
{
    uiMap.capacitors[2]->setTextValue(toString(ui->c3Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c4Value_editingFinished()
{
    uiMap.capacitors[3]->setTextValue(toString(ui->c4Value->text()));

    createPlot();
}

void ToneStackCalculator::on_c5Value_editingFinished()
{
    uiMap.capacitors[4]->setTextValue(toString(ui->c5Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l1Value_editingFinished()
{
    uiMap.inductors[0]->setTextValue(toString(ui->l1Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l2Value_editingFinished()
{
    uiMap.inductors[1]->setTextValue(toString(ui->l2Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l3Value_editingFinished()
{
    uiMap.inductors[2]->setTextValue(toString(ui->l3Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l4Value_editingFinished()
{
    uiMap.inductors[3]->setTextValue(toString(ui->l4Value->text()));

    createPlot();
}

void ToneStackCalculator::on_l5Value_editingFinished()
{
    uiMap.inductors[4]->setTextValue(toString(ui->l5Value->text()));

    createPlot();
}

void ToneStackCalculator::on_rSValue_editingFinished()
{
    uiMap.rS->setTextValue(toString(ui->rSValue->text()));

    createPlot();
}

void ToneStackCalculator::on_rLValue_editingFinished()
{
    uiMap.rL->setTextValue(toString(ui->rLValue->text()));

    createPlot();
}

void ToneStackCalculator::on_resetButton_clicked()
{
    setStack(currentStack);

    createPlot();
}

void ToneStackCalculator::on_actionOpen_triggered()
{
    filename = QFileDialog::getOpenFileName(this, tr("Open a saved circuit"), QDir::homePath(), tr("Tone Stack files (*.tsc);;All Files (*)"));

    if (!filename.isEmpty()) {
        ui->actionSave->setEnabled(true);

        QFile loadFile(filename);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open load file.");
        } else {
            QByteArray saveData = loadFile.readAll();

            QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

            int currentCount = circuits.count();

            if (loadDoc.isObject()) {
                QJsonObject config = loadDoc.object();

                if (config.contains("circuits") && config["circuits"].isArray()) {
                    QJsonArray circuitArray = config["circuits"].toArray();
                    for (int i=0; i < circuitArray.count(); i++) {
                        QJsonValue currentCircuit = circuitArray.at(i);
                        if (currentCircuit.isObject()) {
                            QJsonObject circuitObject = currentCircuit.toObject();
                            circuits.append(circuitObject);
                            if (circuitObject.contains("circuitname") && circuitObject["circuitname"].isString()) {
                                ui->stackSelection->addItem(circuitObject["circuitname"].toString());
                            } else {
                                ui->stackSelection->addItem("<Undefined>");
                            }
                        }
                    }

                    if (circuitArray.count() > 0) {
                        ui->stackSelection->setCurrentIndex(currentCount);
                        setStack(currentCount);
                    }
                }
            }
        }
    }

    createPlot();
}

void ToneStackCalculator::on_actionSave_triggered()
{
    if (!filename.isEmpty()) {
        QFile saveFile(filename);

        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
        } else {
            QJsonObject circuitObject;
            circuit->write(circuitObject);
            saveFile.write(QJsonDocument(circuitObject).toJson());
        }
    }
}

void ToneStackCalculator::on_actionSave_As_triggered()
{
    NameDialog dialog;

    if (dialog.exec() == 1) {
        filename = QFileDialog::getSaveFileName(this, tr("Save the current circuit values"), QDir::homePath(), tr("Tone Stack files (*.tsc);;All Files (*)"));

        if (!filename.isEmpty()) {
            circuit->setCircuitName(dialog.getName());
            circuit->saveCircuit(filename);
        }
    }
}


void ToneStackCalculator::on_pushButton_clicked()
{
    // Snapshot
    if (plot && savedPlots <= 256) {
        savedPlot[savedPlots++] = plot;
        plot = nullptr;
    }
}

void ToneStackCalculator::on_pushButton_2_clicked()
{
    // Colour
    QColor newColour = QColorDialog::getColor(plotColour, this, tr("Select plot colour..."));

    if (newColour.isValid()) {
        plotColour = newColour;

        createPlot();
    }
}

void ToneStackCalculator::on_pushButton_3_clicked()
{
    // Clear
    for (int i = 0; i < savedPlots; i++) {
        scene.removeItem(savedPlot[i]);
    }

    savedPlots = 0;

    createPlot();
}

void ToneStackCalculator::on_actionPrint_triggered()
{
    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Document"));

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QPainter painter;
    painter.begin(&printer);
    painter.setRenderHint(QPainter::Antialiasing);
    scene.render(&painter);
}

void ToneStackCalculator::on_actionTone_Stack_Mode_triggered()
{
    readConfig(tr("tonestacks.tsc"));

    buildCircuitSelection();

    ui->actionTone_Stack_Mode->setChecked(true);
    ui->actionBasic_Electronics_Mode->setChecked(false);
    ui->actionActive_Circuits_Mode->setChecked(false);
}

void ToneStackCalculator::on_actionBasic_Electronics_Mode_triggered()
{
    readConfig(tr("basic/basic.tsc"));

    buildCircuitSelection();

    ui->actionTone_Stack_Mode->setChecked(false);
    ui->actionBasic_Electronics_Mode->setChecked(true);
    ui->actionActive_Circuits_Mode->setChecked(false);
}

void ToneStackCalculator::on_actionActive_Circuits_Mode_triggered()
{
    readConfig(tr("active/active.tsc"));

    buildCircuitSelection();

    ui->actionTone_Stack_Mode->setChecked(false);
    ui->actionBasic_Electronics_Mode->setChecked(false);
    ui->actionActive_Circuits_Mode->setChecked(true);
}
