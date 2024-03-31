#include "Question1.h"

// Call Sensor Classes
void TemperatureSensor::gatherData() {
    std::cout << "Gathering data from Temperature Sensor." << std::endl;
}

void TemperatureSensor::processData() {
    std::cout << "Processing data from Temperature Sensor." << std::endl;
}

void PressureSensor::gatherData() {
    std::cout << "Gathering data from Pressure Sensor." << std::endl;
}

void PressureSensor::processData() {
    std::cout << "Processing data from Pressure Sensor." << std::endl;
}

void PositionSensor::gatherData() {
    std::cout << "Gathering data from Position Sensor." << std::endl;
}

void PositionSensor::processData() {
    std::cout << "Processing data from Position Sensor." << std::endl;
}

// Factory Method
Sensor* SensorFactory::createSensor(const std::string& sensorType) {
    if (sensorType == "Temperature")
        return new TemperatureSensor();
    else if (sensorType == "Pressure")
        return new PressureSensor();
    else if (sensorType == "Position")
        return new PositionSensor();
    else
        return nullptr;
}

// Call Control Class
void AerospaceControlSystem::addSensor(Sensor* sensor) {
    sensors.push_back(sensor);
}

void AerospaceControlSystem::monitorAndAdjust() {
    for (auto sensor : sensors) {
        sensor->gatherData();
        sensor->processData();
        std::cout << "Adjusting controls based on sensor data." << std::endl;
    }
}


int main() {
std::cout << "############### Question 1 ###############" << std::endl;
    AerospaceControlSystem ctrlSys;

    // Add sensors to control system
    ctrlSys.addSensor(SensorFactory::createSensor("Temperature"));
    ctrlSys.addSensor(SensorFactory::createSensor("Pressure"));
    ctrlSys.addSensor(SensorFactory::createSensor("Position"));

    // Monitor and adjust based on sensor data
    ctrlSys.monitorAndAdjust();
std::cout << "############# End Question 1 #############\n" << std::endl;

    return 0;
}