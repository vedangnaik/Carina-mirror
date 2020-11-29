#ifndef SYSTEMDIAGRAMUIHANDLER_H
#define SYSTEMDIAGRAMUIHANDLER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "Draggable.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class SystemDiagram;
}
QT_END_NAMESPACE

class SystemDiagramUIHandler : public QWidget
{
    Q_OBJECT
public:
    SystemDiagramUIHandler(Ui::SystemDiagram* systemDiagramUI);
    ~SystemDiagramUIHandler();
private:
    Ui::SystemDiagram* systemDiagramUI;
};

#endif // SYSTEMDIAGRAMUIHANDLER_H
