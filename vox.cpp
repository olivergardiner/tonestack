#include "vox.h"

Vox::Vox(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4) : Circuit(pot1, pot2, pot3, pot4)
{

}

const char *Vox::getFile()
{
    return "source ../circuits/vox.cir";
}

void Vox::setPotentiometers()
{
    pot1->setVisible(true);
    pot1->setPotentiometer(POT_LOGA, 1000000, 49, "Bass");
    pot2->setVisible(false);
    pot3->setVisible(true);
    pot3->setPotentiometer(POT_LOGA, 1000000, 49, "Treble");
}
