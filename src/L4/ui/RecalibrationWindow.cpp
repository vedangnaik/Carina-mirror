#include "RecalibrationWindow.h"

RecalibrationWindow::RecalibrationWindow(Sensor* s, QWidget *parent) :
    QDialog(parent), sensorToRecalibrate{s}, ui{new Ui::RecalibrationWindow}
{
    LOG(INFO) << s->id << ": User has requested recalibration.";
    ui->setupUi(this);

    // Connect dialog buttons.
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QGridLayout* g = (QGridLayout*)this->ui->pointsWidget->layout();
    // Connect the Add Calibration Point button.
    connect(ui->addCalibrationButton, &QPushButton::clicked, this, [=]() {
        QLineEdit* rawValue = new QLineEdit("1", this);
        QLineEdit* unitsValue = new QLineEdit("1", this);
        QPushButton* deleteBtn = new QPushButton("X", this);
        this->currentCalibrationPoints.emplace_back(rawValue, unitsValue);

        // This function removes the widgets from this row in the layout and then deletes them.
        connect(deleteBtn, &QPushButton::clicked, this, [=]() {
            g->removeWidget(rawValue);
            g->removeWidget(unitsValue);
            g->removeWidget(deleteBtn);

            // Iterate through the vector and remove this pair. Pattern taken from https://en.cppreference.com/w/cpp/container/vector/erase.
            for (auto it = this->currentCalibrationPoints.begin(); it != this->currentCalibrationPoints.end(); ) {
                if (it->first == rawValue && it->second == unitsValue) {
                    it = this->currentCalibrationPoints.erase(it);
                } else {
                    ++it;
                }
            }

            // Delete the actual objects.
            delete rawValue;
            delete unitsValue;
            delete deleteBtn;
        });

        // We don't need to subtract 1 since we are adding a new row; the QGridLayout will automatically allocate space for this row and manage it.
        int row = g->rowCount();
        g->addWidget(rawValue, row, 0);
        g->addWidget(unitsValue, row, 1);
        g->addWidget(deleteBtn, row, 2);
    });

    // Create line edits for each set of calibration points and fill them.
    for (const auto& point : s->calibrationPoints) {
        ui->addCalibrationButton->click();
        this->currentCalibrationPoints.back().first->setText(QString::number(point.first));
        this->currentCalibrationPoints.back().second->setText(QString::number(point.second));
    }
}

RecalibrationWindow::~RecalibrationWindow()
{
    delete ui;
}

void
RecalibrationWindow::accept()
{
    LOG(INFO) << this->sensorToRecalibrate->id << ": User has accepted modified recalibration.";
    // Clear the existing calibration points.
    this->sensorToRecalibrate->calibrationPoints.clear();
    // Copy line edit values back into calibration points, then recalibrate sensor.
    if (this->currentCalibrationPoints.size() < 2) {
        LOG(WARNING) << this->sensorToRecalibrate->id << ": At least two calibration points are required. Default calibration of (1, 1), (2, 2) will be used.";
        this->sensorToRecalibrate->calibrationPoints.emplace_back(1.0, 1.0);
        this->sensorToRecalibrate->calibrationPoints.emplace_back(2.0, 2.0);
    } else {
        for (const auto& p : this->currentCalibrationPoints) {
            this->sensorToRecalibrate->calibrationPoints.emplace_back(p.first->text().toDouble(), p.second->text().toDouble());
        }
    }
    this->sensorToRecalibrate->calibrate();
    this->done(QDialog::Accepted);
}

void RecalibrationWindow::reject()
{
    LOG(INFO) << this->sensorToRecalibrate->id << ": User has rejected modified recalibration.";
    this->done(QDialog::Rejected);
}
