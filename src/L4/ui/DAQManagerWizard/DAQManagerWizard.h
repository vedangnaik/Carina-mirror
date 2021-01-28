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
    DAQManagerWizard(std::vector<std::string> sensorIDs, SVGIC& svgic, QWidget* parent = nullptr);
    static std::unique_ptr<DAQManager> setupDAQManager(std::vector<std::string> sensorIDs, SVGIC& svgic);
    std::unique_ptr<DAQManager> daqm;
    std::map<std::string, bool> abstractDAQData;
private:
    void accept() override;
    void reject() override;
    SVGIC& svgic;
};
