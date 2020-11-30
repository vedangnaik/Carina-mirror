#include "SystemDiagramUIhandler.h"

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

void SystemDiagramUIHandler::renderSystemDiagram(std::vector<std::string> sensorIds, std::vector<std::string> actuatorIds) {
    for (const auto& id : sensorIds) {
        Draggable<QLabel>* sensorValueLabel = new Draggable<QLabel>(this->systemDiagramUI.systemDiagramFrame);
        // ugly hack, it wasn't resizing for some reason. But then, this whole function is a hack xD
        sensorValueLabel->setText("________");
        this->subscribe(id, sensorValueLabel);
        this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(sensorValueLabel);
    }
    for (const auto& id: actuatorIds) {
        Draggable<QPushButton>* aButton = new Draggable<QPushButton>(this->systemDiagramUI.systemDiagramFrame);
        aButton->setCheckable(true);
        aButton->setText(QString::fromStdString(id));
        connect(aButton, &QPushButton::toggled, &this->acic, [=]() {
            this->acic.actuate(id);
        });
        this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(aButton);
    }
    delete this->systemDiagramUI.systemDiagramFrame->layout();
}
