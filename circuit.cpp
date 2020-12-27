#include "circuit.h"

#define spice(X) (ngSpice_Command(const_cast <char *>(X)))

Circuit::Circuit(circuitDefinition *definitions, Potentiometer *pot1, Potentiometer *pot2, Potentiometer *pot3, Potentiometer *pot4)
{
    this->definitions = definitions;
    this->pot1 = pot1;
    this->pot2 = pot2;
    this->pot3 = pot3;
    this->pot4 = pot4;
}

void Circuit::edit()
{
    //editDialog->exec();
}

int Circuit::getCurrentCircuit() const
{
    return currentCircuit;
}

void Circuit::setCurrentCircuit(int value)
{
    currentCircuit = value;

    spice(definitions[currentCircuit].circuitCommand);

    // Alter circuit values

    switch (definitions[currentCircuit].pots) {
    case 1:
        pot1->setVisible(false);
        pot2->setVisible(true);
        pot2->setPotentiometer(&definitions[currentCircuit].potValues[0], 49);
        pot3->setVisible(false);
        break;
    case 2:
        pot1->setVisible(true);
        pot1->setPotentiometer(&definitions[currentCircuit].potValues[0], 49);
        pot2->setVisible(false);
        pot3->setVisible(true);
        pot3->setPotentiometer(&definitions[currentCircuit].potValues[1], 49);
        break;
    case 3:
    default:
        pot1->setVisible(true);
        pot1->setPotentiometer(&definitions[currentCircuit].potValues[0], 49);
        pot2->setVisible(true);
        pot2->setPotentiometer(&definitions[currentCircuit].potValues[1], 49);
        pot3->setVisible(true);
        pot3->setPotentiometer(&definitions[currentCircuit].potValues[2], 49);
        break;
    }
}

const char *Circuit::getImage()
{
    return definitions[currentCircuit].circuitImage;
}
