#pragma once

#include <Arduino.h>

/**
 * @class OTA
 * @brief Kapselt die Logik für Over-The-Air (OTA) Updates.
 *
 * Diese Klasse konfiguriert und startet den ArduinoOTA-Service. Sie stellt Callbacks bereit, um über 
 * den Fortschritt und mögliche Fehler des Update-Prozesses über den Seriellen Monitor zu informieren.
 *
 * Die WLAN-Verbindung muss extern hergestellt werden, bevor die begin()-Methode aufgerufen wird.
 */
class OTA {
public:
    /**
     * @brief Konstruktor.
     * @param hostname Der Name des Geräts im Netzwerk. Wird für die .local-Adresse und im Netzwerk-Scan verwendet.
     * @param password Ein Passwort, das beim Upload eingegeben werden muss.
     */
    OTA(const char* hostname, const char* password);

    /**
     * @brief Initialisiert den OTA-Dienst.
     * Muss aufgerufen werden, NACHDEM eine erfolgreiche WLAN-Verbindung besteht.
     * Prüft, ob eine WLAN-Verbindung besteht, bevor der Dienst gestartet wird.
     * @return true, wenn der Dienst erfolgreich gestartet wurde, andernfalls false.
     */
    bool begin();

    /**
     * @brief Verarbeitet eingehende OTA-Anfragen.
     * Diese Methode MUSS kontinuierlich in der Hauptschleife (loop()) des Programms
     * aufgerufen werden, damit der OTA-Dienst auf Anfragen reagieren kann.
     */
    void handle();

private:
    const char* _hostname;
    const char* _password;
};