#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "src/adapters/gateways/gateways.h"
#include "src/adapters/controllers/controllers.h"
#include "src/adapters/presenters/presenters.h"
#include "src/external/services/services.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class GSMainWindow;
    class State;
    class ProcessSummary;
}
QT_END_NAMESPACE


class GSMainWindowHandler : public QObject {
    Q_OBJECT
public:
    GSMainWindowHandler(QMainWindow* toHandle, PCIC* pcic);
    ~GSMainWindowHandler();

    QHBoxLayout* getAbortButtonLayout();
    QHBoxLayout* getProcessSummaryLayout();
    QHBoxLayout* getProceedButtonLayout();
    QVBoxLayout* getCurrentStateLayout();
private:
    QMainWindow* toHandle;
    Ui::GSMainWindow* mainWindowUI;
};

//====

class StateUIHandler : public QWidget {
    Q_OBJECT
public:
    StateUIHandler(GSMainWindowHandler* gsmwh, ACIC* acic, ClocksModule* cm);
    ~StateUIHandler();

    void displayState(StateDisplayInfo sdi);
    QLabel* timedActuatorHandler(QPushButton* aButton);
private:
    Ui::State* stateUI;

    ACIC* acic;
    ClocksModule* cm;
};

//====

class ProcessUIHandler : public QWidget, public PPOC {
    Q_OBJECT
public:
    ProcessUIHandler(GSMainWindowHandler* gsmwh, StateUIHandler* suih, PCIC* pcic);
    ~ProcessUIHandler();

    void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(StateDisplayInfo sdi);

    void allowProceed(bool permission);
    void allowAbort(bool permission);

private:
    Ui::ProcessSummary* processSummaryUI;
    QPushButton* abortButton;
    QPushButton* proceedButton;

    StateUIHandler* suih;
    PCIC* pcic;
};

#endif // UI_H
