#include "circuitelement.h"

CircuitElement::CircuitElement(QLabel *label, QLineEdit *valueText, int id)
{
    this->label = label;
    this->valueText = valueText;
    this->id = id;
}

void CircuitElement::setVisible(bool visible)
{
    label->setVisible(visible);
    valueText->setVisible(visible);
}

int CircuitElement::getValue()
{
    return value;
}

void CircuitElement::setTextValue(const char *value)
{
    float parsedValue;
    char scale;

    int newValue = 1;

    int n = sscanf_s(value, "%f%c", &parsedValue, &scale, 1u);

    if (n == 2) {
        if (toupper(scale) == toupper(mega)) {
            newValue = parsedValue * 1000000;
        } else if (toupper(scale) == toupper(kilo)) {
            newValue = parsedValue * 1000;
        } else {
            newValue = parsedValue;
        }
    } else if (n == 1) {
        newValue = parsedValue;
    }

    setValue(newValue);
}

void CircuitElement::read(const QJsonObject &json)
{
    if (json.contains("value") && json["value"].isDouble()) {
        setValue(json["value"].toInt());
    }

    alterModel();
}

void CircuitElement::write(QJsonObject &json) const
{
    json["id"] = id;
    json["value"] = value;
}

QByteArray CircuitElement::formatNumber(double value)
{
    char number[32];

    sprintf(number, "%.2f", value);

    int length = strlen(number);
    for (int i=length-1;i >= 0; i--) {
        char test = number[i];
        if (test == '0' || test == '.') {
            number[i] = 0;
        }

        if (test != '0') {
            break;
        }
    }

    return QByteArray(number);
}
