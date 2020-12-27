#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "potentiometer.h"
#include "resistor.h"
#include "capacitor.h"
#include "inductor.h"

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
    int rS;
    int rL;
    int pots;
    pot potValues[4];
    int resistors;
    int rValues[5];
    int capacitors;
    int cValues[5];
    int inductors;
    int lValues[5];
} circuit;

class Circuit
{
public:
    Circuit(circuitDefinition *definitions, Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4, Resistor **resitors, Capacitor **capacitors, Inductor **inductors, Resistor *rS, Resistor *rL);

    int getCurrentCircuit() const;
    void setCurrentCircuit(int value);

    const char *getImage();

protected:
    circuitDefinition *definitions;
    Potentiometer *pot1;
    Potentiometer *pot2;
    Potentiometer *pot3;
    Potentiometer *pot4;
    Resistor **resistors;
    Capacitor **capacitors;
    Inductor **inductors;
    Resistor *rS;
    Resistor *rL;

private:
    int currentCircuit;
};

#endif // CIRCUIT_H
