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
#include "src/L3/presenters/SensorsPresenter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class State;
}
QT_END_NAMESPACE

class StateUIHandler : public QWidget, public StPOC, public SPOC {
    Q_OBJECT
public:
    StateUIHandler(Ui::State& stateUI, ACIC& acic, StCIC& stcic, ClocksModule& cm);

    void displayProcessSummary(const std::vector<std::string> processSummary);
    void displayState(const State& s);
    void allowProceed(bool permission);
    void allowAbort(bool permission);

    void displaySensorValue(const std::string id, const float value);
private:
    QLabel* displayTimedActuator(QPushButton* aButton);
    void subscribe(std::string id, QLabel* label);

    std::map<std::string, QLabel*> sensorDisplaySubscribers;

    Ui::State& stateUI;
    ACIC& acic;
    StCIC& stcic;
    ClocksModule& cm;
};

#endif // STATEUIHANDLER_H
