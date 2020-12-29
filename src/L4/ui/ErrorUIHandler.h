#pragma once

#include "ui_error.h"
#include <iostream>

class ErrorUIHandler {
public:
    void setUI(Ui::Error* errorUI);
    friend ErrorUIHandler& operator<<(ErrorUIHandler& euih, std::string message);
private:
    Ui::Error* errorUI = nullptr;
};

extern ErrorUIHandler euih;
