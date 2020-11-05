#ifndef PROCESSPRESENTER_H
#define PROCESSPRESENTER_H

#include "usecases.h"

class PPOC {
public:
    virtual void displayState(State* s) = 0;
    virtual ~PPOC() {};
};


class ProcessPresenter : public PMOC {
public:
//    ProcessPresenter();
    void displayState(State* s);

    PPOC* ppoc;
};

#endif // PROCESSPRESENTER_H
