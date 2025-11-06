/**
 * PoC Schritt 1: Anzeige eines statischen Bitmaps auf dem OLED.
 * 
 * Ziel: Sicherstellen, dass die Display-Hardware funktioniert und die
 * U8g2-Funktion `drawXBMP` korrekt verwendet wird. Es wird ein
 * einfaches Smiley-Bitmap im XBM-Format angezeigt.
 * 
 * Der Code: Ich habe den Demo-Sketch für das OLED-Display genommen und 
 * alles bis auf das absolut Notwendigste entfernt. Wir fügen ein einfaches, 
 * von Hand erstelltes "Smiley"-Bitmap hinzu und zeigen es an.
 * 
 * Dies ist die Grundlage, auf der die späteren Schritte aufbauen werden.
 */

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// --- Display-Initialisierung ---
// Wir verwenden die bekannte Konfiguration für das SH1106 I2C Display.
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// --- Unser Test-Bitmap im XBM-Format ---
// XBM (X BitMap) ist ein Standard-C-Format, um monochrome Bilder zu speichern.
// Jedes Byte repräsentiert 8 horizontale Pixel. Die Bits werden "rückwärts" gelesen,
// d.h. das niedrigstwertige Bit (LSB) ist das linkeste Pixel.
// Dieses Array definiert ein 32x32 Pixel großes Smiley-Bild.
#define smiley_width 32
#define smiley_height 32
static const unsigned char smiley_bits[] U8X8_PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
   0x00, 0x3c, 0x3c, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x80, 0xff, 0xff, 0x01,
   0xc0, 0xff, 0xff, 0x03, 0xe0, 0x01, 0x80, 0x07, 0xf0, 0x00, 0x00, 0x0f,
   0xf8, 0x20, 0x04, 0x1f, 0xf8, 0x70, 0x0e, 0x1f, 0xfc, 0x70, 0x0e, 0x3f,
   0xfc, 0x70, 0x0e, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f,
   0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x70, 0x0e, 0x3f, 0xfc, 0x70, 0x0e, 0x3f,
   0xf8, 0x70, 0x0e, 0x1f, 0xf8, 0x20, 0x04, 0x1f, 0xf0, 0x00, 0x00, 0x0f,
   0xe0, 0x01, 0x80, 0x07, 0xc0, 0xff, 0xff, 0x03, 0x80, 0xff, 0xff, 0x01,
   0x00, 0x7e, 0x7e, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x00, 0x18, 0x18, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void setup() {
    Serial.begin(115200);
    Serial.println("JpgToBmp - PoC Schritt 1: Statisches Bitmap anzeigen");

    // Display initialisieren
    u8g2.begin();
    
    // Puffer löschen, bevor wir zeichnen
    u8g2.clearBuffer();
    
    // --- Der Kern dieses Tests ---
    // Zeichne das Bitmap in den Puffer.
    // Parameter:
    // x, y:       Die linke obere Ecke, an der das Bitmap gezeichnet wird.
    // width, height: Die Größe des Bitmaps.
    // bits:       Das Byte-Array mit den Pixeldaten.
    // Wir zentrieren das 32x32 Bild auf dem 128x64 Display.
    int x_pos = (128 - smiley_width) / 2;
    int y_pos = (64 - smiley_height) / 2;
    u8g2.drawXBMP(x_pos, y_pos, smiley_width, smiley_height, smiley_bits);
    
    // Den Puffer-Inhalt an das Display senden, um ihn sichtbar zu machen
    u8g2.sendBuffer();

    Serial.println("Bitmap sollte jetzt auf dem Display sichtbar sein.");
}

void loop() {
    // Nichts zu tun hier, das Bild wird einmalig im setup() angezeigt.
    delay(1000);
}