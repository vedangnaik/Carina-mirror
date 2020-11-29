#include "SystemDiagramUIhandler.h"
#include "ui_systemdiagram.h"
#include <iostream>
#include <QHBoxLayout>

SystemDiagramUIHandler::SystemDiagramUIHandler(Ui::SystemDiagram& systemDiagramUI) : systemDiagramUI(systemDiagramUI) {
    Draggable<QPushButton>* btn = new Draggable<QPushButton>(this->systemDiagramUI.systemDiagramFrame);
    btn->setText("test");
    this->systemDiagramUI.systemDiagramFrame->layout()->addWidget(btn);
    delete this->systemDiagramUI.systemDiagramFrame->layout();
}
