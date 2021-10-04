#include "ConcreteActuatorFactory.h"

Actuator* ConcreteActuatorFactory::createActuator(const std::string& id, const QVariantMap& args) {
    if (!args.contains("type")) {
        throw std::domain_error(id + ": Actuator must have a type.");
    }
    std::string type = args["type"].toString().toStdString();

    if (type == "DummyActuator") {
        return this->createDummyActuator(id, args);
    }
    else if (type == "PCA9685Actuator") {
        #ifdef WIRINGPI_AVAILABLE
        return this->createPCA9685Actuator(id, args);
        #else
        throw std::domain_error(id + ": This Carina has not been compiled to support PCA9685 actuators. Please recompile with the -DWIRINGPI_AVAILABLE flag and ensure wiringPiI2C.h is available on your platform.");
        #endif
    }
    else if (type == "SolenoidActuator") {
        #ifdef WIRINGPI_AVAILABLE
        return this->createSolenoidActuator(id, args);
        #else
        throw std::domain_error(id + ": This Carina has not been compiled to support solenoid actuators. Please recompile with the -DWIRINGPI_AVAILABLE flag and ensure wiringPiI2C.h is available on your platform.");
        #endif
    }

    throw std::domain_error(id + ": Type '" + type + "' is invalid or not supported.");
}

Actuator *ConcreteActuatorFactory::createDummyActuator(const std::string &id,  const QVariantMap &args) {
    (void)args; // "Use" this to stop the compiler yelling.
    return new DummyActuator(id);
}

#ifdef WIRINGPI_AVAILABLE
Actuator *ConcreteActuatorFactory::createSolenoidActuator(const std::string &id, const QVariantMap &args) {
    return new SolenoidActuator(id, args["relayChannel"].toUInt(), args["gpioPin"].toUInt(), args["nominallyPowered"].toBool());
}
#endif

#ifdef WIRINGPI_AVAILABLE
Actuator *ConcreteActuatorFactory::createPCA9685Actuator(const std::string &id, const QVariantMap &args) {
    // Make sure all required fields for the constructor are here
    Helpers::checkForKeyAndConversionValidity(args, "channel", QMetaType::UInt, id + ": PCA9685 actuator must contain a positive integer 'channel'.");
    Helpers::checkForKeyAndConversionValidity(args, "openAngle", QMetaType::Double, id + ": PCA9685 actuator must contain a valid real number 'openAngle'.");
    Helpers::checkForKeyAndConversionValidity(args, "closeAngle", QMetaType::Double, id + ": PCA9685 actuator must contain a valid real number 'closeAngle'.");
    Helpers::checkForKeyAndConversionValidity(args, "overAngle", QMetaType::Double, id + ": PCA9685 actuator must contain a valid real number 'overAngle'.");
    Helpers::checkForKeyAndConversionValidity(args, "overDelay", QMetaType::Int, id + ": PCA9685 actuator must contain a valid integer number 'overDelay'.");
    Helpers::checkForKeyAndConversionValidity(args, "servoMin", QMetaType::UInt, id + ": PCA9685 actuator must contain a positive integer 'servoMin'.");
    Helpers::checkForKeyAndConversionValidity(args, "servoMax", QMetaType::UInt, id + ": PCA9685 actuator must contain a positive integer 'servoMax'.");
    Helpers::checkForKeyAndConversionValidity(args, "angleMin", QMetaType::Double, id + ": PCA9685 actuator must contain a real number 'angleMin'.");
    Helpers::checkForKeyAndConversionValidity(args, "angleMax", QMetaType::Double, id + ": PCA9685 actuator must contain a real number 'angleMax'.");
    // Keys for the PWNServoDriver board - these are the unique identifier and the board's I2C address, if applicable
    Helpers::checkForKeyAndConversionValidity(args, "boardID", QMetaType::UInt, id + ": PCA9685 actuator must contain a positive integer 'boardID'.");

    const unsigned int boardID = args["boardID"].toUInt();
    Adafruit_PWMServoDriver* servoDriverBoard;
    // If the board already exists, use that one. Else, create a new PWM board and cache it.
    if (cachedServoDriverBoards.find(boardID) != cachedServoDriverBoards.end()) {
        servoDriverBoard = cachedServoDriverBoards.at(boardID); // This will never be null.
    } else {
        // If the key doesn't exist or is invalid, we use the default address
        try {
            Helpers::checkForKeyAndConversionValidity(args, "boardAddr", QMetaType::UInt, id + ": PCA9685 actuator must contain a positive integer 'boardID'.");
            servoDriverBoard = new Adafruit_PWMServoDriver(args["boardAddr"].toUInt());
        } catch (std::exception& e) {
            LOG(WARNING) << id << ": 'boardAddr' field for PWM servo driver board is either missing or invalid. Using default address of 0x40.";
            servoDriverBoard = new Adafruit_PWMServoDriver();
        }
        // Start the board here. TODO: May not be the best place to do this.
        servoDriverBoard->begin();
        servoDriverBoard->setOscillatorFrequency(27000000);
        servoDriverBoard->setPWMFreq(60.0);
        // Add it to the cache here
        cachedServoDriverBoards.insert({boardID, servoDriverBoard});
    }

    // 🥴
    return new PCA9685Actuator(id, args["channel"].toUInt(), args["openAngle"].toDouble(), args["closeAngle"].toDouble(), args["overAngle"].toDouble(), args["overDelay"].toInt(), args["servoMin"].toUInt(), args["servoMax"].toUInt(), args["angleMin"].toDouble(), args["angleMax"].toDouble(), std::shared_ptr<Adafruit_PWMServoDriver>(servoDriverBoard));
}
#endif
