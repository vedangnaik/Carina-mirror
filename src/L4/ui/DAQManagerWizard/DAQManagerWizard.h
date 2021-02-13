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

enum DAQManagerWizardAction {
    Manufacture,
    Recalibrate,
    Relink
};

class DAQManagerWizard : public QWizard
{
    Q_OBJECT
public:
    DAQManagerWizard(std::unique_ptr<DAQManager> daqm, DAQManagerWizardAction a, std::vector<std::string> sensorIDs = {}, QWidget* parent = nullptr);
    static std::unique_ptr<DAQManager> manufactureDAQManager(std::vector<std::string> sensorIDs);
    static std::unique_ptr<DAQManager> reconfigureDAQManager();
//    static std::unique_ptr<DAQManager> recalibrateDAQs(std::unique_ptr<DAQManager>);
//    static std::unique_ptr<DAQManager> relinkSensors(std::unique_ptr<DAQManager>, std::vector<std::string> sensorIDs);
    std::unique_ptr<DAQManager> getDAQManager();

    std::map<std::string, bool> abstractDAQData;
    std::unique_ptr<DAQManager> daqm;
private:
    bool done;

    void manufactureDAQManager();
//    void recalibrateDAQs();
//    void relinkSensors();

    static DAQManagerWizard* dqm;
};
