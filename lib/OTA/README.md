# 📌 OTA-Bibliothek

Diese Bibliothek kapselt die Funktionalität für Over-The-Air (OTA) Updates mit der `ArduinoOTA`-Bibliothek des ESP32. Sie macht die Integration in ein Projekt sehr einfach und sorgt für detailliertes Feedback über die serielle Schnittstelle während des Update-Prozesses.

Die  WLAN-Verbindung muss extern hergestellt werden, bevor die begin()-Methode aufgerufen wird. Dadurch kann die WLAN-Verbindung von mehreren Netzwerkdiensten (OTA, Webserver, NTP-Client) gemeinsam genutzt werden.

## 📦 Installation & Konfiguration

Die Bibliothek benötigt keine externen Abhängigkeiten, da `ArduinoOTA` Teil des ESP32-Arduino-Core ist.

Für die Nutzung von OTA muss die `platformio.ini`-Datei wie folgt konfiguriert werden:

```ini
[env:esp32-devkitc-v2]
; ...
upload_protocol = espota
upload_port = biodom-mini.local
```
Der `upload_port` ist die mDNS-Adresse (Hostname + Suffix ".local") oder IP-Adresse. 

## 📜 Lizenz

Diese Bibliothek ist eine Abstraktion der `ArduinoOTA`-Bibliothek, die unter der [LGPL-2.1 Lizenz](https://github.com/espressif/arduino-esp32/blob/master/LICENSE) steht.