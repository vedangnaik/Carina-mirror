#pragma once

#include <QLayout>
#include <QLayoutItem>
#include <QWidget>

namespace uihelpers {
    // apparently should be in a source file, but eh
    // inline works, but idk what it does. A bit of a hack
    inline void clearLayout(QLayout* l) {
        QLayoutItem* child;
        while ((child = l->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
}
