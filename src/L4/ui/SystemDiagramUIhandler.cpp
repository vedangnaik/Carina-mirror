#include "SystemDiagramUIhandler.h"

SystemDiagramUIHandler::SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI, ACIC& acic, ClocksModule& cm) : systemDiagramUI(systemDiagramUI), acic(acic), cm(cm) {
    Draggable<QPushButton>* btn = new Draggable<QPushButton>(this->systemDiagramUI.systemDiagramFrame);
    btn->setText("test");
    Draggable<QLabel>* l = new Draggable<QLabel>(this->systemDiagramUI.systemDiagramFrame);
    this->subscribe("sens1", l);

    this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(btn);
    this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(l);

    delete this->systemDiagramUI.systemDiagramFrame->layout();
}

void SystemDiagramUIHandler::subscribe(std::string id, QLabel* label) {
    this->cm.stop();
    this->sensorDisplaySubscribers.insert(std::make_pair(id, label));
    this->cm.start();
    connect(label, &QLabel::destroyed, this, [=]() {
        this->cm.stop();
        this->sensorDisplaySubscribers.erase(id);
        this->cm.start();
    });
}

void SystemDiagramUIHandler::displaySensorValue(const std::string id, const float value) {
    if (this->sensorDisplaySubscribers.find(id) != this->sensorDisplaySubscribers.end()) {
        this->sensorDisplaySubscribers.at(id)->setText(QString::number(value));
    }
}
