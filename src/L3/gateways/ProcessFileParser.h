#pragma once

#include "Actuator.h"
#include "State.h"
#include "Sensor.h"
#include "easylogging++.h"

#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QJsonArray>
#include <QFileDialog>
#include <stdexcept>
#include <filesystem>
#include <tuple>

using std::map;
using std::string;


class ProcessFileParser {
public:
    ProcessFileParser(const string filepath);
    std::tuple<map<const string, Sensor>, map<const string, Actuator>, map<const string, State>> parseProcessFile();
private:
    map<const string, Sensor> parseSensors(const QJsonObject& sensorsObj);
    map<const string, Actuator> parseActuators(const QJsonObject& actuatorsObj);
    map<const string, State> parseStates(const QJsonObject& statesObj, const map<const std::string, Sensor>& sensors, const map<const std::string, Actuator>& actuators);
    map<string, ActuatorCheck> parseActuatorChecks(const QJsonValue& checks, const map<const string, Actuator>& actuators);
    map<string, SensorCheck> parseSensorChecks(const QJsonValue& checks, const map<const string, Sensor>& sensors);

    const string filepath;
};



/*
 * Exceptions that can be thrown during the parsing process.
 */

// This is the base class of all exceptions that can occur when parsing the JSON process file.
// Subclass a new exception from it if a new bug/error needs to be handled.
class ProcessFileParseError : public std::runtime_error {
protected:
    ProcessFileParseError(string message) : std::runtime_error(message) {}
};

// These exceptions handle file-related problems.
class FileOpenError : public ProcessFileParseError {
public:
    FileOpenError(const string fileName) :
        ProcessFileParseError("The process file '" + fileName + "' could not be opened.") {}
};
class InvalidFileTypeError : public ProcessFileParseError {
public:
    InvalidFileTypeError(const string fileName) :
        ProcessFileParseError("The process file '" + fileName + "' must be of type JSON.") {}
};

// Actions whose IDs are not recognized need their state ID to be printed as well.
class InvalidActionIDError : public ProcessFileParseError {
public:
    InvalidActionIDError(string stateID, string actionID) :
        ProcessFileParseError("State '" + stateID + "': '" + actionID + "' is neither an actuator nor a sensor.") {}
};
// Empty actions inside states need the state ID to be printed as well.
class EmptyActionIDError: public ProcessFileParseError {
public:
    EmptyActionIDError(string stateID) :
        ProcessFileParseError("State '" + stateID + "': actions must have non-empty IDs.") {}
};

// These two classes work together to display an invalid range check
class InvalidSensorRangeCheck : public std::exception {
public:
    InvalidSensorRangeCheck(string sensorID) : sensorID(sensorID) {}
    const string sensorID;
};
class InvalidSensorRangeCheckError : public ProcessFileParseError {
public:
    InvalidSensorRangeCheckError(string stateID, string sensorID) :
        ProcessFileParseError("State '" + stateID + "': '" + sensorID + "' range check must be of form [a, b].") {}
};

// Same for invalid actuator position check
class InvalidActuatorPositionCheck : public std::exception {
public:
    InvalidActuatorPositionCheck(string actuatorID) : actuatorID(actuatorID) {}
    const string actuatorID;
};
class InvalidActuatorPositionCheckError : public ProcessFileParseError {
public:
    InvalidActuatorPositionCheckError(string stateID, string actuatorID) :
        ProcessFileParseError("State '" + stateID + "': '" + actuatorID + "' position check must be either 'open' or 'close'.") {}
};

// This exception is the base class for empty IDs for the entites: sensors, actuators, states.
// It ideally will never need to be subclassed beyond these three.
class EmptyIDError : public ProcessFileParseError {
protected:
    EmptyIDError(const string object) :
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
