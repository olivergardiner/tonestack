#ifndef MOONLIGHT_H
#define MOONLIGHT_H

#include "circuit.h"

class Moonlight : public Circuit
{
public:
    Moonlight(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // MOONLIGHT_H
