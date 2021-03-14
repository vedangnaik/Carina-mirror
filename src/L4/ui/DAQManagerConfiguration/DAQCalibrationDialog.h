#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QValidator>
#include <QScrollArea>
#include <QGroupBox>
#include <QLabel>
#include <DAQManager.h>

class DAQCalibrationDialog : public QDialog
{
    Q_OBJECT
public:
    DAQCalibrationDialog(std::unique_ptr<DAQManager> daqm, QWidget *parent = nullptr);
    void accept() override;
    std::unique_ptr<DAQManager> takeDAQManager();
private:
    std::unique_ptr<DAQManager> daqm;
};
