#include "DAQPlaceHolder.h"

DAQPlaceholder::DAQPlaceholder(ClocksModule* cm, SVGIC* svgic) {
    this->cm = cm;
    this->svgic = svgic;
}

void DAQPlaceholder::startAcquisition() {
    std::vector<std::string> sensorIDs = this->svgic->getSensorIDs();
    connect(this->cm->HundredMsTimer, &QTimer::timeout, this, [=]() {
        for (std::string id : sensorIDs) {
            this->svgic->updateValue(id, random() % 1000);
        }
    });
}
