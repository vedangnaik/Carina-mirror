#ifndef SYSTEMDIAGRAMUIHANDLER_H
#define SYSTEMDIAGRAMUIHANDLER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <variant>

#include "Draggable.h"
#include "ActuatorsController.h"
#include "ActuatorsPresenter.h"
#include "SensorsPresenter.h"
#include "ui_SystemDiagram.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class SystemDiagram;
}
QT_END_NAMESPACE

namespace SystemDiagramUI {
class SensorDisplayLabel : public QLabel, public SPOC {
public:
    SensorDisplayLabel(QFrame& parent) : QLabel(&parent) {};
    void displayValue(const float value) override {
        this->setText(QString::number(value));
    }
};

class ActuatorButton : public QPushButton, public APOC {
public:
    ActuatorButton(QFrame& parent) : QPushButton(&parent) {};
    void displayStatus(const bool status) override {
        this->setChecked(status);
    }
};
}

class SystemDiagramUIHandler : public QWidget {
    Q_OBJECT
public:
    SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI, SPIC& spic, APIC& apic, ACIC& acic, std::vector<std::string> sensorIds, std::vector<std::string> actuatorIds);
private slots:
    void togglePositionLock(int state);
private:
    Ui::SystemDiagram& systemDiagramUI;
    // these objects are children of the main UI, so no manual deletion is required.
    std::vector<DraggableBase*> draggables;

    SPIC& spic;
    APIC& apic;
    ACIC& acic;
};

#endif // SYSTEMDIAGRAMUIHANDLER_H
