#pragma once

#include <QWizardPage>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QValidator>
#include "DAQManagerWizard.h"

class DAQCalibrationPage : public QWizardPage
{
    Q_OBJECT
public:
    DAQCalibrationPage(QWidget *parent = nullptr);
    void initializePage() override;
};

