#include "MovableQPushButton.h"

void MovableQPushButton::mouseMoveEvent(QMouseEvent* event) {
    // this static casting is bad practice cause it messes with OOP,
    // but I can't think of any other way to do this. If it's messing
    // with performance, it can be removed, at cost of an uglier UI.
    QFrame* parentFrame = static_cast<QFrame*>(this->parent());
    const QRect& frameRect = parentFrame->geometry();
    // Usage of frameWidth is only correct if the frameStyle is Panel or Box
    // (among others). If this is changed, the calculations here must change
    // as well. A style check could be added here, but eh.
    const int frameWidth = parentFrame->frameWidth();

    QPoint cursorPosOnMoveEnd = event->localPos().toPoint();
    QPoint currentPos = this->pos();
    QPoint unboundNewPos = currentPos + cursorPosOnMoveEnd - cursorPosOnMoveStart;

    int boundedX = std::min(std::max(unboundNewPos.x(), frameWidth), frameRect.width() - frameWidth - this->width());
    int boundedY = std::min(std::max(unboundNewPos.y(), frameWidth), frameRect.height() - frameWidth - this->height());
    this->move(boundedX, boundedY);
}

void MovableQPushButton::mousePressEvent(QMouseEvent *event) {
    this->cursorPosOnMoveStart = event->pos();
    // call the parent's mouse press event to keep the clicking action
    QPushButton::mousePressEvent(event);
}
