#include "SunnyStrategy.h"
#include <iostream>

SunnyStrategy::SunnyStrategy(int temp, int trackTemp) 
    : temperature(temp), trackTemperature(trackTemp) {
    weatherType = "sunny";
    
    rules["weather"] = "sunny";
    rules["canSpeedUp"] = "yes";
    rules["goToPitStop"] = "yes";
    rules["visibility"] = "excellent";
    rules["grip"] = "optimal";
    rules["tireStrategy"] = "mixed";
}

std::string SunnyStrategy::getWeatherType() const {
    return weatherType;
}


int SunnyStrategy::getTireWearMultiplier() const {
    return isHotWeather() ? 2 : 1;
}

int SunnyStrategy::getSpeedMultiplier() const {
    return isHotWeather() ? 90 : 100;
}

std::string SunnyStrategy::getRecommendedTireType() const {
    return isHotWeather() ? "H" : "S";
}

int SunnyStrategy::getTemperature() const {
    return temperature;
}

bool SunnyStrategy::isHotWeather() const {
    return temperature > 30 || trackTemperature > 45;
}

