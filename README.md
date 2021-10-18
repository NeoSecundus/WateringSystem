# Watering System

DISCLAIMER: I cannot guarantee that this works with every setup. With the download of this repository you agree that everything you do with the repository is at your own risk.

## Basic Info

This project takes an ESP8266 board and connects it to one or two soil humidity sensors and a pump. With this Setup you can use a (small) water tank to provide water for your plants as soon as the earth becomes too dry. The System is also connected to Wifi and can be addressed over http. The root (e.g. http://pump1.local/) will return a small web-page that gives a short overview of the available commands. 
It is configurable over Wifi and the current settings and humidity data can be retrieved as well.

## Dependencies

The following libraries need a third-party package repository to be available. In order to install the repository you need to open the Arduino IDE. Go to the File Tab and to Preferences. On the initial Page you will see a section with the label: "Additional Boards Manager URLs: ". Copy this link
(http://arduino.esp8266.com/stable/package_esp8266com_index.json) into the field and restart the Arduino IDE.
Now go to Tools -> Board -> Board Manager. You should see a Package containing the ESP8266 board. Download it. Now the libraries should also be available for download!

You need to download the following libraries from the built-in arduino library manager:
- ESP8266WebServer
- ESP8266mDNS
- ESP8266Wifi
- WifiClient
- ArduinoJson

## Configuration

All necessary configurations should be in the Config.cpp file.  
Here you can set: 
- The Wifi SSID (Wifi Name)
- The Wifi Password
- The id of the Pump
- The pin the pump is linked to
- The initial thresholds
- Watering duration and delay
- Port, Website Name
- The Script execution delay
- IP Address, Subnet, Gateway and DNS servers (If necessary)
- IP Address of the master Server (Not yet in use!)

### Necessary

Some configs are optional but some you must change before trying this out!
- Set the Wifi SSID to your local Wifi's SSID
- Set the Wifi Password (Make sure that only you have access to this file if necessary)
- Set the id of the pump if you have more than one!
- You also need to set the Network information. IP Addresses, Subnet, Gateway and DNS Server
- *You may also need to change the pin ids if you use a different setup!*

## Testing

If you want to test whether the pump is working as intended you can use the test cases in the "ThunderClient_WLANPumpTests.json" file. You may need postman or thunder client to use it though. Otherwise you can always look at the Interface Definition and use curl.

Have Fun! :)