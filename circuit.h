#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "potentiometer.h"
#include "resistor.h"
#include "capacitor.h"
#include "inductor.h"

typedef struct circuitDefinition {
    int id;
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

typedef struct circuitUiMap {
    int circuits;
    QLabel *circuitDisplay;
    QComboBox *stackSelection;
    circuitDefinition *definitions;
    Potentiometer *pot1;
    Potentiometer *pot2;
    Potentiometer *pot3;
    Potentiometer *pot4;
    Resistor *resistors[5];
    Capacitor *capacitors[5];
    Inductor *inductors[5];
    Resistor *rS;
    Resistor *rL;
} circuitUiMap;

class Circuit
{
public:
    Circuit(circuitUiMap *uiMap);

    int getCurrentCircuit() const;
    void setCurrentCircuit(int value);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void saveCircuit(QString filename);
    void openCircuit(QString filename);

    QString getCircuitName() const;
    void setCircuitName(const QString &value);

protected:
    circuitUiMap *uiMap;

private:
    int currentCircuit;
    QString circuitName;

    int findCircuit(int id);
    void configureUi();
};

#endif // CIRCUIT_H
