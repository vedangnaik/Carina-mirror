# Force Qt version 5 or higher.
lessThan(QT_MAJOR_VERSION, 5): error("Qt verion 5 or greater needed.")

# Use c++17.
CONFIG += c++17

# Maximum compiler warnings.
CONFIG += warn_on
QMAKE_CXXFLAGS += -pedantic

# Enable warnings if deprecated Qt libraries are used.
DEFINES += QT_DEPRECATED_WARNINGS
