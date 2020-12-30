#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <QWidget>
#include <QDial>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

#include "resistor.h"
#include "ngspice/sharedspice.h"

#define POT_MAX 99

enum potType {
    POT_LIN,
    POT_LOG,
    POT_LOGA,
    POT_RLOG,
    POT_STYPE
};

typedef struct pot {
    char label[32];
    int value;
    potType type;
} pot;

class Potentiometer : public Resistor
{
public:
    Potentiometer(QDial *dial, QLineEdit *valueText, QComboBox *typeSelect, QLabel *label, int id);

    potType getType() const;
    void setType(int index);

    int getPosition() const;
    void setPosition(int position);

    void setLabel(QString label);
    void setVisible(bool visible);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    QDial *dial;
    QComboBox *typeSelect;

    QString labelText;
    potType type = POT_LIN;
    int position = POT_MAX / 2;
    int id;

    int getPotTypeIndex(potType type);

    potType getPotType(int index);

    void alterModel();

    char *lowercase(const char *mixed);

    double logFunction(double x, double ym);
};

#endif // POTENTIOMETER_H
