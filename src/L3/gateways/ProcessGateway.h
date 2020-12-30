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
#include <filesystem>



struct ProcessData {
    std::map<std::string, Sensor*> sensors;
    std::map<std::string, Actuator*> actuators;
    std::map<std::string, State*> states;
};

class ProcessGateway {
public:
    ProcessGateway(const std::string filepath);
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
 * Exceptions that can be thrown during the parsing process.
 */

// This is the base class of all exceptions that can occur when parsing the JSON process file.
// Subclass a new exception from it if a new bug/error needs to be handled.
class ProcessFileParseError : public std::runtime_error {
protected:
    ProcessFileParseError(std::string message) : std::runtime_error(message) {}
};

// These exceptions handle file-related problems.
class FileOpenError : public ProcessFileParseError {
public:
    FileOpenError(const std::string fileName) :
        ProcessFileParseError("The process file '" + fileName + "' could not be opened.") {}
};
class InvalidFileTypeError : public ProcessFileParseError {
public:
    InvalidFileTypeError(const std::string fileName) :
        ProcessFileParseError("The process file '" + fileName + "' must be of type JSON.") {}
};

// Actions whose IDs are not recognized need their state ID to be printed as well.
class InvalidActionIDError : public ProcessFileParseError {
public:
    InvalidActionIDError(std::string stateID, std::string actionID) :
        ProcessFileParseError("State '" + stateID + "': '" + actionID + "' is neither an actuator nor a sensor.") {}
};
// Empty actions inside states need the state ID to be printed as well.
class EmptyActionIDError: public ProcessFileParseError {
public:
    EmptyActionIDError(std::string stateID) :
        ProcessFileParseError("State '" + stateID + "': actions must have non-empty IDs.") {}
};

// These two classes work together to display an invalid range check
class InvalidSensorRangeCheck : public std::exception {
public:
    InvalidSensorRangeCheck(std::string sensorID) : sensorID(sensorID) {}
    const std::string sensorID;
};
class InvalidSensorRangeCheckError : public ProcessFileParseError {
public:
    InvalidSensorRangeCheckError(std::string stateID, std::string sensorID) :
        ProcessFileParseError("State '" + stateID + "': '" + sensorID + "' range check must be of form [a, b].") {}
};

// Same for invalid actuator position check
class InvalidActuatorPositionCheck : public std::exception {
public:
    InvalidActuatorPositionCheck(std::string actuatorID) : actuatorID(actuatorID) {}
    const std::string actuatorID;
};
class InvalidActuatorPositionCheckError : public ProcessFileParseError {
public:
    InvalidActuatorPositionCheckError(std::string stateID, std::string actuatorID) :
        ProcessFileParseError("State '" + stateID + "': '" + actuatorID + "' position check must be either 'open' or 'close'.") {}
};

// This exception is the base class for empty IDs for the entites: sensors, actuators, states.
// It ideally will never need to be subclassed beyond these three.
class EmptyIDError : public ProcessFileParseError {
protected:
    EmptyIDError(const std::string object) :
        ProcessFileParseError(object + " IDs must be non-empty.") {}
};
class EmptySensorIDError : public EmptyIDError {
public:
    EmptySensorIDError() :
        EmptyIDError("Sensor") {}
};
class EmptyActuatorIDError : public EmptyIDError {
public:
    EmptyActuatorIDError() :
        EmptyIDError("Actuator") {}
};
class EmptyStateIDError : public EmptyIDError {
public:
    EmptyStateIDError() :
        EmptyIDError("State") {}
};
