#include "marshall18.h"

Marshall18::Marshall18(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble) : Circuit(bass, mid, treble)
{

}

const char *Marshall18::getFile()
{
    return "source ../circuits/marshall18.cir";
}

void Marshall18::setPotentiometers()
{
    bass->setVisible(false);
    mid->setVisible(true);
    mid->setPotentiometer(POT_LIN, 500000, 49, "Tone");
    treble->setVisible(false);
}
