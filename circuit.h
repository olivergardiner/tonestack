#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "potentiometer.h"

class Circuit
{
public:
    Circuit(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4);

    void load();

protected:
    Potentiometer *pot1;
    Potentiometer *pot2;
    Potentiometer *pot3;
    Potentiometer *pot4;

private:
    virtual const char *getFile() = 0;
    virtual void setPotentiometers() = 0;
};

#endif // CIRCUIT_H
