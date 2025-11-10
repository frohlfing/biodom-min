#include "SensorBH1750.h"

SensorBH1750::SensorBH1750(uint8_t address, TwoWire* wire)
    : _wire(wire),
      _addr(address),
      _drv(BH1750(address)),
      _lux(NAN),
      _lastError(0),
      _mode(BH1750::CONTINUOUS_HIGH_RES_MODE) {}

bool SensorBH1750::begin() {
    // Die Initialisierung des I2C-Busses (Wire.begin()) sollte im Hauptprogramm erfolgen.
    // Wir prüfen hier, ob der Sensor auf dem Bus antwortet.
    if (!_drv.begin(_mode)) {
        _lastError = 1; // Initialisierung fehlgeschlagen
        return false;
    }

    _lastError = 0;
    return true;
}

bool SensorBH1750::read() {
    float temp = _drv.readLightLevel();

    // Die Bibliothek liefert negative Werte bei einem Lesefehler.
    if (temp < 0) {
        _lux = NAN;
        _lastError = 2; // Lesen fehlgeschlagen
        return false;
    }

    _lux = temp;
    _lastError = 0;
    return true;
}

float SensorBH1750::getLux() const {
    return _lux;
}

int SensorBH1750::getLastError() const {
    return _lastError;
}

const char* SensorBH1750::getErrorMessage() const {
    switch (_lastError) {
        case 0: return "OK";
        case 1: return "Init fehlgeschlagen";
        case 2: return "Messung fehlerhaft";
        default: return "Sensor nicht bereit";
    }
}

void SensorBH1750::setMode(BH1750::Mode mode) {
    if (_mode != mode) {
        _mode = mode;
        _drv.begin(_mode);
    }
}
