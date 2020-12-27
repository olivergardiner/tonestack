#include "potentiometer.h"

Potentiometer::Potentiometer(QDial *dial, QLineEdit *valueText, QComboBox *typeSelect, QLabel *label, int id) : Resistor(label, valueText, id)
{
    this->dial = dial;
    this->typeSelect = typeSelect;
    this->id = id;
}

potType Potentiometer::getType() const
{
    return type;
}

void Potentiometer::setType(const potType &type)
{
    this->type = type;

    typeSelect->setCurrentIndex(type);

    alterModel();
}

void Potentiometer::setType(int index)
{
    potType type = (potType) index;

    this->type = type;

    alterModel();
}

int Potentiometer::getPosition() const
{
    return position;
}

void Potentiometer::setPosition(int position)
{
    this->position = position;

    dial->setValue(position);

    alterModel();
}

void Potentiometer::setPotentiometer(potType type, int value, int position, const char *labelText)
{
    setType(type);
    setValue(value);
    setPosition(position);

    this->labelText = labelText;
    label->setText(labelText);

    alterModel();
}

void Potentiometer::setPotentiometer(pot *value, int position)
{
    setType(value->type);
    setValue(value->value);
    labelText = value->label;
    label->setText(labelText);

    setPosition(position);

    alterModel();
}

void Potentiometer::setVisible(bool visible)
{
    CircuitElement::setVisible(visible);

    dial->setVisible(visible);
    typeSelect->setVisible(visible);
}

void Potentiometer::alterModel()
{
    int rlo;
    int rhi;
    double factor = 0.5;

    switch (type) {
    default:
    case POT_LIN: // Linear
    case POT_STYPE: // Linear for now
        factor = ((double) position) / 99.0;
        break;
    case POT_LOG: // Log
        factor = pow(99.0, ((double) position) / 99.0) / 99.0;
        //factor = logFunction(((double) position) / 99.0, 0.12);
        break;
    case POT_LOGA: // Log
        //factor = pow(99.0, sqrt(((double) position) / 99.0)) / 99.0;
        factor = logFunction(((double) position) / 99.0, 0.25);
        break;
    case POT_RLOG: // Reverse Log - needs to be fixed (currently just Log)
        factor = logFunction(((double) position) / 99.0, 0.88);
    }

    rlo = value * factor;
    rhi = value - rlo;

    char alterHiCmd[256];
    sprintf(alterHiCmd, "alter rp%dhi=%d", id, rhi);
    char alterLoCmd[256];
    sprintf(alterLoCmd, "alter rp%dlo=%d", id, rlo);

    spice(alterHiCmd);
    spice(alterLoCmd);
}

char *Potentiometer::lowercase(const char *mixed) {
    char *lower = _strdup(mixed);
    for (char *p = lower; *p; p++) {
        *p = tolower(*p);
    }

    return lower;
}

double Potentiometer::logFunction(double x, double ym)
{
    // From: https://electronics.stackexchange.com/questions/304692/formula-for-logarithmic-audio-taper-pot
    // y = a.b^x - a
    // b = ((1 / ym) - 1)^2
    // a = 1 / (b - 1)

    double b = pow((1.0 / ym) - 1.0, 2);
    double a = 1 / (b - 1);

    return a * pow(b, x) - a;
}
