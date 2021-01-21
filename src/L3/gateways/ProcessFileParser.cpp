#include "ProcessFileParser.h"

ProcessFileParser::ProcessFileParser(const string filepath)
    : filepath(filepath)
{
    // TODO: Find some C++14 or lower way to acheive this
    //    std::filesystem::path f = this->filepath;
    //    if (f.extension() != ".json") {
    //        throw InvalidFileTypeError(this->filepath);
    //    }
}

std::tuple<const map<const string, Sensor>, const map<const string, Actuator>, const map<const string, const State> > ProcessFileParser::parseProcessFile()
{
    QFile file(QString::fromStdString(this->filepath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.close();
        throw FileOpenError(this->filepath);
    }
    QString val = file.readAll();
    file.close();

    QJsonObject jsonObj = QJsonDocument::fromJson(val.toUtf8()).object();
    auto sensors = this->parseSensors(jsonObj["sensors"].toObject());
    auto actuators = this->parseActuators(jsonObj["actuators"].toObject());
    auto states = this->parseStates(jsonObj["states"].toObject(), sensors, actuators);
    return std::make_tuple(sensors, actuators, states);
}

const map<const string, Sensor>
ProcessFileParser::parseSensors(const QJsonObject& sensorsObj)
{
    map<const string, Sensor> sensors = {};
    for (QString sensorID: sensorsObj.keys()) {
        if (sensorID == "") {
            throw EmptySensorIDError();
        }

        // It appears the QJSONObject removes duplicate keys by itself.
        // This isn't ideal, since it'd be better to warn the user of duplicate keys here.
        // In case their JSON linting software hasn't already done so.
        sensors.insert({
            sensorID.toStdString(),
            Sensor(sensorID.toStdString(), sensorsObj[sensorID].toObject()["name"].toString().toStdString())
        });
    }
    return sensors;
}

const map<const string, Actuator>
ProcessFileParser::parseActuators(const QJsonObject& actuatorsObj)
{
    map<const string, Actuator> actuators;
    for (QString actuatorID: actuatorsObj.keys()) {
        if (actuatorID == "") {
            throw EmptyActuatorIDError();
        }

        actuators.insert({
             actuatorID.toStdString(),
             Actuator(actuatorID.toStdString(), actuatorsObj[actuatorID].toObject()["name"].toString().toStdString())
        });
    }
    return actuators;
}


const map<const string, const State>
ProcessFileParser::parseStates(const QJsonObject& statesObj, const map<const string, Sensor>& sensors, const map<const string, Actuator>& actuators)
{
    map<const string, const State> states;
    for (QString k: statesObj.keys()) {
        QJsonValue v = statesObj[k];

        string stateID = k.toStdString();
        if (stateID == "") {
            throw EmptyStateIDError();
        }

        string name = v["name"].toString().toStdString();
        string safetyRating = v["safetyRating"].toString().toStdString();
        string description = v["description"].toString().toStdString();

        std::vector<string> actionsOrder = {};
        map<string, std::vector<SensorOption>> sensorOptions = {};
        map<string, std::vector<ActuatorOption>> actuatorOptions = {};
        for (QJsonValue action: v["actions"].toArray()) {
            string actionID = action["id"].toString().toStdString();
            if (actionID == "") {
                throw EmptyActionIDError(stateID);
            }

            actionsOrder.push_back(actionID);
            if (sensors.find(actionID) != sensors.end()) {
                sensorOptions.insert({ actionID, {} });
                // add sensor options here, if any
            } else if (actuators.find(actionID) != actuators.end()) {
                actuatorOptions.insert({ actionID, {} });
                if (action["timed"].toBool()) {
                    actuatorOptions.at(actionID).push_back(ActuatorOption::Timed);
                }
                if (action["automatic"].toBool()) {
                    actuatorOptions.at(actionID).push_back(ActuatorOption::Automatic);
                }
            } else {
                throw InvalidActionIDError(stateID, actionID);
            }
        }

        // Parse actions checks.
        map<Transition, map<string, SensorCheck>> sensorChecks;
        map<Transition, map<string, ActuatorCheck>> actuatorChecks;
        try {
            // proceed checks; even if it's empty, the key still needs to be there.
            sensorChecks.insert({Transition::Proceed, parseSensorChecks(v["checks"]["proceed"], sensors)});
            actuatorChecks.insert({Transition::Proceed, parseActuatorChecks(v["checks"]["proceed"], actuators)});
            // abort checks; same as above.
            sensorChecks.insert({Transition::Abort, parseSensorChecks(v["checks"]["abort"], sensors)});
            actuatorChecks.insert({Transition::Abort, parseActuatorChecks(v["checks"]["abort"], actuators)});
        }  catch (InvalidSensorRangeCheck& e) {
            throw InvalidSensorRangeCheckError(stateID, e.sensorID);
        } catch (InvalidActuatorPositionCheck& e) {
            throw InvalidActuatorPositionCheckError(stateID, e.actuatorID);
        }

        map<Transition, string> transitions = {
            { Transition::Proceed, v["transitions"]["proceed"].toString().toStdString() },
            { Transition::Abort, v["transitions"]["abort"].toString().toStdString() }
        };

        states.insert({stateID, State(stateID, name, safetyRating, description, actionsOrder, sensorOptions, actuatorOptions, sensorChecks, actuatorChecks, transitions)});
    }

    return states;
}

map<string, SensorCheck>
ProcessFileParser::parseSensorChecks(const QJsonValue& checks, const map<const string, Sensor>& sensors)
{
    if (checks.toObject().isEmpty()) {
        return {};
    }

    map<string, SensorCheck> sensorChecks;
    for (QString k : checks.toObject().keys()) {
        string sensorID = k.toStdString();
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


map<string, ActuatorCheck>
ProcessFileParser::parseActuatorChecks(const QJsonValue& checks, const map<const string, Actuator>& actuators)
{
    if (checks.toObject().isEmpty()) {
        return {};
    }

    map<string, ActuatorCheck> actuatorChecks;
    for (QString k : checks.toObject().keys()) {
        string actuatorID = k.toStdString();
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
