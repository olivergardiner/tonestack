#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <QWidget>
#include <QDial>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

#include "ngspice/sharedspice.h"

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
    Potentiometer(QDial *dial, QLineEdit *valueText, QComboBox *typeSelect, QLabel *label, const char *labelText);

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
    potType type;
    int value;
    int position;

    int getPotTypeIndex(potType type);

    potType getPotType(int index);

    void alterModel();

    char *lowercase(const char *mixed);

    double logFunction(double x, double ym);
};

#define POT_MAX 99

#endif // POTENTIOMETER_H
