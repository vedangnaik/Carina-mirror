#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "usecases.h"


class ProcessController {
public:
    ProcessController(ProcessManager* ucip) {
        this->ucip = ucip;
    }

    void transition(std::string alphabet);

private:
    ProcessManager* ucip;
};


class ProcessGateway {
public:
    void parseProcessFile(std::string fileName, ProcessManager* pm, SensorsManager* sm);
private:
};

#endif // CONTROLLERS_H
