#ifndef MOVABLEQPUSHBUTTON_H
#define MOVABLEQPUSHBUTTON_H

#include <QPushButton>
#include <iostream>
#include <QMouseEvent>

class MovableQPushButton : public QPushButton {
    using QPushButton::QPushButton;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    QPoint mouseClickStartPoint;
};

#endif // MOVABLEQPUSHBUTTON_H
