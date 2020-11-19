#ifndef GSMAINWINDOWHANDLER_H
#define GSMAINWINDOWHANDLER_H

#include <QObject>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "src/L3/controllers/ProcessController.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class GSMainWindow;
}
QT_END_NAMESPACE

class GSMainWindowHandler : public QMainWindow {
    Q_OBJECT
public:
    GSMainWindowHandler(PCIC* pcic);
    ~GSMainWindowHandler();

    QHBoxLayout* getAbortButtonLayout();
    QHBoxLayout* getProcessSummaryLayout();
    QHBoxLayout* getProceedButtonLayout();
    QVBoxLayout* getCurrentStateLayout();
private:
    Ui::GSMainWindow* mainWindowUI;
};

#endif // GSMAINWINDOWHANDLER_H
