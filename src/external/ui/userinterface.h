#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "src/adapters/gateways/gateways.h"
#include "src/adapters/controllers/controllers.h"
#include "src/adapters/presenters/presenters.h"


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
    ProcessUIHandler(GSMainWindowHandler* gsmwh, PCIC* pcic);
    ~ProcessUIHandler();

    void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(
            std::string name,
            std::string description,
            std::string abortState,
            std::map<int, Sensor*> sensorPos,
            std::map<int, Actuator*> actuatorPos
        );
    void allowProceed(bool permission);
    void allowAbort(bool permission);

private:
    Ui::CurrentState* stateUI;
    Ui::ProcessSummary* processSummaryUI;
    GSMainWindowHandler* gsmwh;
    QPushButton* abortButton;
    QPushButton* proceedButton;
    PCIC* pcic;
};

#endif // UI_H
