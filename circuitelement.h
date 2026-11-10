#ifndef CIRCUITELEMENT_H
#define CIRCUITELEMENT_H

#include <QLabel>
#include <QLineEdit>
#include <QJsonObject>

#include "ngspice/sharedspice.h"

#include "simulationcontroller.h"
#define spice(X) dispatchSpiceCommand(X)

class CircuitElement
{
public:
    CircuitElement(QLabel *label, QLineEdit *valueText, int id);

    void setVisible(bool visible);
    int getValue();
    void setTextValue(const char *value);
    virtual void read(const QJsonObject &json);
    virtual void write(QJsonObject &json) const;

    virtual void setValue(int value) = 0;

protected:
    QByteArray formatNumber(double value);

    QLabel *label;
    QLineEdit *valueText;

    int id;
    int value = 1;

    char mega;
    char kilo;
    char unit;

private:
    virtual void alterModel() = 0;
};

#endif // CIRCUITELEMENT_H
