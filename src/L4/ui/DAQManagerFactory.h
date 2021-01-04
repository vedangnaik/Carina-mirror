#pragma once

#ifdef ULDAQ_AVAILABLE
    #include <uldaq.h>
#endif
#include <QWidget>
#include "DAQManager.h"
#include "ui_daqmanagerfactory.h"
#include <QPushButton>
#include <QLabel>

namespace Ui {
    class DAQManagerFactory;
}

class DAQManagerFactory : public QWidget {
    Q_OBJECT
public:
    explicit DAQManagerFactory(QWidget *parent = nullptr);
    ~DAQManagerFactory();
private slots:
#ifdef ULDAQ_AVAILABLE
    void scanForMCCDAQs();
#endif
private:
    std::map<std::string, DAQDeviceHandler*> DAQs;
    std::unique_ptr<DAQManager> getConfiguredDAQManager();
    Ui::DAQManagerFactory *ui;
};
