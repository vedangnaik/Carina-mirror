#include <QApplication>
#include "L4/ui/GSManager.h"
#include "L2/services/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    QApplication a(argc, argv);
    GSManager gsm;
    gsm.show();
    return a.exec();
}
