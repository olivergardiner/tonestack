#include "moonlight.h"

Moonlight::Moonlight(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble) : Circuit(bass, mid, treble)
{

}

const char *Moonlight::getFile()
{
    return "source ../circuits/moonlight.cir";
}

void Moonlight::setPotentiometers()
{
    bass->setVisible(false);
    mid->setVisible(true);
    mid->setPotentiometer(POT_LIN, 500000, 49, "Tone");
    treble->setVisible(false);
}
