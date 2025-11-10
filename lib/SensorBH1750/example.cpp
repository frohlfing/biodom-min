/**
 * Beispiel zur Nutzung der SensorBH1750-Bibliothek
 *
 * Liest periodisch (1s) die Beleuchtungsstärke in Lux und sendet sie über die serielle Schnittstelle.
 * Format kompatibel mit Serial Plotter (">Label:Wert,Label2:Wert").
 */

#include <Arduino.h>
#include "SensorBH1750.h"

SensorBH1750 sensor; // Standard-I2C-Adresse 0x23

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22); // GPIO21 für SDA, GPIO22 für SCL (default)
    Serial.println("Initialisiere Sensor...");
    if (!sensor.begin()) {
        Serial.print("Initialisierung fehlgeschlagen: ");
        Serial.println(sensor.getErrorMessage());
    }
}

void loop() {
    if (sensor.read()) {
        Serial.print(">LightLux:");
        Serial.println(sensor.getLux());
    } else {
        Serial.print("Fehler ");
        Serial.print(sensor.getLastError());
        Serial.print(": ");
        Serial.println(sensor.getErrorMessage());
    }
    delay(2000);
}
