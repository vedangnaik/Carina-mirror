#pragma once

#include "ui_error.h"

class ErrorUIHandler {
public:
    void setUI(Ui::Error* errorUI);
    friend ErrorUIHandler& operator<<(ErrorUIHandler& euih, std::string message);
private:
    Ui::Error* errorUI;
};

extern ErrorUIHandler euih;
