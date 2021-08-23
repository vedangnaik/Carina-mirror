#pragma once

#include "ActuatorsManager.h"
#include <map>
#include <vector>
#include <algorithm>

class APOC {
public:
    virtual void displayStatus(const bool status) = 0;
    virtual ~APOC() {}
};

class APIC {
public:
    virtual void subscribe(std::string id, APOC* apoc) = 0;
    virtual void unsubscribe(std::string id, APOC* apoc) = 0;
    virtual ~APIC() {}
};

class ActuatorsPresenter : public AMOC, public APIC {
public:
    void notify(const std::string id, const bool status) override;
    void subscribe(std::string id, APOC* apoc) override;
    void unsubscribe(std::string id, APOC* apoc) override;
private:
    std::map<std::string, std::vector<APOC*>> subscribers;
};
