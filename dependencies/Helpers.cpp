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

std::array<std::pair<double, double>, 5>
Helpers::parseCalibrationPointsFromArgs(const std::string& id, const QVariantMap &args) {
    Helpers::checkForKeyAndConversionValidity(args, "calibration", QMetaType::QJsonArray, id + ": Sensor must contain set of 5 valid calibration points.");

    std::array<std::pair<double, double>, 5> calibrationPoints;
    for (int i = 0; i < 5; i++) {
        std::pair<double, double> t{
            args["calibration"].toJsonArray().at(i).toArray().at(0).toDouble(),
            args["calibration"].toJsonArray().at(i).toArray().at(1).toDouble()
        };
        calibrationPoints.at(i) = t;
    }
    return calibrationPoints;
}
