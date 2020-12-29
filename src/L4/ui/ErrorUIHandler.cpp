#include "ErrorUIHandler.h"

void ErrorUIHandler::setUI(Ui::Error* errorUI) {
    this->errorUI = errorUI;
}

ErrorUIHandler& operator<<(ErrorUIHandler& euih, std::string message) {
    // If message if from easylogging++, it has a newline for some reason.
    // This ugly check-and-remove thing deals with that.
    if (message.back() == '\n') {
        message.pop_back();
    }

    // redirect output to std::cout if for some reason the UI has crashed
    // or not been instantiatied yet.
    if (euih.errorUI != nullptr) {
        euih.errorUI->errorTextEdit->append(QString::fromStdString(message));
    } else {
        std::cout << message << std::endl;
    }

    return euih;
}

// instantiating global ErrorUIHandler instance here
ErrorUIHandler euih;
