#include "capacitor.h"

Capacitor::Capacitor(QLabel *label, QLineEdit *valueText, int id) : CircuitElement(label, valueText, id)
{
    mega = 'U';
    kilo = 'N';
    unit = 'P';
}

void Capacitor::setValue(int value)
{
    this->value = value;

    char valueString[32] = "1";
    double scaledValue;

    if (value >= 1000000) {
        scaledValue = ((double) value) / 1000000;
        sprintf(valueString, "%suF", formatNumber(scaledValue).constData());
    } else if (value >= 1000) {
        scaledValue = ((double) value) / 1000;
        sprintf(valueString, "%snF", formatNumber(scaledValue).constData());
    } else {
        sprintf(valueString, "%dpF", value);
    }

    valueText->setText(valueString);

    alterModel();
}

void Capacitor::alterModel()
{
    char alterCmd[256];
    sprintf(alterCmd, "alter c%d=%dP", id, value);

    spice(alterCmd);
}
