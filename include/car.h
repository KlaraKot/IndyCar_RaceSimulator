#ifndef CAR_H
#define CAR_H

#include <string>

enum class PitStopPhase { NOT_IN_PITSTOP, ENTERING_PITSTOP, DRIVING_IN_PITSTOP, WAITING_IN_PITSTOP, EXITING_PITSTOP };

class Car {
  private:
  int tireUse;

  public:
    int x, y;
    int size; 
    char direction;
    char decision;
    bool inPitStop;
    int firstTireType;
    int secondTireType;  
    PitStopPhase pitStopPhase;
    int pitStopProgress; 
    int pitStopWaitTime; 
    

    int currentLap;  
    int lastDirection; 
    bool hasCompletedLap; 
    int startingX, startingY; 
    int finishPosition; 
    
    Car(int initX, int initY, int size, int firstTireType, int secondTireType, int tireUse, char initDirection,
        char initDecision);

    void goToPitStop();
    void decreaseTiresUse(int decreaseAmount);
    int getCarTiresUse();
    int setTiresUse(int value);
    
    // Tire type management
    static int getTireUseFromType(const std::string& tireType);
    void setTireType(const std::string& tireType);
    
    // Lap tracking functions
    void checkLapCompletion(int trackX, int trackY);
    void resetLapCompletion();
    int getCurrentLap() const;
    void setStartingPosition(int startX, int startY);
    int getFinishPosition() const;
    void setFinishPosition(int position);
};

#endif  // CAR_H