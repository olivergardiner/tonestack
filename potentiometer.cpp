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

void Potentiometer::setType(int index)
{
    potType type = (potType) index;

    this->type = type;
    typeSelect->setCurrentIndex(index);

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

double Potentiometer::getPotFactor(int potType)
{
    if (potType < 0 || potType > POT_STYPE) {
        return -1.0;
    }

    return potFactor[potType];
}

void Potentiometer::setPotFactor(int potType, double factor)
{
    if (potType < 0 || potType > POT_STYPE) {
        return;
    }

    potFactor[potType] = factor;
}

void Potentiometer::setPotFactor(int potType, QList<double> factors)
{
    for (int type = 0; type < POT_STYPE + 1; type++) {
        potFactor[type] = factors.at(type);
    }
}

void Potentiometer::setLabel(QString labelText)
{
    this->labelText = labelText;
    label->setText(labelText);
}

void Potentiometer::setVisible(bool visible)
{
    CircuitElement::setVisible(visible);

    dial->setVisible(visible);
    typeSelect->setVisible(visible);
}

void Potentiometer::read(const QJsonObject &json)
{
    Resistor::read(json);

    if (json.contains("type") && json["type"].isDouble()) {
        type = (potType) json["type"].toInt();
    }

    if (json.contains("function") && json["function"].isString()) {
        setLabel(json["function"].toString());
    }

    if (json.contains("ganged") && json["ganged"].isBool()) {
        ganged = json["ganged"].toBool();
    } else {
        ganged = false;
    }

    typeSelect->setCurrentIndex(type);

    position = 49;
    dial->setValue(position);

    setValue(value);
}

void Potentiometer::write(QJsonObject &json) const
{
    Resistor::write(json);

    json["type"] = type;
    json["function"] = labelText;
    json["ganged"] = ganged;
}

bool Potentiometer::isGanged() const
{
    return ganged;
}

void Potentiometer::setSweeper(PotSweep *value)
{
    sweeper = value;
}

void Potentiometer::alterModel()
{
    int rlo;
    int rhi;
    double factor = 0.5;
    double potPosition = ((double) position) / 99.0;

    if (sweeper) {
        factor = sweeper->taperFunction(type, potPosition);
    } else {
        switch (type) {
        default:
        case POT_STYPE: // Linear for now
        case POT_LIN: // Linear
            factor = potPosition;
            break;
        case POT_LOG: // Log
            // factor = pow(99.0, ((double) position) / 99.0) / 99.0;
            // break;
        case POT_LOGA: // Log A
            // factor = pow(99.0, sqrt(((double) position) / 99.0)) / 99.0;
            // break;
        case POT_RLOG: // Reverse Log
            factor = logFunction(potPosition, potFactor[type]);
            break;
        }
    }

    rlo = value * factor;
    rhi = value - rlo;

    alterPot(rlo, rhi, false);

    if (ganged) {
        alterPot(rlo, rhi, true);
    }
}

void Potentiometer::alterPot(int rlo, int rhi, bool gang) {
    char alterHiCmd[256];
    sprintf(alterHiCmd, "alter rp%dhi%s=%d", id, gang ? "2" : "", rhi);
    char alterLoCmd[256];
    sprintf(alterLoCmd, "alter rp%dlo%s=%d", id, gang ? "2" : "", rlo);

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

    //double b = pow((1.0 / ym) - 1.0, 2);
    double b = ((1.0 / ym) - 1.0);
    b = b * b;

    if (b == 1) { // Linear curve but equation then blows up as a becomes infinite so trap and return explicit linear
        return x;
    }

    double a = 1 / (b - 1.0);

    return a * pow(b, x) - a;
}
