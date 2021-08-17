#include "Helpers.h"

void Helpers::clearLayout(QLayout* l) {
    QLayoutItem* child;
    while ((child = l->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    /**
     * Throws an exception is key isn't in args or the QVariant at args[key] cannot be converted to targetTypeID.
     * @param args
     * @param key
     * @param targetTypeId Must be a member of the enumeration QMetaType. Right now, conversion to types outside this enum are not supported.
     * @param exceptionMsg
     */
    void
    checkForKeyAndConversionValidity(const QVariantMap& args, const QString& key, int targetTypeId, const std::string& exceptionMsg) {
        if (!args.contains(key) || !args[key].canConvert(targetTypeId)) {
            throw std::domain_error(exceptionMsg);
        }
    }
}
