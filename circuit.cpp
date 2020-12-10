#include "circuit.h"

#define spice(X) (ngSpice_Command(const_cast <char *>(X)))

Circuit::Circuit(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4)
{
    this->pot1 = pot1;
    this->pot2 = pot2;
    this->pot3 = pot3;
    this->pot4 = pot4;
}

void Circuit::load()
{
    spice(getFile());

    setPotentiometers();
}
