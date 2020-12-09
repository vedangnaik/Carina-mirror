#include "DAQPlaceHolder.h"

DAQPlaceholder::DAQPlaceholder(ClocksModule* cm, SVGIC* svgic) {
    this->cm = cm;
    this->svgic = svgic;
}

void DAQPlaceholder::startAcquisition() {
    connect(this->cm->HundredMsTimer, &QTimer::timeout, this, &DAQPlaceholder::acquire);
}

void DAQPlaceholder::stopAcquisition() {
    disconnect(this->cm->HundredMsTimer, &QTimer::timeout, this, &DAQPlaceholder::acquire);
}

void DAQPlaceholder::acquire() {
    for (std::string id : this->svgic->getSensorIDs()) {
        this->svgic->updateValue(id, random() % 1000);
    }
}
