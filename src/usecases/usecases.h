#ifndef USECASES_H
#define USECASES_H

#include "src/entities/entities.h"


class PMOC {
public:
    virtual void displayState(State* s) = 0;
    virtual ~PMOC() {};
};

class PMIC {
public:
    virtual void createProcess(std::vector<State*> states) = 0;
    virtual void transition(std::string transition) = 0;
    virtual ~PMIC() {};
};

class ProcessManager : public PMIC {
public:
//    ProcessManager();
    void createProcess(std::vector<State*> Q);
    void transition(std::string transition);

    Process* p = NULL;
    PMOC* pmoc;
};

//struct StateDTO {
//    std::string id;
//    std::string name;
//    std::string safetyRating;
//    std::string description;
//    std::vector<std::string> actions;
//    std::string proceedState;
//    std::string abortState;
//};

#endif // USECASES_H
