#include <FastLED.h>

enum Mode
{
  BEGIN,
  led_off,
  led_on,
  led_rainbow,
  led_test,
  END,
};

void led_mode_off(CRGB* LEDs, int NUM_LEDS);
void led_mode_solid_color(CRGB* LEDs, int NUM_LEDS, CRGB color);
void led_mode_rainbow(CRGB* LEDs, int NUM_LEDS);
void led_mode_test(CRGB* LEDs, int NUM_LEDS);
