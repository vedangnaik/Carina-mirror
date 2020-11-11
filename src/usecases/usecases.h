#ifndef USECASES_H
#define USECASES_H

#include "src/entities/entities.h"

//====

class SMOC {
public:
};

class SMIC {
public:
    virtual void addSensors(std::vector<Sensor*> sensors) = 0;
    virtual Sensor* findSensor(std::string id) = 0;
};

class SensorsManager : public SMIC {
public:
    void addSensors(std::vector<Sensor*> sensors);
    Sensor* findSensor(std::string id);
private:
    std::map<std::string, Sensor*> sensors;
    SMOC* smoc;
};

//====

class AMOC {
public:
};

class AMIC {
public:
    virtual void addActuators(std::vector<Actuator*> actuators) = 0;
    virtual Actuator* findActuator(std::string id) = 0;
    virtual void actuate(std::string id) = 0;
};

class ActuatorsManager : public AMIC {
public:
    void addActuators(std::vector<Actuator*> actuators);
    Actuator* findActuator(std::string id);
    void actuate(std::string id);
private:
    std::map<std::string, Actuator*> actuators;
    AMOC* amoc;
};

//====

class PMOC {
public:
    virtual void displayProcessSummary(std::vector<std::string> processSummary) = 0;;
    virtual void displayState(State* s) = 0;
    virtual ~PMOC() {};
};

class PMIC {
public:
    virtual void createProcess(std::vector<State*> states) = 0;
    virtual void startProcess() = 0;
    virtual void transition(Transition t) = 0;
    virtual ~PMIC() {};
};

class ProcessManager : public PMIC {
public:
    ProcessManager(AMIC* amic, SMIC* smic);
    void createProcess(std::vector<State*> Q);
    void startProcess();
    void transition(Transition t);
    void setOutputContract(PMOC* pmoc);
private:
    Process* p = NULL;
    PMOC* pmoc;
    AMIC* amic;
    SMIC* smic;
};

#endif // USECASES_H
