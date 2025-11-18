/**
 * Beispiel zur Nutzung der OLEDDisplaySH1106-Bibliothek.
 * 
 * Das Beispiel initialisiert ein SH1106 I2C OLED Display und schaltet
 * alle 10 Sekunden zwischen den drei Haupt-Anzeigemodi um:
 * 1. Scrolling Log: Zeigt nacheinander Log-Meldungen an, die nach oben scrollen.
 * 2. Dashboard: Zeigt eine 4-Quadranten-Ansicht mit simulierten Messwerten.
 * 3. Fullscreen Alert: Zeigt eine blinkende Warnmeldung.
 * 
 * @note Die `display.update()`-Methode muss in der Hauptschleife aufgerufen
 *       werden, damit zeitbasierte Effekte wie das Blinken funktionieren.
 */

#include <Arduino.h>
#include "OLEDDisplaySH1106.h"

// Erstelle eine Instanz der Bibliotheksklasse (Standard I2C-Pins)
OLEDDisplaySH1106 display;

void setup() {
    Serial.begin(115200);
    Serial.println("Starte OLED-Display-Beispiel...");

    display.begin();
    display.clear();
}

void loop() {
    static int displayMode = 0;
    static unsigned long lastModeChange = 0;

    // Diese Funktion muss immer aufgerufen werden, um z.B. das Blinken zu ermöglichen.
    display.update();

    // Alle 10 Sekunden den Anzeigemodus wechseln
    if (millis() - lastModeChange > 10000) {
        lastModeChange = millis();
        displayMode = (displayMode + 1) % 3; // Schaltet zwischen 0, 1, 2 um

        Serial.printf("Wechsle zu Anzeigemodus %d\n", displayMode);

        switch (displayMode) {
            case 0: // Log-Modus initialisieren
                display.clearLog();
                display.addLogLine("System Start...");
                delay(500);
                display.addLogLine("Sensor A... OK");
                delay(500);
                display.addLogLine("Sensor B... OK");
                delay(500);
                display.addLogLine("WLAN verbinde...");
                delay(500);
                display.addLogLine("Verbunden!");
                delay(500);
                display.addLogLine("IP: 192.168.1.42");
                delay(500);
                display.addLogLine("Zeige Log für 10s");
                break;

            case 1: // Dashboard-Modus initialisieren
                display.setDashboardText(OLEDDisplaySH1106::TOP_LEFT, "Temp: 23.5 C");
                display.setDashboardText(OLEDDisplaySH1106::TOP_RIGHT, "Hum: 45.2 %");
                display.setDashboardText(OLEDDisplaySH1106::BOTTOM_LEFT, "CO2: 415 ppm");
                display.setDashboardText(OLEDDisplaySH1106::BOTTOM_RIGHT, "Druck: 1012 hPa");
                display.showDashboard();
                break;

            case 2: // Alert-Modus initialisieren
                display.showFullscreenAlert("ALARM!", true);
                break;

            default: ;
                break;
        }
    }

    // Im Dashboard-Modus könnten hier Werte aktualisiert werden
    if (displayMode == 1) {
        // Simuliere einen sich ändernden Temperaturwert
        static float temp = 23.5;
        temp += 0.1;
        if (temp > 25.0) temp = 23.5;
        
        display.setDashboardText(OLEDDisplaySH1106::TOP_LEFT, "Temp: " + String(temp, 1) + " C");
        display.showDashboard(); // Erneut zeichnen, um die Änderung anzuzeigen
    }
    
    delay(500); // Verlangsamt die Aktualisierung im Dashboard-Modus
}