#ifndef DRIVER_H
#define DRIVER_H

#include <iostream>
#include "CarController.h"

using namespace std;

class Driver
{
public:
    CarController driverController;
    Driver(const std::string name, CarController &driverController);
private:
    std::string name;
};

#endif // DRIVER_H