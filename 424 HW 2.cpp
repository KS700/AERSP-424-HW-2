// Keegan Scott
// 424 HW 2

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <condition_variable>
#include <queue>
#include <atomic>
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
// Question 1 End--------------------------------------------------------------------------------

// Question 2--------------------------------------------------------------------------------
std::mutex tool_mutex;
std::condition_variable tool_cv;
int available_tools = 5;
int current_robot = 1;
int robots_finished = 0;
std::chrono::steady_clock::time_point start_time;

void performTask(int robotId) {
    std::unique_lock<std::mutex> lock(tool_mutex);
    while ((robotId % 2 != 0 && current_robot != 1) ||
        (robotId % 2 == 0 && current_robot != 2)) {
        tool_cv.wait(lock);
    }

    while (available_tools < 2) {
        tool_cv.wait(lock);
    }
    available_tools -= 2;
    std::cout << "Robot " << robotId << " acquired tools and starts performing a task." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    lock.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(5));

    lock.lock();
    available_tools += 2;
    std::cout << "Robot " << robotId << " finished the task and returned the tools." << std::endl;
    lock.unlock();
    tool_cv.notify_all();

    if (robotId % 2 == 0) {
        current_robot = 1;
    }
    else {
        current_robot = 2;
    }
    tool_cv.notify_all();

    // Increase the counter of robots finished
    {
        std::lock_guard<std::mutex> counter_lock(tool_mutex);
        robots_finished++;
    }

    // Check if all robots have finished
    if (robots_finished == 5) {
        std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
        std::cout << "Total duration: " << duration << " seconds" << std::endl;
    }
}
// Question 2 End--------------------------------------------------------------------------------

// Question 3--------------------------------------------------------------------------------
std::mutex mtx;
std::condition_variable cv;
std::atomic<int> numAircrafts{0};
std::atomic<bool> atcTalking{false};
std::atomic<bool> runwayFree{true};
std::queue<int> waitingAircrafts;

void pilot(int pilotId) {
    std::unique_lock<std::mutex> lock(mtx);

    // Request landing
    std::cout << "Aircraft #" << pilotId << " requesting landing." << std::endl;

    // Wait for ATC to become available or traffic pattern to have space
    while (atcTalking || (!runwayFree && numAircrafts >= 3)) {
        waitingAircrafts.push(pilotId);
        cv.wait(lock);
    }

    // Check if the traffic pattern is full
    if (!runwayFree && numAircrafts >= 3) {
        std::cout << "Approach pattern full. Aircraft #" << pilotId << " redirected to another airport." << std::endl;
        return;
    }

    // Pilot establishes communication with ATC
    atcTalking = true;
    std::cout << "Aircraft #" << pilotId << " is cleared to land." << std::endl;

    // Simulate landing process
    std::this_thread::sleep_for(std::chrono::seconds(1));
    runwayFree = true;

    // ATC becomes available
    atcTalking = false;
    std::cout << "Runway is now free." << std::endl;

    // Notify other pilots and ATC
    cv.notify_all();
}

void airTrafficController() {   // DOES NOT WORK---------------------------------------------------------------------------------------------------------------
    std::unique_lock<std::mutex> lock(mtx);

    while (numAircrafts < 10) {
        // Wait for incoming aircraft
        cv.wait(lock);

        // If all aircraft have landed, exit loop
        if (numAircrafts == 10) {
            break;
        }

        // Check if there are waiting aircrafts
        if (!waitingAircrafts.empty()) {
            int waitingPilotId = waitingAircrafts.front();
            waitingAircrafts.pop();

            // Pilot establishes communication with ATC
            atcTalking = true;
            std::cout << "Aircraft #" << waitingPilotId << " is cleared to land." << std::endl;

            // Simulate landing process
            std::this_thread::sleep_for(std::chrono::seconds(1));
            runwayFree = true;

            // ATC becomes available
            atcTalking = false;
            std::cout << "Runway is now free." << std::endl;

            // Notify other pilots and ATC
            cv.notify_all();
        }
    }
}
// Question 3 End--------------------------------------------------------------------------------

// Question 4--------------------------------------------------------------------------------
//namespace plt = matplotlibcpp;------------------------------------------------------------------------------------------------------------------------------
// Question 4 End--------------------------------------------------------------------------------

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
    start_time = std::chrono::steady_clock::now();

    std::thread robots[5];
    for (int i = 0; i < 5; ++i) {
        robots[i] = std::thread(performTask, i + 1);
    }

    for (int i = 0; i < 5; ++i) {
        robots[i].join();
    }
std::cout << "############# End Question 2 #############\n" << std::endl;

std::cout << "############### Question 3 ###############" << std::endl;
    std::vector<std::thread> aircraftThreads;

    // Create ATC thread
    std::thread atcThread(airTrafficController);

    // Create threads for 10 pilots
    for (int i = 0; i < 10; ++i) {
        aircraftThreads.emplace_back(pilot, i);
        // Simulate random arrival time for each aircraft
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // Increment number of aircrafts
        numAircrafts++;
        // Notify ATC of incoming aircraft
        cv.notify_one();
    }

    // Join pilot threads
    for (auto& t : aircraftThreads) {
        t.join();
    }

    // Join ATC thread
    atcThread.join();
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


// Written answers to final 2 HW questions------MAKE TXT FILES AND ZIP WITH CODE-------------------------------------------------------------------------------------------------------------

/* Question 5--------------------------------------------------------------------------------
    The tool might become duplicated????????????

*/

/* Question 6--------------------------------------------------------------------------------
    One aircraft will be in the process of landing and if that takes too long all others will be diverted. Infinite loop.


*/