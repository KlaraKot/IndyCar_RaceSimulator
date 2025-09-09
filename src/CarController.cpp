#include "CarController.h"

#include <ncurses.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#include "Car.h"

CarController::CarController(Car &car) : car(car) {}
void CarController::moveLeft(int pos_x_track, int pos_y_track, int size) { car.x--; }
void CarController::moveRight(int pos_x_track, int pos_y_track, int size) { car.x++; }
void CarController::moveUp(int pos_x_track, int pos_y_track, int size) { car.y--; }

void CarController::moveDown(int pos_x_track, int pos_y_track, int size) { car.y++; }
void CarController::moveQ(int pos_x_track, int pos_y_track, int size) {
    car.x = pos_x_track;
    car.y = pos_y_track - 5;
    car.direction = 'r';
}

void CarController::driveAroundTheTrack(int pos_x_track, int pos_y_track) {
    int track_size = 20;

    if (car.direction == 'r') {
        // Move right until reaching outside the right edge
        if (car.x >= pos_x_track + track_size + 2) {
            car.direction = 'd';
        } else {
            moveRight(pos_x_track, pos_y_track, track_size);
        }
    } else if (car.direction == 'd') {
        // Move down until reaching outside the bottom edge
        if (car.y >= pos_y_track + track_size + 1) {
            car.direction = 'l';
        } else {
            moveDown(pos_x_track, pos_y_track, track_size);
        }
    } else if (car.direction == 'l') {
        // Move left until reaching outside the left edge
        if (car.x <= pos_x_track - 3) {
            car.direction = 'u';
        } else {
            moveLeft(pos_x_track, pos_y_track, track_size);
        }
    } else if (car.direction == 'u') {
        // Move up until reaching outside the top edge
        if (car.y <= pos_y_track - 2) {
            car.direction = 'r';
        } else {
            moveUp(pos_x_track, pos_y_track, track_size);
        }
    } else if (car.direction == 'q') {
        moveQ(pos_x_track, pos_y_track, track_size);
    }
}

void CarController::moveAroundTheTrack(int currentPosition_x, int currentPosition_y) {}

void CarController::moveToPitstop(int currentPosition_x, int currentPosition_y,
                                  int pitStopPosition_x, int pitStopPosition_y) {}

void CarController::goToPitStop(int pos_x_track, int pos_y_track, int pitStopPosition_x,
                                int pitStopPosition_y) {
    // pit stop is drawn at pos_x_track + 25, pos_y_track
    // pit stop has two parallel lines: at x+5 and x+15
    int pitStopEntryX = pitStopPosition_x + 5;  // Left line of pit stop
    int pitStopExitX = pitStopPosition_x + 15;  // Right line of pit stop

    switch (car.pitStopPhase) {
        case PitStopPhase::ENTERING_PITSTOP: {
            if (car.x < pitStopEntryX) { // move right untill reach pit stop lines
                car.x++;
                car.direction = 'r';
            } else if (car.x >= pitStopEntryX) {
                // enter between the parallel lines
                car.x = pitStopEntryX + 1;  // positioning between lines
                car.pitStopPhase = PitStopPhase::DRIVING_IN_PITSTOP;
                car.pitStopProgress = 0;
            }
             else if (car.y < pitStopPosition_y) {
                car.y++;
                car.direction = 'd';
            } else if (car.y > pitStopPosition_y) {
                car.y--;
                car.direction = 'u';
            }
            break;
        }

        case PitStopPhase::DRIVING_IN_PITSTOP: {
            // Phase 2: drive along the pit stop (between the parallel lines)
            // move down along the pit stop for a certain distance
            if (car.pitStopProgress < 15) {  // go down to 15 units
                car.y++;
                car.direction = 'd';
                car.pitStopProgress++;
            } else {
                // finished driving in pit stop
                car.pitStopPhase = PitStopPhase::WAITING_IN_PITSTOP;
                car.pitStopWaitTime = 0;
                car.setTiresUse(car.secondTireType);
            }
            break;
        }

        case PitStopPhase::WAITING_IN_PITSTOP: {
            // Phase 3: wait in pit stop for 2 seconds
            car.pitStopWaitTime++;
            if (car.pitStopWaitTime >= 10) {
                car.pitStopPhase = PitStopPhase::EXITING_PITSTOP;
                car.pitStopProgress = 0;
            }
            break;
        }

        case PitStopPhase::EXITING_PITSTOP: {
            // Phase 4: exit pit stop. Go down between parallel lines then left to join track
            if (car.pitStopProgress < 7) {
                // go down between the parallel lines
                car.y++;
                car.direction = 'd';
                car.pitStopProgress++;
            } else if (car.x > pitStopEntryX) {
                // move left to exit the pit stop area
                car.x--;
                car.direction = 'l';
            } else {
                car.inPitStop = false;
                car.pitStopPhase = PitStopPhase::NOT_IN_PITSTOP;

                car.direction = 'l';
            }
            break;
        }

        case PitStopPhase::NOT_IN_PITSTOP:
        default:
            break;
    }
}
