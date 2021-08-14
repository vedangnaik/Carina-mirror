#include "SystemDiagramUIHandler.h"

SystemDiagramUIHandler::SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI, SPIC& spic, APIC& apic, ACIC& acic, std::vector<std::string> sensorIds, std::vector<std::string> actuatorIds) :
    systemDiagramUI(systemDiagramUI), spic(spic), apic(apic), acic(acic)
{
    connect(this->systemDiagramUI.lockPositionsCheckbox, &QCheckBox::stateChanged, this, &SystemDiagramUIHandler::togglePositionLock);
    this->systemDiagramUI.lockPositionsCheckbox->setEnabled(true);

    for (const auto& id : sensorIds) {
        auto* s = new Draggable<SystemDiagramUI::SensorDisplayLabel>(*this->systemDiagramUI.systemDiagramFrame, id, "psi");
        s->displayValue(std::nan("NaN"));
        this->spic.subscribe(id, s);
        this->draggables.push_back(s);
        this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(s);
    }

    for (const auto& id : actuatorIds) {
        Draggable<SystemDiagramUI::ActuatorButton>* a = new Draggable<SystemDiagramUI::ActuatorButton>(*this->systemDiagramUI.systemDiagramFrame);
        this->apic.subscribe(id, a);
        a->setCheckable(true);
        a->setText(QString::fromStdString(id));
        connect(a, &QPushButton::clicked, &this->acic, [=]() {
            this->acic.setState(id);
        });
        this->draggables.push_back(a);
        this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(a);
    }

    delete this->systemDiagramUI.systemDiagramFrame->layout();
}

void SystemDiagramUIHandler::togglePositionLock(int state) {
    for (const auto& d : this->draggables) {
        state == Qt::CheckState::Checked ? d->lockPosition() : d->unlockPosition();
    }
}
