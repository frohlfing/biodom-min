#include "SensorDS18B20.h"

SensorDS18B20::SensorDS18B20(uint8_t pin)
    : _oneWire(pin), _sensor(&_oneWire), _temperature(NAN), _lastError(0) {}
    
bool SensorDS18B20::begin() {
    // DallasTemperature-Bibliothek starten
    _sensor.begin();

    if (_sensor.getDeviceCount() == 0) {
        _lastError = 1; // Kein Gerät am OneWire-Bus
        return false;
    }

    // Hole die Sensoradresse und führe einen CRC-Check (Prüfsumme)) durch.
    // Wenn die Kommunikation gestört ist (z.B. durch einen Wackelkontakt), könnte getAddress() nur "Datenmüll" lesen.
    if (!_sensor.getAddress(_addr, 0) || !_sensor.validAddress(_addr)) {
        _lastError = 2; // Sensoradresse ungültig
        return false;
    }

    _lastError = 0; // Alles ok
    return true;
}

bool SensorDS18B20::read() {
    // Temperaturmessung anfordern (dieser Aufruf blockiert ca. 750ms!)
    _sensor.requestTemperaturesByAddress(_addr); 

    // Temperatur auslesen
    float temp = _sensor.getTempC(_addr);

    // Fehler beim Auslesen prüfen (z.B. wenn Sensor während des Betriebs getrennt wurde)
    if (temp == DEVICE_DISCONNECTED_C) {
        _temperature = NAN;
        _lastError = 3; // Sensor getrennt
        return false;
    }

    // Erfolgreiche Messung
    _temperature = temp;
    _lastError = 0;
    return true;
}

float SensorDS18B20::getTemperature() const {
    return _temperature;
}

int SensorDS18B20::getLastError() const {
    return _lastError;
}

const char* SensorDS18B20::getErrorMessage() const {
    switch (_lastError) {
        case 0: return "OK";
        case 1: return "Sensor nicht gefunden";
        case 2: return "Adresse ungueltig";
        case 3: return "Sensor getrennt";
        default: return "Sensor nicht bereit";
    }
}
