// Keegan Scott
// 424 HW 2

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
//#include "matplotlibcpp.h"-------------------------------------------------------------------------------------------------------------------
#include <cmath>


// Question 1--------------------------------------------------------------------------------
// Abstract base class for Sensor
class Sensor {
public:
    virtual void gatherData() = 0;
    virtual void processData() = 0;
};

// Derived class for Temperature Sensor
class TemperatureSensor : public Sensor {
public:
    void gatherData() override {
        std::cout << "Gathering data from Temperature Sensor." << std::endl;
    }

    void processData() override {
        std::cout << "Processing data from Temperature Sensor." << std::endl;
    }
};

// Derived class for Pressure Sensor
class PressureSensor : public Sensor {
public:
    void gatherData() override {
        std::cout << "Gathering data from Pressure Sensor." << std::endl;
    }

    void processData() override {
        std::cout << "Processing data from Pressure Sensor." << std::endl;
    }
};

// Derived class for Position Sensor
class PositionSensor : public Sensor {
public:
    void gatherData() override {
        std::cout << "Gathering data from Position Sensor." << std::endl;
    }

    void processData() override {
        std::cout << "Processing data from Position Sensor." << std::endl;
    }
};

// Factory class for creating sensors
class SensorFactory {
public:
    static Sensor* createSensor(const std::string& sensorType) {
        if (sensorType == "Temperature")
            return new TemperatureSensor();
        else if (sensorType == "Pressure")
            return new PressureSensor();
        else if (sensorType == "Position")
            return new PositionSensor();
        else
            return nullptr;
    }
};

// Control system class
class AerospaceControlSystem {
public:
    void addSensor(Sensor* sensor) {
        sensors.push_back(sensor);
    }

    void monitorAndAdjust() {
        for (auto sensor : sensors) {
            sensor->gatherData();
            sensor->processData();
            std::cout << "Adjusting controls based on sensor data." << std::endl;
        }
    }

private:
    std::vector<Sensor*> sensors;
};

// Question 2--------------------------------------------------------------------------------
void performTask(int robotId, int tool1, int tool2) {
    // Simulating time to grab tools
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Simulating time to perform task
    std::cout << "Robot " << robotId << " is performing its task..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Simulating time to return tools
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Robot " << robotId << " completed its task and returned tools." << std::endl;
}

// Question 3--------------------------------------------------------------------------------
std::mutex mtx; // Mutex for controlling access to the ATC
class ATC {
public:
    void processAircraft(int aircraftId) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate landing process
            mtx.lock(); // Lock mutex to access ATC
            if (trafficPattern.size() < maxTrafficPatternSize) {
                if (!isTalking) {
                    std::cout << "ATC: Pilot " << aircraftId << ", you're clear to land." << std::endl;
                    isTalking = true;
                }
                trafficPattern.push_back(aircraftId);
                mtx.unlock(); // Release mutex
                return;
            }
            else {
                std::cout << "ATC: Traffic pattern is full, Pilot " << aircraftId << " diverting to other airports." << std::endl;
                mtx.unlock(); // Release mutex
                return;
            }
        }
    }

    void releaseATC() {
        mtx.lock(); // Lock mutex to access ATC
        if (!trafficPattern.empty()) {
            int releasedAircraft = trafficPattern.front();
            trafficPattern.erase(trafficPattern.begin());
            std::cout << "ATC: Pilot " << releasedAircraft << ", you've landed. ATC going to sleep." << std::endl;
            isTalking = false;
        }
        mtx.unlock(); // Release mutex
    }

private:
    std::vector<int> trafficPattern;
    bool isTalking = false;
    const int maxTrafficPatternSize = 3;
};

// Question 4--------------------------------------------------------------------------------
//namespace plt = matplotlibcpp;--------------------------------------------------------------------------------------------------------------

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

std::cout << "############### Question 2 ###############" << std::endl;
    // Robot ids and tool ids
    int robots[5] = { 1, 2, 3, 4, 5 };
    int tools[5][2] = { {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 1} };

    // Start threads for each robot
    std::thread threads[5];
    for (int i = 0; i < 5; ++i) {
        threads[i] = std::thread(performTask, robots[i], tools[i][0], tools[i][1]);
    }

    // Join threads
    for (int i = 0; i < 5; ++i) {
        threads[i].join();
    }
std::cout << "############# End Question 2 #############\n" << std::endl;

std::cout << "############### Question 3 ###############" << std::endl;
    ATC atc;
    std::vector<std::thread> threads2;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    for (int i = 1; i <= 10; ++i) {
        int delay = dis(gen);
        std::this_thread::sleep_for(std::chrono::seconds(delay)); // Randomize arrival time
        threads2.push_back(std::thread(&ATC::processAircraft, &atc, i));
    }

    for (auto& t : threads2) {
        t.join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for the last aircraft to land
    atc.releaseATC(); // Release the ATC after the last aircraft lands
std::cout << "############# End Question 3 #############\n" << std::endl;

std::cout << "############### Question 4 ###############" << std::endl;
    //// Create data
    //std::vector<double> x, y;

    //// Generate x values from -10 to 10
    //for (double i = -10; i <= 10; i += 0.1) {
    //    x.push_back(i);
    //    // Calculate y values for the equation y = sin(x)
    //    y.push_back(std::sin(i));
    //}

    //// Plot data
    //plt::plot(x, y);

    //// Set labels
    //plt::xlabel("X");
    //plt::ylabel("Y");
    //plt::title("Plot of y = sin(x)");

    //// Show plot
    //plt::show();
std::cout << "############# End Question 4 #############\n" << std::endl;

    return 0;
}


// Written answers to final 2 HW questions

/* Question 5--------------------------------------------------------------------------------
    The tool might become duplicated????????????

*/

/* Question 6--------------------------------------------------------------------------------
    One aircraft will be in the process of landing and if that takes too long all others will be diverted.????????????


*/