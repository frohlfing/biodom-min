#pragma once

#include <Arduino.h>

/**
 * Klasse für den berührungslosen Füllstandssensor XKC-Y25-NPN.
 *
 * Der Sensor liefert LOW bei Wassererkennung.
 */
class SensorXKCY25NPN {
public:
    /**
     * @brief Konstruktor.
     * @param pin GPIO-Pin, an den der digitale Ausgang (OUT) des Sensors angeschlossen ist.
     * @param useInternalPullup Wenn true, wird pinMode(INPUT_PULLUP) genutzt (falls geeignet).
     */
    explicit SensorXKCY25NPN(uint8_t pin, bool useInternalPullup = false);

    /**
     * @brief Initialisiert den Sensor (setzt den Pin-Modus).
     * @return Gibt immer true zurück, da die Initialisierung nicht fehlschlagen kann.
     */
    bool begin();

    /**
     * @brief Liest den aktuellen Zustand des Sensors.
     * @return Gibt immer true zurück, da das Lesen eines digitalen Pins nicht fehlschlagen kann.
     */
    bool read();

    /**
     * @brief Gibt den zuletzt gelesenen Rohzustand zurück.
     * @return true = Wasser erkannt, false = kein Wasser.
     */
    bool isWaterDetected() const;

    /**
     * @brief Gibt den letzten Fehlercode zurück.
     * @return Fehlercode (immer 0, da keine Fehler erkannt werden können).
     */
    static int getLastError();

    /**
     * @brief Gibt eine Beschreibung des letzten Fehlers zurück.
     * @return Fehlerbeschreibung.
     */
    static const char* getErrorMessage();

private:
    uint8_t _pin;               // Der GPIO-Pin, an dem der Sensor angeschlossen ist.
    bool _useInternalPullup;    // Speichert, ob der interne Pull-Up-Widerstand des MCU verwendet werden soll.
    bool _waterDetected;        // Speichert das Ergebnis der letzten Messung (true, wenn Wasser erkannt wurde).
};
