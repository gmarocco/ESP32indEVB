#include <FastLED.h>

// LED configuration
#define LED_PIN 14
#define NUM_LEDS 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void setup() {
  // Initialize LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(80);

  // Seed random number generator
  randomSeed(analogRead(0));
}

void loop() {
  // Generate a random color
  CRGB randomColor = CRGB(random(0, 256), random(0, 256), random(0, 256));

  // Fade to the random color over 2 seconds
  fadeToColor(randomColor, 500);
}

void fadeToColor(CRGB targetColor, int durationMs) {
  int steps = 256; // Number of steps in the fade
  int delayTime = durationMs / steps;

  for (int step = 0; step <= steps; step++) {
    float fraction = step / float(steps);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = blend(leds[i], targetColor, fraction * 255);
    }
    FastLED.show();
    delay(delayTime);
  }
}