#ifndef AIR_TRAFFIC_CONTROLLER_H
#define AIR_TRAFFIC_CONTROLLER_H

#include <queue>
#include <mutex>

// Class declaration for AirTrafficController
class AirTrafficController {
private:
    bool asleep;                    // Flag to track ATC's state (asleep/awake)
    int trafficPatternCount;        // Number of aircraft in the traffic pattern
    bool runwayClear;               // Flag to track runway status
    std::queue<int> landingQueue;   // Queue for aircraft waiting for landing permission
public:
    std::mutex mtx; // Mutex for thread synchronization
    
    // Constructor
    AirTrafficController();

    // Method to wake up the AirTrafficController
    void wakeUp();

    // Method to make the AirTrafficController fall asleep
    void fallAsleep();

    // Method to check if the AirTrafficController is asleep
    bool isAsleep() const;

    // Method to add an aircraft to the traffic pattern
    void addToTrafficPattern(int id);

    // Method to remove an aircraft from the traffic pattern
    void removeFromTrafficPattern();

    // Method to check if the traffic pattern is full
    bool isTrafficPatternFull() const;
};

// Function to simulate aircraft landing
void simulateLanding(int id, AirTrafficController& atc, long long& totalLandingTime);

#endif // AIR_TRAFFIC_CONTROLLER_H
