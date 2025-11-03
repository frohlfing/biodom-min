/**
 * @file example.cpp
 * @brief Beispiel zur Nutzung der XKC_Y25_NPN-Klasse.
 *
 * Zeigt die grundlegende Verkabelung und das Auslesen des Sensors.
 * Hinweis: Der Sensor wird mit 5-12V versorgt; am OUT benötigt man einen Pull-Up
 * auf +3.3V (z. B. 10k), damit der ESP32 sichere Pegel erhält.
 */

#include <Arduino.h>
#include "XKC_Y25_NPN.h"

// Beispiel: GPIO35 (nur als Eingangs-Pin geeignet)
XKC_Y25_NPN levelSensor(35, false);

void setup() {
    Serial.begin(115200);
    delay(50);
    Serial.println("Starte XKC-Y25-NPN Test...");
    levelSensor.begin();
    Serial.println(levelSensor.wiringNotes());
}

void loop() {
    if (levelSensor.read()) {
        Serial.println(">WaterDetected:1"); // Serial Plotter / Parser kompatibel
    } else {
        Serial.println(">WaterDetected:0");
    }
    delay(1000);
}
