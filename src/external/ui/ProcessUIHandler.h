#ifndef PROCESSUIHANDLER_H
#define PROCESSUIHANDLER_H

#include <QWidget>
#include <vector>
#include <string>
#include <QPushButton>
#include "StateUIHandler.h"
#include "GSMainWindowHandler.h"
#include "src/adapters/controllers/ProcessController.h"
#include "src/adapters/presenters/ProcessPresenter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class ProcessSummary;
}
QT_END_NAMESPACE


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

#endif // PROCESSUIHANDLER_H
