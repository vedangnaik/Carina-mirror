#include "ErrorUIHandler.h"

void ErrorUIHandler::setUI(Ui::Error* errorUI) {
    this->errorUI = errorUI;
}

ErrorUIHandler& operator<<(ErrorUIHandler& euih, std::string message) {
    // If message if from easylogging++, it has a newline for some reason.
    // This ugly check-and-remove thing deals with that.
    if (message.back() == '\n') { message.pop_back(); }
    euih.errorUI->errorTextEdit->append(QString::fromStdString(message));
    return euih;
}

// instantiating global ErrorUIHandler instance here
ErrorUIHandler euih;
