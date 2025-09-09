# 🏁 Race Simulator

A C++ console-based racing simulation with dynamic weather, pit stops, accidents, and lap tracking.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Class Diagram](#class-diagram)
- [Installation](#installation)
- [Configuration](#configuration)

## 🎯 Overview

The Race Simulator is a text-based racing game built in C++ using the ncurses library. It simulates a multi-car race with realistic features including weather effects, pit stops, accidents, and lap counting.

## ✨ Features

- **Multi-car Racing**: Up to 3 cars racing simultaneously
- **Dynamic Weather System**: Sunny and rainy conditions affect tire wear and speed
- **Pit Stop Strategy**: Cars can pit for tire changes and repairs
- **Accident System**: Cars can crash on specified laps and recover via pit stops
- **Lap Tracking**: Accurate lap counting and race completion detection
- **YAML Configuration**: Race parameters configurable via YAML files
- **Real-time Display**: Live race information with ncurses interface
- **Race Results**: Final standings and winner determination

## 🏗️ Class Diagram

````
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│      Car        │    │  CarController  │    │    CarView      │
├─────────────────┤    ├─────────────────┤    ├─────────────────┤
│ - x, y: int     │    │ + driveAroundThe│    │ + drawCar()     │
│ - size: int     │    │   Track()       │    │ + drawTrack()   │
│ - direction: char│    │ + goToPitStop() │    │ + drawPitStop() │
│ - decision: char│    │ + turnRight()   │    │                 │
│ - inPitStop: bool│    │ + turnLeft()    │    │                 │
│ - firstTireType │    │ + turnUp()      │    │                 │
│ - secondTireType│    │ + turnDown()    │    │                 │
│ - pitStopPhase  │    │                 │    │                 │
│ - currentLap    │    │                 │    │                 │
│ - lastDirection │    │                 │    │                 │
│ - hasCompletedLap│    │                 │    │                 │
│ - startingX, Y  │    │                 │    │                 │
│ - finishPosition│    │                 │    │                 │
├─────────────────┤    ├─────────────────┤    ├─────────────────┤
│ + goToPitStop() │    │                 │    │                 │
│ + decreaseTires │    │                 │    │                 │
│ + getCarTiresUse│    │                 │    │                 │
│ + setTiresUse() │    │                 │    │                 │
│ + getTireUseFrom│    │                 │    │                 │
│   Type()        │    │                 │    │                 │
│ + setTireType() │    │                 │    │                 │
│ + checkLapCompl │    │                 │    │                 │
│   etion()       │    │                 │    │                 │
│ + resetLapCompl │    │                 │    │                 │
│   etion()       │    │                 │    │                 │
│ + getCurrentLap │    │                 │    │                 │
│ + setStartingPos│    │                 │    │                 │
│ + getFinishPos  │    │                 │    │                 │
│ + setFinishPos  │    │                 │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         │                       │                       │
         └───────────────────────┼───────────────────────┘
                                 │
                                 ▼
┌─────────────────────────────────────────────────────────────────────────────────┐
│                              TEAM CLASS                                        │
└─────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────┐
│                                Team                                            │
├─────────────────────────────────────────────────────────────────────────────────┤
│ - teamCar: Car&                                                               │
│ - carController: CarController&                                               │
│ - teamName: string                                                             │
│ - driverName: string                                                           │
│ - decision: string                                                             │
│ - strategyVersion: string                                                      │
│ - firstTire: string                                                            │
│ - secondTire: string                                                           │
│ - currentTireType: string                                                      │
│ - accident: string                                                             │
│ - accidentLap: int                                                             │
│ - hasAccidentOccurred: bool                                                    │
├─────────────────────────────────────────────────────────────────────────────────┤
│ + makeDecision(WeatherStrategy&, int totalLaps)                               │
│ + logPitStopPhase()                                                            │
│ + changeTireType()                                                             │
│ + getCurrentTireType()                                                         │
│ + setAccidentConfig(string, int)                                               │
│ + shouldHaveAccident()                                                         │
│ + triggerAccident()                                                            │
│ + handleAccidentRecovery()                                                     │
│ + isAccidentOccurred()                                                         │
└─────────────────────────────────────────────────────────────────────────────────┘


┌─────────────────────────────────────────────────────────────────────────────────┐
│                          WeatherStrategy (Abstract)                            │
├─────────────────────────────────────────────────────────────────────────────────┤
│ - weatherType: string                                                          │
│ - rules: unordered_map<string, string>                                         │
├─────────────────────────────────────────────────────────────────────────────────┤
│ + getWeatherType(): string (pure virtual)                                      │
│ + getTireWearMultiplier(): int (pure virtual)                                  │
│ + getSpeedMultiplier(): int (pure virtual)                                     │
│ + getRecommendedTireType(): string (pure virtual)                              │
└─────────────────────────────────────────────────────────────────────────────────┘
                                │
                    ┌───────────┴───────────┐
                    │                       │
                    ▼                       ▼
┌─────────────────────────┐    ┌─────────────────────────┐
│    SunnyStrategy        │    │     RainStrategy         │
├─────────────────────────┤    ├─────────────────────────┤
│ - temperature: int      │    │ - rainIntensity: int    │
│ - humidity: int         │    │                         │
├─────────────────────────┤    ├─────────────────────────┤
│ + getWeatherType()      │    │ + getWeatherType()      │
│ + getTireWearMultiplier │    │ + getTireWearMultiplier │
│ + getSpeedMultiplier()  │    │ + getSpeedMultiplier()  │
│ + getRecommendedTireType│    │ + getRecommendedTireType│
└─────────────────────────┘    └─────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────┐
│                              RaceConfig                                        │
├─────────────────────────────────────────────────────────────────────────────────┤
│ - name: string                                                                  │
│ - number_of_laps: int                                                           │
│ - number_of_cars: int                                                           │
│ - teams: vector<TeamConfig>                                                     │
└─────────────────────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────────────────────┐
│                              TeamConfig                                        │
├─────────────────────────────────────────────────────────────────────────────────┤
│ - name: string                                                                  │
│ - driver_name: string                                                           │
│ - position: int                                                                 │
│ - strategy: TeamStrategy                                                        │
└─────────────────────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────────────────────┐
│                              TeamStrategy                                       │
├─────────────────────────────────────────────────────────────────────────────────┤
│ - version: string                                                               │
│ - first_tire: string                                                            │
│ - second_tire: string                                                           │
│ - accident: string                                                              │
│ - accident_lap: int                                                             │
└─────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────┐
│                            RaceConfigParser                                     │
├─────────────────────────────────────────────────────────────────────────────────┤
│ + loadFromFile(filename: string): RaceConfig (static)                          │
│ + saveToFile(config: RaceConfig, filename: string) (static)                    │
└─────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────┐
│                          SimulationEngine                                      │
├─────────────────────────────────────────────────────────────────────────────────┤
│ - cars: vector<Car>                                                             │
│ - controllers: vector<CarController>                                            │
│ - views: vector<CarView>                                                        │
│ - teams: vector<Team>                                                           │
│ - drivers: vector<Driver>                                                       │
│ - currentWeather: string                                                        │
│ - weatherChangeCounter: int                                                     │
│ - max_y, max_x: int                                                             │
│ - raceConfig: RaceConfig                                                        │
│ - currentWeatherStrategy: unique_ptr<WeatherStrategy>                          │
├─────────────────────────────────────────────────────────────────────────────────┤
│ + run()                                                                         │
│ + drawTrack(start_y, start_x, size)                                             │
│ + drawPitStop(start_x, start_y)                                                 │
│ + positionCarsAtStart(track_x, track_y)                                         │
│ + changeWeather()                                                               │
│ + notifyTeamsOfWeatherChange()                                                  │
│ + drawWeatherInfo()                                                             │
│ + getCurrentWeather(): string                                                   │
│ + drawRaceInfo()                                                                │
│ + showRaceCompletion()                                                          │
└─────────────────────────────────────────────────────────────────────────────────┘

                              RELATIONSHIPS
the most important: 

SunnyStrategy ──implements──► WeatherStrategy
RainStrategy ──implements──► WeatherStrategy

## 📦 Dependencies

- **C++11 or later**
- **ncurses** - Terminal UI library
- **yaml-cpp** - YAML configuration parsing
- **CMake** - Build system

## 🛠️ Installation

### Prerequisites

1. **Install ncurses**

   ```bash
   # macOS
   brew install ncurses

   # Ubuntu/Debian
   sudo apt-get install libncurses5-dev libncursesw5-dev

````

2. **Install yaml-cpp**:

   ```bash
   # macOS
   brew install yaml-cpp

   # Ubuntu/Debian
   sudo apt-get install libyaml-cpp-dev

   ```

### Build Instructions

1. **navigate to the project directory**:

   ```bash
   cd /path/to/IndyCar_RaceSimulator
   ```

2. **Create build directory**:

   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:

   ```bash
   cmake ..
   ```

4. **Build the project**:

   ```bash
   make
   ```

5. **Run the simulator**:
   ```bash
   ./IndyCar_RaceSimulator
   ```

## ⚙️ Configuration

The race can be configured using the `race_config.yaml` file:

```yaml
race:
  name: "Indy 500"
  number_of_laps: 4
  number_of_cars: 3
  teams:
    - name: "Team A"
      driver_name: "Alex Palou"
      position: 1
      strategy:
        version: "A"
        first_tire: "S"
        second_tire: "H"
        accident: "yes"
        accident_lap: 2
    - name: "Team B"
      driver_name: "Pato O`Ward"
      position: 2
      strategy:
        version: "B"
        first_tire: "H"
        second_tire: "S"
        accident: "no"
        accident_lap: null
    - name: "Team C"
      driver_name: "Will Power"
      position: 3
      strategy:
        version: "A"
        first_tire: "W"
        second_tire: "H"
        accident: "yes"
        accident_lap: 1
```
