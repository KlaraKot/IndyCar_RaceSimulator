#include "RaceConfig.h"
#include <fstream>
#include <iostream>

RaceConfig RaceConfigParser::loadFromFile(const std::string& filename) {
    RaceConfig config;

    try {
        YAML::Node root = YAML::LoadFile(filename);
        
        if (root["race"]) {
            YAML::Node race = root["race"];
            
            config.name = race["name"].as<std::string>();
            config.number_of_laps = race["number_of_laps"].as<int>();
            config.number_of_cars = race["number_of_cars"].as<int>();
            
            if (race["teams"]) {
                YAML::Node teams = race["teams"];
                for (const auto& teamNode : teams) {
                    TeamConfig team;
                    team.name = teamNode["name"].as<std::string>();
                    team.driver_name = teamNode["driver_name"].as<std::string>();
                    team.position = teamNode["position"].as<int>();
                    
                    if (teamNode["strategy"]) {
                        YAML::Node strategy = teamNode["strategy"];
                        team.strategy.version = strategy["version"].as<std::string>();
                        team.strategy.first_tire = strategy["first_tire"].as<std::string>();
                        team.strategy.second_tire = strategy["second_tire"].as<std::string>();
                        
                        if (strategy["accident"]) {
                            team.strategy.accident = strategy["accident"].as<std::string>();
                        } else {
                            team.strategy.accident = "no";
                        }
                        
                        if (strategy["accident_lap"] && !strategy["accident_lap"].IsNull()) {
                            team.strategy.accident_lap = strategy["accident_lap"].as<int>();
                        } else {
                            team.strategy.accident_lap = -1;
                        }
                    }
                    
                    config.teams.push_back(team);
                }
            }
        }
        
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading YAML file: " << e.what() << std::endl;
        std::cerr << "Using default configuration" << std::endl;
        
        config.name = "Default Race";
        config.number_of_laps = 50;
        config.number_of_cars = 3;
        
        for (int i = 0; i < 3; ++i) {
            TeamConfig team;
            team.name = "Team " + std::to_string(i + 1);
            team.driver_name = "Driver " + std::to_string(i + 1);
            team.position = i + 1;
            team.strategy.version = "A";
            team.strategy.first_tire = "S";
            team.strategy.second_tire = "H";
            team.strategy.accident = "no";
            team.strategy.accident_lap = -1;
            config.teams.push_back(team);
        }
    }
    
    return config;
}