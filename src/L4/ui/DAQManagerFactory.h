#pragma once

#include <QWidget>
#include "DAQManager.h"

namespace Ui {
    class DAQManagerFactory;
}

class DAQManagerFactory : public QWidget {
    Q_OBJECT
public:
    explicit DAQManagerFactory(QWidget *parent = nullptr);
    std::unique_ptr<DAQManager> getConfiguredDAQManager();
    ~DAQManagerFactory();

private:
    Ui::DAQManagerFactory *ui;
};
