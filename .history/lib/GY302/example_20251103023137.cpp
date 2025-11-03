/**
 * @file example.cpp
 * @brief Beispiel zur Nutzung der BH1750-Klasse.
 *
 * Liest periodisch (1s) die Beleuchtungsstärke in Lux und sendet sie über die serielle Schnittstelle.
 * Format kompatibel mit Serial Plotter (">Label:Wert,Label2:Wert").
 */

#include <Arduino.h>
#include "BH1750.h"

// Standard-I2C-Adresse 0x23
BH1750 lightSensor(0x23);

void setup() {
    Serial.begin(115200);
    delay(50);
    Serial.println("Starte GY302 (BH1750) Test...");
    Wire.begin(); // Default SDA/SCL Pins benutzen
    if (!lightSensor.begin()) {
        Serial.print("Initialisierung fehlgeschlagen: ");
        Serial.println(lightSensor.getErrorMessage());
    }
}

void loop() {
    if (lightSensor.read()) {
        float lux = lightSensor.getLux();
        Serial.print(">");
        Serial.print("LightLux:"); // Lux-Wert
        Serial.print(lux);
        Serial.println();
    } else {
        Serial.print("Fehler ");
        Serial.print(static_cast<int>(lightSensor.getLastError()));
        Serial.print(": ");
        Serial.println(lightSensor.getErrorMessage());
    }
    delay(1000);
}
