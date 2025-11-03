/**
 * @file example.cpp
 * @brief Beispiel zur Nutzung der CapacitiveSoilSensor-Klasse.
 *
 * Das Beispiel liest alle zwei Sekunden den kapazitiven Bodenfeuchtesensor (analog)
 * und gibt Rohwert sowie Prozent über die serielle Schnittstelle aus.
 *
 * Ein Serial Plotter kann die Daten visualisieren.
 */

#include <Arduino.h>
#include "CapacitiveSoilSensor.h"

// Beispiel: GPIO34 (ESP32 ADC1_6)
CapacitiveSoilSensor soil(34);

void setup() {
    Serial.begin(115200);
    delay(50);
    Serial.println("Starte CapacitiveSoilSensor-Test...");
    soil.begin();
}

void loop() {
    if (soil.read()) {
        Serial.print(">");
        Serial.print("SoilRaw:");      // Rohwert
        Serial.print(soil.getRaw());

        Serial.print(",");
        Serial.print("SoilPercent:");  // Prozent(0-100)
        Serial.print(soil.getPercent());

        Serial.println();
    } else {
        Serial.println("Fehler beim Lesen des Sensors");
    }
    delay(2000);
}
