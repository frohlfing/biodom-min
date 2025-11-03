/**
 * @file GY302_BH1750.h
 * @brief Lokale Kapselung für das Lichtsensormodul GY-302 (BH1750, I2C).
 */

#pragma once
#include <Arduino.h>
#include <Wire.h>

/**
 * @brief Klasse für das GY-302 / BH1750 Lichtsensormodul.
 *
 * Implementiert einen kleinen, stabilen I2C-Treiber (ohne zusätzliche externe Bibliotheken).
 * Misst Beleuchtungsstärke in Lux.
 */
class GY302_BH1750 {
public:
    /**
     * @brief Fehlercodes der Klasse.
     */
    enum Error {
        ErrNone = 0,
        ErrNoI2C = 1,
        ErrNACK  = 2,
        ErrTimeout = 3,
        ErrInvalidData = 4
    };

    /**
     * @brief Konstruktor.
     * @param address I2C-Adresse des Sensors (Standard: 0x23).
     * @param wire Pointer auf TwoWire-Instanz (Standard: &Wire).
     */
    explicit GY302_BH1750(uint8_t address = 0x23, TwoWire* wire = &Wire);

    /**
     * @brief Initialisiert Wire und den Sensor.
     * @return true bei erfolgreicher Initialisierung (Gerät antwortet auf I2C), false sonst.
     */
    bool begin();

    /**
     * @brief Führt eine Messung durch und speichert den aktuellen Lux-Wert intern.
     * @return true bei Erfolg, false bei Fehler (Fehlercode über getLastError()).
     */
    bool read();

    /**
     * @brief Liefert den zuletzt gemessenen Lux-Wert zurück.
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
     * @brief Setzt die Messauflösung / Modus (vereinfachte Auswahl).
     * @param continuousHighRes true = kontinuierlicher High-Res (1 lx Auflösung), false = power down.
     * @note Nach Modus-Wechsel empfiehlt sich ein kurzes Delay vor der ersten Messung.
     */
    void setContinuousHighRes(bool continuousHighRes);

private:
    TwoWire* _wire;
    uint8_t _addr;
    float _lastLux;
    Error _lastError;
    bool _continuousHighRes;

    static constexpr uint8_t CMD_POWER_DOWN = 0x00;
    static constexpr uint8_t CMD_POWER_ON   = 0x01;
    static constexpr uint8_t CMD_RESET      = 0x07;
    static constexpr uint8_t CMD_CONT_H_RES = 0x10; // 1 lx, 120ms
    static constexpr uint8_t CMD_CONT_H_RES2= 0x11; // 0.5 lx (not used)
    static constexpr uint8_t CMD_CONT_L_RES = 0x13; // 4 lx

    bool writeCommand(uint8_t cmd);
    bool readRaw(uint16_t &out);
};
