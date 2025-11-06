# OLEDDisplaySH1106

Diese Bibliothek kapselt die Nutzung des 1.3 Zoll OLED Displays SSH1106.

Das Display hat 128x64 Pixel (monochrom, 1 Bit pro Pixel) und wird über die I2C-Schnittstelle gesteuert.

## Funktionsumfang

*   **Scrolling Log:** Einfaches Hinzufügen von Textzeilen, die automatisch nach oben scrollen, wenn der Bildschirm voll ist. Ideal für Statusmeldungen beim Booten oder während des Betriebs.
*   **Dashboard-Ansicht:** Aufteilung des Bildschirms in vier Quadranten, in denen jeweils ein Textwert und optional ein Icon angezeigt werden können. Perfekt für die Anzeige von mehreren Sensorwerten.
*   **Vollbild-Warnung:** Anzeige einer zentrierten, bildschirmfüllenden Nachricht, die optional blinken kann, um die Aufmerksamkeit des Nutzers zu erregen.

## 📦 Installation & Abhängigkeiten

* Folgende Bibliothek muss in `platformio.ini` eingebunden werden:

```ini
lib_deps =
  olikraus/U8g2 @ ^2.36.15
```

## 📌 Wichtige Hinweise

### Pin-Belegung

Das Display wird über I2C angesteuert. Die Standard-Hardware-I2C-Pins des ESP32 sind:
*   **SDA:** GPIO 21
*   **SCL:** GPIO 22

Stelle sicher, dass keine anderen Geräte auf dem I2C-Bus die gleiche Adresse verwenden.

### U8g2-Konstruktor

Diese Bibliothek ist fest für ein **128x64 SH1106 I2C Display** konfiguriert. Dies ist im Konstruktor `U8G2_SH1106_128X64_NONAME_F_HW_I2C` festgelegt. Für ein anderes Display (z.B. mit SSD1306-Controller oder anderer Auflösung) muss der Konstruktor in der `.cpp`-Datei angepasst werden.

### Der `update()`-Loop

Für zeitbasierte Effekte wie das Blinken der Warnmeldung ist es zwingend erforderlich, die Methode `display.update()` regelmäßig in der `loop()`-Funktion des Hauptprogramms aufzurufen.

### Speicherverbrauch

Die Bibliothek verwendet den "Full Buffer"-Modus (`_F_`) von U8g2. Dies bietet die beste Darstellungsqualität, belegt aber permanent 1024 Bytes (128 * 64 / 8) des RAM-Speichers auf dem ESP32.

## 📜 Lizenz

Diese Bibliothek basiert auf [U8g2 library by oliver (olikraus)](https://github.com/olikraus/u8g2). Sie folgt deren Lizenzbedingungen ([2-Clause BSD License](https://github.com/olikraus/u8g2/blob/master/LICENSE)).
