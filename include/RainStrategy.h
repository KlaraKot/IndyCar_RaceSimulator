#ifndef RAINSTRATEGY_H
#define RAINSTRATEGY_H

#include "WeatherStrategy.h"

class RainStrategy : public WeatherStrategy {
private:
    int rainIntensity;  // 1-10 scale
    
public:
    RainStrategy(int intensity = 5);
    
    // Implement WeatherStrategy interface
    std::string getWeatherType() const override;
    int getTireWearMultiplier() const override;
    int getSpeedMultiplier() const override;
    std::string getRecommendedTireType() const override;
    
    // Rain specific methods
    void setRainIntensity(int intensity);
    int getRainIntensity() const;
    bool isHeavyRain() const;
    bool isLightRain() const;
};

#endif
