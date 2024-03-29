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
    SensorDisplayLabel(QFrame& parent, const std::string& ID, const std::string& unit) : QWidget(&parent) {
        this->id = new QLabel(QString::fromStdString(ID), this);
        this->value = new QLabel(this);
        this->unit = new QLabel(QString::fromStdString(unit), this);
        // Place them horizontally.
        // Minimumize makes the labels resize to fit the value.
        auto* hb = new QHBoxLayout();
        hb->setSizeConstraint(QLayout::SetMinimumSize);
        hb->addWidget(this->id);
        hb->addWidget(this->value);
        hb->addWidget(this->unit);
        delete this->layout();
        this->setLayout(hb);
    };
    void displayValue(const double value) override {
        this->value->setText(QString("").asprintf("%+.6f", value));
    }
private:
    QLabel* id = nullptr;
    QLabel* value = nullptr;
    QLabel* unit = nullptr;
};

class ActuatorButton : public QPushButton, public APOC {
public:
    explicit ActuatorButton(QFrame& parent) : QPushButton(&parent) {}
    void displayStatus(const bool status) override {
        this->setChecked(status);
    }
};
}

class SystemDiagramUIHandler : public QWidget {
    Q_OBJECT
public:
    SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI, SPIC& spic, APIC& apic, ACIC& acic, const std::vector<std::string>& sensorIDs, const std::vector<std::string>& actuatorIDs);
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
