#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

#include "potentiometer.h"
#include "potsweep.h"

#define SWEEP_RESOLUTION 20

enum algorithm {
    ALG_ABC,
    ALG_LOG
};

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog, public PotSweep
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    double getPotFactor(int potType);
    QJsonObject write() const;
    void read(const QJsonObject &json);

    double taperFunction(int type, double sweep);

private slots:
    void on_logFactor_valueChanged(double arg1);

    void on_logAFactor_valueChanged(double arg1);

    void on_rLogFactor_valueChanged(double arg1);

    void on_algorithm_currentIndexChanged(int index);

private:
    Ui::SettingsDialog *ui;

    QList<double> potFactors = { 0.5, 0.12, 0.25, 0.88, 0.5};
    QList<QGraphicsItemGroup *> potPlots = { nullptr, nullptr, nullptr, nullptr, nullptr };
    QList<QColor> potColours = {
        QColor(40, 40, 40, 255),
        QColor(191, 40, 40, 255),
        QColor(40, 191, 40, 255),
        QColor(40, 40, 191, 255),
        QColor(191, 40, 191, 255)
    };
    QGraphicsScene potentiometerResponse;

    int algorithm = ALG_ABC;

    void buildPotentiometerResponseScene();

    void plotPot(int potType);

    double logFunction(double x, double ym);
};

#endif // SETTINGSDIALOG_H
