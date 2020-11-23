#include "MovableQPushButton.h"

void MovableQPushButton::mouseMoveEvent(QMouseEvent* event) {
//    this->setText("X: " +
//                  QString::number(this->pos().x()) +
//                  ". Moved X: " +
//                  QString::number(event->localPos().x()));
//    this->resize(this->sizeHint().width(), this->sizeHint().height());
    this->move(this->pos() + event->localPos().toPoint());
}
