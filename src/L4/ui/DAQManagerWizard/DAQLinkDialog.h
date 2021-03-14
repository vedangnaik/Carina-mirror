#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QValidator>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QLabel>
#include <DAQManager.h>

class DAQLinkDialog : public QDialog
{
    Q_OBJECT
public:
    DAQLinkDialog(std::unique_ptr<DAQManager> daqm, QWidget *parent = nullptr);
    void accept() override;
    std::unique_ptr<DAQManager> takeDAQManager();
private:
    std::unique_ptr<DAQManager> daqm;
};

