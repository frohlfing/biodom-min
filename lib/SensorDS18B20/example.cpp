/**
 * Beispiel zur Nutzung der SensorDS18B20-Bibliothek
 * 
 * Das Beispiel liest alle zwei Sekunden die Bodentemperatur vom Sensor und sendet sie über die serielle Schnittstelle. 
 * 
 * Ein [Serial Plotter](https://github.com/badlogic/serial-plotter) könnte die Daten entgegennehmen und visualisieren.
 */

#include <Arduino.h>
#include "SensorDS18B20.h"

SensorDS18B20 sensor(4); // GPIO4

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
        Serial.print(">Temperatur:"); // °C
        Serial.println(sensor.getTemperature());
    } else {
        Serial.print("Fehler ");
        Serial.print(sensor.getLastError());
        Serial.print(": ");
        Serial.println(sensor.getErrorMessage());
    }
    delay(2000);
}
