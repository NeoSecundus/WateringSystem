#include <Arduino.h>

#include "WaterPump.cpp"
#include "Hygrometer.cpp"
#include "Config.cpp"

class WaterSystem {
    private:
    int unitid = UNITID;
    int sigPin = SIG_PIN;
    int tankPin = TANK_PIN;
    char host[8];
    float hum = 0;
    float maxHum = HIGH_HUM_THRESH;
    float minHum = LOW_HUM_THRESH;
    WaterPump pump = WaterPump(PUMP_PIN);
    Hygrometer hygro = Hygrometer(A0);
    bool tankFilled = false;
    bool sigOn = false;
    bool justWatered = false;
    int jWDelay = WATERING_DELAY;
    int wDuration = WATERING_DURATION;
    int jWTimer = 0;

    public:
    WaterSystem() {
        snprintf(host, 6, "pump%d", UNITID);
        pinMode(SIG_PIN, OUTPUT);
        pinMode(TANK_PIN, INPUT);
    }

    const char* getHost() { return host; }

    void pumpOn() { 
        pump.start(); 
        justWatered = true;
        jWTimer = 0;
    }

    void pumpOff() { 
        pump.stop(); 
        justWatered = false;
        jWTimer = 0;
    }

    void togglePump() { 
        if (pump.isRunning()) {
            pumpOff();
        } else {
            pumpOn();
        }
    }

    String getPumpStatus() {
        if (pump.isRunning()) {
            return "running";
        } else {
            return "stopped";
        }
    }

    void setJWDelay(int delay) {
        if (delay >= 0 && delay <= 3600)
            jWDelay = delay;
    }

    int getJWDelay() {
        return jWDelay;
    }

    void setWDuration(int duration) {
        if (duration > 0 && duration < 60) {
            wDuration = duration;
        }
    }

    int getWDuration() {
        return wDuration;
    }

    float getHum() {
        return hum;
    }

    float getLowHumThresh() {
        return minHum;
    }

    float getHighHumThresh() {
        return maxHum;
    }

    void setHumThresh(const float lowThresh, const float highThresh) {
        if (highThresh < 1 && highThresh > 0) {
            maxHum = highThresh;
        }

        if (lowThresh < 1 && lowThresh > 0) {
            minHum = lowThresh;
        }
    }

    bool getTankFilled() {
        return tankFilled;
    }

    void update() {
        if (!justWatered) {
            hum = hygro.measure();
            tankFilled = digitalRead(tankPin);

            if (!tankFilled) {
                if (sigOn) {
                    digitalWrite(sigPin, LOW);
                } else {
                    digitalWrite(sigPin, HIGH);
                } 
                sigOn = !sigOn;
                
                pump.stop();
                return;
            } else {
                digitalWrite(sigPin, LOW);
            }

            if (hum > maxHum) {
                pump.stop();
            } else if (hum < minHum) {
                pump.start();
                justWatered = true;
            }
        } else {
            jWTimer++;
            if (jWTimer == wDuration * (1000 / MAIN_DELAY)) {
                pump.stop();
            } else if (jWTimer == (5 + jWDelay) * (1000 / MAIN_DELAY)) {
                justWatered = false;
                jWTimer = 0;
            }
        }
    }
};
