#ifndef MOVABLEQLABEL_H
#define MOVABLEQLABEL_H

#include <QLabel>
#include <QFrame>
#include <QMouseEvent>

class MovableQLabel : public QLabel {
public:
    MovableQLabel(const QString& text, QFrame* parent, Qt::WindowFlags f = Qt::WindowFlags()) : QLabel(text, parent, f), parentFrame(parent) {};
    MovableQLabel(QFrame* parent, Qt::WindowFlags f = Qt::WindowFlags()) : QLabel(parent, f), parentFrame(parent) {};
    ~MovableQLabel() = default;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    QPoint cursorPosOnMoveStart;
    QFrame* parentFrame;
};

#endif // MOVABLEQLABEL_H
