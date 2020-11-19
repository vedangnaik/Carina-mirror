#ifndef STATEUIHANDLER_H
#define STATEUIHANDLER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include "ui_state.h"
#include "src/L2/services/ClocksModule.h"
#include "src/L3/controllers/ActuatorsController.h"
#include "src/L3/controllers/StatesController.h"
#include "src/L3/presenters/StatesPresenter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class State;
}
QT_END_NAMESPACE

class StateUIHandler : public QWidget, public StPOC {
    Q_OBJECT
public:
    StateUIHandler(Ui::State* stateUI, ACIC& acic, StCIC& stcic, ClocksModule& cm);
    ~StateUIHandler();

    void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(const State& s);
    void allowProceed(bool permission);
    void allowAbort(bool permission);
private:
    QLabel* timedActuatorHandler(QPushButton* aButton);

    Ui::State* stateUI;
    ACIC& acic;
    StCIC& stcic;
    ClocksModule& cm;
};

#endif // STATEUIHANDLER_H
