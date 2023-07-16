#include <Arduino.h>
#include <FastLED.h>

#define BUILTIN_LED 8 // pin for built in led

#define NUM_LEDS 60 // Number of leds
#define LED_PIN 5 // Data pin for led strip

#define SWITCH_A 9
#define SWITCH_B 10
#define SWITCH_C 4
#define SWITCH_D 3

CRGB g_LEDs[NUM_LEDS] = {0};    // Frame buffer for FastLED
int g_brightness = 180;
const int MAX_BRIGHTNESS = 180;
const int MIN_BRIGHTNESS = 0;
int g_maxpower = 1000;

enum Mode
{
  led_off,
  led_on,
  led_rainbow
}led_mode;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_A, INPUT_PULLUP);
  pinMode(SWITCH_B, INPUT_PULLUP);
  pinMode(SWITCH_C, INPUT_PULLUP);
  pinMode(SWITCH_D, INPUT_PULLUP);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);
  FastLED.setBrightness(g_brightness);
  FastLED.setMaxPowerInMilliWatts(g_maxpower);
}

int current_hue = 0;
int hue_step = 3 ;
int rainbow_timer = 0;
const int rainbow_speed = 50;

void loop() {
  if(digitalRead(SWITCH_A) == LOW) {led_mode = led_off;}
  if(digitalRead(SWITCH_B) == LOW) {led_mode = led_on;}
  if(digitalRead(SWITCH_C) == LOW) {led_mode = led_rainbow;}
  if(digitalRead(SWITCH_D) == LOW)
  {
      
  }
  switch (led_mode)
  {
    case led_off:
      FastLED.clear();
      FastLED.show();
      break;
    case led_on:
      fill_solid(g_LEDs, NUM_LEDS, CRGB::White);
      FastLED.show();
      break;
    case led_rainbow:
      fill_rainbow(g_LEDs, NUM_LEDS, current_hue, hue_step);
      if((millis() - rainbow_timer) > rainbow_speed)
      {
        current_hue += hue_step;
        rainbow_timer = millis();
        FastLED.show();
      }
      break;
  }
}