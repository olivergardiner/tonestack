#ifndef RESISTOR_H
#define RESISTOR_H

#include "circuitelement.h"

class Resistor : public CircuitElement
{
public:
    Resistor(QLabel *label, QLineEdit *valueText, int id);

    void setValue(int value);

private:
    void alterModel();
};

#endif // RESISTOR_H
