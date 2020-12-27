#include "circuit.h"

#define spice(X) (ngSpice_Command(const_cast <char *>(X)))

Circuit::Circuit(circuitDefinition *definitions, Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4, Resistor **resistors, Capacitor **capacitors, Inductor **inductors, Resistor *rS, Resistor *rL)
{
    this->definitions = definitions;
    this->pot1 = pot1;
    this->pot2 = pot2;
    this->pot3 = pot3;
    this->pot4 = pot4;
    this->resistors = resistors;
    this->capacitors = capacitors;
    this->inductors = inductors;
    this->rS = rS;
    this->rL = rL;
}

int Circuit::getCurrentCircuit() const
{
    return currentCircuit;
}

void Circuit::setCurrentCircuit(int value)
{
    currentCircuit = value;

    circuitDefinition *currentDefinition = &definitions[currentCircuit];

    spice(currentDefinition->circuitCommand);

    // Alter circuit values

    switch (currentDefinition->pots) {
    case 1:
        pot1->setVisible(false);
        pot2->setVisible(true);
        pot2->setPotentiometer(&currentDefinition->potValues[0], 49);
        pot3->setVisible(false);
        break;
    case 2:
        pot1->setVisible(true);
        pot1->setPotentiometer(&currentDefinition->potValues[0], 49);
        pot2->setVisible(false);
        pot3->setVisible(true);
        pot3->setPotentiometer(&currentDefinition->potValues[1], 49);
        break;
    case 3:
    default:
        pot1->setVisible(true);
        pot1->setPotentiometer(&currentDefinition->potValues[0], 49);
        pot2->setVisible(true);
        pot2->setPotentiometer(&currentDefinition->potValues[1], 49);
        pot3->setVisible(true);
        pot3->setPotentiometer(&currentDefinition->potValues[2], 49);
        break;
    }

    for (int r=0; r < 5; r++) {
        Resistor * resistor = resistors[r];
        bool visible = r < currentDefinition->resistors;
        resistor->setVisible(visible);
        if (visible) {
            resistor->setValue(currentDefinition->rValues[r]);
        }
    }

    for (int c=0; c < 5; c++) {
        Capacitor *capacitor = capacitors[c];
        bool visible = c < currentDefinition->capacitors;
        capacitor->setVisible(visible);
        if (visible) {
            capacitor->setValue(currentDefinition->cValues[c]);
        }
    }

    for (int l=0; l < 5; l++) {
        Inductor *inductor = inductors[l];
        bool visible = l < currentDefinition->inductors;
        inductor->setVisible(visible);
        if (visible) {
            inductor->setValue(currentDefinition->lValues[l]);
        }
    }

    rS->setValue(currentDefinition->rS);
    rL->setValue(currentDefinition->rL);
}

const char *Circuit::getImage()
{
    return definitions[currentCircuit].circuitImage;
}
