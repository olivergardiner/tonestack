#ifndef MARSHALL_H
#define MARSHALL_H

#include "circuit.h"

class Marshall : public Circuit
{
public:
    Marshall(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // MARSHALL_H
