#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "circuitelement.h"

class Capacitor : public CircuitElement
{
public:
    Capacitor(QLabel *label, QLineEdit *valueText, int id);

    void setValue(int value);

private:
    void alterModel();
};

#endif // CAPACITOR_H
