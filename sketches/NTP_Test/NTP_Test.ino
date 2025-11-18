/**
 * NTP (Network Time Protocol)
 *
 * Beispiel zur Zeitsynchronisation mit einem NTP-Server auf dem ESP32.
 */

#include <WiFi.h>
#include <time.h> // Die C-Standardbibliothek für Zeitfunktionen

// Konfiguration
//constexpr char HOSTNAME[] = "biodom-mini-dev";
//constexpr char WIFI_SSID[] = "WOLKE7";
//constexpr char WIFI_PASSWORD[] = "1234";
#include "secrets.h"

// NTP-Server und Zeitzonen
const char* NTP_SERVER = "pool.ntp.org";    // NTP-Server
constexpr long  GMT_OFFSET = 3600;              // Mitteleuropäische Zeit (MEZ) = UTC+1 = 3600 Sekunden für Zeitzone "Berlin"
constexpr int   DAYLIGHT_OFFSET = 3600;         // Sommerzeit (MESZ) = UTC+2, also zusätzliche 3600 Sekunden (Sommer-/Winterzeit wird automatisch umgestellt)

void setup() {
    Serial.begin(115200);

    // 1. Mit dem WLAN verbinden
    Serial.print("Verbinde mit WLAN...");
    WiFi.mode(WIFI_STA); // Station Mode (Client)
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

    // 2. NTP-Client initialisieren
    configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTP_SERVER); // startet Hintergrund-Task für Zeitsynchronisation 

    // 3. Warten auf die erste Synchronisation
    Serial.print("Synchronisiere Zeit...");
    tm timeInfo{};
    while (!getLocalTime(&timeInfo)) {
        delay(100);
        Serial.print(".");
    }
    Serial.println("\nZeit erfolgreich synchronisiert!");

    // Uhrzeit ausgeben
    Serial.print("Aktuelle Zeit: ");
    Serial.print(timeInfo.tm_hour);
    Serial.print(":");
    Serial.print(timeInfo.tm_min);
    Serial.print(":");
    Serial.println(timeInfo.tm_sec);
}

void loop() {
    tm timeInfo{};
    if (!getLocalTime(&timeInfo)) {
        Serial.println("Fehler beim Abrufen der Zeit."); // dürfte nie vorkommen, da im Setup die Zeit synchronisiert wurde
        delay(1000);
        return;
    }

    // Zeitinformationen formatieren und ausgeben
    char timeString[20];
    strftime(timeString, sizeof(timeString), "%d.%m.%Y %H:%M:%S", &timeInfo);
    Serial.println(timeString);
    delay(1000);
}