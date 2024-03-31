#pragma once

#include <iostream>
#include <string>
#include <vector>

// Overall Class
class Sensor {
public:
    virtual void gatherData() = 0;
    virtual void processData() = 0;
};

// Class Inheritance
class TemperatureSensor : public Sensor {
public:
    void gatherData() override;
    void processData() override;
};

// Class Inheritance
class PressureSensor : public Sensor {
public:
    void gatherData() override;
    void processData() override;
};

// Class Inheritance
class PositionSensor : public Sensor {
public:
    void gatherData() override;
    void processData() override;
};

// Factory Method
class SensorFactory {
public:
    static Sensor* createSensor(const std::string& sensorType);
};

// Control Class
class AerospaceControlSystem {
public:
    void addSensor(Sensor* sensor);
    void monitorAndAdjust();

private:
    std::vector<Sensor*> sensors;
};
