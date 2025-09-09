#include "SimulationEngine.h"

#include <ncurses.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <algorithm>

#include "Car.h"
#include "CarView.h"
#include "SunnyStrategy.h"
#include "RainStrategy.h"

SimulationEngine::SimulationEngine() : currentWeather("sunny"), weatherChangeCounter(0), currentWeatherStrategy(nullptr) {
    // load race configuration from YAML file
    std::string yamlFile = "../race_config.yml";
    if (std::ifstream(yamlFile).good()) {
        raceConfig = RaceConfigParser::loadFromFile(yamlFile);
    } else {
        // Use default configuration
        std::cerr << "YAML file not found, using default configuration" << std::endl;
        raceConfig.name = "Default Race";
        raceConfig.number_of_laps = 50;
        raceConfig.number_of_cars = 3;
            
        // Add default teams
        for (int i = 0; i < 3; ++i) {
            TeamConfig team;
            team.name = "Team " + std::to_string(i + 1);
            team.driver_name = "Driver " + std::to_string(i + 1);
            team.position = i + 1;
            team.strategy.version = "A";
            team.strategy.first_tire = "S";
            team.strategy.second_tire = "H";
                        raceConfig.teams.push_back(team);
        }
        
    }
        
    currentWeatherStrategy = std::make_unique<SunnyStrategy>(25, 35);
    
    // Create cars based on configuration
    for (int i = 0; i < raceConfig.number_of_cars; ++i) {
        std::string firstTireType = "S"; // Default to Soft
        std::string secondTireType = "H"; // Default to Hard
        if (i < raceConfig.teams.size()) {
            firstTireType = raceConfig.teams[i].strategy.first_tire;
            secondTireType = raceConfig.teams[i].strategy.second_tire;
        }
        int firstInitialTires = Car::getTireUseFromType(firstTireType);
        int secondInitialTires = Car::getTireUseFromType(secondTireType);
        cars.push_back(Car(0, 0, 2, firstInitialTires, secondInitialTires, firstInitialTires, 'r', '1' + i));
    }

    // Create views, controllers, drivers, and teams based on configuration
    for (size_t i = 0; i < cars.size(); ++i) {
        views.emplace_back(cars[i], i + 1);
    }
    for (Car &car : cars) {
        controllers.emplace_back(car);
    }
    for (size_t i = 0; i < cars.size(); ++i) {
        drivers.emplace_back("name", controllers[i]);
    }
    
    for (size_t i = 0; i < cars.size() && i < raceConfig.teams.size(); ++i) {
        const auto& teamConfig = raceConfig.teams[i];
        teams.emplace_back(teamConfig.name, teamConfig.driver_name, cars[i], controllers[i]);
        
        teams[i].strategyVersion = teamConfig.strategy.version;
        teams[i].firstTire = teamConfig.strategy.first_tire;
        teams[i].secondTire = teamConfig.strategy.second_tire;
        teams[i].currentTireType = teamConfig.strategy.first_tire;
        
        teams[i].setAccidentConfig(teamConfig.strategy.accident, teamConfig.strategy.accident_lap);
    }
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    start_color();

    // Define color pairs for different cars
    init_pair(1, COLOR_RED, COLOR_BLACK);      // Car 1: Red
    init_pair(2, COLOR_GREEN, COLOR_BLACK);    // Car 2: Green
    init_pair(3, COLOR_BLUE, COLOR_BLACK);     // Car 3: Blue
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);   // Car 4: Yellow
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);  // Car 5: Magenta

    getmaxyx(stdscr, max_y, max_x);

    // car positioning based on track position
    int track_x = (max_x - 20) / 2;
    int track_y = (max_y - 20) / 2;
    positionCarsAtStart(track_x, track_y);

    int debug_height = 10;
    int debug_width = 50;
    int start_y_1 = 0;  
    int start_x_1 = 0;
}

SimulationEngine::~SimulationEngine() { endwin(); }

void SimulationEngine::run() {
    int pos_y_track = (max_y - 20) / 2;
    int pos_x_track = (max_x - 20) / 2;

    drawTrack(pos_y_track, pos_x_track, 20);
    drawPitStop(pos_x_track + 25, pos_y_track);
    for (auto &carView : views) {
        carView.draw();
    }
    refresh();

    // Show all team positions and tire status
    for (size_t i = 0; i < teams.size() && i < 3; ++i) {
        mvprintw(max_y - 4 + i, 0, "Team %s (%s) car at (%d, %d), dir: %c, tires: %d (%s), inPit: %s",
                 teams[i].teamName.c_str(), teams[i].driverName.c_str(), teams[i].teamCar.x, teams[i].teamCar.y,
                 teams[i].teamCar.direction, teams[i].teamCar.getCarTiresUse(), teams[i].getCurrentTireType().c_str(),
                 teams[i].teamCar.inPitStop ? "YES" : "NO");
    }

    refresh();

    while (true) {
        
        // check for car race completion and track order
        static int nextFinishPosition = 1;
        for (size_t i = 0; i < cars.size(); ++i) {
            if (cars[i].getCurrentLap() >= raceConfig.number_of_laps && cars[i].getFinishPosition() == 0) {
                cars[i].setFinishPosition(nextFinishPosition);
                nextFinishPosition++;
            }
        }
        
        // check if all cars have finished
        bool raceCompleted = true;
        for (const auto& car : cars) {
            if (car.getCurrentLap() < raceConfig.number_of_laps) {
                raceCompleted = false;
                break;
            }
        }
        
        if (raceCompleted) {
            showRaceCompletion();
            break;
        }
        
        // check for weather changes every 50 frames (about 10 seconds)
        weatherChangeCounter++;
        if (weatherChangeCounter >= 50) {
            changeWeather();
            weatherChangeCounter = 0;
        }
        
        
        for (size_t i = 0; i < teams.size(); ++i) {
             if (currentWeatherStrategy) {
                teams[i].makeDecision(pos_x_track, pos_y_track, *currentWeatherStrategy, raceConfig.number_of_laps);
            }
            
            // check for lap completion only if car hasn't finished the race
            if (cars[i].getCurrentLap() < raceConfig.number_of_laps) {
                cars[i].checkLapCompletion(pos_x_track, pos_y_track);
            }
        }
        
        // reset lap completion flags for next frame (only for cars that haven't finished)
        for (auto& car : cars) {
            if (car.getCurrentLap() < raceConfig.number_of_laps) {
                car.resetLapCompletion();
            }
        }
        

        clear();

        drawPitStop(pos_x_track + 25, pos_y_track);
        drawTrack(pos_y_track, pos_x_track, 20);

        for (auto &carView : views) {
            carView.draw();
        }


        drawRaceInfo();
        drawWeatherInfo();
        
        for (size_t i = 0; i < teams.size() && i < 3; ++i) {
            mvprintw(max_y - 4 + i, 0, "Team %s (%s) car at (%d, %d), dir: %c, tires: %d (%s), inPit: %s",
                     teams[i].teamName.c_str(), teams[i].driverName.c_str(), teams[i].teamCar.x, teams[i].teamCar.y,
                     teams[i].teamCar.direction, teams[i].teamCar.getCarTiresUse(), teams[i].getCurrentTireType().c_str(),
                     teams[i].teamCar.inPitStop ? "YES" : "NO");
            
            if (teams[i].isAccidentOccurred()) {
                mvprintw(max_y - 4 + i, 80, "CRASHED on lap %d", teams[i].teamCar.getCurrentLap());
            }
        }

        refresh();
        usleep(200000);
    }
}

void SimulationEngine::drawTrack(int start_y, int start_x, int size) {
    for (int i = start_y; i < start_y + size; i++) {
        mvaddch(i, start_x, '*');
        mvaddch(i, start_x + size, '*');
    }

    for (int i = start_x + 1; i < start_x + size; i++) {
        mvaddch(start_y, i, '*');
        mvaddch(start_y + size - 1, i, '*');
    }
}

void SimulationEngine::drawPitStop(int start_x, int start_y) {
    for (int i = start_y; i < start_y + 20; i++) {
        mvaddch(i, start_x + 5, '|');
        mvaddch(i, start_x + 15, '|');
    }
}

void SimulationEngine::positionCarsAtStart(int track_x, int track_y) {
    for (size_t i = 0; i < cars.size(); ++i) {
        int car_spacing = 4;        // distance between cars 
        int offset_from_track = 6;  // Distance before track start

        cars[i].x = track_x - offset_from_track - (i * car_spacing);  
        cars[i].y = track_y - 2; 
        cars[i].direction = 'r';  

        if (cars[i].x < 1) {
            cars[i].x = 1 + (i * 2); 
        }

        if (cars[i].y < 1) {
            cars[i].y = 1;
        }
        if (cars[i].x >= track_x) {
            cars[i].x = track_x - 1; 
        }
        
        cars[i].setStartingPosition(track_x, track_y);
    }
}

void SimulationEngine::changeWeather() {
    if (currentWeather == "sunny") {
        currentWeatherStrategy = std::make_unique<RainStrategy>(5);
        currentWeather = "rainy";
    } else {
        currentWeatherStrategy = std::make_unique<SunnyStrategy>(25, 35);
        currentWeather = "sunny";
    }
}

void SimulationEngine::drawWeatherInfo() {
    int weather_x = max_x - 20;
    int weather_y = 1;
    
    if (currentWeatherStrategy) {
        // Draw weather box
        mvprintw(weather_y, weather_x, "Weather: %s", currentWeatherStrategy->getWeatherType().c_str());
        mvprintw(weather_y + 1, weather_x, "Tire Wear: %dx", currentWeatherStrategy->getTireWearMultiplier());
        mvprintw(weather_y + 2, weather_x, "Speed: %d%%", currentWeatherStrategy->getSpeedMultiplier());
        mvprintw(weather_y + 3, weather_x, "Tire: %s", currentWeatherStrategy->getRecommendedTireType().c_str());
        
        if (currentWeather == "rainy") {
            mvprintw(weather_y + 4, weather_x, "RAINY");
        } else {
            mvprintw(weather_y + 4, weather_x, "SUNNY");
        }
    }
}

std::string SimulationEngine::getCurrentWeather() const {
    return currentWeather;
}

void SimulationEngine::drawRaceInfo() {
    int info_x = max_x - 35;
    int info_y = 1;
    
    mvprintw(info_y, info_x, "Race: %s", raceConfig.name.c_str());
    mvprintw(info_y + 1, info_x, "Laps: %d", raceConfig.number_of_laps);
    mvprintw(info_y + 2, info_x, "Cars: %d", raceConfig.number_of_cars);
    
    // current lap info
    int lap_y = info_y + 4;
    mvprintw(lap_y, info_x, "Current Laps:");
    for (size_t i = 0; i < cars.size() && i < 3; ++i) {
        mvprintw(lap_y + 1 + i, info_x, "Car %zu: Lap %d", i + 1, cars[i].getCurrentLap());
    }
    
}


void SimulationEngine::showRaceCompletion() {
    clear();
    
    int center_x = max_x / 2;
    int center_y = max_y / 2;
    
    for (int i = center_x - 30; i <= center_x + 30; i++) {
        mvaddch(center_y - 10, i, '*');
        mvaddch(center_y + 12, i, '*');
    }
    for (int i = center_y - 10; i <= center_y + 12; i++) {
        mvaddch(i, center_x - 30, '*');
        mvaddch(i, center_x + 30, '*');
    }

    mvprintw(center_y - 6, center_x - 15, "FINISH!");
    
    std::vector<std::pair<int, int>> results;
    for (size_t i = 0; i < cars.size(); ++i) {
        results.push_back({cars[i].getFinishPosition(), i});
    }
    std::sort(results.begin(), results.end(), std::less<std::pair<int, int>>());
    
    int winnerIndex = results[0].second;
    std::string winnerName = teams[winnerIndex].teamName;
    std::string winnerDriver = teams[winnerIndex].driverName;
    int winnerLaps = cars[winnerIndex].getCurrentLap();
    
    mvprintw(center_y - 2, center_x - 20, "WINNER: %s (%s) - %d laps", 
             winnerName.c_str(), winnerDriver.c_str(), winnerLaps);
    
    mvprintw(center_y, center_x - 15, "INDY CAR STANDINGS");
    mvprintw(center_y + 1, center_x - 20, "Position | Team | Driver | Laps");
    mvprintw(center_y + 2, center_x - 20, "---------|------|--------|-----");
    
    for (size_t i = 0; i < results.size(); ++i) {
        int carIndex = results[i].second;
        int lapCount = cars[carIndex].getCurrentLap();
        std::string teamName = teams[carIndex].teamName;
        std::string driverName = teams[carIndex].driverName;
        
        std::string position = (i == 0) ? "1st place" : (i == 1) ? "2nd place" : (i == 2) ? "3rd place" : "---";
        mvprintw(center_y + 3 + i, center_x - 20, "%s %2d. | %-8s | %-12s | %2d", 
                 position.c_str(), i + 1, teamName.c_str(), driverName.c_str(), lapCount);
    }
    mvprintw(center_y + 11, center_x - 20, "Press x to exit");
    
    refresh();
    getch();
}
