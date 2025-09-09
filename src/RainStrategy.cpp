#include "RainStrategy.h"
#include <iostream>

RainStrategy::RainStrategy(int intensity) : rainIntensity(intensity) {
    weatherType = "rainy";
    rules["weather"] = "rainy";
    rules["canSpeedUp"] = "no";
    rules["goToPitStop"] = "yes";
    rules["tireType"] = "W";
}

std::string RainStrategy::getWeatherType() const {
    return weatherType;
}


int RainStrategy::getTireWearMultiplier() const {
    // Rain causes more tire wear
    return isHeavyRain() ? 3 : 2;
}

int RainStrategy::getSpeedMultiplier() const {
    // Rain reduces speed
    return isHeavyRain() ? 50 : 70;  // in %
}

std::string RainStrategy::getRecommendedTireType() const {
    return "W";
}

void RainStrategy::setRainIntensity(int intensity) {
    rainIntensity = (intensity < 1) ? 1 : (intensity > 10) ? 10 : intensity;
}

int RainStrategy::getRainIntensity() const {
    return rainIntensity;
}

bool RainStrategy::isHeavyRain() const {
    return rainIntensity >= 7;
}

bool RainStrategy::isLightRain() const {
    return rainIntensity <= 3;
}
