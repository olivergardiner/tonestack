#include "inductor.h"

Inductor::Inductor(QLabel *label, QLineEdit *valueText, int id) : CircuitElement(label, valueText, id)
{
    mega = 'H';
    kilo = 'M';
    unit = 'U';
}

void Inductor::setValue(int value)
{
    this->value = value;

    char valueString[32] = "1";
    double scaledValue;

    if (value >= 1000000) {
        scaledValue = ((double) value) / 1000000;
        sprintf(valueString, "%sH", formatNumber(scaledValue).constData());
    } else if (value >= 1000) {
        scaledValue = ((double) value) / 1000;
        sprintf(valueString, "%smH", formatNumber(scaledValue).constData());
    } else {
        sprintf(valueString, "%duH", value);
    }

    valueText->setText(valueString);

    alterModel();
}

void Inductor::alterModel()
{
    char alterCmd[256];
    sprintf(alterCmd, "alter l%d=%dU", id, value);

    spice(alterCmd);
}
