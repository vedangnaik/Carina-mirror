#ifndef GSUI_H
#define GSUI_H

#include <QMainWindow>
#include "layer3.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GSUI; }
QT_END_NAMESPACE

class ProcessPresenter : public QMainWindow, public ProcessManagerOutputContract
{
    Q_OBJECT

public:
    ProcessPresenter(QWidget *parent = nullptr);
    ~ProcessPresenter();

    ProcessManager* pm;
    SensorsManager* sm;

private slots:
    void proceedButton_onClick();
    void openProcessFromFile_onClick();

private:
    Ui::GSUI *ui;
};
#endif // GSUI_H
