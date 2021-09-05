#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QFormLayout>
#include "Sensor.h"
#include "ui_RecalibrationWindow.h"
#include "easylogging++.h"
#include <QPushButton>

namespace Ui {
class RecalibrationWindow;
}

class RecalibrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RecalibrationWindow(Sensor* sensor, QWidget *parent = nullptr);
    ~RecalibrationWindow() override;

private:
    void accept() override;
    void reject() override;

    Sensor* sensorToRecalibrate;
    Ui::RecalibrationWindow* ui;
    std::vector<std::pair<QLineEdit*, QLineEdit*>> currentCalibrationPoints;
};
