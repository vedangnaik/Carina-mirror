#pragma once

#include <QObject>
#include "ActuatorsManager.h"

class ACIC : public QObject {
    Q_OBJECT
public:
    virtual void actuate(std::string id) = 0;
    virtual ~ACIC() {};
};

class ActuatorsController : public ACIC {
public:
    ActuatorsController(AMIC& amic) : amic(amic) {};
    void actuate(std::string id);
private:
   AMIC& amic;
};
