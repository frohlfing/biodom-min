// In einer neuen Datei ISensor.h

#pragma once

// Dies ist eine abstrakte Basisklasse, die als Interface dient.
// Sie definiert, welche Funktionen eine Sensor-Klasse bereitstellen muss.
class ISensor {
public:
    // Rein virtuelle Funktionen (gekennzeichnet durch "= 0").
    // Jede erbende Klasse MUSS diese implementieren.
    virtual bool begin() = 0;
    virtual bool read() = 0;
    virtual int getLastError() const = 0;
    virtual const char* getErrorMessage() const = 0;

    // Ein virtueller Destruktor ist bei Basisklassen guter Stil, 
    // um Speicherprobleme bei Polymorphie zu vermeiden.
    virtual ~ISensor() {} 
};