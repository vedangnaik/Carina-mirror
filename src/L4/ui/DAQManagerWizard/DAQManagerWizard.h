#pragma once

#include <QWizard>
#include <QPushButton>
#include "DAQManager.h"
#include "easylogging++.h"
#include "DAQScanPage.h"
#include "DAQCalibrationPage.h"
#include "DAQLinkingPage.h"
#include "AbstractDAQ.h"
#include "DummyDAQ.h"
#include "SerialPortDAQ.h"
#include "AiMCCDAQ.h"



class DAQManagerWizard : public QWizard
{
    Q_OBJECT
public:
    DAQManagerWizard(std::vector<std::string> sensorIDs, QWidget* parent = nullptr);
    void accept() override;
    void reject() override;
    std::map<std::string, bool> abstractDAQData;

    static std::unique_ptr<DAQManager> manufactureDAQManager(std::vector<std::string> sensorIDs);
    static std::unique_ptr<DAQManager> reconfigureDAQManager();
    static DAQManagerWizard* dqm;
private:
    bool done;

    static std::unique_ptr<DAQManager> assembleDAQManager();
};
