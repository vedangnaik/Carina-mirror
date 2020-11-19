#ifndef PROCESSUIHANDLER_H
#define PROCESSUIHANDLER_H

#include <QWidget>
#include <vector>
#include <string>
#include <QPushButton>
#include "StateUIHandler.h"
#include "GSManager.h"
#include "src/L3/controllers/ProcessController.h"
#include "src/L3/presenters/StatesPresenter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class ProcessSummary;
}
QT_END_NAMESPACE


class ProcessUIHandler : public QWidget, public SPOC {
    Q_OBJECT
public:
    ProcessUIHandler(GSManager* gsmwh, StateUIHandler* suih, PCIC* pcic);
    ~ProcessUIHandler();

    void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(const State& s);

    void allowProceed(bool permission);
    void allowAbort(bool permission);

private:
    Ui::ProcessSummary* processSummaryUI;
    QPushButton* abortButton;
    QPushButton* proceedButton;

    StateUIHandler* suih;
    PCIC* pcic;
};

#endif // PROCESSUIHANDLER_H
