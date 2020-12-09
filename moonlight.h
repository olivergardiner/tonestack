#ifndef MOONLIGHT_H
#define MOONLIGHT_H

#include "circuit.h"

class Moonlight : public Circuit
{
public:
    Moonlight(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // MOONLIGHT_H
