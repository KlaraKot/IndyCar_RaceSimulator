#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include "Car.h"

class CarController {
  public:
    CarController(Car &car);
    Car &car;

    void moveLeft(int pos_x_track, int pos_y_track, int size);
    void moveRight(int pos_x_track, int pos_y_track, int size);
    void moveUp(int pos_x_track, int pos_y_track, int size);
    void moveDown(int pos_x_track, int pos_y_track, int size);
    void moveQ(int pos_x_track, int pos_y_track, int size);
    void driveAroundTheTrack(int pos_x_track, int pos_y_track);
    void moveAroundTheTrack(int currentPosition_x, int currentPosition_y);
    void moveToPitstop(int currentPosition_x, int currentPosition_y, int pitStopPosition_x,
                       int pitStopPosition_y);
    void goToPitStop(int pos_x_track, int pos_y_track, int pitStopPosition_x,
                     int pitStopPosition_y);
};

#endif