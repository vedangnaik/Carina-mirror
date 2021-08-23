#include "Helpers.h"

void Helpers::clearLayout(QLayout* l) {
    QLayoutItem* child;
    while ((child = l->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
}

/**
     * Throws an exception is key isn't in args or the QVariant at args[key] cannot be converted to targetTypeID.
     * @param args
     * @param key
     * @param targetTypeId Must be a member of the enumeration QMetaType. Right now, conversion to types outside this enum are not supported.
     * @param exceptionMsg
     */
void
Helpers::checkForKeyAndConversionValidity(const QVariantMap& args, const QString& key, int targetTypeId, const std::string& exceptionMsg) {
    if (!args.contains(key) || !args[key].canConvert(targetTypeId)) {
        throw std::domain_error(exceptionMsg);
    }
}

std::vector<std::pair<double, double>>
Helpers::parseCalibrationPointsFromArgs(const std::string& id, const QVariantMap &args) {
    // It appears that canConvert returns false with QJsonArray even though the conversion is fine. Hence, we are checking with QJsonValue and isArray.
    Helpers::checkForKeyAndConversionValidity(args, "calibration", QMetaType::QJsonValue, id + ": Sensor must contain a set of valid calibration points.");
    if (!args["calibration"].toJsonValue().isArray()) {
        throw std::domain_error(id + ": Calibration points must be a valid JSON array.");
    }

    QJsonArray pointsFromFile = args["calibration"].toJsonArray();
    std::vector<std::pair<double, double>> calibrationPoints;
    for (const auto& point : pointsFromFile) {
        // Error checking for the individual point set
        if (!point.isArray() || point.toArray().size() != 2) {
            throw std::domain_error(id + ": Calibration point must be a valid JSON array with exactly 2 elements.");
        }

        std::pair<double, double> t {
            point.toArray().at(0).toDouble(),
            point.toArray().at(1).toDouble()
        };
        calibrationPoints.push_back(t);
    }
    return calibrationPoints;
}
