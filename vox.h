#ifndef VOX_H
#define VOX_H

#include "circuit.h"

class Vox : public Circuit
{
public:
    Vox(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // VOX_H
