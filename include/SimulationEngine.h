#ifndef SIMULATION_H
#define SIMULATION_H

#include "car.h"
#include "CarController.h"
#include "CarView.h"
#include "Team.h"
#include "Driver.h"
#include "RaceConfig.h"
#include "WeatherStrategy.h"
#include <iostream>
#include <ncurses.h>
#include <string>
#include <memory>

class SimulationEngine
{
    std::vector<Car> cars;
    std::vector<CarController> controllers;
    std::vector<CarView> views;
    std::vector<Team> teams;
    std::vector<Driver> drivers;
    std::string currentWeather;
    int weatherChangeCounter;
    int max_y, max_x;
    RaceConfig raceConfig;
    std::shared_ptr<WeatherStrategy> currentWeatherStrategy;

public:
    SimulationEngine();

    ~SimulationEngine();

    void run();

    void drawTrack(int start_y, int start_x, int size);

    void drawPitStop(int start_x, int start_y);
    
    void positionCarsAtStart(int track_x, int track_y);
    
    void changeWeather();
    void notifyTeamsOfWeatherChange();
    void drawWeatherInfo();
    std::string getCurrentWeather() const;
    
    void drawRaceInfo();
    void showRaceCompletion();
};

#endif