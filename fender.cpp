#include "fender.h"

Fender::Fender(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4) : Circuit(pot1, pot2, pot3, pot4)
{

}

const char *Fender::getFile()
{
    return "source ../circuits/fender.cir";
}

void Fender::setPotentiometers()
{
    pot1->setVisible(true);
    pot1->setPotentiometer(POT_LOGA, 250000, 49, "Bass");
    pot2->setVisible(true);
    pot2->setPotentiometer(POT_LIN, 10000, 49, "Mid");
    pot3->setVisible(true);
    pot3->setPotentiometer(POT_LOGA, 250000, 49, "Treble");
}
