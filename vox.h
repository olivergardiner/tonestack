#ifndef VOX_H
#define VOX_H

#include "circuit.h"

class Vox : public Circuit
{
public:
    Vox(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble);

private:
    const char *getFile();
    void setPotentiometers();
};

#endif // VOX_H
