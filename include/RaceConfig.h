#ifndef RACECONFIG_H
#define RACECONFIG_H

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

struct TeamStrategy {
    std::string version;
    std::string first_tire;
    std::string second_tire;
    std::string accident;
    int accident_lap;
};

struct TeamConfig {
    std::string name;
    std::string driver_name;
    int position;
    TeamStrategy strategy;
};

struct RaceConfig {
    std::string name;
    int number_of_laps;
    int number_of_cars;
    std::vector<TeamConfig> teams;
};

class RaceConfigParser {
public:
    static RaceConfig loadFromFile(const std::string& filename);
    static void saveToFile(const RaceConfig& config, const std::string& filename);
};

#endif // RACECONFIG_H
