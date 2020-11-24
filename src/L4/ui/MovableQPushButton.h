#ifndef MOVABLEQPUSHBUTTON_H
#define MOVABLEQPUSHBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QFrame>
#include <iostream>

class MovableQPushButton : public QPushButton {
public:
    // The three constructors for QPushButton have been overloaded to enforce that a
    // parent QFrame is passed in. This ensures that the geometry calculations in
    // the mouseMoveEvent work properly. It's kinda restrictive, but there you go.
    // I can now see why movable widgets aren't included as an option by default in Qt lol
    MovableQPushButton(const QIcon& icon, const QString& text, QFrame* parent) : QPushButton(icon, text, parent), parentFrame(parent) {};
    MovableQPushButton(const QString& text, QFrame* parent) : QPushButton(text, parent), parentFrame(parent) {};
    MovableQPushButton(QFrame* parent) : QPushButton(parent), parentFrame(parent) {};
    ~MovableQPushButton() = default;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    QPoint cursorPosOnMoveStart;
    QFrame* parentFrame;
};

#endif // MOVABLEQPUSHBUTTON_H
