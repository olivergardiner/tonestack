#include "potentiometer.h"

#define spice(X) (ngSpice_Command(const_cast <char *>(X)))

Potentiometer::Potentiometer(QDial * dial, QLineEdit * valueText, QComboBox * typeSelect, QLabel * label, int id)
{
    this->dial = dial;
    this->valueText = valueText;
    this->typeSelect = typeSelect;
    this->label = label;
    this->id = id;
}

potType Potentiometer::getType() const
{
    return type;
}

void Potentiometer::setType(const potType &type)
{
    this->type = type;

    typeSelect->setCurrentIndex(getPotTypeIndex(type));

    alterModel();
}

void Potentiometer::setType(int index)
{
    potType type = getPotType(index);

    this->type = type;

    alterModel();
}

int Potentiometer::getValue() const
{
    return value;
}

void Potentiometer::setValue(int value)
{
    this->value = value;

    char valueString[32] = "1";
    double scaledValue;

    if (value >= 1000000) {
        scaledValue = ((double) value) / 1000000;
        sprintf(valueString, "%.2fMEG", scaledValue);
    } else if (value > 1000) {
        scaledValue = ((double) value) / 1000;
        sprintf(valueString, "%.2fK", scaledValue);
    } else {
        sprintf(valueString, "%d", value);
    }

    valueText->setText(valueString);

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

void Potentiometer::setVisible(bool visible)
{
    dial->setVisible(visible);
    valueText->setVisible(visible);
    typeSelect->setVisible(visible);
    label->setVisible(visible);
}

int Potentiometer::getPotTypeIndex(potType type)
{
    switch (type) {
    case POT_LIN:
        return 0;
        break;
    case POT_LOG:
        return 1;
        break;
    case POT_LOGA:
        return 2;
        break;
    case POT_RLOG:
        return 3;
        break;
    case POT_STYPE:
        return 4;
        break;
    }

    return 0;
}

potType Potentiometer::getPotType(int index)
{
    switch (index) {
    case 0:
        return POT_LIN;
        break;
    case 1:
        return POT_LOG;
        break;
    case 2:
        return POT_LOGA;
        break;
    case 3:
        return POT_RLOG;
        break;
    case 4:
        return POT_STYPE;
        break;
    }

    return POT_LIN;
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
