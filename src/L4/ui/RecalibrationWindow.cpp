#include "RecalibrationWindow.h"

RecalibrationWindow::RecalibrationWindow(Sensor* s, QWidget *parent) :
    QDialog(parent), sensorToRecalibrate{s}, ui{new Ui::RecalibrationWindow}
{
    ui->setupUi(this);
    LOG(INFO) << s->id;
    // Connect dialog buttons.
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QGridLayout* g = (QGridLayout*)this->ui->pointsWidget->layout();
    // Connect the Add Calibration Point button.
    connect(ui->addCalibrationButton, &QPushButton::clicked, this, [=]() {
        QLineEdit* rawValue = new QLineEdit("1", this);
        QLineEdit* unitsValue = new QLineEdit("1", this);
        QPushButton* deleteBtn = new QPushButton("X", this);
        // This function removes the widgets from this row in the layout and then deletes them.
        connect(deleteBtn, &QPushButton::clicked, this, [=]() {
            g->removeWidget(rawValue);
            g->removeWidget(unitsValue);
            g->removeWidget(deleteBtn);
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
        // We just click the add button ourselves to add the point, then change the LineEdit values.
        ui->addCalibrationButton->click();
        int lastRow = g->rowCount() - 1; // The row count - 1 is the index of the last row, which is the one just added.
        QLineEdit* rawValue = (QLineEdit*)g->itemAtPosition(lastRow, 0)->widget();
        rawValue->setText(QString::number(point.first));
        QLineEdit* unitsValue = (QLineEdit*)g->itemAtPosition(lastRow, 1)->widget();
        unitsValue->setText(QString::number(point.second));
    }
}

RecalibrationWindow::~RecalibrationWindow()
{
    delete ui;
}

void
RecalibrationWindow::accept()
{
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
