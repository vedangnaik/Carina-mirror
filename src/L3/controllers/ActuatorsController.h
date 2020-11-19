#ifndef ACTUATORCONTROLLER_H
#define ACTUATORCONTROLLER_H

#include <QObject>
#include "src/L2/usecases/ActuatorsManager.h"
#include "src/L3/gateways/ActuatorStatusGateway.h"

class ACIC : public QObject {
    Q_OBJECT
public:
    virtual void actuate(std::string id) = 0;
};

class ActuatorsController : public ACIC {
public:
    ActuatorsController(AMIC& amic) : amic(amic) {};
    void actuate(std::string id);
private:
   AMIC& amic;
};

#endif // ACTUATORCONTROLLER_H
