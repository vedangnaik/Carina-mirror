#include "SystemDiagramUIHandler.h"

SystemDiagramUIHandler::SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI, SPIC& spic, APIC& apic, ACIC& acic, const std::vector<std::string>& sensorIDs, const std::vector<std::string>& actuatorIDs) :
    systemDiagramUI(systemDiagramUI), spic(spic), apic(apic), acic(acic)
{
    connect(this->systemDiagramUI.lockPositionsCheckbox, &QCheckBox::stateChanged, this, &SystemDiagramUIHandler::togglePositionLock);
    this->systemDiagramUI.lockPositionsCheckbox->setEnabled(true);

    for (const auto& id : sensorIDs) {
        auto* s = new Draggable<SystemDiagramUI::SensorDisplayLabel>(*this->systemDiagramUI.systemDiagramFrame, id, "psi");
        s->displayValue(std::nan("NaN"));
        this->spic.subscribe(id, s);
        this->draggables.push_back(s);
        this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(s);
    }

    for (const auto& id : actuatorIDs) {
        auto* a = new Draggable<SystemDiagramUI::ActuatorButton>(*this->systemDiagramUI.systemDiagramFrame);
        this->apic.subscribe(id, a);
        a->setCheckable(true);
        a->setText(QString::fromStdString(id));
        connect(a, &QPushButton::toggled, &this->acic, [=](bool checked) {
            this->acic.setState(id, checked);
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
