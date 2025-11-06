/**
 * Unit-Test für die MicroSDCard-Bibliothek.
 * 
 * Dieser Test überprüft die grundlegende Funktionalität des SD-Kartenmoduls.
 * Er stellt sicher, dass die Karte nicht nur initialisiert, sondern auch
 * beschrieben und modifiziert werden kann.
 */

#include <Arduino.h>
#include <unity.h>
#include "MicroSDCard.h"

// GPIO-Pin für den Chip Select der SD-Karte
const uint8_t SD_CS_PIN = 16;

// Erstelle eine Instanz der zu testenden Klasse
MicroSDCard sdCard(SD_CS_PIN);

/**
 * @brief Testet die Initialisierung und eine einfache Datei-I/O-Operation.
 * 
 * Dieser Test prüft drei Dinge:
 * 1. Kann die SD-Karte erfolgreich initialisiert werden (`begin()`).
 * 2. Kann eine Test-Datei erfolgreich erstellt und geschrieben werden (`writeFile()`).
 * 3. Kann diese Test-Datei anschließend wieder gelöscht werden (`deleteFile()`).
 */
void test_sdcard_initialization_and_io() {
    TEST_ASSERT_TRUE_MESSAGE(sdCard.begin(), "SD-Karten-Initialisierung fehlgeschlagen. Verkabelung oder Karte prüfen.");
    
    const char* testFile = "/unittest.txt";
    
    TEST_ASSERT_TRUE_MESSAGE(sdCard.writeFile(testFile, "test"), "Schreiben der Test-Datei fehlgeschlagen.");
    
    TEST_ASSERT_TRUE_MESSAGE(sdCard.deleteFile(testFile), "Löschen der Test-Datei fehlgeschlagen.");
}

void setup() {
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_sdcard_initialization_and_io);
    UNITY_END();
}

void loop() {
    // Nichts zu tun hier
}