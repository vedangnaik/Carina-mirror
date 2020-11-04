#include "layer3.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QJsonArray>

void ProcessController::transition(std::string alphabet) {
    this->ucip->transition(alphabet);
}


void ProcessGateway::parseProcessFile(std::string fileName, ProcessManager* pm, SensorsManager* sm) {
    QFile file(QString::fromStdString(fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << file.errorString().toStdString() << std::endl;
    }
    QString val = file.readAll();
    file.close();
    QJsonObject jsonObj = QJsonDocument::fromJson(val.toUtf8()).object();

    QJsonObject sensors = jsonObj.value("sensors").toObject();
    for (QString k: sensors.keys()) {
        QJsonObject v = sensors.value(k).toObject();
        std::string id = k.toStdString();
        std::string name = v.value("name").toString().toStdString();
        sm->createSensor(id, name);
    }


    QJsonObject states = jsonObj.value("states").toObject();
    if (!states.contains("start")) { return; }

    std::vector<StateDTO> Q;
    for (QString k: states.keys()) {
        QJsonObject v = states.value(k).toObject();
        struct StateDTO s;

        s.id = k.toStdString();
        s.name = v.value("name").toString().toStdString();
        s.safetyRating = v.value("safetyRating").toString().toStdString();
        s.description = v.value("description").toString().toStdString();

        std::vector<std::string> actions = {};
        for (auto a: v.value("actions").toArray()) {
            actions.push_back(a.toString().toStdString());
        }
        s.actions = actions;

        QJsonObject transitions = v.value("transitions").toObject();
        s.proceedState = transitions.value("proceed").toString().toStdString();
        s.abortState = transitions.value("abort").toString().toStdString();

        Q.push_back(s);
    }
    pm->createProcess(Q);
}
