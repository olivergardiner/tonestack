#ifndef FENDER_H
#define FENDER_H

#include "circuit.h"

class Fender : public Circuit
{
public:
    Fender(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // FENDER_H
