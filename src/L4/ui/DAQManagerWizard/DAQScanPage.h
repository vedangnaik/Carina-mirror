#pragma once

#include <QWizardPage>
#include <QComboBox>
#include <QCheckBox>
#include "DAQManagerWizard.h"
#include "ui_DAQManagerFactory.h"

class DAQScanPage : public QWizardPage
{
    Q_OBJECT
public:
    DAQScanPage(QWidget* parent = nullptr);
    void initializePage() override;
private:
    Ui::DAQManagerFactory *ui;
};
