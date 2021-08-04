#include <Arduino.h>

class WaterPump {
    private:
    int runPin;
    bool running = false;

    public:
    WaterPump(const int runPin) {
        this->runPin = runPin;
        pinMode(runPin, OUTPUT);
    }

    bool isRunning() {
        return running;
    }

    void start() {
        digitalWrite(runPin, 1);
        running = true;
    }

    void stop() {
        digitalWrite(runPin, 0);
        running = false;
    }

    void toggle() {
        if (running) {
            stop();
        } else {
            start();
        }

        running != running;
    }
};
