#include "OTA.h"
#include <ArduinoOTA.h>
#include <WiFi.h> // Nur für Typendefinitionen wie ota_error_t benötigt

OTA::OTA(const char* hostname, const char* password)
    : _hostname(hostname), _password(password) {}

bool OTA::begin() {
    // Der OTA-Dienst kann nur gestartet werden, wenn eine WLAN-Verbindung besteht.
    if (WiFi.status() != WL_CONNECTED) {
        return false; // Fehler: Keine WLAN-Verbindung.
    }
    
    // Konfiguriere den Hostnamen für eine einfache Erkennung im Netzwerk
    ArduinoOTA.setHostname(_hostname);

    // Setze ein Passwort für sichere Uploads
    ArduinoOTA.setPassword(_password);

    // Konfiguriere Callbacks, um detailliertes Feedback über den Seriellen Monitor zu geben.
    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "Sketch";
            else // U_SPIFFS
                type = "Dateisystem";
            Serial.println("Starte OTA-Update: " + type);
        })
        .onEnd([]() {
            Serial.println("\nUpdate erfolgreich abgeschlossen!");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Fortschritt: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
            Serial.printf("Fehler[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Authentifizierung fehlgeschlagen");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin fehlgeschlagen");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Verbindung fehlgeschlagen");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Empfang fehlgeschlagen");
            else if (error == OTA_END_ERROR) Serial.println("Ende fehlgeschlagen");
            // Ein halt() wird hier nicht aufgerufen, damit das Hauptprogramm weiterlaufen kann.
        });

    ArduinoOTA.begin();
    return true;
}

void OTA::handle() {
    ArduinoOTA.handle();
}