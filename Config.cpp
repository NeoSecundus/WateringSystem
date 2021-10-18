#include <ESP8266WiFi.h>


#ifndef SSID
    // WiFi Settings
    #define SSID "WLAN-SSID"
    #define WPASS "PASS"
    
    // WaterSystem Settings
    #define UNITID 1
    #define PUMP_PIN 4
    #define SIG_PIN 14
    #define TANK_PIN 15
    #define LOW_HUM_THRESH 0.35
    #define HIGH_HUM_THRESH 0.5
    #define WATERING_DURATION 10 // In Seconds
    #define WATERING_DELAY 10 // In Seconds
    
    // SERVER & IP Settings
    #define PORT 12121
    #define WPNAME String("Plants")
    #define MAIN_DELAY 250 // In Milliseconds

    const IPAddress ipaddr(192, 168, 1, 200 + UNITID);
    const IPAddress gateway(192, 168, 0, 1);
    const IPAddress subnet(255, 255, 255, 0);
    const IPAddress dns(192, 168, 0, 1);
    const IPAddress dns2(8, 8, 8, 8);
    const IPAddress masterServerIP(192, 168, 1, 10);

    // HTML Index and Error Pages
    const String INDEX_PAGE = R"===(
<DOCTYPE html>
<html lang="en">
<head>
    <title>)===" + WPNAME + R"===(</title>
    <meta charset="UTF-8" />
    <style>
        * {
            font-family: monospace;
            font-size: calc(0.9vw + 6px);
        }
        h1 {
            margin: auto;
            margin-bottom: 0.5em;
            font-size: 2.4rem;
            text-align: center;
        }
        h2 {
            text-align: center;
        }
        p {
            background-color: lightblue;
            border: 2px solid gray;
            margin: auto;
            margin-bottom: 1em;
            width: 80%;
        }
        label {
            font-weight: bold;
            font-size: 1.2rem;
        }
    </style>
</head>
<body>
    <h1>Command Index</h1>
    <p>
        <label>GET /data</label><br/>
        Pulls current data from the Pump in json format.
    </p>
    <p>
        <label>POST /toggle</label><br/>
        Toggles the pump on and off depending on status.
    </p>
    <p>
        <label>POST /set/humidity-threshold</label><br/>
        Sets the humidity threshold the pump will get activated and deactivated at.<br/><br/>

        Send JSON: {"lowThresh": "(float)", "highThresh": "(float)"}<br/>
        Data Rules: 0 <= lowThresh <= highThresh <= 1
    </p>
    <p>
        <label>POST /set/watering-delay</label><br/>
        Sets the minimum delay that the system should wait before watering again.<br/> 
        This should be done so the water can settle and doesn't overflow. <br/>
        The default is 10 Seconds.<br/><br/>

        Send JSON: {"delay": "(int)"}<br/>
        Data Rules: delay is in Seconds. 0 <= delay <= 3600 (1 hour)
    </p>
    <p>
        <label>POST /set/watering-duration</label><br/>
        Sets the duration the pump is running a time.<br/> 
        The default is 10 Seconds.<br/><br/>

        Send JSON: {"duration": "(int)"}<br/>
        Data Rules: delay is in Seconds. 0 <= delay <= 60 (1 minute)
    </p>

    <h2>The full documentation can be found in the Interface definition in ./docs </h2>
</body>
</html>
    )===";

    const String ERROR_404 = R"===(
<DOCTYPE html>
<html lang="en">
<head>
    <title>)===" + WPNAME + R"===(</title>
    <meta charset="UTF-8" />
    <style>
        h1 {
            color: red;
            background-color: #444;
            text-align: center;
            margin: 12vh 5vw;
            font-size: 4vw;
            padding: 10vh;
        }
    </style>
</head>
<body>
    <h1>ERROR 404<br/>You probably took a wrong turn!</h1>
</body>
</html>
    )===";
#endif