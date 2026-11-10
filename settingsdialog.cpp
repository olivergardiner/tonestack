#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->logFactor->setValue(potFactors.at(POT_LOG));
    ui->logAFactor->setValue(potFactors.at(POT_LOGA));
    ui->rLogFactor->setValue(potFactors.at(POT_RLOG));

    ui->algorithm->addItem("a(b^x) + c", 0);
    ui->algorithm->addItem("(a^x) / a", 1);

    buildPotentiometerResponseScene();
    plotPot(POT_LOG);
    plotPot(POT_LOGA);
    plotPot(POT_RLOG);

    ui->potentiometerResponse->setScene(&potentiometerResponse);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

double SettingsDialog::getPotFactor(int potType)
{
    if (potType < 0 || potType > POT_RLOG) {
        return 0.49; // Out of array bounds - return a linear taper
    }

    return potFactors.at(potType);
}

void SettingsDialog::buildPotentiometerResponseScene()
{
    potentiometerResponse.addLine(0, 0, 0, 250); // Y axis
    potentiometerResponse.addLine(0, 250, 250, 250); // X axis

    QPen dbPen;
    dbPen.setColor(QColor(0, 127, 127, 255));

    for (int i=0; i < 11; i++) {
        qreal offset = i * 25;
        potentiometerResponse.addLine(0, 250 - offset, -10, 250 - offset, dbPen);
        potentiometerResponse.addLine(offset, 250, offset, 260, dbPen);
    }
}

void SettingsDialog::plotPot(int potType)
{
    if (potPlots.at(potType)) { // Remove the previous plot, if it exists
        delete potPlots.at(potType);
    }

    QPen plotPen;
    plotPen.setColor(potColours.at(potType));

    QList<QGraphicsItem *> segments;
    qreal x = 0.0;
    qreal y = 250.0;
    for (int i = 1; i <= SWEEP_RESOLUTION; i++) {
        qreal sweep = ((qreal) i) / SWEEP_RESOLUTION;
        qreal newX = sweep * 250;
        qreal newY = 250 * (1 - taperFunction(potType, sweep));
        segments.append(potentiometerResponse.addLine(x, y, newX, newY, plotPen));

        x = newX;
        y = newY;
    }

    potPlots.replace(potType, potentiometerResponse.createItemGroup(segments));

    if (potType == POT_LOG) {
        plotPot(POT_STYPE); // The S taper borrows the Log function
    }
}

double SettingsDialog::logFunction(double x, double ym)
{
    // From: https://electronics.stackexchange.com/questions/304692/formula-for-logarithmic-audio-taper-pot
    // y = a.b^x - a
    // b = ((1 / ym) - 1)^2
    // a = 1 / (b - 1)
    if (ym > 0.499 && ym < 0.501) { // Linear curve but equation then blows up as a becomes infinite so trap and return explicit linear
        return x;
    }

    if (ym < 0.01) {
        ym = 0.001;
    }

    double b = ((1.0 / ym) - 1.0);
    b = b * b;

    double a = 1 / (b - 1.0);

    return a * pow(b, x) - a;
}

double SettingsDialog::taperFunction(int type, double sweep) {
    double factor;

    switch (type) {
    default:
    case POT_LIN: // Linear
        factor = sweep;
        break;
    case POT_LOG: // Log
        if (algorithm == ALG_LOG) {
            factor = pow(99.0, sweep) / 99.0;
        } else {
            factor = logFunction(sweep, potFactors.at(type));
        }
        break;
    case POT_LOGA: // Log A
        if (algorithm == ALG_LOG) {
            factor = pow(99.0, sqrt(sweep)) / 99.0;
        } else {
           factor = logFunction(sweep, potFactors.at(type));
        }
        break;
    case POT_RLOG: // Reverse Log
        if (algorithm == ALG_LOG) {
            factor = 1.0 - (pow(99.0, 1.0 - sweep) / 99.0);
        } else {
            factor = logFunction(sweep, potFactors.at(type));
        }
        break;
    case POT_STYPE: // Approximated as a log function followed by a an r-log function, scaled
        double scaledSweep = sweep * 2.0;
        if (sweep < 0.5) {
            factor = taperFunction(POT_LOG, scaledSweep) * 0.5;
        } else {
            factor = (1.0 - taperFunction(POT_LOG, 2.0 - scaledSweep)) * 0.5;
            factor += 0.5;
        }
        break;
    }

    return factor;
}

void SettingsDialog::on_logFactor_valueChanged(double arg1)
{
    potFactors.replace(POT_LOG, arg1);
    plotPot(POT_LOG);
}

void SettingsDialog::on_logAFactor_valueChanged(double arg1)
{
    potFactors.replace(POT_LOGA, arg1);
    plotPot(POT_LOGA);
}

void SettingsDialog::on_rLogFactor_valueChanged(double arg1)
{
    potFactors.replace(POT_RLOG, arg1);
    plotPot(POT_RLOG);
}

void SettingsDialog::on_algorithm_currentIndexChanged(int index)
{
    algorithm = index;

    if (index == ALG_ABC) {
        ui->groupBox->setEnabled(true);
    } else {
        ui->groupBox->setDisabled(true);
    }

    plotPot(POT_LOG);
    plotPot(POT_LOGA);
    plotPot(POT_RLOG);
}

QJsonObject SettingsDialog::write() const
{
    return {{"algorithm", algorithm},
            {"log", potFactors.at(POT_LOG)},
            {"logA", potFactors.at(POT_LOGA)},
            {"reverseLog", potFactors.at(POT_RLOG)}};
}

void SettingsDialog::read(const QJsonObject &json)
{
    // Legacy catalogues retain the user's current taper settings.
    if (json.isEmpty())
        return;
    const auto factor = [&json](const char *key, double fallback) {
        const double value = json.value(key).toDouble(fallback);
        return qBound(0.0, value, 1.0);
    };
    ui->logFactor->setValue(factor("log", 0.12));
    ui->logAFactor->setValue(factor("logA", 0.25));
    ui->rLogFactor->setValue(factor("reverseLog", 0.88));
    ui->algorithm->setCurrentIndex(qBound(0, json.value("algorithm").toInt(), 1));
}
