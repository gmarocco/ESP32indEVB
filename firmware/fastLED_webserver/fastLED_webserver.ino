#include <WiFi.h>
#include <FastLED.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

// LED configuration
#define LED_PIN 14
#define NUM_LEDS 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Brightness level (default)
uint8_t brightness = 128;

// HTML for the web page
const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 LED Control</title>
  <style>
    body { font-family: Arial; text-align: center; }
    .control { margin: 10px; }
  </style>
</head>
<body>
  <h1>ESP32 LED Controller</h1>
  <div class="control">
    <label for="colorPicker">Color:</label>
    <input type="color" id="colorPicker" value="#ff0000">
  </div>
  <div class="control">
    <label for="brightnessSlider">Brightness:</label>
    <input type="range" id="brightnessSlider" min="0" max="255" value="128">
  </div>
  <button onclick="setColorAndBrightness()">Set Color and Brightness</button>
  <script>
    function setColorAndBrightness() {
      const color = document.getElementById('colorPicker').value;
      const brightness = document.getElementById('brightnessSlider').value;
      const xhr = new XMLHttpRequest();
      xhr.open('GET', `/setColor?color=${color.substring(1)}&brightness=${brightness}`, true);
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";

WiFiServer server(80);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client Connected");
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("GET /setColor?") >= 0) {
      int colorStart = request.indexOf("color=") + 6;
      int colorEnd = request.indexOf("&", colorStart);
      String colorCode = request.substring(colorStart, colorEnd);

      int brightnessStart = request.indexOf("brightness=") + 11;
      int brightnessEnd = request.indexOf(" ", brightnessStart);
      String brightnessValue = request.substring(brightnessStart, brightnessEnd);

      Serial.println("Received Color Code: " + colorCode);
      Serial.println("Received Brightness: " + brightnessValue);

      if (colorCode.length() == 6) {
        uint32_t color = strtol(colorCode.c_str(), NULL, 16);
        setStripColor(color);
      }

      brightness = brightnessValue.toInt();
      FastLED.setBrightness(brightness);
      FastLED.show();
    }

    // Serve the webpage
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n");
    client.print(webpage);
    client.stop();
    Serial.println("Client Disconnected");
  }
}

void setStripColor(uint32_t color) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}

