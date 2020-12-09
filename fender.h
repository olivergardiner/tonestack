#ifndef FENDER_H
#define FENDER_H

#include "circuit.h"

class Fender : public Circuit
{
public:
    Fender(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // FENDER_H
