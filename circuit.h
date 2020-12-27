#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "potentiometer.h"

typedef enum e_potFunction {
    POT_BASS,
    POT_MIDDLE,
    POT_TREBLE,
    POT_ALL
} potFunction;

typedef struct circuitDefinition {
    const char circuitLabel[64];
    const char circuitImage[64];
    const char circuitCommand[64];
    int pots;
    pot potValues[4];
    int resistors;
    int rValues[32];
    int capacitors;
    int cValues[32];
    int inductors;
    int lValues[32];
} circuit;

class Circuit
{
public:
    Circuit(circuitDefinition *definitions, Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4);

    void edit();

    int getCurrentCircuit() const;
    void setCurrentCircuit(int value);

    const char *getImage();

protected:
    circuitDefinition *definitions;
    Potentiometer *pot1;
    Potentiometer *pot2;
    Potentiometer *pot3;
    Potentiometer *pot4;

private:
    int currentCircuit;
};

#endif // CIRCUIT_H
