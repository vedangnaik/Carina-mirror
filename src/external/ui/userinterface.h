#ifndef GSUI_H
#define GSUI_H

#include <QMainWindow>
#include "src/adapters/gateways/gateways.h"
#include "src/adapters/controllers/controllers.h"
#include "src/adapters/presenters/presenters.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GSUI; }
QT_END_NAMESPACE

class UserInterface : public QMainWindow, public PPOC {
    Q_OBJECT

public:
    UserInterface(PCIC* pcic, QWidget *parent = nullptr);
    ~UserInterface();
    void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(State* s);
    void toggleProceed(bool yes);
    void toggleAbort(bool yes);

    PCIC* pcic;
private:
    Ui::GSUI *ui;
};
#endif // GSUI_H
