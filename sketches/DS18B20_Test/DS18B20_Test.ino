/**
 * Funktionstest für den Bodentemperatursensor DS18B20
 */

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);

void setup(void) {
    Serial.begin(115200);
    Serial.println("DS18B20 Test");
    sensor.begin();
}

void loop(void) {
    sensor.requestTemperatures();
    delay(1500);
    float tempC = sensor.getTempCByIndex(0);
    if (tempC != DEVICE_DISCONNECTED_C) {
        Serial.print("Temperature:");
        Serial.println(tempC);
    } else {
        Serial.println("Error: Could not read temperature data");
    }
}
