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
    class CurrentState;
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

    PCIC* pcic;
private:
    QMainWindow* toHandle;
    Ui::GSMainWindow* mainWindowUI;
};



class ProcessUIHandler : public QWidget, public PPOC {
    Q_OBJECT
public:
    ProcessUIHandler(GSMainWindowHandler* gsmwh, PCIC* pcic, ACIC* acic, ClocksModule* cm);
    ~ProcessUIHandler();

    void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(State* s, std::map<std::string, Actuator*> actuators, std::map<std::string, Sensor*> sensors);

    void allowProceed(bool permission);
    void allowAbort(bool permission);

private:
    QLabel* timedActuatorHandler(QPushButton* aButton);

    Ui::CurrentState* stateUI;
    Ui::ProcessSummary* processSummaryUI;
    QPushButton* abortButton;
    QPushButton* proceedButton;

    GSMainWindowHandler* gsmwh;
    PCIC* pcic;
    ACIC* acic;
    ClocksModule* cm;
};

#endif // UI_H
