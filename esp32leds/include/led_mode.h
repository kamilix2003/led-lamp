#include <FastLED.h>
#include <Arduino.h>

const int MODE_COUNT = 6;

enum Mode
{
  BEGIN,
  led_off = 0,
  led_on,
  led_rainbow,
  led_comet,
  led_police,
  led_test,
  END,
};

void led_mode_off(CRGB* LEDs, int NUM_LEDS);
void led_mode_solid_color(CRGB* LEDs, int NUM_LEDS, CRGB color);
void led_mode_rainbow(CRGB* LEDs, int NUM_LEDS);
int led_mode_comet(CRGB* LEDs, int NUM_LEDS);
void led_mode_police(CRGB* LEDs, int NUM_LEDS);
void led_mode_test(CRGB* LEDs, int NUM_LEDS);
