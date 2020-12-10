#ifndef MARSHALL18_H
#define MARSHALL18_H

#include "circuit.h"

class Marshall18 : public Circuit
{
public:
    Marshall18(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // MARSHALL18_H
