#ifndef SYSTEMDIAGRAMUIHANDLER_H
#define SYSTEMDIAGRAMUIHANDLER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include "src/L4/ui/Draggable.h"
#include "src/L3/controllers/ActuatorsController.h"
#include "src/L3/presenters/SensorsPresenter.h"
#include "src/L2/usecases/SensorsManager.h"
#include "src/L2/services/ClocksModule.h"
#include "ui_systemdiagram.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class SystemDiagram;
}
QT_END_NAMESPACE

class SystemDiagramUIHandler : public QWidget {
    Q_OBJECT
public:
    SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI, ACIC& acic, ClocksModule& cm) : systemDiagramUI(systemDiagramUI), acic(acic), cm(cm) {};
    void renderSystemDiagram(std::vector<std::string> sensorIds, std::vector<std::string> actuatorIds);

    void displaySensorValue(const std::string id, const float value);
private:
    void subscribe(std::string id, QLabel* label);

    std::map<std::string, QLabel*> sensorDisplaySubscribers;
    Ui::SystemDiagram& systemDiagramUI;
    ACIC& acic;
    ClocksModule& cm;
};

#endif // SYSTEMDIAGRAMUIHANDLER_H
