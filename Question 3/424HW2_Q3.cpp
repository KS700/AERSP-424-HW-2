#include "Question3.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>

AirTrafficController::AirTrafficController() : asleep(true), trafficPatternCount(0), runwayClear(true) {}

void AirTrafficController::wakeUp() {
    asleep = false;
    std::cout << "ATC wakes up.\n";
}

void AirTrafficController::fallAsleep() {
    if (trafficPatternCount == 0) {
        asleep = true;
        std::cout << "ATC falls asleep.\n";
    }
}

bool AirTrafficController::isAsleep() const {
    return asleep;
}

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

void AirTrafficController::removeFromTrafficPattern() {
    trafficPatternCount--;
    std::cout << "Runway is now free.\n";
    runwayClear = true; // Set runway status to clear after aircraft leaves

    // Check if there are aircraft waiting in the landing queue
    if (!landingQueue.empty()) {
        int nextAircraft = landingQueue.front(); // Get the next aircraft in line
        landingQueue.pop(); // Remove the aircraft from the queue
        std::cout << "ATC approves landing for Aircraft #" << nextAircraft << " (from queue).\n";
        runwayClear = false; // Set runway status to not clear
    }
}

bool AirTrafficController::isTrafficPatternFull() const {
    return trafficPatternCount >= 3;
}

void simulateLanding(int id, AirTrafficController& atc, long long& totalLandingTime) {
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    {
        std::lock_guard<std::mutex> guard(atc.mtx); // Lock to ensure ATC state consistency
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
    // Continue with landing process or other actions after landing
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> guard(atc.mtx); // Lock to ensure ATC state consistency
        atc.removeFromTrafficPattern();
        atc.fallAsleep();  // Check if ATC needs to fall asleep after each landing
    }
    auto end = std::chrono::high_resolution_clock::now(); // End time for each landing
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start); // Calculate landing duration
    totalLandingTime += duration.count(); // Add landing duration to total time
}

int main() {
std::cout << "############### Question 3 ###############" << std::endl;
    AirTrafficController atc;
    long long landingTime = 0;

    std::thread aircraftThreads[10];
    for (int i = 0; i < 10; ++i) {
        aircraftThreads[i] = std::thread(simulateLanding, i + 1, std::ref(atc), std::ref(landingTime));
        std::this_thread::sleep_for(std::chrono::milliseconds(125));
    }

    for (int i = 0; i < 10; ++i) {
        aircraftThreads[i].join();
    }

    std::cout << "Total landing time: " << landingTime << " seconds\n";
std::cout << "############# End Question 3 #############\n" << std::endl;

    return 0;
}