#ifndef INDUCTOR_H
#define INDUCTOR_H

#include "circuitelement.h"

class Inductor : public CircuitElement
{
public:
    Inductor(QLabel *label, QLineEdit *valueText, int id);

    void setValue(int value);

private:
    void alterModel();
};

#endif // INDUCTOR_H
