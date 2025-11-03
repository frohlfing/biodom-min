/**
 * @file GY302.h
 * @brief Lokale Kapselung für das Lichtsensormodul GY-302 (BH1750, I2C).
 *
 * Diese Wrapper-Klasse verwendet intern die BH1750-Bibliothek von Christofer Laws (claws/BH1750)
 * für die eigentliche Protokoll-/I2C-Logik.
 */

#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h> // externe Bibliothek (claws/BH1750)

/**
 * @brief Klasse für das GY-302 / BH1750 Lichtsensormodul.
 *
 * Dieser Wrapper kapselt die BH1750-Bibliothek in eine leichte, testbare Schnittstelle,
 * die dem Stil der bestehenden Projektbibliotheken folgt.
 */
class GY302 {
public:
    /**
     * @brief Fehlercodes der Klasse.
     */
    enum Error {
        ErrNone = 0,
        ErrNotInitialized = 1,
        ErrReadFailed = 2
    };

    /**
     * @brief Konstruktor.
     * @param address I2C-Adresse des Sensors (Standard: 0x23).
     * @param wire Pointer auf TwoWire-Instanz (Default: &Wire).
     */
    explicit GY302(uint8_t address = 0x23, TwoWire* wire = &Wire);

    /**
     * @brief Initialisiert das I2C-Interface und den BH1750-Treiber.
     * @return true bei erfolgreicher Initialisierung, false bei Fehler.
     */
    bool begin();

    /**
     * @brief Führt eine Messung durch und speichert den aktuellen Lux-Wert intern.
     * @return true bei Erfolg, false bei Fehler (Fehlercode über getLastError()).
     */
    bool read();

    /**
     * @brief Liefert den zuletzt gemessenen Lux-Wert.
     * @return Lux als float; bei Fehler wird NAN zurückgegeben.
     */
    float getLux() const;

    /**
     * @brief Liefert den zuletzt gesetzten Fehlercode.
     * @return Error-Code.
     */
    Error getLastError() const;

    /**
     * @brief Liefert eine kurze, lesbare Fehlermeldung zum letzten Fehler.
     * @return C-String mit Beschreibung.
     */
    const char* getErrorMessage() const;

    /**
     * @brief Setzt den Messmodus des internen BH1750-Treibers.
     * @param mode BH1750-Modus aus der BH1750-Bibliothek (z. B. BH1750::CONT_HIGH_RES_MODE).
     */
    void setMode(uint8_t mode);

private:
    TwoWire* _wire;
    uint8_t _addr;
    BH1750 _drv;
    float _lastLux;
    Error _lastError;
    uint8_t _mode;
};
