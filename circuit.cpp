#include "circuit.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

Circuit::Circuit(circuitUiMap *uiMap)
{
    this->uiMap = uiMap;
}

int Circuit::getCurrentCircuit() const
{
    return currentCircuit;
}

void Circuit::setCurrentCircuit(int value)
{
    currentCircuit = value;

    circuitDefinition *currentDefinition = &uiMap->definitions[currentCircuit];

    configureUi();

    spice(currentDefinition->circuitCommand);

    // Alter circuit values

    switch (currentDefinition->pots) {
    case 1:
        uiMap->pot2->setPotentiometer(&currentDefinition->potValues[0], 49);
        break;
    case 2:
        uiMap->pot1->setPotentiometer(&currentDefinition->potValues[0], 49);
        uiMap->pot3->setPotentiometer(&currentDefinition->potValues[1], 49);
        break;
    case 3:
    default:
        uiMap->pot1->setPotentiometer(&currentDefinition->potValues[0], 49);
        uiMap->pot2->setPotentiometer(&currentDefinition->potValues[1], 49);
        uiMap->pot3->setPotentiometer(&currentDefinition->potValues[2], 49);
        break;
    }

    for (int r=0; r < 5; r++) {
        Resistor * resistor = uiMap->resistors[r];
        if (r < currentDefinition->resistors) {
            resistor->setValue(currentDefinition->rValues[r]);
        }
    }

    for (int c=0; c < 5; c++) {
        Capacitor *capacitor = uiMap->capacitors[c];
        if (c < currentDefinition->capacitors) {
            capacitor->setValue(currentDefinition->cValues[c]);
        }
    }

    for (int l=0; l < 5; l++) {
        Inductor *inductor = uiMap->inductors[l];
        if (l < currentDefinition->inductors) {
            inductor->setValue(currentDefinition->lValues[l]);
        }
    }

    uiMap->rS->setValue(currentDefinition->rS);
    uiMap->rL->setValue(currentDefinition->rL);
}

void Circuit::configureUi()
{
    circuitDefinition *currentDefinition = &uiMap->definitions[currentCircuit];

    uiMap->stackSelection->setCurrentIndex(currentCircuit);

    QPixmap circuitImage(currentDefinition->circuitImage);
    circuitImage = circuitImage.scaled(469, 326, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    uiMap->circuitDisplay->setPixmap(circuitImage);

    switch (currentDefinition->pots) {
    case 1:
        uiMap->pot1->setVisible(false);
        uiMap->pot2->setVisible(true);
        uiMap->pot2->setLabel(currentDefinition->potValues[1].label);
        uiMap->pot3->setVisible(false);
        break;
    case 2:
        uiMap->pot1->setVisible(true);
        uiMap->pot1->setLabel(currentDefinition->potValues[0].label);
        uiMap->pot2->setVisible(false);
        uiMap->pot3->setVisible(true);
        uiMap->pot3->setLabel(currentDefinition->potValues[2].label);
        break;
    case 3:
    default:
        uiMap->pot1->setVisible(true);
        uiMap->pot1->setLabel(currentDefinition->potValues[0].label);
        uiMap->pot2->setVisible(true);
        uiMap->pot2->setLabel(currentDefinition->potValues[1].label);
        uiMap->pot3->setVisible(true);
        uiMap->pot3->setLabel(currentDefinition->potValues[2].label);
        break;
    }

    for (int r=0; r < 5; r++) {
        Resistor * resistor = uiMap->resistors[r];
        resistor->setVisible(r < currentDefinition->resistors);
    }

    for (int c=0; c < 5; c++) {
        Capacitor *capacitor = uiMap->capacitors[c];
        capacitor->setVisible(c < currentDefinition->capacitors);
    }

    for (int l=0; l < 5; l++) {
        Inductor *inductor = uiMap->inductors[l];
        inductor->setVisible(l < currentDefinition->inductors);
    }
}

void Circuit::read(const QJsonObject &json)
{
    if (json.contains("id") && json["id"].isDouble()) {
        currentCircuit = findCircuit(json["id"].toDouble());

        spice(uiMap->definitions[currentCircuit].circuitCommand);
    }

    if (json.contains("rS") && json["rS"].isObject()) {
        uiMap->rS->read(json["rS"].toObject());
    }

    if (json.contains("rL") && json["rL"].isObject()) {
        uiMap->rL->read(json["rL"].toObject());
    }

    if (json.contains("pot1") && json["pot1"].isObject()) {
        uiMap->pot1->read(json["pot1"].toObject());
        uiMap->pot1->setVisible(true);
    } else {
        uiMap->pot1->setVisible(false);
    }

    if (json.contains("pot2") && json["pot2"].isObject()) {
        uiMap->pot2->read(json["pot2"].toObject());
        uiMap->pot2->setVisible(true);
    } else {
        uiMap->pot2->setVisible(false);
    }

    if (json.contains("pot3") && json["pot3"].isObject()) {
        uiMap->pot3->read(json["pot3"].toObject());
        uiMap->pot3->setVisible(true);
    } else {
        uiMap->pot3->setVisible(false);
    }

    if (json.contains("resistors") && json["resistors"].isArray()) {
        QJsonArray resistorArray = json["resistors"].toArray();
        int length = resistorArray.count();
        for (int r=0; r < 5; r++) {
            if (r < length) {
                QJsonValue resistorValue = resistorArray.at(r);
                if (resistorValue.isObject()) {
                    uiMap->resistors[r]->read(resistorValue.toObject());
                    uiMap->resistors[r]->setVisible(true);
                } else {
                    uiMap->resistors[r]->setVisible(false);
                }
            }
        }
    }

    if (json.contains("capacitors") && json["capacitors"].isArray()) {
        QJsonArray capacitorArray = json["capacitors"].toArray();
        int length = capacitorArray.count();
        for (int c=0; c < length; c++) {
            QJsonValue capacitorValue = capacitorArray.at(c);
            if (capacitorValue.isObject()) {
                uiMap->capacitors[c]->read(capacitorValue.toObject());
                uiMap->capacitors[c]->setVisible(true);
            } else {
                uiMap->capacitors[c]->setVisible(false);
            }
        }
    }

    if (json.contains("inductors") && json["inductors"].isArray()) {
        QJsonArray inductorArray = json["inductors"].toArray();
        int length = inductorArray.count();
        for (int l=0; l < length; l++) {
            QJsonValue inductorValue = inductorArray.at(l);
            if (inductorValue.isObject()) {
                uiMap->inductors[l]->read(inductorValue.toObject());
                uiMap->inductors[l]->setVisible(true);
            } else {
                uiMap->inductors[l]->setVisible(false);
            }
        }
    }
}

void Circuit::write(QJsonObject &json) const
{
    circuitDefinition *currentDefinition = &uiMap->definitions[currentCircuit];

    QJsonArray circuits;

    QJsonObject currentCircuit;

    currentCircuit["id"] = currentDefinition->id;
    currentCircuit["circuitname"] = circuitName;
    currentCircuit["image"] = currentDefinition->circuitImage;
    currentCircuit["command"] = currentDefinition->circuitCommand;
    currentCircuit["circuit"] = currentDefinition->circuitLabel;

    QJsonObject rSObject;
    uiMap->rS->write(rSObject);
    currentCircuit["rS"] = rSObject;

    QJsonObject rLObject;
    uiMap->rL->write(rLObject);
    currentCircuit["rL"] = rLObject;

    QJsonArray resistorObjects;
    for (int r = 0; r < currentDefinition->resistors; r++) {
        QJsonObject resistorObject;
        uiMap->resistors[r]->write(resistorObject);
        resistorObjects.append(resistorObject);
    }
    currentCircuit["resistors"] = resistorObjects;

    QJsonArray capacitorObjects;
    for (int c = 0; c < currentDefinition->capacitors; c++) {
        QJsonObject capacitorObject;
        uiMap->capacitors[c]->write(capacitorObject);
        capacitorObjects.append(capacitorObject);
    }
    currentCircuit["capacitors"] = capacitorObjects;

    QJsonArray inductorObjects;
    for (int l = 0; l < currentDefinition->inductors; l++) {
        QJsonObject inductorObject;
        uiMap->inductors[l]->write(inductorObject);
        inductorObjects.append(inductorObject);
    }
    currentCircuit["inductors"] = inductorObjects;

    QJsonObject pot1Object;
    QJsonObject pot2Object;
    QJsonObject pot3Object;
    switch (currentDefinition->pots) {
    case 1:
        uiMap->pot2->write(pot2Object);
        currentCircuit["pot2"] = pot2Object;
        break;
    case 2:
        uiMap->pot1->write(pot1Object);
        currentCircuit["pot1"] = pot1Object;
        uiMap->pot3->write(pot3Object);
        currentCircuit["pot3"] = pot3Object;
        break;
    case 3:
    default:
        uiMap->pot1->write(pot1Object);
        currentCircuit["pot1"] = pot1Object;
        uiMap->pot2->write(pot2Object);
        currentCircuit["pot2"] = pot2Object;
        uiMap->pot3->write(pot3Object);
        currentCircuit["pot3"] = pot3Object;
        break;
    }

    circuits.append(currentCircuit);

    json["circuits"] = circuits;
}

void Circuit::saveCircuit(QString filename)
{
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
    } else {
        QJsonObject circuitObject;
        write(circuitObject);
        saveFile.write(QJsonDocument(circuitObject).toJson());
    }
}

void Circuit::openCircuit(QString filename)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
    } else {
        QByteArray saveData = loadFile.readAll();

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        if (loadDoc.isObject()) {
            QJsonObject config = loadDoc.object();

            if (config.contains("circuits") && config["circuits"].isArray()) {
                QJsonValue circuitObject = config["circuits"].toArray().at(0);
                if (circuitObject.isObject()) {
                    read(circuitObject.toObject());
                }
            }
        }

        read(loadDoc.object());
    }
}

QString Circuit::getCircuitName() const
{
    return circuitName;
}

void Circuit::setCircuitName(const QString &value)
{
    circuitName = value;
}

int Circuit::findCircuit(int id)
{
    for (int i=0; i < uiMap->circuits; i++) {
        if (uiMap->definitions[i].id == id) {
            return i;
        }
    }

    return 0;
}

