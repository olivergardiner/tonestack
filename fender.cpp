#include "fender.h"

Fender::Fender(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble) : Circuit(bass, mid, treble)
{

}

const char *Fender::getFile()
{
    return "source ../circuits/fender.cir";
}

void Fender::setPotentiometers()
{
    bass->setVisible(true);
    bass->setPotentiometer(POT_LOG, 250000, 49, "Bass");
    mid->setVisible(true);
    mid->setPotentiometer(POT_LIN, 10000, 49, "Mid");
    treble->setVisible(true);
    treble->setPotentiometer(POT_LOG, 250000, 49, "Treble");
}
