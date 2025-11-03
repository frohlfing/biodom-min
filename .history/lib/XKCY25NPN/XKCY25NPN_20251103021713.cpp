#include "XKC_Y25_NPN.h"

XKC_Y25_NPN::XKC_Y25_NPN(uint8_t pin, bool useInternalPullup)
    : _pin(pin), _useInternalPullup(useInternalPullup), _lastState(false) {}

void XKC_Y25_NPN::begin() {
    if (_useInternalPullup) {
        pinMode(_pin, INPUT_PULLUP);
    } else {
        pinMode(_pin, INPUT);
    }
}

bool XKC_Y25_NPN::read() {
    // Sensor: OUT pulled LOW when water is present (NPN sink).
    // Mit externem Pull-Up zieht OUT bei Trockenheit auf HIGH.
    int v = digitalRead(_pin);
    // Wenn OUT == LOW -> Wasser erkannt
    _lastState = (v == LOW);
    return _lastState;
}

bool XKC_Y25_NPN::isWaterDetected() const {
    return _lastState;
}

const char* XKC_Y25_NPN::wiringNotes() const {
    return "VCC: 5-12V; OUT: open-collector NPN -> use 10k pull-up to 3.3V; GND common";
}
