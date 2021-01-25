#include "circuit.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

Circuit::Circuit(circuitUiMap *uiMap)
{
    this->uiMap = uiMap;
}

void Circuit::read(const QJsonObject &json)
{
    if (json.contains("command") && json["command"].isString()) {
        command = json["command"].toString();

        spice(command.toStdString().c_str());
    }

    if (json.contains("range") && json["range"].isDouble()) {
        range = json["range"].toInt();
    } else {
        range = 0; // Default value which corresponds to 0dB to -40dB
    }

    if (json.contains("image") && json["image"].isString()) {
        image = json["image"].toString();

        QPixmap circuitImage(image);
        circuitImage = circuitImage.scaled(469, 326, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        uiMap->circuitDisplay->setPixmap(circuitImage);
    }

    if (json.contains("rS") && json["rS"].isObject()) {
        uiMap->rS->read(json["rS"].toObject());
    }

    if (json.contains("rL") && json["rL"].isObject()) {
        uiMap->rL->read(json["rL"].toObject());
    }

    if (json.contains("pot1") && json["pot1"].isObject()) {
        uiMap->pot1->read(json["pot1"].toObject());
        pot1 = true;
    } else {
        pot1 = false;
    }
    uiMap->pot1->setVisible(pot1);

    if (json.contains("pot2") && json["pot2"].isObject()) {
        uiMap->pot2->read(json["pot2"].toObject());
        pot2 = true;
    } else {
        pot2 = false;
    }
    uiMap->pot2->setVisible(pot2);

    if (json.contains("pot3") && json["pot3"].isObject()) {
        uiMap->pot3->read(json["pot3"].toObject());
        pot3 = true;
    } else {
        pot3 = false;
    }
    uiMap->pot3->setVisible(pot3);

    if (json.contains("resistors") && json["resistors"].isArray()) {
        QJsonArray resistorArray = json["resistors"].toArray();
        for (int r=0; r < 5; r++) {
            QJsonValue resistorValue = resistorArray.at(r);
            if (resistorValue.isObject()) {
                uiMap->resistors[r]->read(resistorValue.toObject());
                uiMap->resistors[r]->setVisible(true);
            } else {
                uiMap->resistors[r]->setVisible(false);
            }
        }

        resistors = resistorArray.count();
    }

    if (json.contains("capacitors") && json["capacitors"].isArray()) {
        QJsonArray capacitorArray = json["capacitors"].toArray();
        for (int c=0; c < 5; c++) {
            QJsonValue capacitorValue = capacitorArray.at(c);
            if (capacitorValue.isObject()) {
                uiMap->capacitors[c]->read(capacitorValue.toObject());
                uiMap->capacitors[c]->setVisible(true);
            } else {
                uiMap->capacitors[c]->setVisible(false);
            }
        }

        capacitors = capacitorArray.count();
    }

    if (json.contains("inductors") && json["inductors"].isArray()) {
        QJsonArray inductorArray = json["inductors"].toArray();
        for (int l=0; l < 5; l++) {
            QJsonValue inductorValue = inductorArray.at(l);
            if (inductorValue.isObject()) {
                uiMap->inductors[l]->read(inductorValue.toObject());
                uiMap->inductors[l]->setVisible(true);
            } else {
                uiMap->inductors[l]->setVisible(false);
            }
        }

        inductors = inductorArray.count();
    }
}

void Circuit::write(QJsonObject &json) const
{
    QJsonArray circuits;

    QJsonObject currentCircuit;

    currentCircuit["circuitname"] = circuitName;
    currentCircuit["range"] = range;
    currentCircuit["image"] = image;
    currentCircuit["command"] = command;

    QJsonObject rSObject;
    uiMap->rS->write(rSObject);
    currentCircuit["rS"] = rSObject;

    QJsonObject rLObject;
    uiMap->rL->write(rLObject);
    currentCircuit["rL"] = rLObject;

    QJsonArray resistorObjects;
    for (int r = 0; r < resistors; r++) {
        QJsonObject resistorObject;
        uiMap->resistors[r]->write(resistorObject);
        resistorObjects.append(resistorObject);
    }
    currentCircuit["resistors"] = resistorObjects;

    QJsonArray capacitorObjects;
    for (int c = 0; c < capacitors; c++) {
        QJsonObject capacitorObject;
        uiMap->capacitors[c]->write(capacitorObject);
        capacitorObjects.append(capacitorObject);
    }
    currentCircuit["capacitors"] = capacitorObjects;

    QJsonArray inductorObjects;
    for (int l = 0; l < inductors; l++) {
        QJsonObject inductorObject;
        uiMap->inductors[l]->write(inductorObject);
        inductorObjects.append(inductorObject);
    }
    currentCircuit["inductors"] = inductorObjects;

    QJsonObject pot2Object;
    QJsonObject pot3Object;

    if (pot1) {
        QJsonObject pot1Object;
        uiMap->pot1->write(pot1Object);
        currentCircuit["pot1"] = pot1Object;
    }

    if (pot2) {
        QJsonObject pot2Object;
        uiMap->pot2->write(pot2Object);
        currentCircuit["pot2"] = pot2Object;
    }

    if (pot3) {
        QJsonObject pot3Object;
        uiMap->pot3->write(pot3Object);
        currentCircuit["pot3"] = pot3Object;
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

int Circuit::getRange() const
{
    return range;
}
