/**
 * Beispiel zur Nutzung der SensorCapacitiveSoil-Bibliothek
 *
 * Das Beispiel liest alle zwei Sekunden den kapazitiven Bodenfeuchtesensor (analog)
 * und gibt Rohwert sowie Prozent über die serielle Schnittstelle aus.
 *
 * Ein Serial Plotter kann die Daten visualisieren.
 */

#include <Arduino.h>
#include "SensorCapacitiveSoil.h"

SensorCapacitiveSoil sensor(34); // GPIO34 (Analogpin)

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
        Serial.print(">SoilRaw:");      // Rohwert
        Serial.print(sensor.getRaw());
        Serial.print(",SoilPercent:");  // Prozent(0-100)
        Serial.println(sensor.getPercent());
    } else {
        Serial.print("Fehler ");
        Serial.print(sensor.getLastError());
        Serial.print(": ");
        Serial.println(sensor.getErrorMessage());
    }
    delay(2000);
}
