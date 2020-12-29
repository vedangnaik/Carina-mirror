#include "ErrorUIHandler.h"

void ErrorUIHandler::setUI(Ui::Error* errorUI) {
    this->errorUI = errorUI;
}

ErrorUIHandler& operator<<(ErrorUIHandler& euih, std::string message) {
    euih.errorUI->errorTextEdit->append(QString::fromStdString(message));
    return euih;
}

ErrorUIHandler euih;
