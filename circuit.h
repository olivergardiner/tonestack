#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "potentiometer.h"

class Circuit
{
public:
    Circuit(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble);

    void load();

protected:
    Potentiometer *bass;
    Potentiometer *mid;
    Potentiometer *treble;

private:
    virtual const char *getFile() = 0;
    virtual void setPotentiometers() = 0;
};

#endif // CIRCUIT_H
