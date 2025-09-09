#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <unordered_map>

#include "Car.h"
#include "CarController.h"
#include "WeatherStrategy.h"

class Team {
  public:
    Car &teamCar;
    CarController &carController;
    std::string teamName;
    std::string driverName;
    std::string decision;

    std::string strategyVersion;
    std::string firstTire;
    std::string secondTire;
    std::string currentTireType;

    std::string accident;
    int accidentLap;
    bool hasAccidentOccurred;
    
    Team(std::string initName, std::string driverName, Car &teamCar, CarController &carController);

    void makeDecision(int x, int y, const WeatherStrategy& weatherStrategy, int totalLaps); 
        
    void logPitStopPhase();
    
    void changeTireType();
    std::string getCurrentTireType() const;
    
    void setAccidentConfig(const std::string& accident, int accidentLap);
    bool shouldHaveAccident() const;
    void triggerAccident();
    void handleAccidentRecovery();
    bool isAccidentOccurred() const;
};

#endif  // TEAM_H