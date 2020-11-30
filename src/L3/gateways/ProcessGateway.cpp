#include "ProcessGateway.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QJsonArray>
#include <QFileDialog>


struct ProcessData ProcessGateway::parseProcessFile() {
    QFile file(QString::fromStdString(this->filepath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // error here
    }
    QString val = file.readAll();
    file.close();

    QJsonObject jsonObj = QJsonDocument::fromJson(val.toUtf8()).object();
    std::map<std::string, Sensor*> sensors = this->parseSensors(jsonObj["sensors"].toObject());
    std::map<std::string, Actuator*> actuators = this->parseActuators(jsonObj["actuators"].toObject());
    std::map<std::string, State*> states = this->parseStates(jsonObj["states"].toObject(), sensors, actuators);
    return ProcessData {
        sensors,
        actuators,
        states
    };
}


std::map<std::string, Sensor*> ProcessGateway::parseSensors(QJsonObject sensorsObj) {
    std::map<std::string, Sensor*> sensors = {};
    for (QString k: sensorsObj.keys()) {   
        sensors[k.toStdString()] = new Sensor(k.toStdString(), sensorsObj[k].toObject()["name"].toString().toStdString());
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

        std::string id = k.toStdString();
        std::string name = v["name"].toString().toStdString();
        std::string safetyRating = v["safetyRating"].toString().toStdString();
        std::string description = v["description"].toString().toStdString();

        std::vector<std::string> actionsOrder = {};
        std::map<std::string, std::vector<SensorOption>> sensorOptions = {};
        std::map<std::string, std::vector<ActuatorOption>> actuatorOptions = {};
        for (QJsonValue action: v["actions"].toArray()) {
            std::string id = action["id"].toString().toStdString();
            if (id == "") {
                // error here
            }

            actionsOrder.push_back(id);
            if (sensors.find(id) != sensors.end()) {
                sensorOptions[id] = {};
                // add sensor options here
            } else if (actuators.find(id) != actuators.end()) {
                actuatorOptions[id] = {};
                if (action["timed"].toBool()) {
                    actuatorOptions[id].push_back(ActuatorOption::Timed);
                }
                if (action["automatic"].toBool()) {
                    actuatorOptions[id].push_back(ActuatorOption::Automatic);
                }
            } else {
                // error here
            }
        }

        std::map<Transition, std::map<std::string, SensorCheck>> sensorChecks = {
            {Transition::Proceed, {}},
            {Transition::Abort, {}}
        };
        std::map<Transition, std::map<std::string, ActuatorCheck>> actuatorChecks = {
            {Transition::Proceed, {}},
            {Transition::Abort, {}}
        };
        if (v["checks"]["proceed"].isObject()) {
            sensorChecks[Transition::Proceed] = parseSensorChecks(v["checks"]["proceed"], sensors);
            actuatorChecks[Transition::Proceed] = parseActuatorChecks(v["checks"]["proceed"], actuators);
        } if (v["checks"]["abort"].isObject()) {
            sensorChecks[Transition::Abort] = parseSensorChecks(v["checks"]["abort"], sensors);
            actuatorChecks[Transition::Abort] = parseActuatorChecks(v["checks"]["abort"], actuators);
        }

        std::map<Transition, std::string> transitions = {
            { Transition::Proceed, v["transitions"]["proceed"].toString().toStdString() },
            { Transition::Abort, v["transitions"]["abort"].toString().toStdString() }
        };

        states[id] = new State(id, name, safetyRating, description, actionsOrder, sensorOptions, actuatorOptions, sensorChecks, actuatorChecks, transitions);
    }

    return states;
}

std::map<std::string, SensorCheck> ProcessGateway::parseSensorChecks(QJsonValue checks, std::map<std::string, Sensor*> sensors) {
    std::map<std::string, SensorCheck> sensorChecks;
    for (QString k : checks.toObject().keys()) {
        std::string id = k.toStdString();
        if (sensors.find(id) != sensors.end()) {
            QJsonValue range = checks[k];
            if (range.isArray()) {
                sensorChecks[id] = SensorCheck {
                    range.toArray()[0].toInt(),
                    range.toArray()[1].toInt()
                };
            } else {
                // not array error here
            }
        } else {
            // ignore, not a sensor
        }
    }
    return sensorChecks;
}


std::map<std::string, ActuatorCheck> ProcessGateway::parseActuatorChecks(QJsonValue checks, std::map<std::string, Actuator*> actuators) {
    std::map<std::string, ActuatorCheck> actuatorChecks;
    for (QString k : checks.toObject().keys()) {
        std::string id = k.toStdString();
        if (actuators.find(id) != actuators.end()) {
            QString check = checks[k].toString();
            if (check == "close") {
                actuatorChecks[id] = ActuatorCheck { false };
            } else if (check == "open") {
                actuatorChecks[id] = ActuatorCheck { true };
            } else {
                // invalid check error here
            }
        } else {
            // ignore, not an actuator
        }
    }
    return actuatorChecks;
}
