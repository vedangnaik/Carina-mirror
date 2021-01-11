#pragma once

#include <QDialog>

namespace Ui {
    class SensorToDAQLinker;
}

class SensorToDAQLinker : public QDialog {
    Q_OBJECT
public:
    explicit SensorToDAQLinker(QWidget *parent = nullptr);
    ~SensorToDAQLinker();
private slots:
    void reject() override;
    void accept() override;
private:
    Ui::SensorToDAQLinker *ui;
};

