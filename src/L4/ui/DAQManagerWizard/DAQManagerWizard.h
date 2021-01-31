#pragma once

#include <QWizard>
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
    DAQManagerWizard(std::unique_ptr<DAQManager> daqm, QWidget* parent = nullptr);
    static std::unique_ptr<DAQManager> manufactureDAQManager(std::vector<std::string> sensorIDs);
    static std::unique_ptr<DAQManager> recalibrateDAQs(std::unique_ptr<DAQManager>);

    std::map<std::string, bool> abstractDAQData;
    std::unique_ptr<DAQManager> daqm;
private:
    void manufactureDAQManager();
    void recalibrateDAQs();
};
