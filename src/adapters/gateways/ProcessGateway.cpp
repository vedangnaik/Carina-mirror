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
    return ProcessData {
        this->parseSensors(jsonObj["sensors"].toObject()),
        this->parseActuators(jsonObj["actuators"].toObject()),
        this->parseStates(jsonObj["states"].toObject())
    };
}


std::vector<Sensor*> ProcessGateway::parseSensors(QJsonObject sensorsObj) {
    std::vector<Sensor*> sensors = {};
    for (QString k: sensorsObj.keys()) {
        sensors.push_back(
            new Sensor(
                k.toStdString(),
                sensorsObj[k].toObject()["name"].toString().toStdString()
            )
        );
    }
    return sensors;
}


std::vector<Actuator*> ProcessGateway::parseActuators(QJsonObject actuatorsObj) {
    std::vector<Actuator*> actuators = {};
    for (QString k: actuatorsObj.keys()) {
        actuators.push_back(
            new Actuator(
                k.toStdString(),
                actuatorsObj[k].toObject()["name"].toString().toStdString()
            )
        );
    }
    return actuators;
}


std::vector<State*> ProcessGateway::parseStates(QJsonObject statesObj) {
    std::vector<State*> states;
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

            std::vector<unsigned int> actionOptions = {};
            bool timed = action["timed"].toBool();
            bool automatic = action["automatic"].toBool();
            QString check_position = action["check_position"].toString();

            if (timed) actionOptions.push_back(ActuatorOption::Timed);
            if (automatic) actionOptions.push_back(ActuatorOption::Automatic);
            // Same for any new options here

            s->actionOptions.push_back(std::make_pair(id, actionOptions));
        }

        s->transitions[Transition::Proceed] = v["transitions"]["proceed"].toString().toStdString();
        s->transitions[Transition::Abort] = v["transitions"]["abort"].toString().toStdString();

        s->actionsChecks[Transition::Proceed] = this->parseStateChecks(v["checks"]["proceed"].toObject());
        s->actionsChecks[Transition::Abort] = this->parseStateChecks(v["checks"]["proceed"].toObject());

        states.push_back(s);
    }
    return states;
}


std::map<std::string, std::vector<unsigned int>> ProcessGateway::parseStateChecks(QJsonObject checksObj) {
    std::map<std::string, std::vector<unsigned int>> actionsChecks;
    for (QString id: checksObj.keys()) {
        QJsonValue check = checksObj[id];
        std::vector<unsigned int> checks = {};

        if (check.isString()) {
            if (check.toString() == "open") {
                checks.push_back(ActuatorCheck::Open);
            } else if (check.toString() == "close") {
                checks.push_back(ActuatorCheck::Close);
            } else {
                // error handling here
            }
        } else if (check.isArray()) {
            // sensor range check here
        }

        actionsChecks[id.toStdString()] = checks;
    }
    return actionsChecks;
}
