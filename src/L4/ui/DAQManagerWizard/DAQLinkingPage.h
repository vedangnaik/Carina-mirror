#pragma once

#include <QWizardPage>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QValidator>
#include "DAQManagerWizard.h"
#include "UIHelpers.h"

class DAQLinkingPage : public QWizardPage
{
    Q_OBJECT
public:
    DAQLinkingPage(std::vector<std::string> sensorIDs, QWidget *parent = nullptr);
    void initializePage() override;
    void cleanupPage() override;
    std::map<std::string, std::string> sensorLinks;
};

