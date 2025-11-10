#pragma once

#include <Arduino.h>

/**
 * Klasse für den kapazitiven Bodenfeuchtesensor V1.2 (Capacitive Soil Moisture Sensor v1.2, analog)
 *
 * Wandelt den analogen Messwert in Prozent um (0–100 %).
 */
class SensorCapacitiveSoil {
public:
    /**
     * @brief Konstruktor mit Angabe des analogen Eingangs und optionalen Kalibrierungswerten
     * 
     * dryValue muss größer als wetValue sein!
     * 
     * @param analogPin GPIO-Analog-Pin, an dem der Sensor angeschlossen ist.
     * @param dryValue Rohwert für trockenen Boden (höherer Analogwert).
     * @param wetValue Rohwert für nassen Boden (niedrigerer Analogwert).
     */
    explicit SensorCapacitiveSoil(uint8_t analogPin, int dryValue = 2500, int wetValue = 1100);

    /**
     * @brief Initialisiert den Sensor.
     * Führt eine erste Testmessung durch, um die Verbindung zu prüfen.
     * @return true bei Erfolg, andernfalls false.
     */
    bool begin();

    /**
     * @brief Liest die Bodenfeuchte vom Sensor.
     * @return true bei erfolgreichem Auslesen, false bei Fehler.
     */
    bool read();

    /**
     * @brief Gibt doe zuletzt gelesene Bodenfeuchte als Rohwert zurück.
     * @return Rohwert (int).
     */
    int getRaw() const;

    /**
     * @brief Gibt die zuletzt berechnete Bodenfeuchte in Prozent zurück.
     * @return Feuchtigkeit in Prozent (0–100).
     */
    int getPercent() const;

    /**
     * @brief Setzt Kalibrierungswerte zur Laufzeit.
     * @param dryValue Rohwert für trockenen Boden.
     * @param wetValue Rohwert für nassen Boden.
     */
    void setCalibration(int dryValue, int wetValue);

    /**
     * @brief Gibt den letzten Fehlercode zurück.
     * @return Fehlercode (0=OK, 1=Wert ausserhalb d. Bereichs, 2=Ungültige Kalibrierung)
     */
    int getLastError() const;

    /**
     * @brief Gibt eine Beschreibung des letzten Fehlers zurück.
     * @return Fehlerbeschreibung (max. 21 Zeichen).
     */
    const char* getErrorMessage() const;

private:
    uint8_t _pin;   // Analogpin
    int _dry;       // Rohwert dry (trocken)
    int _wet;       // Rohwert wet (nass)
    int _raw;       // letzter Messert (Rohwert)
    int _percent;   // letzter Messert in Prozent
    int _lastError; // Fehlercode
};
