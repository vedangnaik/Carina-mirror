#include "SensorsPresenter.h"

void SensorsPresenter::displaySensorValue(const std::string id, const float value) {
    try {
        for (SPOC* spoc : this->subscribers.at(id)) {
            spoc->displaySensorValue(value);
        }
    }  catch (std::out_of_range& e) {
        std::cout << "Sensor index not found in subscribers map\n";
        std::cout << e.what() << '\n';
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
