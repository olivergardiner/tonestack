#include "resistor.h"

Resistor::Resistor(QLabel *label, QLineEdit *valueText, int id) : CircuitElement(label, valueText, id)
{
    mega = 'M';
    kilo = 'K';
    unit = 'R';
}

void Resistor::setValue(int value)
{
    this->value = value;

    char valueString[32] = "1";
    double scaledValue;

    if (value >= 1000000) {
        scaledValue = ((double) value) / 1000000;
        sprintf(valueString, "%sM", formatNumber(scaledValue).constData());

    } else if (value >= 1000) {
        scaledValue = ((double) value) / 1000;
        sprintf(valueString, "%sK", formatNumber(scaledValue).constData());
    } else {
        sprintf(valueString, "%dR", value);
    }

    valueText->setText(valueString);

    alterModel();
}

void Resistor::alterModel()
{
    char alterCmd[256];
    sprintf(alterCmd, "alter r%d=%d", id, value);

    spice(alterCmd);
}
