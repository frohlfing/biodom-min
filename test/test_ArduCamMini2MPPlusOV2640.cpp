/**
 * Unit-Test für die ArduCamMini2MPPlusOV2640-Bibliothek.
 * 
 * Dieser Test überprüft die grundlegende Kommunikation mit der ArduCAM-Hardware.
 * Er stellt sicher, dass die Kamera korrekt initialisiert werden kann, was sowohl
 * die SPI- als auch die I2C-Schnittstelle testet.
 */

#include <Arduino.h>
#include <unity.h>
#include "ArduCamMini2MPPlusOV2640.h"

// GPIO-Pin für den Chip Select der Kamera
const uint8_t CAM_CS_PIN = 17;

// Erstelle eine Instanz der zu testenden Klasse
ArduCamMini2MPPlusOV2640 camera(CAM_CS_PIN);

/**
 * @brief Testet die Initialisierungssequenz der Kamera.
 * 
 * Die `begin()`-Methode gibt nur dann `true` zurück, wenn:
 * 1. Die SPI-Kommunikation mit dem ArduCAM-Modul funktioniert.
 * 2. Der OV2640-Sensor über I2C gefunden und seine Chip-ID korrekt ausgelesen wurde.
 * Dieser einzelne Test ist daher ein umfassender Hardware-Verbindungstest.
 */
void test_camera_initialization() {
    TEST_ASSERT_TRUE_MESSAGE(camera.begin(), "Kamera-Initialisierung fehlgeschlagen. Verkabelung (SPI & I2C) und Stromversorgung prüfen.");
}

void setup() {
    // Eine Verzögerung geben, damit der PlatformIO Serial Monitor verbinden kann
    delay(2000); 

    UNITY_BEGIN();
    RUN_TEST(test_camera_initialization);
    UNITY_END();
}

void loop() {
    // Nichts zu tun hier
}