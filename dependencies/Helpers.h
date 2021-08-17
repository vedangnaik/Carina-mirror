#pragma once

#include <QLayout>
#include <QLayoutItem>
#include <QWidget>
#include <QVariantMap>
#include <QString>
#include <string>

namespace Helpers {
    void clearLayout(QLayout* l);
    void checkForKeyAndConversionValidity(const QVariantMap& args, const QString& key, int targetTypeId, const std::string& exceptionMsg);
};
