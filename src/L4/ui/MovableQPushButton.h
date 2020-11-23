#ifndef MOVABLEQPUSHBUTTON_H
#define MOVABLEQPUSHBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QFrame>

class MovableQPushButton : public QPushButton {
    using QPushButton::QPushButton;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    QPoint cursorPosOnMoveStart;
};

#endif // MOVABLEQPUSHBUTTON_H
