#ifndef SENSORSCONTROLLER_H
#define SENSORSCONTROLLER_H

#include <QObject>
#include "src/usecases/SensorsManager.h"

class SCIC : public QObject {
    Q_OBJECT
public:
    virtual void updateValue(std::string id, float value) = 0;
    virtual ~SCIC() {}
};


class SensorsController : public SCIC {
public:
    SensorsController(SMIC* smic);
    void updateValue(std::string id, float value);
private:
    SMIC* smic;
};

#endif // SENSORSCONTROLLER_H
