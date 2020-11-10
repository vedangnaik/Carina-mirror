#include "gateways.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QJsonArray>
#include <QFileDialog>


struct ProcessData ProcessGateway::parseProcessFile(std::string fileName) {
    QFile file(QString::fromStdString(fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << file.errorString().toStdString() << std::endl;
    }
    QString val = file.readAll();
    file.close();
    QJsonObject jsonObj = QJsonDocument::fromJson(val.toUtf8()).object();
    return ProcessData {
        this->parseSensors(jsonObj.value("sensors").toObject()),
        this->parseActuator(jsonObj.value("actuators").toObject()),
        this->parseState(jsonObj.value("states").toObject())
    };
}


std::vector<Sensor*> ProcessGateway::parseSensors(QJsonObject sensorsObj) {
    std::vector<Sensor*> sensors = {};
    for (QString k: sensorsObj.keys()) {
        QJsonObject sensorObj = sensorsObj.value(k).toObject();
        sensors.push_back(
            new Sensor(
                k.toStdString(),
                sensorObj.value("name").toString().toStdString()
            )
        );
    }
    return sensors;
}


std::vector<Actuator*> ProcessGateway::parseActuator(QJsonObject actuatorsObj) {
    std::vector<Actuator*> actuators = {};
    for (QString k: actuatorsObj.keys()) {
        QJsonObject actuatorObj = actuatorsObj.value(k).toObject();
        actuators.push_back(
            new Actuator(
                k.toStdString(),
                actuatorObj.value("name").toString().toStdString()
            )
        );
    }
    return actuators;
}


std::vector<State*> ProcessGateway::parseState(QJsonObject statesObj) {
    std::vector<State*> states;
    for (QString k: statesObj.keys()) {
        QJsonObject v = statesObj.value(k).toObject();
        State* s = new State();
        s->id = k.toStdString();
        s->name = v.value("name").toString().toStdString();
        s->safetyRating = v.value("safetyRating").toString().toStdString();
        s->description = v.value("description").toString().toStdString();

        for (auto a: v.value("actions").toArray()) {
            QJsonObject action = a.toObject();
            std::string id = action.value("id").toString().toStdString();
            if (id == "") {
                // error here
            }

            std::vector<unsigned int> actionOptions = {};
            bool timed = action.value("timed").toBool();
            bool automatic = action.value("automatic").toBool();
            QString check_position = action.value("check_position").toString();

            if (timed) actionOptions.push_back(ActuatorOption::Timed);
            if (automatic) actionOptions.push_back(ActuatorOption::Automatic);
            // Same for any new options here

            s->actionOptions.push_back(std::make_pair(id, actionOptions));
        }

        QJsonObject transitions = v.value("transitions").toObject();
        s->transitions[Transition::Proceed] = transitions.value("proceed").toString().toStdString();
        s->transitions[Transition::Abort] = transitions.value("abort").toString().toStdString();

        QJsonObject proceedChecks = v.value("checks").toObject().value("proceed").toObject();
        std::map<std::string, std::vector<unsigned int>> pc;
        for (QString id: proceedChecks.keys()) {
            std::vector<unsigned int> checks = {};
            if (proceedChecks.value(id).toString() == "open") {
                checks.push_back(ActuatorCheck::Open);
            } else if (proceedChecks.value(id).toString() == "close") {
                checks.push_back(ActuatorCheck::Close);
            } else {
                // shit
            }
            pc[id.toStdString()] = checks;
            s->actionChecks[Transition::Proceed] = pc;
        }

        QJsonObject abortChecks = v.value("checks").toObject().value("abort").toObject();

        states.push_back(s);
    }
    return states;
}
