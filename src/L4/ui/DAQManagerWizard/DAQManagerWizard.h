#pragma once

#include <QWizard>
#include "easylogging++.h"
#include "DAQScanPage.h"
#include "DAQCalibrationPage.h"

class DAQManagerWizard : public QWizard
{
    Q_OBJECT
public:
    DAQManagerWizard(QWidget* parent = nullptr);
    void accept() override;
    void reject() override;
    std::map<std::string, bool> abstractDAQData;
};




