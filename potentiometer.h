#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <QWidget>
#include <QDial>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

#include "resistor.h"
#include "ngspice/sharedspice.h"
#include "potsweep.h"

#define POT_MAX 99

typedef struct pot {
    char label[32];
    int value;
    potType type;
    bool ganged;
} pot;

class Potentiometer : public Resistor
{
public:
    Potentiometer(QDial *dial, QLineEdit *valueText, QComboBox *typeSelect, QLabel *label, int id);

    potType getType() const;
    void setType(int index);

    int getPosition() const;
    void setPosition(int position);

    double getPotFactor(int potType);
    void setPotFactor(int potType, double factor);
    void setPotFactor(int potType, QList<double> factors);

    void setLabel(QString label);
    void setVisible(bool visible);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool isGanged() const;

    void setSweeper(PotSweep *value);

private:
    PotSweep *sweeper = nullptr;
    QDial *dial;
    QComboBox *typeSelect;

    QString labelText;
    potType type = POT_LIN;
    int position = POT_MAX / 2;
    bool ganged;
    int id;

    double potFactor[POT_STYPE + 1] = { 0.5, 0.12, 0.25, 0.88, 0.5};

    int getPotTypeIndex(potType type);

    potType getPotType(int index);

    void alterModel();

    void alterPot(int rlo, int rhi, bool gang);

    char *lowercase(const char *mixed);

    double logFunction(double x, double ym);
};

#endif // POTENTIOMETER_H
