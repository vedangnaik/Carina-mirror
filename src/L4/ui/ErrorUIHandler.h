#pragma once

#include "ui_Error.h"
#include <iostream>

class ErrorUIHandler : std::basic_streambuf<char> {
public:
    ErrorUIHandler(Ui::Error& errorUI);
    ~ErrorUIHandler();
    std::streamsize xsputn(const char *p, std::streamsize n) override;
    std::basic_streambuf<char>::int_type overflow(int_type v) override;
private:
    Ui::Error& errorUI;
    std::streambuf* coutBufferBackup;
};
