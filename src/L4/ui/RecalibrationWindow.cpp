#include "RecalibrationWindow.h"

RecalibrationWindow::RecalibrationWindow(Sensor* s, QWidget *parent) :
    QDialog(parent), sensorToRecalibrate{s}, ui{new Ui::RecalibrationWindow}
{
    ui->setupUi(this);
    LOG(INFO) << s->id;
    // Connect dialog buttons
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    // Copy calibration values into line edits
    QFormLayout* f = (QFormLayout*)this->ui->pointsWidget->layout();
    // The rows are numbered from 1 to 5 instead of 0 to 4, so we have two counters in here
    for (int row = 1, i = 0; row <= 5; row++, i++) {
        QLineEdit* rawValue = (QLineEdit*)f->itemAt(row, QFormLayout::LabelRole)->widget();
        rawValue->setText(QString::number(s->calibrationPoints.at(i).first));
        QLineEdit* unitsValue = (QLineEdit*)f->itemAt(row, QFormLayout::FieldRole)->widget();
        unitsValue->setText(QString::number(s->calibrationPoints.at(i).second));
    }

}

RecalibrationWindow::~RecalibrationWindow()
{
    delete ui;
}

void RecalibrationWindow::accept() {
    // Copy line edit values back into calibration points, then recalibrate sensor
    QFormLayout* f = (QFormLayout*)this->ui->pointsWidget->layout();
    // The rows are numbered from 1 to 5 instead of 0 to 4, so we have two counters in here
    for (int row = 1, i = 0; row <= 5; row++, i++) {
        QLineEdit* rawValue = (QLineEdit*)f->itemAt(row, QFormLayout::LabelRole)->widget();
        this->sensorToRecalibrate->calibrationPoints.at(i).first = rawValue->text().toDouble();
        QLineEdit* unitsValue = (QLineEdit*)f->itemAt(row, QFormLayout::FieldRole)->widget();
        this->sensorToRecalibrate->calibrationPoints.at(i).second = unitsValue->text().toDouble();
    }
    this->sensorToRecalibrate->calibrate();
    this->done(QDialog::Accepted);
}
