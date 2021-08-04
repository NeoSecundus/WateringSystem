#include <Arduino.h>

class Hygrometer {
    public:
    Hygrometer(const int sensorPin) {
        this->sensorPin = sensorPin;
    }

    // Measures humidity 
    float measure() {
        int raw = analogRead(sensorPin);
        // raw = 1024 - raw - 200; // NOW => LOWEST = 0, HIGHEST = 300

        return 1.0f - ((float)raw / 1024.0f);
    }

    private:
    int sensorPin;
};
