#include "ActuatorsPresenter.h"

void ActuatorsPresenter::notify(const std::string id, const bool status) {
    if (this->subscribers.find(id) != this->subscribers.end()) {
        for (APOC* apoc : this->subscribers.at(id)) {
            apoc->displayStatus(status);
        }
    }
}

void ActuatorsPresenter::subscribe(std::string id, APOC *apoc) {
    if (this->subscribers.find(id) == this->subscribers.end()) {
        this->subscribers.insert(std::make_pair(id, std::vector<APOC*>{apoc}));
    } else {
        this->subscribers.at(id).push_back(apoc);
    }
}

void ActuatorsPresenter::unsubscribe(std::string id, APOC* apoc) {
    auto& v = this->subscribers.at(id);
    v.erase(std::remove(v.begin(), v.end(), apoc), v.end());
}
