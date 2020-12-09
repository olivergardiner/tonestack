#include "marshall.h"

Marshall::Marshall(Potentiometer *bass, Potentiometer *mid, Potentiometer *treble) : Circuit(bass, mid, treble)
{

}

const char *Marshall::getFile()
{
    return "source ../circuits/marshall.cir";
}

void Marshall::setPotentiometers()
{
    bass->setVisible(true);
    bass->setPotentiometer(POT_LOG, 1000000, 49, "Bass");
    mid->setVisible(true);
    mid->setPotentiometer(POT_LIN, 25000, 49, "Mid");
    treble->setVisible(true);
    treble->setPotentiometer(POT_LIN, 220000, 49, "Treble");
}
