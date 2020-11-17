#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <vector>

enum class SensorOption {

};

struct SensorCheck {
    int a;
    int b;
};

class Sensor {
public:
    Sensor(std::string id, std::string name);
    std::string id;
    std::string name;
    std::vector<float> values;
};

#endif // SENSOR_H
