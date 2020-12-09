#include "vox.h"

Vox::Vox(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble) : Circuit(bass, mid, treble)
{

}

const char *Vox::getFile()
{
    return "source ../circuits/vox.cir";
}

void Vox::setPotentiometers()
{
    bass->setVisible(true);
    bass->setPotentiometer(POT_LOG, 1000000, 49, "Bass");
    mid->setVisible(false);
    treble->setVisible(true);
    treble->setPotentiometer(POT_LOGA, 1000000, 49, "Treble");
}
