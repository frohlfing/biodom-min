/**
 * Beispiel zur Nutzung der SensorAM2302-Bibliothek
 * 
 * Das Beispiel liest alle zwei Sekunden die Raumtemperatur und die Luftfeuchtigkeit vom Sensor und sendet sie über die serielle Schnittstelle. 
 * 
 * Ein [Serial Plotter](https://github.com/badlogic/serial-plotter) könnte die Daten entgegennehmen und visualisieren.
 */

#include <Arduino.h>
#include "SensorAM2302.h"

SensorAM2302 sensor(13); // GPIO13

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
        Serial.print(sensor.getTemperature());
        Serial.print(",Luftfeuchtigkeit:"); // Prozent
        Serial.println(sensor.getHumidity());
    } else {
        Serial.print("Fehler ");
        Serial.print(sensor.getLastError());
        Serial.print(": ");
        Serial.println(sensor.getErrorMessage());
    }
    delay(2000); 
}
