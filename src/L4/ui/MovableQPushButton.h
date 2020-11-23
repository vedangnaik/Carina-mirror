#ifndef MOVABLEQPUSHBUTTON_H
#define MOVABLEQPUSHBUTTON_H

#include <QPushButton>
#include <iostream>
#include <QMouseEvent>

class MovableQPushButton : public QPushButton {
    using QPushButton::QPushButton;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
};

#endif // MOVABLEQPUSHBUTTON_H
