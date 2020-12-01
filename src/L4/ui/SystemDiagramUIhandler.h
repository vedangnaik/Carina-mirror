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

namespace SystemDiagramUI {
class SensorDisplayLabel : public QLabel, public SPOC {
public:
    void displaySensorValue(const float value) override {
        this->setText(QString::number(value));
    }
};
}

class SystemDiagramUIHandler : public QWidget {
    Q_OBJECT
public:
    SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI, SPIC& spic, ACIC& acic, std::vector<std::string> sensorIds, std::vector<std::string> actuatorIds);
private:
    Ui::SystemDiagram& systemDiagramUI;
    SPIC& spic;
    ACIC& acic;
};

#endif // SYSTEMDIAGRAMUIHANDLER_H
