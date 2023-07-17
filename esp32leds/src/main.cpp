#include <Arduino.h>
#include <FastLED.h>
#include <math.h>

#define BUILTIN_LED 8
#define NUM_LEDS 60 // Number of leds
#define LED_PIN 5 // Data pin for led strip

#define SWITCH_A 9
#define SWITCH_B 10
#define SWITCH_C 4
#define SWITCH_D 3

CRGB g_LEDs[NUM_LEDS] = {0}; // Frame buffer for FastLED
int g_brightness = 180;
const int MAX_BRIGHTNESS = 220;
const int MIN_BRIGHTNESS = 0;
int g_maxpower = 1200;
int g_maxmilliamps = 1800;
int g_maxvolt = 5;
bool brightness_increase = false;
bool brightness_direction_switch = false;

enum Mode
{
  BEGIN,
  led_off,
  led_on,
  led_rainbow,
  END,
};
Mode led_mode = Mode::led_on;
Mode led_mode_previous;
std::string led_mode_names[3] = {
  "leds white",
  "leds rainbow",
  "leds off",
};
bool led_mode_switch = false;
  
int current_hue = 0;
int hue_step = 3 ;
int rainbow_timer = 0;
int rainbow_speed = 50;

// timeout
u_int a_timer = 0;
u_int b_timer = 0;
u_int c_timer = 0;
u_int d_timer = 0;
bool timeout(u_int* p_timer, u_int timeout_time)
{
  if(*p_timer == 0) 
  {
    *p_timer = millis();
    return true;
  }
  if((millis() - *p_timer) > timeout_time)
  {
    *p_timer = 0;
  }
  return false;
}

void setup() {

  // pins setup
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_A, INPUT_PULLUP);
  pinMode(SWITCH_B, INPUT_PULLUP);
  pinMode(SWITCH_C, INPUT_PULLUP);
  pinMode(SWITCH_D, INPUT_PULLUP);
  
  // serial communication setup
  Serial.begin(115200);

  // fastled setup
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);
  FastLED.setBrightness(g_brightness);
  // FastLED.setMaxPowerInMilliWatts(g_maxpower);
  FastLED.setMaxPowerInVoltsAndMilliamps(g_maxvolt, g_maxmilliamps);
}

void loop() {
  // switch A handling
  if(digitalRead(SWITCH_A) == LOW && timeout(&a_timer, 200)) 
  {
    if(led_mode == Mode::led_off) {led_mode = led_mode_previous;}
    else 
    {
      led_mode_previous = led_mode;
      led_mode = Mode::led_off;
    }
  }
  
  // switch B handling
  if(digitalRead(SWITCH_B) == LOW && timeout(&b_timer, 200)) 
  {
    led_mode = (Mode)((int)led_mode+1);
    if(led_mode == Mode::END) {led_mode = (Mode)((int)Mode::BEGIN+2);}
  }

  // switch C handling
  if(digitalRead(SWITCH_C) == LOW && timeout(&c_timer, 10))
  {
    if(g_brightness < MAX_BRIGHTNESS && brightness_increase)
    {
      if(g_brightness < 2) {g_brightness+1;}
      g_brightness += log(g_brightness+1);
      FastLED.setBrightness(g_brightness);
    }
    if(g_brightness > MIN_BRIGHTNESS && !brightness_increase)
    {
      g_brightness -= log(g_brightness-1);
      FastLED.setBrightness(g_brightness);
    }
    brightness_direction_switch = true;
    Serial.printf("brightness: %i\n", FastLED.getBrightness());
  }
  if(digitalRead(SWITCH_C) == HIGH && brightness_direction_switch)
  {
    brightness_increase = !brightness_increase;
    if(brightness_increase) {digitalWrite(BUILTIN_LED, HIGH);}
    else {digitalWrite(BUILTIN_LED, LOW);}
    brightness_direction_switch = false;
  }
  // switch D handling



  // led update
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
    default:
      fill_solid(g_LEDs, NUM_LEDS, CRGB::White);
      FastLED.show();
      break;
  }
}