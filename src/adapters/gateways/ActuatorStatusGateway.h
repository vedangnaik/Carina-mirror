#ifndef ACTUATORSTATUSGATEWAY_H
#define ACTUATORSTATUSGATEWAY_H

#include <string>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

class ActuatorStatusGateway
{
public:
    ActuatorStatusGateway(std::string statusFilePath);
    void updateActutatorStatus(std::string id, bool status);
private:
    std::string statusFilePath;
};

#endif // ACTUATORSTATUSGATEWAY_H
