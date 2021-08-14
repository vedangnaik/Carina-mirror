#pragma once

#include "State.h"
#include "easylogging++.h"
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QVariantMap>
#include <QJsonObject>
#include <iostream>
#include <QJsonArray>
#include <QFileDialog>
#include <stdexcept>
#include <filesystem>
#include <tuple>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
#include <vector>
#include <utility>


class ProcessFileParser {
public:
    static std::tuple<
        const std::map<const std::string, QVariantMap>, 
        const std::map<const std::string, QVariantMap>,
        const std::map<const std::string, const State>
    >
    parseProcessFile(const std::string& filepath);

private:
    static std::map<const std::string, QVariantMap> parseSensors(const QJsonObject& sensorsObj);
    static std::map<const std::string, QVariantMap> parseActuators(const QJsonObject& actuatorsObj);
    static std::map<const std::string, const State> parseStates(const QJsonObject& statesObj, std::vector<std::string> sensorIDs, std::vector<std::string> actuatorIDs);
    static std::map<std::string, SensorCheck> parseSensorChecks(const QJsonValue& checks, std::vector<std::string> sensorIDs);
    static std::map<std::string, ActuatorCheck> parseActuatorChecks(const QJsonValue& checks, std::vector<std::string> actuatorIDs);
};



/*
 * Exceptions that can be thrown during the parsing process.
 */

// This is the base class of all exceptions that can occur when parsing the JSON process file.
// Subclass a new exception from it if a new bug/error needs to be handled.
class ProcessFileParseError : public std::runtime_error {
protected:
    explicit ProcessFileParseError(const std::string& message) : std::runtime_error(message) {}
};

// These exceptions handle file-related problems.
class FileOpenError : public ProcessFileParseError {
public:
    explicit FileOpenError(const std::string& fileName) :
        ProcessFileParseError("The process file '" + fileName + "' could not be opened.") {}
};
class InvalidFileTypeError : public ProcessFileParseError {
public:
    explicit InvalidFileTypeError(const std::string& fileName) :
        ProcessFileParseError("The process file '" + fileName + "' must be of type JSON.") {}
};

// Actions whose IDs are not recognized need their state ID to be printed as well.
class InvalidActionIDError : public ProcessFileParseError {
public:
    InvalidActionIDError(const std::string& stateID, const std::string& actionID) :
        ProcessFileParseError("State '" + stateID + "': '" + actionID + "' is neither an actuator nor a sensor.") {}
};
// Empty actions inside states need the state ID to be printed as well.
class EmptyActionIDError: public ProcessFileParseError {
public:
    explicit EmptyActionIDError(const std::string& stateID) :
        ProcessFileParseError("State '" + stateID + "': actions must have non-empty IDs.") {}
};

// These two classes work together to display an invalid range check
class InvalidSensorRangeCheck : public std::exception {
public:
    explicit InvalidSensorRangeCheck(std::string sensorID) : sensorID(std::move(sensorID)) {}
    const std::string sensorID;
};
class InvalidSensorRangeCheckError : public ProcessFileParseError {
public:
    InvalidSensorRangeCheckError(const std::string& stateID, const std::string& sensorID) :
        ProcessFileParseError("State '" + stateID + "': '" + sensorID + "' range check must be of form [a, b].") {}
};

// Same for invalid actuator position check
class InvalidActuatorPositionCheck : public std::exception {
public:
    explicit InvalidActuatorPositionCheck(std::string actuatorID) : actuatorID(std::move(actuatorID)) {}
    const std::string actuatorID;
};
class InvalidActuatorPositionCheckError : public ProcessFileParseError {
public:
    InvalidActuatorPositionCheckError(const std::string& stateID, const std::string& actuatorID) :
        ProcessFileParseError("State '" + stateID + "': '" + actuatorID + "' position check must be either 'open' or 'close'.") {}
};

// This exception is the base class for empty IDs for the entites: sensors, actuators, states.
// It ideally will never need to be subclassed beyond these three.
class EmptyIDError : public ProcessFileParseError {
protected:
    explicit EmptyIDError(const std::string& object) :
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
