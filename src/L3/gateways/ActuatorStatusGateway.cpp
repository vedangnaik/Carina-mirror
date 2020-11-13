#include "ActuatorStatusGateway.h"

ActuatorStatusGateway::ActuatorStatusGateway(std::string statusFilePath) {
    this->statusFilePath = statusFilePath;
    if (!QFileInfo::exists(QString::fromStdString(this->statusFilePath)) ||
            !QFileInfo(QString::fromStdString(this->statusFilePath)).isFile()) {
        QFile file(QString::fromStdString(this->statusFilePath));
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            // exception out here, shit
            return;
        }
    } else {
        // the file exists, you can open and hopefully read it normally
    }
}

void ActuatorStatusGateway::updateActutatorStatus(std::string id, bool status) {
    QFile file(QString::fromStdString(this->statusFilePath));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // shit, exception here
            return;
        }
        QString val = file.readAll();
        file.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject jsonObj = jsonDoc.object();
        jsonObj.insert(QString::fromStdString(id), QJsonValue(status));
        jsonDoc.setObject(jsonObj);

        if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
            // shit, exception here
            return;
        }
        file.write(jsonDoc.toJson());
        file.close();
}
