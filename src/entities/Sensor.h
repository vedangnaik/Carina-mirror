#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <vector>

class Sensor {
public:
    Sensor(std::string id, std::string name);
    std::string id;
    std::string name;
    std::vector<float> values;
};

#endif // SENSOR_H
