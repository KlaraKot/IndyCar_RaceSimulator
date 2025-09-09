#ifndef WEATHERSTRATEGY_H
#define WEATHERSTRATEGY_H

#include <string>
#include <unordered_map>

class WeatherStrategy {
protected:
    std::string weatherType;
    std::unordered_map<std::string, std::string> rules;
    
public:
    virtual ~WeatherStrategy() = default;
    
    virtual std::string getWeatherType() const = 0;
    virtual int getTireWearMultiplier() const = 0;
    virtual int getSpeedMultiplier() const = 0;
    virtual std::string getRecommendedTireType() const = 0;
};

#endif
