#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <string>
#include <ArduinoJson.h>

#include "Config.cpp"
#include "WaterSystem.cpp"

/*
* CONFIGURATION VARIABELS IN Config.cpp
*/

WaterSystem waterSystem = WaterSystem();
ESP8266WebServer webServer(PORT);


// ===== REQ HANDLERS SECTION =====

// Sends back Error on failed request.
void handleError() {
  webServer.sendHeader("Connection", "close");
  webServer.send(404, "text/html", ERROR_404);
}

void handleIndex() {
  webServer.sendHeader("Connection", "close");
  String body = "<h1>Command Index</h1><main></main>";
  webServer.send(200, "text/html", INDEX_PAGE);
}

void handleDataReq() {
  DynamicJsonDocument doc(512);
  doc["host"] = waterSystem.getHost();
  doc["unitid"] = UNITID;
  doc["humidity"] = waterSystem.getHum();
  doc["highThresh"] = waterSystem.getHighHumThresh();
  doc["lowThresh"] = waterSystem.getLowHumThresh();
  doc["tankFilled"] = waterSystem.getTankFilled();
  doc["pumpStatus"] = waterSystem.getPumpStatus();
  doc["wateringDelay"] = waterSystem.getJWDelay();
  doc["wateringTime"] = waterSystem.getWDuration();
  
  char res[512];
  serializeJson(doc, res);
  webServer.send(200, "application/json", res);
}

void handleToggleReq() {
  waterSystem.togglePump();
  String res = "{\"success\":true,\"msg\":\"Pump is now ";
  res += waterSystem.getPumpStatus();
  res += "\"}";

  webServer.send(200, "application/json", res);
}

void handleSetHumThresh() {
  if (webServer.hasArg("plain")) {
    DynamicJsonDocument doc(96);
    deserializeJson(doc, webServer.arg("plain"));
    if (doc.containsKey("lowThresh") && doc.containsKey("highThresh")) {
      String lowStr = doc.getMember("lowThresh");
      String highStr = doc.getMember("highThresh");
      float low = lowStr.toFloat();
      float high = highStr.toFloat();
      
      if (low < 0 || low > 1 || high < 0 || high > 1 || low > high) {
        webServer.send(406, "application/json", "{\"success\":false,\"msg\":\"Hum thresholds must be between 1.0 and 0.0\"}");
      } else {
        waterSystem.setHumThresh(low, high);
        webServer.send(200, "application/json", "{\"success\":true,\"msg\":\"Thresholds set\"}");
      }
    }
  }
  webServer.send(406, "application/json", "{\"success\":false,\"msg\":\"Invalid json body!\"}");
}

void handleSetWateringDelay() {
  if (webServer.hasArg("plain")) {
    DynamicJsonDocument doc(64);
    deserializeJson(doc, webServer.arg("plain"));
    if (doc.containsKey("delay")) {
      String delayStr = doc.getMember("delay");
      int delay = delayStr.toInt();
      
      if (delay < 0 || delay > 3600) {
        webServer.send(406, "application/json", "{\"success\":false,\"msg\":\"Delay must be between 0 and 3600\"}");
      } else {
        waterSystem.setJWDelay(delay);
        webServer.send(200, "application/json", "{\"success\":true,\"msg\":\"Delay set\"}");
      }
    }
  }
  webServer.send(406, "application/json", "{\"success\":false,\"msg\":\"Invalid json body!\"}");
}

void handleSetWateringDuration() {
  if (webServer.hasArg("plain")) {
    DynamicJsonDocument doc(64);
    deserializeJson(doc, webServer.arg("plain"));
    if (doc.containsKey("duration")) {
      String durationStr = doc.getMember("duration");
      int duration = durationStr.toInt();
      
      if (duration < 0 || duration > 60) {
        webServer.send(406, "application/json", "{\"success\":false,\"msg\":\"Duration must be between 0 and 60\"}");
      } else {
        waterSystem.setWDuration(duration);
        webServer.send(200, "application/json", "{\"success\":true,\"msg\":\"Duration set\"}");
      }
    }
  }
  webServer.send(406, "application/json", "{\"success\":false,\"msg\":\"Invalid json body!\"}");
}

void initServer() {
  webServer.onNotFound(handleError);
  webServer.on("/", HTTP_GET, handleIndex);
  webServer.on("/data", HTTP_GET, handleDataReq);
  webServer.on("/toggle", HTTP_POST, handleToggleReq);
  webServer.on("/set/humidity-threshold", HTTP_POST, handleSetHumThresh);
  webServer.on("/set/watering-delay", HTTP_POST, handleSetWateringDelay);
  webServer.on("/set/watering-duration", HTTP_POST, handleSetWateringDuration);

  webServer.begin();
}
// ===== REQ HANDLERS SECTION END =====



void setup(void) {
  Serial.begin(115200);

  Serial.println("Booting...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.config(ipaddr, gateway, subnet, dns, dns2);
  WiFi.begin(SSID, WPASS);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(SSID, WPASS);
    Serial.println("WiFi failed, retrying.");
  }
  MDNS.begin(waterSystem.getHost());

  initServer();
  
  MDNS.addService("http", "tcp", PORT);
  Serial.printf("Server ready! Open http://%s:%d in your browser\n", waterSystem.getHost(), PORT);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  waterSystem.setHumThresh(LOW_HUM_THRESH, HIGH_HUM_THRESH);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop(void) {
  delay(MAIN_DELAY);
  // Serial.printf("Hum: %.3f\r", waterSystem.getHum());

  waterSystem.update();

  webServer.handleClient();
  MDNS.update();
}
