#include "ConcreteSensorFactory.h"

Sensor* ConcreteSensorFactory::createSensor(const std::string& id, const QVariantMap& args) {
    if (args["type"] == "DummyDAQ") {
        // Create list of calibration points
        std::array<std::pair<double, double>, 5> calibrationPoints;
        if (args.contains("calibration")) {
            for (int i = 0; i < 5; i++) {
                std::pair<double, double> t{
                    args["calibration"].toJsonArray().at(i).toArray().at(0).toDouble(),
                    args["calibration"].toJsonArray().at(i).toArray().at(1).toDouble()
                };
                calibrationPoints.at(i) = t;
            }
        } else {
            // TODO: Exception
        }

        return new DummyDAQ(id, calibrationPoints);

    } else if (args["type"] == "OneWireDAQ") {
        // TODO: Use QVariantMap to decipher this.
        return nullptr;

    } else {
        // TODO: Exception
        return nullptr;
    }
}