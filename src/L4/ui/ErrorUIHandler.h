#pragma once

#include "ui_Error.h"
#include <iostream>

class ErrorUIHandler : std::basic_streambuf<char> {
public:
    ErrorUIHandler();
    ~ErrorUIHandler();
    std::streamsize xsputn(const char *p, std::streamsize n) override;
    std::basic_streambuf<char>::int_type overflow(int_type v) override;

    void setUI(Ui::Error* errorUI);
private:
    Ui::Error* errorUI = nullptr;
    std::streambuf* coutBufferBackup;
};

extern ErrorUIHandler euih;
