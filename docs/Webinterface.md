## Webinterface

Wir werden ein **asynchrones Webinterface** aufbauen. Das ist entscheidend, damit die Bedienung des Webinterfaces und die Steuerung des Gewächshauses sich nicht gegenseitig blockieren.

Hier ist der Plan für den **Grundaufbau**:

1.  **Bibliotheken installieren:** Wir benötigen die Bibliothek [ESPAsyncWebServer 3.9.0 by ESP32Async](https://github.com/ESP32Async/ESPAsyncWebServer).
2.  **Dateisystem (SPIFFS) einrichten:** Das HTML/CSS/JS für die Webseite legen wir in eine separate Datei, nicht in den C++-Code.
3.  **Eine `WebInterface`-Klasse erstellen:** Diese kapselt die gesamte Server-Logik.
4.  **Der Grundaufbau wird Folgendes können:**
    *   Eine einfache HTML-Seite ausliefern.
    *   Eine WebSocket-Verbindung zum Browser aufbauen.
    *   Die aktuellen Sensordaten **einmal pro Sekunde** vom ESP32 an die Webseite senden.
    *   Die Webseite wird diese Daten empfangen und anzeigen.

### Schritt 1: `platformio.ini` anpassen

Wir müssen die Webserver-Bibliothek hinzufügen und PlatformIO anweisen, das Dateisystem (SPIFFS) für unsere Webseite zu erstellen und hochzuladen.

**Erweitere deine `platformio.ini`-Datei:**

```ini
lib_deps =
    esp32async/ESPAsyncWebServer @ ^3.9.0
    ; esphome/AsyncTCP-esphome @ ^2.1.2

; --- Konfiguration für das SPIFFS-Dateisystem ---
board_build.filesystem = spiffs

```
**Wichtiger Hinweis:** Nach dem Hinzufügen des `extra_scripts` musst du eventuell einmalig VS Code neu starten, damit PlatformIO das Skript erkennt.

---

### Schritt 2: Die Webseite erstellen (`index.html`)

1.  Erstelle im **Hauptverzeichnis** deines Projekts (auf derselben Ebene wie `src`, `lib`, `platformio.ini`) einen neuen Ordner namens `data`.
2.  In diesem `data`-Ordner erstellst du eine Datei namens `index.html`.

**Füge diesen Code in `data/index.html` ein:**

```html
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Biodom Mini</title>
    <style>
        body { font-family: sans-serif; background-color: #282c34; color: #abb2bf; text-align: center; }
        .container { max-width: 600px; margin: auto; padding: 20px; }
        .sensor { background-color: #3c4049; border-radius: 8px; padding: 15px; margin: 10px; }
        h1, h2 { color: #61afef; }
        .value { font-size: 1.5em; font-weight: bold; color: #98c379; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Biodom Mini - Dashboard</h1>
        
        <div class="sensor">
            <h2>Raumklima</h2>
            <p>Temperatur: <span id="airTemp" class="value">--.-</span> °C</p>
            <p>Luftfeuchte: <span id="humidity" class="value">--</span> %</p>
        </div>
        
        <div class="sensor">
            <h2>Bodenklima</h2>
            <p>Temperatur: <span id="soilTemp" class="value">--.-</span> °C</p>
            <p>Feuchtigkeit: <span id="soilMoisture" class="value">--</span> %</p>
        </div>
    </div>

    <script>
        // Startet eine WebSocket-Verbindung zur IP-Adresse des ESP32
        const gateway = `ws://${window.location.hostname}/ws`;
        let websocket;

        // Diese Funktion wird nach dem Laden der Seite aufgerufen
        window.addEventListener('load', () => {
            initWebSocket();
        });

        function initWebSocket() {
            console.log('Versuche, WebSocket zu öffnen...');
            websocket = new WebSocket(gateway);
            websocket.onopen    = onOpen;
            websocket.onclose   = onClose;
            websocket.onmessage = onMessage;
        }

        function onOpen(event) {
            console.log('Verbindung hergestellt.');
        }

        function onClose(event) {
            console.log('Verbindung getrennt.');
            // Versuche nach 2 Sekunden, die Verbindung erneut herzustellen
            setTimeout(initWebSocket, 2000);
        }

        function onMessage(event) {
            // Empfangene Daten sind ein JSON-String
            console.log(event.data);
            const data = JSON.parse(event.data);
            
            // Aktualisiere die HTML-Elemente mit den neuen Werten
            document.getElementById('airTemp').innerText = data.airTemp.toFixed(1);
            document.getElementById('humidity').innerText = data.humidity.toFixed(0);
            document.getElementById('soilTemp').innerText = data.soilTemp.toFixed(1);
            document.getElementById('soilMoisture').innerText = data.soilMoisture;
        }
    </script>
</body>
</html>
```

---

### Schritt 3: Die `WebInterface`-Bibliothek erstellen

Erstelle den Ordner `lib/WebInterface` mit den Dateien `WebInterface.h` und `WebInterface.cpp`.

#### `WebInterface.h`
```cpp
#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

class WebInterface {
public:
    WebInterface(uint16_t port = 80);

    bool begin();

    void broadcastSensorData(float airTemp, float humidity, float soilTemp, int soilMoisture);

private:
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    
    AsyncWebServer _server;
    AsyncWebSocket _ws;
};
```

#### `WebInterface.cpp`
```cpp
#include "WebInterface.h"
#include <SPIFFS.h>

WebInterface::WebInterface(uint16_t port)
    : _server(port), _ws("/ws") {}

bool WebInterface::begin() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Web-Fehler: Keine WLAN-Verbindung.");
        return false;
    }

    if (!SPIFFS.begin(true)) {
        Serial.println("Web-Fehler: SPIFFS konnte nicht initialisiert werden.");
        return false;
    }

    // WebSocket-Events an unsere Handler-Funktion binden
    _ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        this->onWsEvent(server, client, type, arg, data, len);
    });
    _server.addHandler(&_ws);

    // HTTP-Route für die Hauptseite ("/")
    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });

    _server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Seite nicht gefunden");
    });

    _server.begin();
    Serial.println("WebInterface-Dienst bereit.");
    return true;
}

void WebInterface::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket-Client #%u verbunden von %s\n", client->id(), client->remoteIP().toString().c_str());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket-Client #%u getrennt\n", client->id());
    } else if (type == WS_EVT_DATA) {
        // Hier werden wir später Befehle vom Browser empfangen
        Serial.println("Daten vom Client empfangen - wird noch nicht verarbeitet.");
    }
}

void WebInterface::broadcastSensorData(float airTemp, float humidity, float soilTemp, int soilMoisture) {
    // Erstelle einen JSON-String mit den Sensordaten.
    // TODO: Später durch die robustere ArduinoJson-Bibliothek ersetzen.
    String json = "{\"airTemp\":" + String(airTemp) +
                  ",\"humidity\":" + String(humidity) +
                  ",\"soilTemp\":" + String(soilTemp) +
                  ",\"soilMoisture\":" + String(soilMoisture) + "}";

    // Sende den JSON-String an alle verbundenen WebSocket-Clients
    _ws.textAll(json);
}
```

---

### Schritt 4: Integration in `main.cpp`

**1. Header und globales Objekt hinzufügen:**
```cpp
// In main.cpp
// ...
#include "NTPClock.h"
#include "WebInterface.h" // <-- NEU

// ...
NTPClock ntpClock;
WebInterface webInterface; // <-- NEUES OBJEKT
```

**2. `setup()` anpassen:**
```cpp
// In der setup()-Funktion von main.cpp
void setup() {
    // ...
    // 5) Netzwerk, Zeit und Webinterface initialisieren
    setupWifi();
    if (!ota.begin()) { /* ... */ }
    if (!ntpClock.begin()) { /* ... */ }
    if (!webInterface.begin()) { // <-- NEU
        halt("WEB FEHLER", "Serverstart fehlgeschlagen");
    }

    // ...
}
```

**3. `loop()` anpassen:**
Wir senden die Daten im selben Takt, in dem das Display aktualisiert wird.
```cpp
// In der loop()-Funktion von main.cpp
void loop() {
    // ...
    if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
        lastDisplayUpdate = currentTime;
        handleDisplay();

        // --- NEU: Sende Daten an das Webinterface ---
        // Prüfe, ob die Werte gültig sind, bevor sie gesendet werden.
        if (!isnan(currentAirTemp) && currentSoilMoisture != -1) {
            webInterface.broadcastSensorData(currentAirTemp, currentHumidity, currentSoilTemp, currentSoilMoisture);
        }
    }
    // ...
}
```

---

### Schritt 5: Der Workflow

1.  **Dateisystem hochladen (einmalig):**
    *   Klicke in PlatformIO auf das Alien-Kopf-Icon (PIO) in der linken Leiste.
    *   Wähle unter `ova` -> `Platform` den Task **"Upload Filesystem Image"**.
    *   Dadurch wird die `index.html` auf den ESP32 geladen. Dies musst du nur wiederholen, wenn du die `index.html` änderst.
2.  **Sketch hochladen (per OTA):**
    *   Lade den `main.cpp`-Sketch wie gewohnt hoch.
3.  **Testen:**
    *   Öffne einen Browser auf einem Gerät im selben WLAN.
    *   Gib die IP-Adresse deines ESP32 oder `http://biodom-mini.local` ein.
    *   Die Webseite sollte erscheinen und sich nach einem kurzen Moment mit den Live-Daten füllen.

Das ist der vollständige Grundaufbau. Von hier aus können wir das Interface jetzt schrittweise erweitern, z.B. um Sollwerte vom Browser an den ESP32 zu senden.
