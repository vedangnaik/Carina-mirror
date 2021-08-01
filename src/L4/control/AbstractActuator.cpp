#include "AbstractActuator.h"

AbstractActuator::AbstractActuator(const std::string deviceID)
    : deviceID{deviceID}, state{false}
{}
