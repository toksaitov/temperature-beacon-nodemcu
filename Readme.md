temperature-beacon-nodemcu
==========================

_temperature-beacon-nodemcu_ is an Arduino sketch for the NodeMCU v3
board to send temperature data received from the LM35 sensor
to the [temperature-back](https://github.com/toksaitov/temperature-back)
server.

## Requirements

* Arduino IDE `>= 1.8.7`
* Any NodeMCU v3 board

## Usage

Open the `temperature-beacon-nodemcu.ino` sketch in the Arduino IDE and add
Wi-Fi access point information for one or several networks (to fallback).

```C
#define ACCESS_POINT_1_SSID     "SSID 1"
#define ACCESS_POINT_1_PASSWORD "SSID 1 Password"
#define ACCESS_POINT_2_SSID     "Optional SSID 2"
#define ACCESS_POINT_2_PASSWORD "SSID 2 Password"
#define ACCESS_POINT_3_SSID     "Optional SSID 3"
#define ACCESS_POINT_3_PASSWORD "SSID 3 Password"
```

Point the device to your [deployed](https://github.com/toksaitov/temperature-back) back end.

```C
#define BEACON_ENDPOINT "http://example.com/record"
```

Connect the device or a programmer, compile, and upload the program.

## Credits

*temperature-beacon-nodemcu* was created by [Dmitrii Toksaitov](https://github.com/toksaitov).

