#pragma once

#include <iostream>
#include <string>
#include <vector>

class Sensor {
public:
    virtual void gatherData() = 0;
    virtual void processData() = 0;
};

class TemperatureSensor : public Sensor {
public:
    void gatherData() override;
    void processData() override;
};

class PressureSensor : public Sensor {
public:
    void gatherData() override;
    void processData() override;
};

class PositionSensor : public Sensor {
public:
    void gatherData() override;
    void processData() override;
};

class SensorFactory {
public:
    static Sensor* createSensor(const std::string& sensorType);
};

class AerospaceControlSystem {
public:
    void addSensor(Sensor* sensor);
    void monitorAndAdjust();

private:
    std::vector<Sensor*> sensors;
};
