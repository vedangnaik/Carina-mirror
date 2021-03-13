#include <QApplication>
#include <chrono>
#include <ctime>
#include "GSManager.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);

    // Create a new logger for logging all the sensor data.
    // We will use the default logger for normal Carina events.
    el::Loggers::getLogger("sensorValueLogger");

    // Configure the default logger.
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    // The filename will be the current date and time.
    std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string now = std::string(std::ctime(&now_time));
    std::replace(now.begin(), now.end(), ' ', '_');
    defaultConf.setGlobally(el::ConfigurationType::Filename, now + "_events.log");
    el::Loggers::reconfigureLogger("default", defaultConf);

    // Configure the sensor value logger.
    // It will use the same date and time, but not write to stdout.
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Filename, now + "_data.log");
    defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
    el::Loggers::reconfigureLogger("sensorValueLogger", defaultConf);

    // Start the QApplication. Logging can be used immediately.
    QApplication a(argc, argv);
    GSManager gsm;
    LOG(INFO) << "Carina has started.";
    gsm.show();
    int r = a.exec();
    LOG(INFO) << "Carina has ended.";

    return r;
}
