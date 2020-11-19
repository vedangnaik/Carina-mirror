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
    State(std::string id,
          std::string name,
          std::string safetyRating,
          std::string description,
          std::vector<std::string> actionsOrder,
          std::map<std::string, std::vector<SensorOption>> sensorOptions,
          std::map<std::string, std::vector<ActuatorOption>> actuatorOptions,
          std::map<Transition, std::map<std::string, SensorCheck>> sensorChecks,
          std::map<Transition, std::map<std::string, ActuatorCheck>> actuatorChecks,
          std::map<Transition, std::string> transitions
        ):
        id(id),
        name(name),
        safetyRating(safetyRating),
        description(description),
        actionsOrder(actionsOrder),
        sensorOptions(sensorOptions),
        actuatorOptions(actuatorOptions),
        sensorChecks(sensorChecks),
        actuatorChecks(actuatorChecks),
        transitions(transitions) {};

    std::map<Transition, std::map<std::string, SensorCheck>> getSensorChecks() {
        return this->sensorChecks;
    }

    std::map<Transition, std::map<std::string, ActuatorCheck>> getActuatorChecks() {
        return this->actuatorChecks;
    }

    std::map<Transition, std::string> getTransitions() {
        return this->transitions;
    }

    const std::string id;
    const std::string name;
    const std::string safetyRating;
    const std::string description;

    const std::vector<std::string> actionsOrder;
    const std::map<std::string, std::vector<SensorOption>> sensorOptions;
    const std::map<std::string, std::vector<ActuatorOption>> actuatorOptions;
    const std::map<Transition, std::map<std::string, SensorCheck>> sensorChecks;
    const std::map<Transition, std::map<std::string, ActuatorCheck>> actuatorChecks;

    const std::map<Transition, std::string> transitions;
};

#endif // STATE_H
