#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "usecases.h"


class ProcessGateway {
public:
    void parseProcessFile(std::string fileName, ProcessManager* pm, SensorsManager* sm);
};

#endif // CONTROLLERS_H
