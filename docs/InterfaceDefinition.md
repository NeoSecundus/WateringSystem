# Interface definition

1. [GET Interfaces](#get-interfaces)
   1. [Get Data](#get-data)
2. [POST Interfaces](#post-interfaces)
   1. [Toggle Pump](#toggle-pump)
   2. [Set Humidity Threshold](#set-humidity-threshold)
   3. [Set Watering Delay](#set-watering-delay)
   4. [Set Watering Duration](#set-watering-duration)

## GET Interfaces

### Get Data

**Request:**  
URL: /data  

**Response:**
HTTP Code: 200  
Content-Type: application/json  
Body:

```json
{
    "host": "(str) hostname",
    "unitid": "(int) id of unit",
    "humidity": "(float) sensor humidity level",
    "highThresh": "(float) high humidity threshold of watering system",
    "lowThresh": "(float) low humidity threshold of watering system",
    "tankFilled": "(bool) enough water in tank?",
    "pumpStatus": "(str) either 'running' || 'stopped'",
    "wateringDelay": "(int) Delay to wait before watering again. To let water settle."
}
```

## POST Interfaces

### Toggle Pump

**Request:**  
URL: /toggle
Content-Type: any
Data: `NULL`  

**Response:**
HTTP Code: 200
Content-Type: application/json  
Body: `{"success": true, "msg": "Pump is now running/stopped"}`

### Set Humidity Threshold

**Request:**  
URL: /set/humidity-threshold
Content-Type: application/json
Data: `{"lowThresh": "(float)", "highThresh": "(float)"}`  
Data Rules: 0 <= lowThresh <= highThresh <= 1

**Response:**
HTTP Code: 200
Content-Type: application/json  
Body: `{"success": true, "msg": "Thresholds set"}`

**On Error:**
HTTP Code: 406
Content-Type: application/json
Body: `{"success": false, "msg":"(str) Error message"}`

### Set Watering Delay

**Request:**
URL: /set/watering-delay  
Content-Type: application/json
Data: `{"delay": "(int)"}`  
Data Rules: `delay` is in Seconds. 0 <= `delay` <= 3600 (1 hour)

**Response:**
HTTP Code: 200  
Content-Type: application/json  
Body: `{"success": true, "msg": "Delay set"}`  

**On Error:**
HTTP Code: 406  
Content-Type: application/json
Body: `{"success": false, "msg": "Error Message"}`  

### Set Watering Duration

**Request:**
URL: /set/watering-duration 
Content-Type: application/json
Data: `{"duration": "(int)"}`  
Data Rules: `duration` is in Seconds. 0 <= `duration` <= 60 (1 minute)

**Response:**
HTTP Code: 200  
Content-Type: application/json  
Body: `{"success": true, "msg": "Duration set"}`  

**On Error:**
HTTP Code: 406  
Content-Type: application/json
Body: `{"success": false, "msg": "Error Message"}`  