#include "SensorLDR5528.h"
#include <math.h>

SensorLDR5528::SensorLDR5528(uint8_t analogPin, float fixedResistorOhm, uint16_t adcMax) 
  : _pin(analogPin), _fixedResistorOhm(fixedResistorOhm), _adcMax(adcMax), _raw(0), _resistance(NAN), _lux(NAN), _lastError(0) {}

bool SensorLDR5528::begin() {
    // Für den analogen Sensor besteht die "Initialisierung" aus einem ersten
    // erfolgreichen Leseversuch, um die Verbindung zu verifizieren.
    return read();
}

bool SensorLDR5528::read() {
    // --- 1. Oversampling zur Rauschreduzierung ---
    // Wir nehmen mehrere Messungen und bilden den Durchschnitt, um einen stabileren Wert zu erhalten.
    uint32_t sum = 0;
    const uint8_t samples = 5;
    for (uint8_t i = 0; i < samples; ++i) {
        sum += analogRead(_pin);
        delay(5); // Kurze Pause zwischen den Messungen
    }
    _raw = static_cast<uint16_t>(sum / samples);

    // --- 2. Plausibilitätsprüfung ---
    // Ein Wert ganz am Anfang oder Ende der Skala ist physikalisch unwahrscheinlich und
    // deutet auf einen Fehler hin (z.B. Kurzschluss nach GND oder VCC, offener Pin).
    // Wir lassen eine kleine Toleranz von 1 ADC-Schritt.
    if (_raw >= (_adcMax - 1) || _raw <= 1) {
        _resistance = NAN;
        _lux = NAN;
        _lastError = 1; // Fehler: Wert unplausibel
        return false;
    }

    // --- 3. Widerstandsberechnung basierend auf dem Spannungsteiler ---
    // Formel: R_LDR = R_pullup * (Vcc / Vout - 1)
#if defined(ARDUINO_ARCH_ESP32)
    const float vcc = 3.3f;
#else
    const float vcc = 5.0f;
#endif

    // Umwandlung des ADC-Rohwerts in die Spannung am Messpunkt (Vout)
    float vout = (static_cast<float>(_raw) / static_cast<float>(_adcMax)) * vcc;
    
    // Formel für Pull-Down-Schaltung: VCC --[LDR]-- Vout --[R_fixed]-- GND
    // Vout = VCC * R_fixed / (R_LDR + R_fixed)
    // ==> R_LDR = R_fixed * (VCC / Vout - 1)
    if (vout <= 0) { // Schutz vor Division durch Null
      _resistance = INFINITY;
    } else {
      _resistance = _fixedResistorOhm * (vcc / vout - 1.0f);
    }

    // --- 4. Approximative Umrechnung von Widerstand in Lux ---
    // Diese Formel ist eine empirische Annäherung und dient nur als Schätzwert.
    _lux = resistanceToLux(_resistance);
    
    // --- 5. Erfolgreicher Abschluss ---
    _lastError = 0; // Fehlercode zurücksetzen
    return true;
}

uint16_t SensorLDR5528::getRaw() const {
  return _raw;
}

float SensorLDR5528::getResistance() const {
  return _resistance;
}

float SensorLDR5528::getLux() const {
  return _lux;
}

void SensorLDR5528::setAdcMax(uint16_t adcMax) {
  _adcMax = adcMax;
}

void SensorLDR5528::setFixedResistorOhm(float fixedResistorOhm) {
  _fixedResistorOhm = fixedResistorOhm;
}

float SensorLDR5528::adcToVoltage(uint16_t adc) const {
  // adc / adcMax * Vcc
#if defined(ARDUINO_ARCH_ESP32)
  const float vcc = 3.3f;
#else
  const float vcc = 5.0f;
#endif
  return (static_cast<float>(adc) / static_cast<float>(_adcMax)) * vcc;
}

float SensorLDR5528::resistanceToLux(float r) const {
  // Vereinfachte empirische Formel:
  // Für typische LDRs gilt näherungsweise R ~ a * lux^b
  // -> lux ~ (R / a)^(1/b)
  // Werte a,b grob gewählt für allgemeine LDRs; kalibrieren für Genauigkeit.
  if (!isfinite(r) || r <= 0.0f) return NAN;

  const float a = 50000.0f; // Justagewert
  const float b = -0.8f;    // Justagewert

  // lux = (r / a)^(1/b)
  float ratio = r / a;
  if (ratio <= 0.0f) return NAN;
  float lux = pow(ratio, 1.0f / b);
  return lux;
}

int SensorLDR5528::getLastError() const {
    return _lastError;
}

const char* SensorLDR5528::getErrorMessage() const {
    switch(_lastError) {
        case 0: return "OK";
        case 1: return "Wert unplausibel";
        default: return "Unbekannter Fehler";
    }
}
