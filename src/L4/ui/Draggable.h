#pragma once

#include <QWidget>
#include <QFrame>
#include <QMouseEvent>
#include <iostream>

class AbstractDraggable {
public:
    virtual void unlockPosition() = 0;
    virtual void lockPosition() = 0;
    virtual ~AbstractDraggable() {};
};

template <typename T>
class Draggable : public AbstractDraggable, public T {
    static_assert(std::is_base_of<QWidget, T>::value, "T must inherit from QWidget");
public:
    template<class... Args>
    Draggable(QFrame& parent, Args... args) : T(parent, args...), parentFrame{&parent} {}
    void unlockPosition() override { this->positionLocked = false; }
    void lockPosition() override { this->positionLocked = true; }
protected:
    void mouseMoveEvent(QMouseEvent* event) override {
        // If you're not allowed to move, this function terminates immediately.
        if (this->positionLocked) { return; }

        const QRect& frameRect = this->parentFrame->geometry();
        // Usage of frameWidth is only correct if the frameStyle is Panel or Box
        // (among others). If this is changed, the calculations here must change
        // as well. A style check could be added here, but eh.
        const int frameWidth = this->parentFrame->frameWidth();

        QPoint cursorPosOnMoveEnd = event->localPos().toPoint();
        QPoint currentPos = this->pos();
        QPoint unboundNewPos = currentPos + cursorPosOnMoveEnd - cursorPosOnMoveStart;

        int boundedX = std::min(std::max(unboundNewPos.x(), frameWidth), frameRect.width() - frameWidth - this->width());
        int boundedY = std::min(std::max(unboundNewPos.y(), frameWidth), frameRect.height() - frameWidth - this->height());
        this->move(boundedX, boundedY);
    };
    void mousePressEvent(QMouseEvent* event) override {        
        this->cursorPosOnMoveStart = event->pos();
        // call the parent's mouse press event to keep the clicking action, only if
        // the guy is not allowed to move. Otherwise, the clicking action is disabled
        // to prevent a misclick while dragging.
        if (this->positionLocked) { T::mousePressEvent(event); }
    };
private:
    bool positionLocked = false;
    QPoint cursorPosOnMoveStart;
    QFrame* parentFrame;
};
