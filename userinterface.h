#ifndef GSUI_H
#define GSUI_H

#include <QMainWindow>
#include "gateways.h"
#include "controllers.h"
#include "presenters.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GSUI; }
QT_END_NAMESPACE

class UserInterface : public QMainWindow, public PPOC {
    Q_OBJECT

public:
    UserInterface(PCIC* pcic, QWidget *parent = nullptr);
    ~UserInterface();
    void displayState(State* s);

    PCIC* pcic;
private:
    Ui::GSUI *ui;
};
#endif // GSUI_H
