#include "circuit.h"

#define spice(X) (ngSpice_Command(const_cast <char *>(X)))

Circuit::Circuit(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble)
{
    this->bass = bass;
    this->mid = mid;
    this->treble = treble;
}

void Circuit::load()
{
    spice(getFile());

    setPotentiometers();
}
