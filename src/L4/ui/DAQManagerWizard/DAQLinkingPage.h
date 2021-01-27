#pragma once

#include <QWizardPage>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QValidator>
#include "DAQManagerWizard.h"

class DAQLinkingPage : public QWizardPage
{
    Q_OBJECT
public:
    DAQLinkingPage(std::vector<std::string> sensorIDs, QWidget *parent = nullptr);
    void initializePage() override;
private:
    const std::vector<std::string> sensorIDs;
};

