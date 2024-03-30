#ifndef AIR_TRAFFIC_CONTROLLER_H
#define AIR_TRAFFIC_CONTROLLER_H

#include <queue>
#include <mutex>

class AirTrafficController {
private:
    bool asleep; 
    int trafficPatternCount;
    bool runwayClear; 
    std::queue<int> landingQueue; 
    

public:
    std::mutex mtx; 
    
    AirTrafficController();

    void wakeUp();

    void fallAsleep();

    bool isAsleep() const;

    void addToTrafficPattern(int id);

    void removeFromTrafficPattern();

    bool isTrafficPatternFull() const;
};

void simulateLanding(int id, AirTrafficController& atc, long long& totalLandingTime);

#endif // AIR_TRAFFIC_CONTROLLER_H
