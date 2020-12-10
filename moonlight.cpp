#include "moonlight.h"

Moonlight::Moonlight(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4) : Circuit(pot1, pot2, pot3, pot4)
{

}

const char *Moonlight::getFile()
{
    return "source ../circuits/moonlight.cir";
}

void Moonlight::setPotentiometers()
{
    pot1->setVisible(false);
    pot2->setVisible(true);
    pot2->setPotentiometer(POT_LIN, 500000, 49, "Tone");
    pot3->setVisible(false);
}
