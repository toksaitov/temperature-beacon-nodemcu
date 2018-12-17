#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include "ESP8266WiFi.h"
#include "ESP8266WiFiMulti.h"
#include "ESP8266HTTPClient.h"

#define SERIAL_SPEED 74880
#define SERIAL_DELAY 100

#define ACCESS_POINT_1_SSID     "SSID 1"
#define ACCESS_POINT_1_PASSWORD "SSID 1 Password"
#define ACCESS_POINT_2_SSID     "Optional SSID 2"
#define ACCESS_POINT_2_PASSWORD "SSID 2 Password"
#define ACCESS_POINT_3_SSID     "Optional SSID 3"
#define ACCESS_POINT_3_PASSWORD "SSID 3 Password"

#define BEACON_ENDPOINT    "http://temperature.auca.space:8080/record"
#define BEACON_SETUP_DELAY 4000
#define BEACON_INTERVAL    500

#define MINIMUM_TEMPERATURE -55.0f
#define MAXIMUM_TEMPERATURE  150.0f
#define FILTERING_CUTOFF     0.6f

#define ANALOG_PIN A0 // the analog NodeMCU pin to which the output pin of
                      // LM35 is connected

ESP8266WiFiMulti wireless_client;
HTTPClient http_client;

static float previous_temperature = NAN;

void filter_measurement(
         float *previous_parameter,
         float *parameter,
         float minimum,
         float maximum,
         float cutoff
     )
{
  *parameter = fmin(fmax(*parameter, minimum), maximum);

  if (isnan(*previous_parameter)) {
      *previous_parameter = *parameter;
  }

  *parameter = cutoff * (*previous_parameter) + (1.0f - cutoff) * (*parameter);
  *previous_parameter = *parameter;
}

float read_temperature_sensor()
{
  int rawValue = analogRead(ANALOG_PIN);
  float voltage = rawValue / 1024.0 * 3300;
  float temperatureCelsius = voltage * 0.1;

  return temperatureCelsius;
}

void setup()
{
    Serial.begin(SERIAL_SPEED);

    wireless_client.addAP(ACCESS_POINT_1_SSID, ACCESS_POINT_1_PASSWORD);
    wireless_client.addAP(ACCESS_POINT_2_SSID, ACCESS_POINT_2_PASSWORD);
    wireless_client.addAP(ACCESS_POINT_3_SSID, ACCESS_POINT_3_PASSWORD);
    http_client.setReuse(true);
}

void loop()
{
    delay(BEACON_INTERVAL);

    if (wireless_client.run() == WL_CONNECTED) {
        float temperature = read_temperature_sensor();
        filter_measurement(
            &previous_temperature,
            &temperature,
            MINIMUM_TEMPERATURE,
            MAXIMUM_TEMPERATURE,
            FILTERING_CUTOFF
        );

        http_client.begin(BEACON_ENDPOINT);
        http_client.addHeader("Content-Type", "application/x-www-form-urlencoded");
        http_client.POST(String("t=") + String(temperature) + String("&h=0"));
        http_client.end();
    }
}

