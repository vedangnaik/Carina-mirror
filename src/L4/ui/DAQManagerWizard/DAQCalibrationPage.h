#pragma once

#include <QWizardPage>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QValidator>
#include "DAQManagerWizard.h"
#include "UIHelpers.h"

class DAQCalibrationPage : public QWizardPage
{
    Q_OBJECT
public:
    DAQCalibrationPage(QWidget *parent = nullptr);
    void initializePage() override;
    void cleanupPage() override;
    std::map<std::string, std::pair<std::array<double, 5>, std::array<double, 5>>> calibrationPoints;
};

