#include "ProcessGateway.h"

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
        actuators[k.toStdString()] = new Actuator(k.toStdString(), actuatorsObj[k].toObject()["name"].toString().toStdString());
    }
    return actuators;
}


std::map<std::string, State*> ProcessGateway::parseStates(QJsonObject statesObj, std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators) {
    std::map<std::string, State*> states;
    for (QString k: statesObj.keys()) {
        QJsonValue v = statesObj[k];

        std::string stateID = k.toStdString();
        std::string name = v["name"].toString().toStdString();
        std::string safetyRating = v["safetyRating"].toString().toStdString();
        std::string description = v["description"].toString().toStdString();

        std::vector<std::string> actionsOrder = {};
        std::map<std::string, std::vector<SensorOption>> sensorOptions = {};
        std::map<std::string, std::vector<ActuatorOption>> actuatorOptions = {};
        for (QJsonValue action: v["actions"].toArray()) {
            std::string actionID = action["id"].toString().toStdString();
            if (actionID == "") {
                throw EmptyActionIDError(stateID);
            }

            actionsOrder.push_back(actionID);
            if (sensors.find(actionID) != sensors.end()) {
                sensorOptions[actionID] = {};
                // add sensor options here, if any
            } else if (actuators.find(actionID) != actuators.end()) {
                actuatorOptions[actionID] = {};
                if (action["timed"].toBool()) {
                    actuatorOptions[actionID].push_back(ActuatorOption::Timed);
                }
                if (action["automatic"].toBool()) {
                    actuatorOptions[actionID].push_back(ActuatorOption::Automatic);
                }
            } else {
                throw InvalidActionIDError(stateID, actionID);
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

        // Parse actions checks.
        try {
            if (v["checks"]["proceed"].isObject()) {
                sensorChecks[Transition::Proceed] = parseSensorChecks(v["checks"]["proceed"], sensors);
                actuatorChecks[Transition::Proceed] = parseActuatorChecks(v["checks"]["proceed"], actuators);
            } if (v["checks"]["abort"].isObject()) {
                sensorChecks[Transition::Abort] = parseSensorChecks(v["checks"]["abort"], sensors);
                actuatorChecks[Transition::Abort] = parseActuatorChecks(v["checks"]["abort"], actuators);
            }
        }  catch (InvalidSensorRangeCheck& e) {
            throw InvalidSensorRangeCheckError(stateID, e.sensorID);
        } catch (InvalidActuatorPositionCheck& e) {
            throw InvalidActuatorPositionCheckError(stateID, e.actuatorID);
        }

        std::map<Transition, std::string> transitions = {
            { Transition::Proceed, v["transitions"]["proceed"].toString().toStdString() },
            { Transition::Abort, v["transitions"]["abort"].toString().toStdString() }
        };

        states[stateID] = new State(stateID, name, safetyRating, description, actionsOrder, sensorOptions, actuatorOptions, sensorChecks, actuatorChecks, transitions);
    }

    return states;
}

std::map<std::string, SensorCheck> ProcessGateway::parseSensorChecks(QJsonValue checks, std::map<std::string, Sensor*> sensors) {
    std::map<std::string, SensorCheck> sensorChecks;
    for (QString k : checks.toObject().keys()) {
        std::string sensorID = k.toStdString();
        if (sensors.find(sensorID) != sensors.end()) {
            QJsonValue range = checks[k];
            if (range.isArray()) {
                sensorChecks[sensorID] = SensorCheck {
                    range.toArray()[0].toInt(),
                    range.toArray()[1].toInt()
                };
            } else {
                throw InvalidSensorRangeCheck(sensorID);
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
        std::string actuatorID = k.toStdString();
        if (actuators.find(actuatorID) != actuators.end()) {
            QString check = checks[k].toString();
            if (check == "close") {
                actuatorChecks[actuatorID] = ActuatorCheck { false };
            } else if (check == "open") {
                actuatorChecks[actuatorID] = ActuatorCheck { true };
            } else {
                throw InvalidActuatorPositionCheck(actuatorID);
            }
        } else {
            // ignore, not an actuator
        }
    }
    return actuatorChecks;
}
