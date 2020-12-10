#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <QWidget>
#include <QDial>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

#include "ngspice/sharedspice.h"

#define POT_MAX 99

typedef enum e_potType {
    POT_LIN,
    POT_LOG,
    POT_LOGA,
    POT_RLOG,
    POT_STYPE
} potType;

class Potentiometer
{
public:
    Potentiometer(QDial *dial, QLineEdit *valueText, QComboBox *typeSelect, QLabel *label, int id);

    potType getType() const;
    void setType(const potType &type);
    void setType(int index);

    int getValue() const;
    void setValue(int value);

    int getPosition() const;
    void setPosition(int position);

    void setPotentiometer(potType type, int value, int position, const char *label);

    void setVisible(bool visible);

private:
    QDial *dial;
    QLineEdit *valueText;
    QComboBox *typeSelect;
    QLabel *label;

    const char *labelText;
    potType type = POT_LIN;
    int value = 1000000;
    int position = POT_MAX / 2;
    int id;

    int getPotTypeIndex(potType type);

    potType getPotType(int index);

    void alterModel();

    char *lowercase(const char *mixed);

    double logFunction(double x, double ym);
};

#endif // POTENTIOMETER_H
