#include "ErrorUIHandler.h"

// ErrorUIHandler redirects std::cout into it's own buffer, and prints that directly to a QTextEdit.
// Upon destruction, it resets std::cout to it's original state.
ErrorUIHandler::ErrorUIHandler(Ui::Error& errorUI)
    : errorUI{errorUI}
{
    this->coutBufferBackup = std::cout.rdbuf();
    std::cout.rdbuf(this);
}

ErrorUIHandler::~ErrorUIHandler()
{
    std::cout.rdbuf(this->coutBufferBackup);
}

// TODO: Figure out exactly what this function does and adapt this properly.
std::basic_streambuf<char>::int_type
ErrorUIHandler::overflow(int_type v)
{
    this->errorUI.errorTextEdit->append("overflow()");
    return v;
}

// TODO: Figure out exactly what this function does and adapt this properly.
std::streamsize
ErrorUIHandler::xsputn(const char *p, std::streamsize n)
{
    this->errorUI.errorTextEdit->append(QString(p));
    return n;
}
