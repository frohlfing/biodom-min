#include "SensorCapacitiveSoil.h"

SensorCapacitiveSoil::SensorCapacitiveSoil(uint8_t analogPin, int dryValue, int wetValue)
    : _pin(analogPin), _dry(dryValue), _wet(wetValue), _raw(-1), _percent(-1), _lastError(0) {}

bool SensorCapacitiveSoil::begin() {
    // Für den analogen Sensor besteht die "Initialisierung" aus einem ersten
    // erfolgreichen Leseversuch, um die Verbindung zu verifizieren.
    return read();
}

bool SensorCapacitiveSoil::read() {
    // Es wird vorausgesetzt, dass dry größer wet ist!
    if (_dry <= _wet) {
        _raw = -1;
        _percent = -1;
        _lastError = 1; // Kalibrierung ungültig
        return false;
    }

    // analogRead Rückgabewerte können je nach Plattform variieren (0..4095 auf ESP32)
    const int temp = analogRead(_pin);

    // Liegt der Messwert im erwarteten Bereich? 
    // Falls der Sensor nicht angeschlossen ist, hängt der analoge Pin in der Luft und liefert zufällige Werte (wirkt wie eine Antenne).
    const int tolerance = static_cast<int>(static_cast<float>(_dry - _wet) * 0.25f); // 25 % Toleranz
    if (temp < _wet - tolerance || temp > _dry + tolerance) {
        _raw = -1;
        _percent = -1;
        _lastError = 2; // Messwert unplausibel
        return false;
    }
    
    // Erfolgreiche Messung
    _raw = temp;

    // Rohwert auf Prozent umrechnen.
    const int percent = map(temp, _dry, _wet, 0, 100); //  Wandelt den analogen Messwert (temp) linear in einen Prozentwert um.
    _percent = constrain(percent, 0, 100); // Stellt sicher, dass der berechnete Prozentwert immer im gültigen Bereich von 0 bis 100 liegt.

    _lastError = 0;
    return true;
}

int SensorCapacitiveSoil::getRaw() const {
    return _raw;
}

int SensorCapacitiveSoil::getPercent() const {
    return _percent;
}

void SensorCapacitiveSoil::setCalibration(int dryValue, int wetValue) {
    _dry = dryValue;
    _wet = wetValue;
}

int SensorCapacitiveSoil::getLastError() const {
    return _lastError;
}

const char* SensorCapacitiveSoil::getErrorMessage() const {
    switch (_lastError) {
        case 0: return "OK";
        case 1: return "Falsche Kalibrierung";
        case 2: return "Messwert unplausibel";
        default: return "Sensor nicht bereit";
    }
}
