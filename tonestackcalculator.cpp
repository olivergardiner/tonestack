#include "tonestackcalculator.h"
#include "assetpaths.h"
#include <ui_tonestackcalculator.h>
#include <QSignalBlocker>
#include <QSaveFile>
#include <QStatusBar>
#include <QInputDialog>
#include <QTreeWidget>
#include <QHeaderView>
#include <algorithm>
#include <QStyledItemDelegate>

class ComparisonNameDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index) const override
    {
        return index.column()==0 ? QStyledItemDelegate::createEditor(parent,option,index) : nullptr;
    }
};

int ranges = 3;
int rangeStart[3] = {0, 20, 40};

ToneStackCalculator::ToneStackCalculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ToneStackCalculator)
{
    connect(&simulation, &SimulationController::ready, this, &ToneStackCalculator::renderPlot);
    connect(&simulation, &SimulationController::failed, this, [this](const QString &error) {
        statusBar()->showMessage(tr("Simulation failed: ") + error);
    });
    connect(&simulation, &SimulationController::busyChanged, this, [this](bool busy) {
        if (busy) {
            statusBar()->showMessage(tr("Simulating..."));
            ui->pushButton->setEnabled(false);
        }
    });

    readConfig(tr("tonestacks.tsc"));

    filename = tr("");

    plotColour = QColor(191, 40, 40, 255);

    ui->setupUi(this);
    menuBar()->addMenu(tr("View"))->addAction(ui->comparisonsDock->toggleViewAction());
    ui->comparisonsList->setItemDelegate(new ComparisonNameDelegate(ui->comparisonsList));
    ui->comparisonsList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(ui->comparisonsList, &QTreeWidget::itemChanged, this, [this](QTreeWidgetItem *item,int column) {
        const int index=ui->comparisonsList->indexOfTopLevelItem(item);
        if (index<0 || index>=comparisons.size()) return;
        if (column==0) {
            const auto name=item->text(0).trimmed();
            if (!name.isEmpty()) comparisons[index].name=name;
            else {
                const QSignalBlocker blocker(ui->comparisonsList);
                item->setText(0,comparisons[index].name);
            }
            comparisons[index].visible=item->checkState(0)==Qt::Checked;
            if (comparisons[index].trace) comparisons[index].trace->setVisible(comparisons[index].visible);
        }
    });
    connect(ui->comparisonsList, &QTreeWidget::itemSelectionChanged,this,&ToneStackCalculator::updateComparisonButtons);
    connect(ui->restoreComparison,&QPushButton::clicked,this,[this] {
        restoreComparisonAt(ui->comparisonsList->indexOfTopLevelItem(ui->comparisonsList->currentItem()));
    });
    connect(ui->deleteComparison,&QPushButton::clicked,this,[this] {
        deleteComparisonAt(ui->comparisonsList->indexOfTopLevelItem(ui->comparisonsList->currentItem()));
    });
    connect(ui->colourComparison,&QPushButton::clicked,this,[this] {
        const int index=ui->comparisonsList->indexOfTopLevelItem(ui->comparisonsList->currentItem());
        if(index<0 || index>=comparisons.size()) return;
        const auto colour=QColorDialog::getColor(comparisons[index].colour,this,tr("Comparison colour"));
        if(!colour.isValid()) return;
        comparisons[index].colour=colour;
        ui->comparisonsList->topLevelItem(index)->setForeground(0,colour);
        QPixmap swatch(12,12); swatch.fill(colour); ui->comparisonsList->topLevelItem(index)->setIcon(0,QIcon(swatch));
        redrawComparisons();
    });
    updateComparisonButtons();

    uiMap.pot1 = new Potentiometer(ui->pot1Position, ui->pot1Value, ui->pot1Type, ui->pot1Label, 1);
    uiMap.pot2 = new Potentiometer(ui->pot2Position, ui->pot2Value, ui->pot2Type, ui->pot2Label, 2);
    uiMap.pot3 = new Potentiometer(ui->pot3Position, ui->pot3Value, ui->pot3Type, ui->pot3Label, 3);
    uiMap.pot4 = nullptr; // Not yet supported by the UI

    uiMap.pot1->setSweeper(&settingsDialog);
    uiMap.pot2->setSweeper(&settingsDialog);
    uiMap.pot3->setSweeper(&settingsDialog);

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

    helpBrowser.setHelp("/help/tonestack.qhc", "whitecottage.tonestack.help/doc/index.html");
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

    currentRange = qBound(0,r,2);
    if(plot && !currentResponse.isEmpty()) { delete plot; plot=drawTrace(currentResponse,plotColour); }
    redrawComparisons();
}

void ToneStackCalculator::buildCircuitSelection()
{
    const QSignalBlocker blocker(ui->stackSelection);
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
    if (!circuits.isEmpty()) { setStack(0); createPlot(); }
}

void ToneStackCalculator::setStack(int stack)
{
    if (stack < 0 || stack >= circuits.size()) return;
    const QSignalBlocker b1(ui->pot1Position), b2(ui->pot2Position), b3(ui->pot3Position);
    const QSignalBlocker b4(ui->pot1Type), b5(ui->pot2Type), b6(ui->pot3Type);
    QJsonValue currentCircuit = circuits.at(stack);
    if (currentCircuit.isObject()) {
        const auto object = currentCircuit.toObject();
        settingsDialog.read(object.value("taperSettings").toObject());
        circuit->read(object);
    }

    showRange(circuit->getRange());
    currentStack = stack;
    filename = tr("");
    ui->actionSave->setEnabled(false);
}

void ToneStackCalculator::createPlot()
{
    QJsonObject root;
    circuit->write(root);
    requestedConfiguration=root["circuits"].toArray().first().toObject();
    requestedConfiguration["taperSettings"]=settingsDialog.write();
    responseCurrent=false;
    simulation.request();
}

QGraphicsItemGroup *ToneStackCalculator::drawTrace(const QVector<double> &values,const QColor &colour)
{
    QList<QGraphicsItem*> segments;
    QPen pen(colour);
    const double offset=10.0*rangeStart[currentRange];
    const double interval=decade/40.0;
    for(int i=1;i<values.size();++i) {
        const double y1=-200*log10(values[i-1]+1e-9)+offset;
        const double y2=-200*log10(values[i]+1e-9)+offset;
        const double dy=y2-y1;
        double start=0,end=1;
        if(std::abs(dy)<1e-12) {
            if(y1<0 || y1>400) continue;
        } else {
            double a=(0-y1)/dy,b=(400-y1)/dy;
            if(a>b) std::swap(a,b);
            start=std::max(0.0,a); end=std::min(1.0,b);
            if(start>end) continue;
        }
        segments.append(scene.addLine(interval*(i-1+start),y1+start*dy,
                                      interval*(i-1+end),y1+end*dy,pen));
    }
    return scene.createItemGroup(segments);
}
void ToneStackCalculator::renderPlot(const QVector<double> &magnitudes)
{
    if(magnitudes.size()<2) return;
    statusBar()->clearMessage();
    currentResponse=magnitudes;
    responseConfiguration=requestedConfiguration;
    responseCurrent=true;
    ui->pushButton->setEnabled(comparisons.size()<256);
    delete plot;
    plot=drawTrace(currentResponse,plotColour);
}
void ToneStackCalculator::redrawComparisons()
{
    for(auto &comparison:comparisons) {
        delete comparison.trace;
        comparison.trace=drawTrace(comparison.magnitudes,comparison.colour);
        comparison.trace->setVisible(comparison.visible);
    }
}
bool ToneStackCalculator::addComparison(const QString &name)
{
    if(!responseCurrent || currentResponse.size()<2 || name.trimmed().isEmpty() || comparisons.size()>=256) return false;
    Comparison comparison;
    comparison.name=name.trimmed();
    comparison.colour=plotColour;
    comparison.configuration=responseConfiguration;
    comparison.magnitudes=currentResponse;
    comparisons.append(comparison);
    const QSignalBlocker blocker(ui->comparisonsList);
    auto item=new QTreeWidgetItem(ui->comparisonsList);
    item->setFlags(item->flags()|Qt::ItemIsEditable|Qt::ItemIsUserCheckable);
    item->setText(0,comparison.name);
    item->setText(1,comparison.configuration["circuitname"].toString());
    const int captured=qBound(0,comparison.configuration["range"].toInt(),2);
    item->setText(2,QString("%1 to %2 dB").arg(rangeStart[captured]).arg(rangeStart[captured]-40));
    item->setCheckState(0,Qt::Checked);
    item->setForeground(0,comparison.colour);
    QPixmap swatch(12,12); swatch.fill(comparison.colour); item->setIcon(0,QIcon(swatch));
    item->setToolTip(0,tr("Restore settings to edit the circuit that produced this curve."));
    ui->comparisonsList->setCurrentItem(item);
    comparisons.last().trace=drawTrace(comparison.magnitudes,comparison.colour);
    ui->comparisonsDock->show();
    ui->pushButton->setEnabled(comparisons.size()<256);
    updateComparisonButtons();
    return true;
}
void ToneStackCalculator::restoreComparisonAt(int index)
{
    if(index<0 || index>=comparisons.size()) return;
    const auto comparison=comparisons[index];
    // Append a separate editable entry; do not overwrite a catalogue default.
    auto object=comparison.configuration;
    const QSignalBlocker blocker(ui->stackSelection);
    circuits.append(object);
    ui->stackSelection->addItem(object["circuitname"].toString());
    ui->stackSelection->setCurrentIndex(circuits.size()-1);
    setStack(circuits.size()-1);
    plotColour=comparison.colour;
    createPlot();
}
void ToneStackCalculator::deleteComparisonAt(int index)
{
    if(index<0 || index>=comparisons.size()) return;
    delete comparisons[index].trace;
    comparisons.removeAt(index);
    delete ui->comparisonsList->takeTopLevelItem(index);
    ui->pushButton->setEnabled(responseCurrent && comparisons.size()<256);
    updateComparisonButtons();
}
void ToneStackCalculator::updateComparisonButtons()
{
    const bool selected=ui->comparisonsList->currentItem()!=nullptr;
    ui->restoreComparison->setEnabled(selected);
    ui->colourComparison->setEnabled(selected);
    ui->deleteComparison->setEnabled(selected);
}
void ToneStackCalculator::readConfig(QString filename)
{
    QFile configFile(QDir(AssetPaths::circuitsDirectory()).filePath(filename));

    if (!configFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open config file.");
    } else {
        QByteArray configData = configFile.readAll();

        QJsonDocument configDoc(QJsonDocument::fromJson(configData));

        if (configDoc.isObject()) {
            QJsonObject config = configDoc.object();

            if (config.contains("circuits") && config["circuits"].isArray()) {
                circuits = {};
                for (const auto &entry : config["circuits"].toArray())
                    if (entry.isObject())
                        circuits.append(AssetPaths::resolved(entry.toObject(), QFileInfo(configFile).absolutePath()));
            }
        }
    }
}

void ToneStackCalculator::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Tonestack"),
                       tr("Tonestack " TONESTACK_VERSION "\n\n"
                          "Tonestack is a circuit simulation application for analysing passive and active tone stacks and filters\n\n"
                          "Circuit simulation is implemented using NgSpice\n\n"
                          "whitecottage.org.uk/tonestack-calculator/\n\n"
                          "Tonestack is free software and distributed under the GNU General Public Licence v3.0"));
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
    uiMap.pot1->setTextValue(ui->pot1Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_pot2Value_editingFinished()
{
    uiMap.pot2->setTextValue(ui->pot2Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_pot3Value_editingFinished()
{
    uiMap.pot3->setTextValue(ui->pot3Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_r1Value_editingFinished()
{
    uiMap.resistors[0]->setTextValue(ui->r1Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_r2Value_editingFinished()
{
    uiMap.resistors[1]->setTextValue(ui->r2Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_r3Value_editingFinished()
{
    uiMap.resistors[2]->setTextValue(ui->r3Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_r4Value_editingFinished()
{
    uiMap.resistors[3]->setTextValue(ui->r4Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_r5Value_editingFinished()
{
    uiMap.resistors[4]->setTextValue(ui->r5Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_c1Value_editingFinished()
{
    uiMap.capacitors[0]->setTextValue(ui->c1Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_c2Value_editingFinished()
{
    uiMap.capacitors[1]->setTextValue(ui->c2Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_c3Value_editingFinished()
{
    uiMap.capacitors[2]->setTextValue(ui->c3Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_c4Value_editingFinished()
{
    uiMap.capacitors[3]->setTextValue(ui->c4Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_c5Value_editingFinished()
{
    uiMap.capacitors[4]->setTextValue(ui->c5Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_l1Value_editingFinished()
{
    uiMap.inductors[0]->setTextValue(ui->l1Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_l2Value_editingFinished()
{
    uiMap.inductors[1]->setTextValue(ui->l2Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_l3Value_editingFinished()
{
    uiMap.inductors[2]->setTextValue(ui->l3Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_l4Value_editingFinished()
{
    uiMap.inductors[3]->setTextValue(ui->l4Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_l5Value_editingFinished()
{
    uiMap.inductors[4]->setTextValue(ui->l5Value->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_rSValue_editingFinished()
{
    uiMap.rS->setTextValue(ui->rSValue->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_rLValue_editingFinished()
{
    uiMap.rL->setTextValue(ui->rLValue->text().toLatin1().constData());

    createPlot();
}

void ToneStackCalculator::on_centrePotsButton_clicked()
{
    QJsonObject root;
    circuit->write(root);
    const auto configuration=root["circuits"].toArray().first().toObject();
    const QSignalBlocker b1(ui->pot1Position),b2(ui->pot2Position),b3(ui->pot3Position);
    Potentiometer *pots[]={uiMap.pot1,uiMap.pot2,uiMap.pot3};
    for(int i=0;i<3;++i)
        if(configuration.contains(QString("pot%1").arg(i+1)))
            pots[i]->setPosition(POT_MAX/2);
    createPlot();
}

void ToneStackCalculator::on_resetButton_clicked()
{
    const QString savedFilename = filename;
    setStack(currentStack);
    filename = savedFilename;
    ui->actionSave->setEnabled(!filename.isEmpty());
    createPlot();
}

void ToneStackCalculator::on_actionOpen_triggered()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open a saved circuit"), QDir::homePath(), tr("Tone Stack files (*.tsc);;All Files (*)"));
    if (path.isEmpty())
        return;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Open circuit"), file.errorString());
        return;
    }
    QJsonParseError error;
    const auto document = QJsonDocument::fromJson(file.readAll(), &error);
    const auto entries = document.object().value("circuits").toArray();
    if (error.error != QJsonParseError::NoError || entries.isEmpty()) {
        QMessageBox::warning(this, tr("Open circuit"), tr("The file does not contain a circuit catalogue."));
        return;
    }
    const int first = circuits.count();
    {
        const QSignalBlocker blocker(ui->stackSelection);
        for (const auto &entry : entries) {
            if (!entry.isObject())
                continue;
            const auto object = AssetPaths::resolved(entry.toObject(), QFileInfo(path).absolutePath());
            circuits.append(object);
            ui->stackSelection->addItem(object.value("circuitname").toString("<Undefined>"));
        }
        if (circuits.count() == first)
            return;
        ui->stackSelection->setCurrentIndex(first);
    }
    setStack(first);
    filename = path;
    ui->actionSave->setEnabled(true);
    createPlot();
}

bool ToneStackCalculator::saveToFile(const QString &path)
{
    QJsonObject root;
    circuit->write(root);
    auto entries = root.value("circuits").toArray();
    auto object = entries.at(0).toObject();
    object["taperSettings"] = settingsDialog.write();
    entries[0] = AssetPaths::forSave(object, QFileInfo(path).absolutePath());
    root["circuits"] = entries;
    root["formatVersion"] = 2;
    QSaveFile file(path);
    const auto data = QJsonDocument(root).toJson();
    if (!file.open(QIODevice::WriteOnly) || file.write(data) != data.size() || !file.commit()) {
        QMessageBox::warning(this, tr("Save circuit"), file.errorString());
        return false;
    }
    circuits[currentStack] = object;
    ui->stackSelection->setItemText(currentStack, circuit->getCircuitName());
    return true;
}

void ToneStackCalculator::on_actionSave_triggered()
{
    if (!filename.isEmpty())
        saveToFile(filename);
}

void ToneStackCalculator::on_actionSave_As_triggered()
{
    NameDialog dialog;
    if (dialog.exec() != QDialog::Accepted)
        return;
    const QString path = QFileDialog::getSaveFileName(this, tr("Save the current circuit values"), QDir::homePath(), tr("Tone Stack files (*.tsc);;All Files (*)"));
    if (path.isEmpty())
        return;
    const QString previousName = circuit->getCircuitName();
    circuit->setCircuitName(dialog.getName());
    if (saveToFile(path)) {
        filename = path;
        ui->actionSave->setEnabled(true);
    } else {
        circuit->setCircuitName(previousName);
    }
}
void ToneStackCalculator::on_pushButton_clicked()
{
    if(!responseCurrent) return;
    bool ok=false;
    const auto name=QInputDialog::getText(this,tr("Add comparison"),tr("Name:"),QLineEdit::Normal,
        tr("%1 — %2").arg(circuit->getCircuitName()).arg(comparisons.size()+1),&ok);
    if(ok) addComparison(name);
}
void ToneStackCalculator::on_pushButton_2_clicked()
{
    const auto colour=QColorDialog::getColor(plotColour,this,tr("Select plot colour..."));
    if(!colour.isValid()) return;
    plotColour=colour;
    if(!currentResponse.isEmpty()) {
        delete plot; plot=drawTrace(currentResponse,plotColour);
    }
}
void ToneStackCalculator::on_pushButton_3_clicked()
{
    while(!comparisons.isEmpty()) deleteComparisonAt(comparisons.size()-1);
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

void ToneStackCalculator::on_actionSettings_triggered()
{
    const auto previous=settingsDialog.write();
    if(settingsDialog.exec()==QDialog::Accepted) {
        // Apply the new taper to every pot without reparsing rounded UI text.
        QJsonObject root;
        circuit->write(root);
        const auto configuration=root["circuits"].toArray().first().toObject();
        Potentiometer *pots[]={uiMap.pot1,uiMap.pot2,uiMap.pot3};
        for(int i=0;i<3;++i)
            if(configuration.contains(QString("pot%1").arg(i+1)))
                pots[i]->setPosition(pots[i]->getPosition());
        createPlot();
    } else {
        settingsDialog.read(previous);
    }
}
void ToneStackCalculator::on_actionTonestack_Help_triggered()
{
    helpBrowser.show();
}
