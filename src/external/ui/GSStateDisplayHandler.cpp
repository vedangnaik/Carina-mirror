#include "userinterface.h"
#include "ui_GSStateDisplay.h"

GSStateDisplayHandler::GSStateDisplayHandler() {
    this->ui = new Ui::GSStateDisplay;
    ui->setupUi(nullptr);
}
