#include "SensorsPresenter.h"

void SensorsPresenter::displaySensorValue(const std::string id, const float value) {
    for (auto spoc : this->subscribers) {
        spoc->displaySensorValue(id, value);
    }
}

void SensorsPresenter::subscribe(SPOC *spoc) {
    this->subscribers.push_back(spoc);
}
