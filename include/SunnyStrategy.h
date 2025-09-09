#ifndef SUNNYSTRATEGY_H
#define SUNNYSTRATEGY_H

#include "WeatherStrategy.h"

class SunnyStrategy : public WeatherStrategy {
private:
    int temperature;
    int trackTemperature;
    
public:
    SunnyStrategy(int temp = 25, int trackTemp = 35);
    
    std::string getWeatherType() const override;
    int getTireWearMultiplier() const override;
    int getSpeedMultiplier() const override;
    std::string getRecommendedTireType() const override;

    int getTemperature() const;
    bool isHotWeather() const;
    std::string getTireStrategy() const;
};

#endif
