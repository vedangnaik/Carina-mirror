#pragma once

#include <QWizard>
#include "DAQManager.h"
#include "easylogging++.h"
#include "DAQScanPage.h"
#include "DAQCalibrationPage.h"
#include "DAQLinkingPage.h"
#include "AbstractDAQ.h"
#include "DummyDAQ.h"

class DAQManagerWizard : public QWizard
{
    Q_OBJECT
public:
    DAQManagerWizard(std::vector<std::string> sensorIDs, QWidget* parent = nullptr);
    void accept() override;
    void reject() override;
    std::map<std::string, bool> abstractDAQData;
    std::unique_ptr<DAQManager> daqm;
};




