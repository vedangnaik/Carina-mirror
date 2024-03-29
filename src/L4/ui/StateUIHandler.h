#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QTimer>
#include <QMessageBox>

#include "Helpers.h"
#include "ActuatorsController.h"
#include "StatesController.h"
#include "SensorsPresenter.h"
#include "ActuatorsPresenter.h"
#include "StatesPresenter.h"
#include "ui_State.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class State;
}
QT_END_NAMESPACE

namespace StateUI {
class SensorDisplayLabel : public QLabel, public SPOC {
public:
    void displayValue(const double value) override {
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
    StateUIHandler(Ui::State& stateUI, SPIC& spic, APIC& apic, StPIC& stpic, ACIC& acic, StCIC& stcic);

    void displayProcessSummary(std::vector<std::string> processSummary) override;
    void displayState(const State& s) override;
    void allowProceed(bool permission) override;
    void allowAbort(bool permission) override;
    void allowFailedChecksOverride(std::vector<std::string> failures, Transition t) override;
private:
    QLabel* displayTimedActuator(QPushButton* aButton);

    Ui::State& stateUI;
    QTimer* actuatorButtonTimer;
    // presenters to subscribe to
    SPIC& spic;
    APIC& apic;
    StPIC& stpic;
    // controllers to report actions to
    ACIC& acic;
    StCIC& stcic;
};
