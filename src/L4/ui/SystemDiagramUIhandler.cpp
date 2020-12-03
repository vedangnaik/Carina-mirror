#include "SystemDiagramUIhandler.h"

SystemDiagramUIHandler::SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI, SPIC& spic, APIC& apic, ACIC& acic, std::vector<std::string> sensorIds, std::vector<std::string> actuatorIds) : systemDiagramUI(systemDiagramUI), spic(spic), apic(apic), acic(acic) {
    connect(this->systemDiagramUI.lockPositionsCheckbox, &QCheckBox::stateChanged, this, &SystemDiagramUIHandler::togglePositionLock);
    this->systemDiagramUI.lockPositionsCheckbox->setEnabled(true);

    for (const auto& id : sensorIds) {
        SystemDiagramUI::SensorDisplayLabel* sensorDisplayer = new SystemDiagramUI::SensorDisplayLabel();
        this->spic.subscribe(id, sensorDisplayer);
        sensorDisplayer->setText("__ NaN __");

        Draggable<QWidget>* wrp = new Draggable<QWidget>(this->systemDiagramUI.systemDiagramFrame);
        this->draggables.push_back(wrp);
        QHBoxLayout* hl = new QHBoxLayout(wrp);
        hl->addWidget(new QLabel(QString::fromStdString(id) + ": "));
        hl->addWidget(sensorDisplayer);
        wrp->setLayout(hl);
        this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(wrp);
    }

    for (const auto& id : actuatorIds) {
        SystemDiagramUI::ActuatorButton* a = new SystemDiagramUI::ActuatorButton();
        this->apic.subscribe(id, a);
        a->setCheckable(true);
        a->setText(QString::fromStdString(id));
        connect(a, &QPushButton::clicked, &this->acic, [=]() {
            this->acic.actuate(id);
        });

        Draggable<QWidget>* wrp = new Draggable<QWidget>(this->systemDiagramUI.systemDiagramFrame);
        this->draggables.push_back(wrp);
        QHBoxLayout* hl = new QHBoxLayout(wrp);
        hl->addWidget(new QLabel(QString::fromStdString(id) + ": "));
        hl->addWidget(a);
        wrp->setLayout(hl);
        this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(wrp);
    }

    delete this->systemDiagramUI.systemDiagramFrame->layout();
}

void SystemDiagramUIHandler::togglePositionLock(int state) {
    if (state == Qt::CheckState::Checked) {
        for (auto& d : this->draggables) { d->lockPosition(); }
    } else {
        for (auto& d : this->draggables) { d->unlockPosition(); }
    }
}
