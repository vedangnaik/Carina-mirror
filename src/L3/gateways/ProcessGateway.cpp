#include "ProcessGateway.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QJsonArray>
#include <QFileDialog>


struct ProcessData ProcessGateway::parseProcessFile(std::string fileName) {
    QFile file(QString::fromStdString(fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // error here
    }
    QString val = file.readAll();
    file.close();

    QJsonObject jsonObj = QJsonDocument::fromJson(val.toUtf8()).object();
    std::map<std::string, Sensor*> sensors = this->parseSensors(jsonObj["sensors"].toObject());
    std::map<std::string, Actuator*> actuators = this->parseActuators(jsonObj["actuators"].toObject());
    return ProcessData {
        sensors,
        actuators,
        this->parseStates(jsonObj["states"].toObject(), sensors, actuators)
    };
}


std::map<std::string, Sensor*> ProcessGateway::parseSensors(QJsonObject sensorsObj) {
    std::map<std::string, Sensor*> sensors = {};
    for (QString k: sensorsObj.keys()) {
        sensors[k.toStdString()] = new Sensor(k.toStdString(),
                    sensorsObj[k].toObject()["name"].toString().toStdString());
    }
    return sensors;
}


std::map<std::string, Actuator*> ProcessGateway::parseActuators(QJsonObject actuatorsObj) {
    std::map<std::string, Actuator*> actuators;
    for (QString k: actuatorsObj.keys()) {
        actuators[k.toStdString()] = new Actuator(k.toStdString(),
                    actuatorsObj[k].toObject()["name"].toString().toStdString());
    }
    return actuators;
}


std::map<std::string, State*> ProcessGateway::parseStates(QJsonObject statesObj, std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators) {
    std::map<std::string, State*> states;
    for (QString k: statesObj.keys()) {
        QJsonValue v = statesObj[k];
        State* s = new State();
        s->id = k.toStdString();
        s->name = v["name"].toString().toStdString();
        s->safetyRating = v["safetyRating"].toString().toStdString();
        s->description = v["description"].toString().toStdString();

        for (QJsonValue action: v["actions"].toArray()) {
            std::string id = action["id"].toString().toStdString();
            if (id == "") {
                // error here
            }

            s->actionsOrder.push_back(id);
            if (sensors.find(id) != sensors.end()) {
                // add sensor options here
            } else if (actuators.find(id) != actuators.end()) {
                if (action["timed"].toBool()) {
                    s->actuatorOptions[id].push_back(ActuatorOption::Timed);
                }
                if (action["automatic"].toBool()) {
                    s->actuatorOptions[id].push_back(ActuatorOption::Automatic);
                }
            } else {
                // error here
            }
        }

        if (v["checks"]["proceed"].isObject()) {
            this->parseStateChecks(v["checks"]["proceed"], sensors, actuators, s, Transition::Proceed);
        } if (v["checks"]["abort"].isObject()) {
            this->parseStateChecks(v["checks"]["abort"], sensors, actuators, s, Transition::Abort);
        }

        s->transitions[Transition::Proceed] = v["transitions"]["proceed"].toString().toStdString();
        s->transitions[Transition::Abort] = v["transitions"]["abort"].toString().toStdString();

        states[s->id] = s;
    }

    return states;
}


void ProcessGateway::parseStateChecks(QJsonValue checks, std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators, State* s, Transition t) {
    for (QString k : checks.toObject().keys()) {
        std::string id = k.toStdString();
        if (sensors.find(id) != sensors.end()) {
            QJsonValue range = checks[k];
            if (range.isArray()) {
                s->sensorChecks[t][id] = SensorCheck {
                    range.toArray()[0].toInt(),
                    range.toArray()[1].toInt()
                };
            } else {
                // not array error here
            }
        } else if (actuators.find(id) != actuators.end()) {
            QString check = checks[k].toString();
            if (check == "close") {
                s->actuatorChecks[t][id] = ActuatorCheck { false };
            } else if (check == "open") {
                s->actuatorChecks[t][id] = ActuatorCheck { true };
            } else {
                // invalid check error here
            }
        } else {
            // invalid key error here
        }
    }
}
