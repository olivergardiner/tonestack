#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "potentiometer.h"
#include "resistor.h"
#include "capacitor.h"
#include "inductor.h"

typedef struct circuitUiMap {
    QLabel *circuitDisplay;
    QComboBox *stackSelection;
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

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void saveCircuit(QString filename);
    void openCircuit(QString filename);

    QString getCircuitName() const;
    void setCircuitName(const QString &value);

protected:
    circuitUiMap *uiMap;

private:
    QString circuitName;
    QString command;
    QString image;
    int resistors;
    int capacitors;
    int inductors;
    bool pot1;
    bool pot2;
    bool pot3;
};

#endif // CIRCUIT_H
