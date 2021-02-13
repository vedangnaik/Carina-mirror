#include <QApplication>
#include "GSManager.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);

    QApplication a(argc, argv);
    GSManager gsm;
    LOG(INFO) << "Carina has started.";

    gsm.show();
    int r = a.exec();

    LOG(INFO) << "Carina has ended.";
    return r;
}
