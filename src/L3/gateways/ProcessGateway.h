#pragma once

#include "src/L1/entities/Actuator.h"
#include "src/L1/entities/State.h"
#include "src/L1/entities/Sensor.h"
#include "src/L2/services/easylogging++.h"

#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QJsonArray>
#include <QFileDialog>
#include <stdexcept>



struct ProcessData {
    std::map<std::string, Sensor*> sensors;
    std::map<std::string, Actuator*> actuators;
    std::map<std::string, State*> states;
};

class ProcessGateway {
public:
    ProcessGateway(const std::string filepath) : filepath(filepath) {};
    struct ProcessData parseProcessFile();
private:
    std::map<std::string, Sensor*> parseSensors(QJsonObject sensorsObj);
    std::map<std::string, Actuator*> parseActuators(QJsonObject actuatorsObj);
    std::map<std::string, State*> parseStates(QJsonObject statesObj, std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators);
    std::map<std::string, ActuatorCheck> parseActuatorChecks(QJsonValue checks, std::map<std::string, Actuator*> actuators);
    std::map<std::string, SensorCheck> parseSensorChecks(QJsonValue checks, std::map<std::string, Sensor*> sensors);

    const std::string filepath;
};



/*
 * Custom exceptions
 */

class ProcessFileParseException : public std::runtime_error {
public:
    ProcessFileParseException(std::string message) : std::runtime_error(message) {}
};

class InvalidActionIDError : public ProcessFileParseException {
public:
    InvalidActionIDError(std::string stateID, std::string actionID) :
        ProcessFileParseException("State '" + stateID + "': '" + actionID + "' is neither an actuator nor a sensor.") {}
};

class EmptyActionIDError: public ProcessFileParseException {
public:
    EmptyActionIDError(std::string stateID) :
        ProcessFileParseException("State '" + stateID + "': actions must have non-empty IDs.") {}
};

// These two classes work together to display an invalid range check
class InvalidSensorRangeCheck : public std::exception {
public:
    InvalidSensorRangeCheck(std::string sensorID) : sensorID(sensorID) {}
    const std::string sensorID;
};
class InvalidSensorRangeCheckError : public ProcessFileParseException {
public:
    InvalidSensorRangeCheckError(std::string stateID, std::string sensorID) :
        ProcessFileParseException("State '" + stateID + "': '" + sensorID + "' range check must be of form [a, b].") {}
};

// same for invalid actuator position check
class InvalidActuatorPositionCheck : public std::exception {
public:
    InvalidActuatorPositionCheck(std::string actuatorID) : actuatorID(actuatorID) {}
    const std::string actuatorID;
};
class InvalidActuatorPositionCheckError : public ProcessFileParseException {
public:
    InvalidActuatorPositionCheckError(std::string stateID, std::string actuatorID) :
        ProcessFileParseException("State '" + stateID + "': '" + actuatorID + "' position check must be either 'open' or 'close'.") {}
};

// This exception handles empty IDs for the entites: sensors, actuators, states.
class EmptyIDError : public ProcessFileParseException {
public:
    EmptyIDError(const std::string object) :
        ProcessFileParseException(object + " IDs must be non-empty.") {}
};
