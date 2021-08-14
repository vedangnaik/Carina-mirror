#include "SensorsPresenter.h"

void SensorsPresenter::notify(std::string id, double value) {
    if (this->subscribers.find(id) != this->subscribers.end()) {
        for (SPOC* spoc : this->subscribers.at(id)) {
            spoc->displayValue(value);
        }
    }
}

void SensorsPresenter::subscribe(std::string id, SPOC* spoc) {
    std::lock_guard<std::mutex> guard(this->subscribersMutex);
    if (this->subscribers.find(id) == this->subscribers.end()) {
        this->subscribers.insert(std::make_pair(id, std::vector<SPOC*>{spoc}));
    } else {
        this->subscribers.at(id).push_back(spoc);
    }
    // lock_guard goes out of scope, unlocks mutex here
}

void SensorsPresenter::unsubscribe(std::string id, SPOC* spoc) {
    std::lock_guard<std::mutex> guard(this->subscribersMutex);
    auto& v = this->subscribers.at(id);
    v.erase(std::remove(v.begin(), v.end(), spoc), v.end());
    // lock_guard goes out of scope, unlocks mutex here
}
