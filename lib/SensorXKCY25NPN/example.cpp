/**
 * Beispiel zur Nutzung der SensorXKCY25NPN-Bibliothek
 */

#include <Arduino.h>
#include "SensorXKCY25NPN.h"

SensorXKCY25NPN sensor(35, false); // GPIO35 (nur als Eingangs-Pin geeignet)

void setup() {
    Serial.begin(115200);
    Serial.println("Initialisiere Sensor...");
    if (!sensor.begin()) {
        Serial.print("Initialisierung fehlgeschlagen: ");
        Serial.println(sensor.getErrorMessage());
    }
}

void loop() {
    if (sensor.read()) {
        Serial.print(">WaterDetected:"); // °C
        Serial.println(sensor.isWaterDetected() ? "1" : "0");
    } else {
        Serial.print("Fehler ");
        Serial.print(sensor.getLastError());
        Serial.print(": ");
        Serial.println(sensor.getErrorMessage());
    }
    delay(2000);
}
