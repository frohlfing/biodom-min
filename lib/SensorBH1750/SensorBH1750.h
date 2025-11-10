#pragma once

#include <Arduino.h>
#include <BH1750.h>

/**
 * @brief Klasse für den Lichtsensor GY-302 BH1750
 */
class SensorBH1750 {
public:
    /**
     * @brief Konstruktor.
     * @param address I2C-Adresse des Sensors (Standard: 0x23).
     * @param wire Pointer auf TwoWire-Instanz (Default: &Wire).
     */
    explicit SensorBH1750(uint8_t address = 0x23, TwoWire* wire = &Wire);

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
     * @brief Gibt den letzten Fehlercode zurück.
     * @return Fehlercode
     */
    int getLastError() const;

    /**
     * @brief Liefert eine kurze, lesbare Fehlermeldung zum letzten Fehler.
     * @return C-String mit Beschreibung.
     */
    const char* getErrorMessage() const;

    /**
     * @brief Ändert den Messmodus des Sensors zur Laufzeit.
     * 
     * Der BH1750 kann in verschiedenen Modi betrieben werden, die sich in Auflösung,
     * Messzeit und Stromverbrauch unterscheiden. Z.B.:
     * - BH1750::CONTINUOUS_HIGH_RES_MODE (Standard): Kontinuierliche Messung, hohe Auflösung.
     * - BH1750::ONE_TIME_HIGH_RES_MODE: Einzelmessung, danach geht der Sensor in den Power-Down-Modus.
     * 
     * @param mode Der gewünschte Messmodus aus der BH1750-Bibliothek.
     */
    void setMode(BH1750::Mode mode);

private:
    TwoWire* _wire;     // Pointer auf die I2C-Schnittstelle (z.B. &Wire), ermöglicht flexible Nutzung von I2C0/I2C1.
    uint8_t _addr;      // Die I2C-Adresse des Sensors (z.B. 0x23).
    BH1750 _drv;        // Die Instanz der zugrundeliegenden Bibliothek.
    float _lux;         // Speichert den zuletzt erfolgreich gemessenen Lichtwert in Lux.
    int _lastError;     // Speichert den Fehlercode der letzten Operation (0 = OK).
    BH1750::Mode _mode; // Speichert den aktuell konfigurierten Messmodus des Sensors.
};
