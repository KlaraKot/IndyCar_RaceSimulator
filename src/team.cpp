#include "Team.h"
#include "Car.h"
#include "CarController.h"

#include <ncurses.h>

#include <fstream>
#include <iostream>



Team::Team(std::string initName, std::string driverName, Car &teamCar, CarController &carController)
    : teamName(initName), driverName(driverName), teamCar(teamCar), carController(carController) {
    strategyVersion = "A";
    accident = "no";
    accidentLap = -1;
    hasAccidentOccurred = false;
}


void Team::makeDecision(int x, int y, const WeatherStrategy& weatherStrategy, int totalLaps) {
    if (teamCar.getCurrentLap() >= totalLaps) {
        return; 
    }
    
    if (shouldHaveAccident()) {
        triggerAccident();
    }
    
    int tireWearMultiplier = weatherStrategy.getTireWearMultiplier();
    int speedMultiplier = weatherStrategy.getSpeedMultiplier();
    
    int speedIntensity = 1; 
    if (weatherStrategy.getWeatherType() == "sunny") {
        speedIntensity = 2;
     } else if (weatherStrategy.getWeatherType() == "rainy") {
        speedIntensity = 1;
    }
    
    bool justCompletedLap = (teamCar.direction == 'r' && teamCar.lastDirection == 'u');
    bool shouldPitStop = (teamCar.getCarTiresUse() < 2 || hasAccidentOccurred) && !teamCar.inPitStop && justCompletedLap;
    
    if (hasAccidentOccurred && justCompletedLap && !teamCar.inPitStop) {
        shouldPitStop = true;
    }

    teamCar.lastDirection = teamCar.direction;

    if(shouldPitStop){
        teamCar.goToPitStop();
    }
    
    handleAccidentRecovery();

    if (teamCar.inPitStop) {
        int pitStopPosition_x = x + 25; 
        int pitStopPosition_y = y;
        carController.goToPitStop(x, y, pitStopPosition_x, pitStopPosition_y);
    }
    else {
        for (int i = 0; i < speedIntensity; i++) {
            carController.driveAroundTheTrack(x, y);
        }
        
        int tireWearAmount = 2 * tireWearMultiplier;
        teamCar.decreaseTiresUse(tireWearAmount);
    }
}

void Team::changeTireType() {
    if (currentTireType == firstTire) {
        currentTireType = secondTire;
        teamCar.setTireType(secondTire);
    }
}

std::string Team::getCurrentTireType() const {
    return currentTireType;
}

void Team::setAccidentConfig(const std::string& accident, int accidentLap) {
    this->accident = accident;
    this->accidentLap = accidentLap;
    this->hasAccidentOccurred = false;
}

bool Team::shouldHaveAccident() const {
    bool shouldAccident = (accident == "yes" && 
                          accidentLap >= 0 && 
                          !hasAccidentOccurred && 
                          teamCar.getCurrentLap() == accidentLap);
    return shouldAccident;
}

void Team::triggerAccident() {
    hasAccidentOccurred = true;
}

void Team::handleAccidentRecovery() {
    if (hasAccidentOccurred && !teamCar.inPitStop) {
        hasAccidentOccurred = false;
    }
}


bool Team::isAccidentOccurred() const {
    return hasAccidentOccurred;
}
