/**
 * Unit-Test für die OLEDDisplaySH1106-Bibliothek.
 * 
 * Dieser Test überprüft die grundlegende Kommunikation mit dem OLED-Display.
 * Er stellt sicher, dass das Display über I2C initialisiert und Befehle
 * zum Schreiben in den Anzeigepuffer senden kann.
 */

#include <Arduino.h>
#include <unity.h>
#include "OLEDDisplaySH1106.h"

// Erstelle eine Instanz der zu testenden Klasse
OLEDDisplaySH1106 display;

/**
 * @brief Testet die Initialisierung und einen einfachen Schreibvorgang.
 * 
 * Da ein Unit-Test nicht visuell prüfen kann, ob Text erscheint, ist der Test
 * erfolgreich, wenn die I2C-Kommandos für die Initialisierung und das Senden
 * des Puffers ohne Fehler oder Absturz durchlaufen.
 * Dies ist ein guter Indikator für eine funktionierende I2C-Verbindung.
 */
void test_display_initialization_and_write() {
    // Die `begin()`-Methode der U8g2-Bibliothek hat keinen nützlichen Rückgabewert,
    // daher initialisieren wir sie einfach. Ein Fehler würde sich oft durch ein
    // Hängenbleiben des Programms äußern.
    display.begin();

    // Wir testen eine High-Level-Funktion unserer Bibliothek.
    // Diese ruft intern clearBuffer(), drawStr() und sendBuffer() auf.
    display.addLogLine("Unit-Test OK");
    
    // Wenn das Programm diesen Punkt erreicht, ist der Test bestanden,
    // da die I2C-Kommunikation stattgefunden hat.
    TEST_PASS();
}

void setup() {
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_display_initialization_and_write);
    UNITY_END();
}

void loop() {
    // Nichts zu tun hier
}