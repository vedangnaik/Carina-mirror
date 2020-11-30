#include "SensorsPresenter.h"

void SensorsPresenter::displaySensorValue(const std::string id, const float value) {
    if (this->subscribers.find(id) != this->subscribers.end()) {
        for (SPOC* spoc : this->subscribers.at(id)) {
            spoc->displaySensorValue(value);
        }
    }
}

void SensorsPresenter::subscribe(std::string id, SPOC* spoc) {
    this->cm.stop();
    if (this->subscribers.find(id) == this->subscribers.end()) {
        this->subscribers.insert(std::make_pair(id, std::vector<SPOC*>{spoc}));
    } else {
        this->subscribers.at(id).push_back(spoc);
    }
    this->cm.start();
}

void SensorsPresenter::unsubscribe(std::string id, SPOC* spoc) {
    this->cm.stop();
    auto v = this->subscribers.at(id);
    v.erase(std::remove(v.begin(), v.end(), spoc), v.end());
    this->cm.start();
}
