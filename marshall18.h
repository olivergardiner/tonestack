#ifndef MARSHALL18_H
#define MARSHALL18_H

#include "circuit.h"

class Marshall18 : public Circuit
{
public:
    Marshall18(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // MARSHALL18_H
