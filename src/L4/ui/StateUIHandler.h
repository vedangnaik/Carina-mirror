#ifndef STATEUIHANDLER_H
#define STATEUIHANDLER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "ui_state.h"
#include "GSMainWindowHandler.h"
#include "src/L2/services/ClocksModule.h"
#include "src/L3/controllers/ActuatorsController.h"
#include "src/L3/presenters/ProcessPresenter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class State;
}
QT_END_NAMESPACE

class StateUIHandler : public QWidget {
    Q_OBJECT
public:
    StateUIHandler(GSMainWindowHandler* gsmwh, ACIC* acic, SMIC* smic, ClocksModule* cm);
    ~StateUIHandler();
    void displayState(StateDisplayInfo sdi);
private:
    QLabel* timedActuatorHandler(QPushButton* aButton);

    Ui::State* stateUI;
    ACIC* acic;
    SMIC* smic;
    ClocksModule* cm;
};

#endif // STATEUIHANDLER_H
