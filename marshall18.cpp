#include "marshall18.h"

Marshall18::Marshall18(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4) : Circuit(pot1, pot2, pot3, pot4)
{

}

const char *Marshall18::getFile()
{
    return "source ../circuits/marshall18.cir";
}

void Marshall18::setPotentiometers()
{
    pot1->setVisible(false);
    pot2->setVisible(true);
    pot2->setPotentiometer(POT_LIN, 500000, 49, "Tone");
    pot3->setVisible(false);
}
