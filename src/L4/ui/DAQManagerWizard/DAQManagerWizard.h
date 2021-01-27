#pragma once

#include <QWizard>
#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include "ui_DAQManagerFactory.h"
#include "easylogging++.h"

class DAQManagerWizard : public QWizard
{
    Q_OBJECT
public:
    DAQManagerWizard(QWidget* parent = nullptr);
    void accept() override;
    std::map<std::string, bool> abstractDAQData;
};


class DataSourceAcquisitionPage : public QWizardPage
{
    Q_OBJECT
public:
    DataSourceAcquisitionPage(QWidget* parent = nullptr);
    void initializePage() override;
private:
    Ui::DAQManagerFactory *ui;
};



class DummyLastPage: public QWizardPage
{
    Q_OBJECT
public:
    DummyLastPage(QWidget* parent = nullptr);
    void initializePage() override;
private:
    QLabel* l;
};
