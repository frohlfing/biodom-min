/**
 * Beispiel zur Nutzung der OTA-Bibliothek
 * 
 * Das Beispiel verbindet sich mit dem WLAN und startet den OTA-Dienst.
 * Anschließend gibt es minütlich eine Statusmeldung aus.
 * 
 * Nach dem ersten Upload per USB kann der Sketch via WLAN aktualisiert werden.
 * Ändern Sie z.B. die "Hello World"-Nachricht und laden Sie den Sketch erneut hoch.
 */

#include <Arduino.h>
#include <WiFi.h>
#include "OTA.h"

// Ersetzen Sie dies mit Ihren WLAN-Zugangsdaten
const char* WIFI_SSID = "DEIN_WLAN_NAME";
const char* WIFI_PASSWORD = "DEIN_WLAN_PASSWORT";

// Konfiguration für OTA
const char* HOSTNAME = "ota-beispiel";
const char* OTA_PASSWORD = "ota-passwort";

OTA ota(HOSTNAME, OTA_PASSWORD);

void setup() {
    Serial.begin(115200);
    while (!Serial);
    Serial.println("\nOTA Beispiel-Sketch");

    // 1. Mit dem WLAN verbinden
    Serial.print("Verbinde mit WLAN...");
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOSTNAME);  
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWLAN verbunden!");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());
    Serial.print("Host: ");
    Serial.println(WiFi.getHostname());

    // 2. OTA-Dienst starten
    ota.begin();
}

void loop() {
    // 3. OTA-Handler in der Hauptschleife aufrufen
    ota.handle();

    // Beispiel-Code, der im Hintergrund läuft
    static unsigned long lastMessageTime = 0;
    if (millis() - lastMessageTime > 60000) {
        lastMessageTime = millis();
        Serial.println("Hello World! Warte auf OTA-Update...");
    }
}