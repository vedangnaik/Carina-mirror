#ifndef STATEUIHANDLER_H
#define STATEUIHANDLER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include "src/L2/services/ClocksModule.h"
#include "src/L3/controllers/ActuatorsController.h"
#include "src/L3/controllers/StatesController.h"
#include "src/L3/presenters/SensorsPresenter.h"
#include "src/L3/presenters/ActuatorsPresenter.h"
#include "src/L3/presenters/StatesPresenter.h"
#include "ui_state.h"
#include "UIHelpers.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class State;
}
QT_END_NAMESPACE

namespace StateUI {
class SensorDisplayLabel : public QLabel, public SPOC {
public:
    void displayValue(const float value) override {
        this->setText(QString::number(value));
    }
};

class ActuatorButton : public QPushButton, public APOC {
public:
    void displayStatus(const bool status) override {
        this->setChecked(status);
    }
};
}

class StateUIHandler : public QWidget, public StPOC {
    Q_OBJECT
public:
    StateUIHandler(Ui::State& stateUI, SPIC& spic, APIC& apic, ACIC& acic, StCIC& stcic);

    void displayProcessSummary(const std::vector<std::string> processSummary) override;
    void displayState(const State& s) override;
    void allowProceed(bool permission) override;
    void allowAbort(bool permission) override;
private:
    QLabel* displayTimedActuator(QPushButton* aButton);

    Ui::State& stateUI;
    QTimer* actuatorButtonTimer;
    SPIC& spic;
    APIC& apic;
    ACIC& acic;
    StCIC& stcic;
};

#endif // STATEUIHANDLER_H
