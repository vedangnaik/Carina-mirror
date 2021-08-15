#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QFormLayout>
#include "Sensor.h"
#include "ui_RecalibrationWindow.h"
#include "easylogging++.h"

namespace Ui {
class RecalibrationWindow;
}

class RecalibrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RecalibrationWindow(Sensor* sensor, QWidget *parent = nullptr);
    ~RecalibrationWindow();

private:
    void accept() override;

    Sensor* sensorToRecalibrate;
    Ui::RecalibrationWindow* ui;
};
