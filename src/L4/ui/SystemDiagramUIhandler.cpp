#include "SystemDiagramUIhandler.h"

SystemDiagramUIHandler::SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI, SPIC& spic, ACIC& acic, std::vector<std::string> sensorIds, std::vector<std::string> actuatorIds) : systemDiagramUI(systemDiagramUI), spic(spic), acic(acic) {
    for (const auto& id : sensorIds) {
        SystemDiagramUI::SensorDisplayLabel* s = new SystemDiagramUI::SensorDisplayLabel();
        this->spic.subscribe(id, s);
        s->setText("__ NaN __");

        Draggable<QWidget>* sensorDisplayer = new Draggable<QWidget>(this->systemDiagramUI.systemDiagramFrame);
        QHBoxLayout* hl = new QHBoxLayout(sensorDisplayer);
        hl->addWidget(new QLabel(QString::fromStdString(id) + ": "));
        hl->addWidget(s);
        sensorDisplayer->setLayout(hl);
        this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(sensorDisplayer);
    }

    for (const auto& id : actuatorIds) {
        Draggable<QPushButton>* a = new Draggable<QPushButton>(this->systemDiagramUI.systemDiagramFrame);
        a->setCheckable(true);
        a->setText(QString::fromStdString(id));
        connect(a, &QPushButton::toggled, &this->acic, [=]() {
            this->acic.actuate(id);
        });
        this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(a);
    }

    delete this->systemDiagramUI.systemDiagramFrame->layout();
}
