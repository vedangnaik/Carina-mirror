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

    struct ProcessData pdata;
    QString val = file.readAll();
    file.close();
    QJsonObject jsonObj = QJsonDocument::fromJson(val.toUtf8()).object();

    QJsonObject sensorsObj = jsonObj.value("sensors").toObject();
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
    pdata.sensors = sensors;

    // Same for actuators here
    std::vector<Actuator*> actuators = {};
    pdata.actuators = actuators;

    // Now for states
    QJsonObject statesObj = jsonObj.value("states").toObject();
    std::vector<State*> states;
    for (QString k: statesObj.keys()) {
        QJsonObject v = statesObj.value(k).toObject();
        State* s = new State();

        s->id = k.toStdString();
        s->name = v.value("name").toString().toStdString();
        s->safetyRating = v.value("safetyRating").toString().toStdString();
        s->description = v.value("description").toString().toStdString();
        for (auto a: v.value("actions").toArray()) {
            s->actions.push_back(a.toString().toStdString());
        }
        QJsonObject transitions = v.value("transitions").toObject();
        s->proceedState = transitions.value("proceed").toString().toStdString();
        s->abortState = transitions.value("abort").toString().toStdString();

        states.push_back(s);
    }
    pdata.states = states;

    return pdata;
}
