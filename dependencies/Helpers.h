#pragma once

#include <QLayout>
#include <QLayoutItem>
#include <QWidget>
#include <QVariantMap>
#include <QString>
#include <string>
#include <QJsonArray>

namespace Helpers {
    void clearLayout(QLayout* l);
    void checkForKeyAndConversionValidity(const QVariantMap& args, const QString& key, int targetTypeId, const std::string& exceptionMsg);
    std::array<std::pair<double, double>, 5> parseCalibrationPointsFromArgs(const std::string& id, const QVariantMap& args);
}
