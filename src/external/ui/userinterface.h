#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include "src/adapters/gateways/gateways.h"
#include "src/adapters/controllers/controllers.h"
#include "src/adapters/presenters/presenters.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class GSMainWindow;
}
QT_END_NAMESPACE

class GSMainWindowHandler : public QObject, public PPOC {
    Q_OBJECT
public:
    GSMainWindowHandler(QMainWindow* toHandle, PCIC* pcic);
    ~GSMainWindowHandler();
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

    PCIC* pcic;
private:
    QMainWindow* toHandle;
    Ui::GSMainWindow *ui;
};
#endif // UI_H
