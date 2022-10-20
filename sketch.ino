// ESP32 sketch

#include "WiFi.h"
#include "AsyncUDP.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIDTH  128
#define HEIGHT 64

// ((WIDTH * HEIGHT) / 8) = ((128 * 64) / 8) = 1024
#define BUFFER_SIZE 1024

Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, 4);
const char *ssid = "wifi_name";
const char *password = "password";
AsyncUDP udp;

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    // computer address
    udp.connect(IPAddress(192,168,0,103), 7755);

}

void loop() {
  display.clearDisplay();

  display.drawFastHLine(0, 32, 128, SSD1306_WHITE);
  display.drawFastVLine(64, 0, 128, SSD1306_WHITE);
 
  display.display();
  
  udp.write(display.getBuffer(), BUFFER_SIZE);
}
  
