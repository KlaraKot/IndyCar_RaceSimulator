#include "car.h"

#include "Driver.h"

Car::Car(int initX, int initY, int size, int firstTireType, int secondTireType, int tireUse, char initDirection,
         char initDecision)
    : x(initX),
      y(initY),
      size(size),
      firstTireType(firstTireType),
      secondTireType(secondTireType),
      tireUse(tireUse),
      direction(initDirection),
      decision(initDecision),
      inPitStop(false),
      pitStopPhase(PitStopPhase::NOT_IN_PITSTOP),
      pitStopProgress(0),
      pitStopWaitTime(0),
      currentLap(0),
      lastDirection(initDirection),
      hasCompletedLap(false),
      startingX(initX),
      startingY(initY),
      finishPosition(0) {
    setStartingPosition(initX, initY);
}

void Car::decreaseTiresUse(int decreaseAmount) {
    tireUse -= decreaseAmount;
    if (tireUse < 0) {
        tireUse = 0;
    }
}


int Car::getCarTiresUse() { return tireUse; }

int Car::setTiresUse(int value) { 
    tireUse = value; 
    return tireUse;
}


void Car::goToPitStop() {
    inPitStop = true;
    pitStopPhase = PitStopPhase::ENTERING_PITSTOP;
    pitStopProgress = 0;
}

int Car::getTireUseFromType(const std::string& tireType) {
    if (tireType == "S") {
        return 200;  // Soft tires
    } else if (tireType == "H") {
        return 500;  // Hard tires
    } else if (tireType == "W") {
        return 300;  // Wet tires
    } else {
        return 300;  // Default tires
    }
}

void Car::setTireType(const std::string& tireType) {
    tireUse = getTireUseFromType(tireType);
}

void Car::checkLapCompletion(int trackX, int trackY) {
    bool nearStart = (x >= startingX - 3 && x <= startingX + 3 && y >= startingY - 3 && y <= startingY + 3);
    
    bool backToStartDirection = (direction == 'r' && lastDirection == 'u');

    if (nearStart && backToStartDirection && !inPitStop && !hasCompletedLap) {
        currentLap++;
        hasCompletedLap = true;
    }
    
}


void Car::resetLapCompletion() {
    hasCompletedLap = false;
}

int Car::getCurrentLap() const {
    return currentLap;
}

void Car::setStartingPosition(int startX, int startY) {
    startingX = startX;
    startingY = startY;
}

int Car::getFinishPosition() const {
    return finishPosition;
}

void Car::setFinishPosition(int position) {
    finishPosition = position;
}
