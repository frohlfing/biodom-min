#include "SensorXKCY25NPN.h"

SensorXKCY25NPN::SensorXKCY25NPN(uint8_t pin, bool useInternalPullup)
    : _pin(pin), _useInternalPullup(useInternalPullup), _waterDetected(false) {}

bool SensorXKCY25NPN::begin() {
    if (_useInternalPullup) {
        pinMode(_pin, INPUT_PULLUP);
    } else {
        pinMode(_pin, INPUT);
    }

    // Eine erste Messung durchführen, um einen Startwert zu haben.
    return read(); 
}

bool SensorXKCY25NPN::read() {
    // Der Sensor-Ausgang ist ein NPN Open-Collector.
    // Er zieht den Pin auf LOW, wenn Wasser erkannt wird.
    // Mit einem Pull-Up-Widerstand ist der Pin HIGH, wenn kein Wasser da ist.
    int pinState = digitalRead(_pin);

    // Wasser wird erkannt, wenn der Pin LOW ist.
    _waterDetected = (pinState == LOW);
    
    return true; // Das Lesen eines digitalen Pins schlägt nicht fehl.
}

bool SensorXKCY25NPN::isWaterDetected() const {
    return _waterDetected;
}

int SensorXKCY25NPN::getLastError() {
    // Dieser Sensor hat keine Fehlererkennung.
    return 0;
}

const char* SensorXKCY25NPN::getErrorMessage() {
    // Entsprechend gibt es nur die "OK"-Meldung.
    return "OK";
}
