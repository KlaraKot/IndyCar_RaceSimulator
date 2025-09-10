#include "Driver.h"
#include "car.h"
#include "CarController.h"
#include <iostream>

Driver::Driver(std::string name, CarController &driverController) : name(name), driverController(driverController) {}