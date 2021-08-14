#pragma once

#include <QObject>
#include "ActuatorsManager.h"

class ACIC : public QObject {
    Q_OBJECT
public:
    virtual void setState(std::string id, bool status) = 0;
    virtual ~ACIC() {};
};

class ActuatorsController : public ACIC {
public:
    ActuatorsController(AMIC& amic);
    void setState(std::string id, bool status) override;
private:
   AMIC& amic;
};
