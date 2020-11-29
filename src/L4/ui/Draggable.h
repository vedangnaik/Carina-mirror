#ifndef DRAGGABLE_H
#define DRAGGABLE_H

#include <QWidget>
#include <QFrame>
#include <QMouseEvent>
#include <iostream>

template <typename T>
class Draggable : public T {
    static_assert(std::is_base_of<QWidget, T>::value, "T must inherit from QWidget");
public:
    Draggable(QFrame* parent) : T(parent), parentFrame{parent} {
        std::cout << "Constructed" << std::endl;
    };
    ~Draggable() {
        std::cout << "Destructed" << std::endl;
    }
protected:
    void mouseMoveEvent(QMouseEvent* event) override {
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
        // call the parent's mouse press event to keep the clicking action
        T::mousePressEvent(event);
    };
private:
    QPoint cursorPosOnMoveStart;
    QFrame* parentFrame;
};

#endif // DRAGGABLE_H
