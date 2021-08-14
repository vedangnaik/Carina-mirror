#include "ProcessFileParser.h"

ProcessFileParser::ProcessFileParser(const std::string filepath)
    : filepath(filepath)
{
    // TODO: Find some C++14 or lower way to acheive this
    //    std::filesystem::path f = this->filepath;
    //    if (f.extension() != ".json") {
    //        throw InvalidFileTypeError(this->filepath);
    //    }
}

std::tuple<
    const std::map<const std::string, QVariantMap>, 
    const std::map<const std::string, QVariantMap>, 
    const std::map<const std::string, const State>
>
ProcessFileParser::parseProcessFile()
{
    QFile file(QString::fromStdString(this->filepath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.close();
        throw FileOpenError(this->filepath);
    }
    QString val = file.readAll();
    file.close();

    QJsonObject jsonObj = QJsonDocument::fromJson(val.toUtf8()).object();
    // Parse the sensor and actuator IDs and arguments needs for the constructor
    auto sensors = this->parseSensors(jsonObj["sensors"].toObject());
    auto actuators = this->parseActuators(jsonObj["actuators"].toObject());
    // Retreive just the IDs for the States parsing
    std::vector<std::string> sensorIDs, actuatorIDs;
    for (const auto& p : sensors)   { sensorIDs.push_back(p.first);   }
    for (const auto& p : actuators) { actuatorIDs.push_back(p.first); }
    auto states = this->parseStates(jsonObj["states"].toObject(), sensorIDs, actuatorIDs);

    return std::make_tuple(sensors, actuators, states);
}

const std::map<const std::string, QVariantMap>
ProcessFileParser::parseSensors(const QJsonObject& sensorsObj)
{
    std::map<const std::string, QVariantMap> sensors;
    for (QString sensorID: sensorsObj.keys()) {
        if (sensorID == "") {
            throw EmptySensorIDError();
        }
        // It appears the QJSONObject removes duplicate keys by itself.
        // This isn't ideal, since it'd be better to warn the user of duplicate keys here.
        // In case their JSON linting software hasn't already done so.
        sensors.insert({
            sensorID.toStdString(),
            sensorsObj[sensorID].toObject().toVariantMap()
        });
    }
    return sensors;
}

const std::map<const std::string, QVariantMap>
ProcessFileParser::parseActuators(const QJsonObject& actuatorsObj)
{
    std::map<const std::string, QVariantMap> actuators;
    for (QString actuatorID: actuatorsObj.keys()) {
        if (actuatorID == "") {
            throw EmptyActuatorIDError();
        }

        actuators.insert({
             actuatorID.toStdString(),
             actuatorsObj[actuatorID].toObject().toVariantMap()
        });
    }
    return actuators;
}


const std::map<const std::string, const State>
ProcessFileParser::parseStates(
    const QJsonObject& statesObj, 
    const std::vector<const std::string> sensorIDs, 
    const std::vector<const std::string> actuatorIDs
)
{
    std::map<const std::string, const State> states;
    for (QString k: statesObj.keys()) {
        QJsonValue v = statesObj[k];

        std::string stateID = k.toStdString();
        if (stateID == "") {
            throw EmptyStateIDError();
        }

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
            if (std::find(sensorIDs.begin(), sensorIDs.end(), actionID) != sensorIDs.end()) {
                sensorOptions.insert({ actionID, {} });
                // add sensor options here, if any
            } else if (std::find(actuatorIDs.begin(), actuatorIDs.end(), actionID) != actuatorIDs.end()) {
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
        std::map<Transition, std::map<std::string, SensorCheck>> sensorChecks;
        std::map<Transition, std::map<std::string, ActuatorCheck>> actuatorChecks;
        try {
            // proceed checks; even if it's empty, the key still needs to be there.
            sensorChecks.insert({Transition::Proceed, parseSensorChecks(v["checks"]["proceed"], sensorIDs)});
            actuatorChecks.insert({Transition::Proceed, parseActuatorChecks(v["checks"]["proceed"], actuatorIDs)});
            // abort checks; same as above.
            sensorChecks.insert({Transition::Abort, parseSensorChecks(v["checks"]["abort"], sensorIDs)});
            actuatorChecks.insert({Transition::Abort, parseActuatorChecks(v["checks"]["abort"], actuatorIDs)});
        }  catch (InvalidSensorRangeCheck& e) {
            throw InvalidSensorRangeCheckError(stateID, e.sensorID);
        } catch (InvalidActuatorPositionCheck& e) {
            throw InvalidActuatorPositionCheckError(stateID, e.actuatorID);
        }

        std::map<Transition, std::string> transitions = {
            { Transition::Proceed, v["transitions"]["proceed"].toString().toStdString() },
            { Transition::Abort, v["transitions"]["abort"].toString().toStdString() }
        };

        states.insert({stateID, State(stateID, name, safetyRating, description, actionsOrder, sensorOptions, actuatorOptions, sensorChecks, actuatorChecks, transitions)});
    }

    return states;
}

std::map<std::string, SensorCheck>
ProcessFileParser::parseSensorChecks(const QJsonValue& checks, const std::vector<const std::string> sensorIDs)
{
    if (checks.toObject().isEmpty()) {
        return {};
    }

    std::map<std::string, SensorCheck> sensorChecks;
    for (QString k : checks.toObject().keys()) {
        std::string sensorID = k.toStdString();
        if (std::find(sensorIDs.begin(), sensorIDs.end(), sensorID) != sensorIDs.end()) {
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


std::map<std::string, ActuatorCheck>
ProcessFileParser::parseActuatorChecks(const QJsonValue& checks, const std::vector<const std::string> actuatorIDs)
{
    if (checks.toObject().isEmpty()) {
        return {};
    }

    std::map<std::string, ActuatorCheck> actuatorChecks;
    for (QString k : checks.toObject().keys()) {
        std::string actuatorID = k.toStdString();
        if (std::find(actuatorIDs.begin(), actuatorIDs.end(), actuatorID) != actuatorIDs.end()) {
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
