#include <QApplication>
#include "src/L4/ui/GSManager.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GSManager gsm;
    gsm.show();
    return a.exec();
}
