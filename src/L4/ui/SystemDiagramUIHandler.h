#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <variant>
#include <cmath>

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
class SensorDisplayLabel : public QWidget, public SPOC {
public:
    SensorDisplayLabel(QFrame& parent) : QWidget(&parent) {
        this->l = new QLabel(this);
        this->v = new QLabel(this);
        QHBoxLayout* hb = new QHBoxLayout();
        hb->addWidget(this->l);
        hb->addWidget(this->v);
        delete this->layout();
        this->setLayout(hb);
    };
    void displayValue(const float value) override {
        this->v->setText(QString::number(value));
    }
    void setLabel(std::string label) {
        this->l->setText(QString::fromStdString(label));
    }
private:
    QLabel* v = nullptr;
    QLabel* l = nullptr;
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
    std::vector<AbstractDraggable*> draggables;

    SPIC& spic;
    APIC& apic;
    ACIC& acic;
};
