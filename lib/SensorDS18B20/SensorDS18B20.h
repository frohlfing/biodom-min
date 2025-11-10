#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>

/**
 * Klasse für den Bodentemperatursensor DS18B20
 */
class SensorDS18B20 {
public:
    /**
     * @brief Konstruktor mit Angabe des GPIO-Pins.
     * @param pin GPIO-Pin, an dem der Sensor angeschlossen ist.
     */
    explicit SensorDS18B20(uint8_t pin);

    /**
     * @brief Initialisiert den Sensor.
     * Führt eine erste Testmessung durch, um die Verbindung zu prüfen.
     * @return true bei Erfolg, andernfalls false.
     */
    bool begin();

    /**
     * @brief Liest die Bodentemperatur vom Sensor.
     * 
     * Dieser Aufruf blockiert ca. 750ms!
     * 
     * @return true bei erfolgreichem Auslesen, false bei Fehler.
     */
    bool read();

    /**
     * @brief Gibt die zuletzt gemessene Temperatur in °C.
     * @return Temperatur (float), oder NAN bei Fehler.
     */
    float getTemperature() const;

    /**
     * @brief Gibt den letzten Fehlercode zurück.
     * @return Fehlercode (0=OK, 1=nicht gefunden, 2=Adresse ungültig, 3=getrennt)
     */
    int getLastError() const;

    /**
     * @brief Gibt eine Beschreibung des letzten Fehlers zurück.
     * @return Fehlerbeschreibung (max. 21 Zeichen).
     */
    const char* getErrorMessage() const;

private:
    OneWire _oneWire;              // OneWire-Instanz für die Kommunikation
    DallasTemperature _sensor;     // DallasTemperature-Wrapper
    DeviceAddress _addr;           // Sensoradresse
    float _temperature;            // Letzte gemessene Temperatur
    int _lastError;                // Fehlercode
};
