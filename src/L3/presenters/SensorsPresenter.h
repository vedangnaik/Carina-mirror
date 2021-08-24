#pragma once

#include "SensorsManager.h"
#include <stdexcept>
#include <map>
#include <mutex>
#include <algorithm>

class SPOC {
public:
    virtual void displayValue(double value) = 0;
    virtual ~SPOC() = default;
};

class SPIC {
public:
    virtual void subscribe(std::string id, SPOC* spoc) = 0;
    virtual void unsubscribe(std::string id, SPOC* spoc) = 0;
    virtual ~SPIC() = default;
};

class SensorsPresenter : public SMOC, public SPIC {
public:
    void notify(std::string id, double value) override;
    void subscribe(std::string id, SPOC* spoc) override;
    void unsubscribe(std::string id, SPOC* spoc) override;
private:
    std::map<std::string, std::vector<SPOC*>> subscribers;
    std::mutex subscribersMutex;
};
