#include "RecalibrationWindow.h"

RecalibrationWindow::RecalibrationWindow(Sensor* s, QWidget *parent) :
    QDialog(parent), sensorToRecalibrate{s}, ui{new Ui::RecalibrationWindow}
{
    ui->setupUi(this);
    LOG(INFO) << s->id;
    // Connect dialog buttons
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    // Create line edits for each set of calibration points and fill them.
    QFormLayout* f = (QFormLayout*)this->ui->pointsWidget->layout();
    // The rows are numbered from 1 to 5 instead of 0 to 4, so we have two counters in here
    for (const auto& point : s->calibrationPoints) {
        QLineEdit* rawValue = new QLineEdit(QString::number(point.first));
        QLineEdit* unitsValue = new QLineEdit(QString::number(point.second));
        f->addRow(rawValue, unitsValue);
    }
}

RecalibrationWindow::~RecalibrationWindow()
{
    delete ui;
}

void RecalibrationWindow::accept() {
    // Clear the existing calibration points.
    this->sensorToRecalibrate->calibrationPoints.clear();
    // Copy line edit values back into calibration points, then recalibrate sensor.
    QFormLayout* f = (QFormLayout*)this->ui->pointsWidget->layout();
    // We skip the first row since that's the header.
    for (int row = 1; row < f->rowCount(); row++) {
        QLineEdit* rawValue = (QLineEdit*)f->itemAt(row, QFormLayout::LabelRole)->widget();
        QLineEdit* unitsValue = (QLineEdit*)f->itemAt(row, QFormLayout::FieldRole)->widget();
        std::pair<double, double> t {rawValue->text().toDouble(), unitsValue->text().toDouble()};
        this->sensorToRecalibrate->calibrationPoints.push_back(t);
    }
    this->sensorToRecalibrate->calibrate();
    this->done(QDialog::Accepted);
}
