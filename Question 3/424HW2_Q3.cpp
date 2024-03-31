#include "Question3.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>

// AirTrafficController constructor
AirTrafficController::AirTrafficController() : asleep(true), trafficPatternCount(0), runwayClear(true) {}

// Wake up the Air Traffic Controller (ATC)
void AirTrafficController::wakeUp() {
    asleep = false;
    std::cout << "ATC wakes up.\n";
}

// Have ATC fall asleep if the traffic pattern is empty
void AirTrafficController::fallAsleep() {
    if (trafficPatternCount == 0) {
        asleep = true;
        std::cout << "ATC falls asleep.\n";
    }
}

// Check if the ATC is asleep
bool AirTrafficController::isAsleep() const {
    return asleep;
}

// Add an aircraft to the traffic pattern
void AirTrafficController::addToTrafficPattern(int id) {
    trafficPatternCount++;
    std::cout << "Aircraft #" << id << " requesting landing.\n";
    if (trafficPatternCount == 1 && runwayClear) {
        std::cout << "ATC approves landing for Aircraft #" << id << ".\n";
        runwayClear = false;
    }
    else {
        std::cout << "Aircraft #" << id << " waits for runway clearance.\n";
        landingQueue.push(id);
    }
}

// Remove an aircraft from the traffic pattern
void AirTrafficController::removeFromTrafficPattern() {
    trafficPatternCount--;
    std::cout << "Runway is now free.\n";
    runwayClear = true; 

    // Check if there are aircraft waiting in the landing queue
    if (!landingQueue.empty()) {
        int nextAircraft = landingQueue.front(); 
        landingQueue.pop(); 
        std::cout << "ATC approves landing for Aircraft #" << nextAircraft << " (from queue).\n";
        runwayClear = false; 
    }
}

// Check if the traffic pattern is full
bool AirTrafficController::isTrafficPatternFull() const {
    return trafficPatternCount >= 3;
}

// Function to simulate landing/diverting of an aircraft
void simulateLanding(int id, AirTrafficController& atc, long long& totalLandingTime) {
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    {
        std::lock_guard<std::mutex> guard(atc.mtx);
        if (atc.isAsleep()) {
            atc.wakeUp();
        }
        if (!atc.isTrafficPatternFull()) {
            atc.addToTrafficPattern(id);
        }
        else {
            std::cout << "Aircraft #" << id << " diverts to another airport.\n";
            return;
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> guard(atc.mtx); 
        atc.removeFromTrafficPattern();
        atc.fallAsleep();  
    }
    auto end = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start); 
    totalLandingTime += duration.count(); 
}

int main() {
std::cout << "############### Question 3 ###############" << std::endl;
    AirTrafficController atc;
    long long landingTime = 0;

    // Create threads for 10 aircraft
    std::thread aircraftThreads[10];
    for (int i = 0; i < 10; ++i) {
        aircraftThreads[i] = std::thread(simulateLanding, i + 1, std::ref(atc), std::ref(landingTime));
        std::this_thread::sleep_for(std::chrono::milliseconds(125));
    }

    // Join threads
    for (int i = 0; i < 10; ++i) {
        aircraftThreads[i].join();
    }

    // Output total landing time for all 10 aircraft together
    std::cout << "Total landing time: " << landingTime << " seconds\n";
std::cout << "############# End Question 3 #############\n" << std::endl;

    return 0;
}