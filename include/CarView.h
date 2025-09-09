#ifndef CARVIEW_H
#define CARVIEW_H

#include "Car.h"
#include <iostream>

class CarView
{
    Car &car;
    int colorId;

public:
    CarView(Car &car, int colorId = 1);
    void draw();
    void setColor(int colorId);
};

#endif