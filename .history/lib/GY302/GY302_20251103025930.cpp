#include "GY302.h"

GY302::GY302(uint8_t address, TwoWire* wire)
    : _wire(wire),
      _addr(address),
      _drv(BH1750(address)),
      _lastLux(NAN),
      _lastError(ErrNotInitialized),
      _mode(BH1750::CONT_HIGH_RES_MODE) {}

bool GY302::begin() {
    if (!_wire) {
        _lastError = ErrNotInitialized;
        return false;
    }

    _wire->begin();
    // BH1750 library erwartet, dass Wire bereits initialisiert ist.
    if (!_drv.begin(_mode)) {
        _lastError = ErrNotInitialized;
        return false;
    }

    _lastError = ErrNone;
    return true;
}

bool GY302::read() {
    // Die claws/BH1750 Bibliothek liefert negative Werte bei Fehler (konvention der Lib).
    float lux = _drv.readLightLevel();
    if (lux < 0.0f) {
        _lastLux = NAN;
        _lastError = ErrReadFailed;
        return false;
    }

    _lastLux = lux;
    _lastError = ErrNone;
    return true;
}

float GY302::getLux() const {
    return _lastLux;
}

GY302::Error GY302::getLastError() const {
    return _lastError;
}

const char* GY302::getErrorMessage() const {
    switch (_lastError) {
        case ErrNone: return "Erfolgreich";
        case ErrNotInitialized: return "Nicht initialisiert oder I2C nicht verfügbar";
        case ErrReadFailed: return "Messung fehlgeschlagen";
        default: return "Unbekannter Fehler";
    }
}

void GY302::setMode(uint8_t mode) {
    _mode = mode;
    // Reconfigure driver if already initialized
    if (_lastError == ErrNone) {
        // begin(mode) in claws/BH1750 setzt den Modus neu
        _drv.begin(_mode);
    }
}
