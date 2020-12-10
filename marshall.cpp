#include "marshall.h"

Marshall::Marshall(Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4) : Circuit(pot1, pot2, pot3, pot4)
{

}

const char *Marshall::getFile()
{
    return "source ../circuits/marshall.cir";
}

void Marshall::setPotentiometers()
{
    pot1->setVisible(true);
    pot1->setPotentiometer(POT_LOG, 1000000, 49, "Bass");
    pot2->setVisible(true);
    pot2->setPotentiometer(POT_LIN, 25000, 49, "Mid");
    pot3->setVisible(true);
    pot3->setPotentiometer(POT_LIN, 220000, 49, "Treble");
}
