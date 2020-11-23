#include "MovableQPushButton.h"

void MovableQPushButton::mouseMoveEvent(QMouseEvent* event) {
//    this->setText("X: " +
//                  QString::number(this->pos().x()) +
//                  ". Moved X: " +
//                  QString::number(this->mouseClickStartPoint.x()));
//    this->resize(this->sizeHint().width(), this->sizeHint().height());
    this->move(this->pos() + event->localPos().toPoint() - this->mouseClickStartPoint);
}

void MovableQPushButton::mousePressEvent(QMouseEvent *event) {
    this->mouseClickStartPoint = event->pos();
}
