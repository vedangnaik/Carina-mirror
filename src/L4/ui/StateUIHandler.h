#ifndef STATEUIHANDLER_H
#define STATEUIHANDLER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include "src/L2/services/ClocksModule.h"
#include "src/L3/controllers/ActuatorsController.h"
#include "src/L3/controllers/StatesController.h"
#include "src/L3/presenters/StatesPresenter.h"
#include "src/L3/presenters/SensorsPresenter.h"
#include "ui_state.h"
#include "UIHelpers.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class State;
}
QT_END_NAMESPACE

class SensorDisplayLabel : public QLabel, public SPOC {
public:
    void displaySensorValue(const float value) override;
};

class StateUIHandler : public QWidget, public StPOC {
    Q_OBJECT
public:
    StateUIHandler(Ui::State& stateUI, SPIC& spic, ACIC& acic, StCIC& stcic, ClocksModule& cm);

    void displayProcessSummary(const std::vector<std::string> processSummary) override;
    void displayState(const State& s) override ;
    void allowProceed(bool permission) override ;
    void allowAbort(bool permission) override ;

    void displaySensorValue(const std::string id, const float value);
private:
    QLabel* displayTimedActuator(QPushButton* aButton);
    void subscribe(std::string id, QLabel* label);

    std::map<std::string, QLabel*> sensorDisplaySubscribers;
    Ui::State& stateUI;
    SPIC& spic;
    ACIC& acic;
    StCIC& stcic;
    ClocksModule& cm;
};

#endif // STATEUIHANDLER_H
