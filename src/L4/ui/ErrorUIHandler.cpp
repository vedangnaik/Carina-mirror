#include "ErrorUIHandler.h"

// ErrorUIHandler redirects std::cout into it's own buffer, and prints that directly to a QTextEdit.
// Upon destruction, it resets std::cout to it's original state.
ErrorUIHandler::ErrorUIHandler()
{
    this->coutBufferBackup = std::cout.rdbuf();
    std::cout.rdbuf(this);
}

ErrorUIHandler::~ErrorUIHandler()
{
    std::cout.rdbuf(this->coutBufferBackup);
}

void
ErrorUIHandler::setUI(Ui::Error* errorUI)
{
    this->errorUI = errorUI;
}

// This is called when a std::endl has been inserted into the stream
std::basic_streambuf<char>::int_type
ErrorUIHandler::overflow(int_type v)
{
    this->errorUI->errorTextEdit->append("overflow()");
    if (v == '\n') {
    }
    return v;
}

std::streamsize
ErrorUIHandler::xsputn(const char *p, std::streamsize n)
{
//    QString str(p);
//    if(str.contains("\n")) {
//        QStringList strSplitted = str.split("\n");
//        (msgObj->*msgHandler)(strSplitted.at(0)); //Index 0 is still on the same old line
//        for(int i = 1; i < strSplitted.size(); i++) {
//            (msgObj->*msgHandler)("\\    " + strSplitted.at(i));
//        }
//    } else {
//        (msgObj->*msgHandler)(str);
//    }
    this->errorUI->errorTextEdit->append("xsputn()");
    this->errorUI->errorTextEdit->append(QString(p));
    return n;
}
