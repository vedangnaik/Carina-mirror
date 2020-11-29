#include "SystemDiagramUIhandler.h"
#include "ui_systemdiagram.h"

SystemDiagramUIHandler::SystemDiagramUIHandler(Ui::SystemDiagram* systemDiagramUI) : systemDiagramUI(systemDiagramUI) {

}

SystemDiagramUIHandler::~SystemDiagramUIHandler() {
    delete this->systemDiagramUI;
}
