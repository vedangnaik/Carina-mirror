#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>
#include <map>
#include "Sensor.h"
#include "Actuator.h"

enum Transition {
    Proceed,
    Abort
};

class State {
public:
    std::string id;
    std::string name;
    std::string safetyRating;
    std::string description;

    std::vector<std::string> actionOrder;
    std::map<std::string, std::vector<SensorOption>> sensorOptions;
    std::map<std::string, std::vector<ActuatorOption>> actuatorOptions;
    std::map<Transition, std::map<std::string, SensorCheck>> sensorChecks;
    std::map<Transition, std::map<std::string, ActuatorCheck>> actuatorChecks;

    std::map<Transition, std::string> transitions;
};

//    std::vector<std::pair<std::string, std::vector<unsigned int>>> actionOptions;
//    std::map<Transition, std::map<std::string, std::vector<unsigned int>>> actionsChecks;


#endif // STATE_H
