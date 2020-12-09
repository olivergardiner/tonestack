#ifndef MARSHALL_H
#define MARSHALL_H

#include "circuit.h"

class Marshall : public Circuit
{
public:
    Marshall(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // MARSHALL_H
