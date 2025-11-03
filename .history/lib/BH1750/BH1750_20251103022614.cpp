#include "GY302_BH1750.h"

GY302_BH1750::GY302_BH1750(uint8_t address, TwoWire* wire)
    : _wire(wire), _addr(address), _lastLux(NAN), _lastError(ErrNoI2C), _continuousHighRes(true) {}

bool GY302_BH1750::begin() {
    if (!_wire) {
        _lastError = ErrNoI2C;
        return false;
    }
    _wire->begin();
    // Power on
    if (!writeCommand(CMD_POWER_ON)) {
        _lastError = ErrNACK;
        return false;
    }
    // Reset device (per datasheet: only in power on mode)
    writeCommand(CMD_RESET);

    // Default to continuous high-res mode
    if (_continuousHighRes) {
        if (!writeCommand(CMD_CONT_H_RES)) {
            _lastError = ErrNACK;
            return false;
        }
    } else {
        writeCommand(CMD_POWER_DOWN);
    }

    _lastError = ErrNone;
    delay(10);
    return true;
}

bool GY302_BH1750::writeCommand(uint8_t cmd) {
    _wire->beginTransmission(_addr);
    _wire->write(cmd);
    uint8_t rc = _wire->endTransmission();
    return (rc == 0);
}

bool GY302_BH1750::read() {
    // Ensure powered on / in measurement mode
    if (_continuousHighRes) {
        if (!writeCommand(CMD_CONT_H_RES)) {
            _lastError = ErrNACK;
            _lastLux = NAN;
            return false;
        }
        // Measurement time per datasheet ~120ms
        delay(120);
    } else {
        // Single measurement mode not implemented here; require continuous H-res for simplicity
        if (!writeCommand(CMD_CONT_H_RES)) {
            _lastError = ErrNACK;
            _lastLux = NAN;
            return false;
        }
        delay(120);
    }

    uint16_t raw = 0;
    if (!readRaw(raw)) {
        _lastLux = NAN;
        return false;
    }

    // Convert to lux: per datasheet lux = raw / 1.2
    _lastLux = static_cast<float>(raw) / 1.2f;
    _lastError = ErrNone;
    return true;
}

bool GY302_BH1750::readRaw(uint16_t &out) {
    // Request 2 bytes
    _wire->requestFrom(static_cast<int>(_addr), 2);
    unsigned long start = millis();
    while (_wire->available() < 2) {
        if (millis() - start > 200) { // timeout 200 ms
            _lastError = ErrTimeout;
            return false;
        }
    }
    int msb = _wire->read();
    int lsb = _wire->read();
    if (msb < 0 || lsb < 0) {
        _lastError = ErrInvalidData;
        return false;
    }
    out = (static_cast<uint16_t>(msb) << 8) | static_cast<uint16_t>(lsb);
    return true;
}

float GY302_BH1750::getLux() const {
    return _lastLux;
}

GY302_BH1750::Error GY302_BH1750::getLastError() const {
    return _lastError;
}

const char* GY302_BH1750::getErrorMessage() const {
    switch (_lastError) {
        case ErrNone: return "Erfolgreich";
        case ErrNoI2C: return "I2C Interface nicht verfügbar";
        case ErrNACK: return "I2C NACK/No ACK vom Gerät";
        case ErrTimeout: return "I2C Lesezeitüberschreitung";
        case ErrInvalidData: return "Ungültige I2C-Daten";
        default: return "Unbekannter Fehler";
    }
}

void GY302_BH1750::setContinuousHighRes(bool continuousHighRes) {
    _continuousHighRes = continuousHighRes;
}
