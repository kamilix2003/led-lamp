#include <Arduino.h>
#include <FastLED.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>

#include "../include/led_mode.h"
#include "../include/utility.h"
#include "../include/brightness.h"
#include "../include/emotes.h"

#define BUILTIN_LED 8
#define NUM_LEDS 60 // Number of leds
#define LED_PIN 5 // Data pin for led strip

#define SWITCH_A 0
#define SWITCH_B 10
#define SWITCH_C 1
#define SWITCH_D 2


// fastled declarations
CRGB g_LEDs[NUM_LEDS] = {0}; // Frame buffer for FastLED
int led_brightness = 180;
int g_maxpower = 1200;
int g_maxmilliamps = 1500;
int g_maxvolt = 5;

// mode declarations
Mode led_mode = Mode::led_on;
Mode led_mode_previous;

// brightness declarations
bool brightness_increment_direction = false;
bool brightness_direction_switch = false;

// LCD declarations
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool update_display = true;
bool backlight = true;

// timeout
int a_timer = 0;
int b_timer = 0;
int c_timer = 0;
int d_timer = 0;
int display_uptdate_timer = 0;

void setup() {
  // pins setup
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  // switch setup
  pinMode(SWITCH_A, INPUT_PULLUP);
  pinMode(SWITCH_B, INPUT_PULLUP);
  pinMode(SWITCH_C, INPUT_PULLUP);
  pinMode(SWITCH_D, INPUT_PULLUP);
  
  // serial communication setup
  Serial.begin(115200);

  // LCD setup
  lcd.init();
  lcd.createChar(0, bulb);
  lcd.setBacklight(150);
  lcd.setCursor(0, 0);
  lcd.print("Led mode: ");
  lcd.setCursor(0, 1);
  // lcd.print("Brightness: ");
  lcd.write(0);
  lcd.print(":");

  // FastLed setup
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);
  FastLED.setBrightness(led_brightness);
  // FastLED.setMaxPowerInMilliWatts(g_maxpower);
  FastLED.setMaxPowerInVoltsAndMilliamps(g_maxvolt, g_maxmilliamps);

}
int test;
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
    update_display = true;
  }
  
  // switch B handling
  if(digitalRead(SWITCH_B) == LOW && timeout(&b_timer, 200)) 
  {
    cycle_enum(&led_mode);
    update_display = true;
  }

  // switch C handling
  if(digitalRead(SWITCH_C) == LOW && timeout(&c_timer, 10))
  {
    increment_brightness(&led_brightness, brightness_increment_direction);
    FastLED.setBrightness(led_brightness);
    brightness_direction_switch = true;
    update_display = true;
  }
  if(digitalRead(SWITCH_C) == HIGH && brightness_direction_switch)
  {
    brightness_increment_direction = !brightness_increment_direction;
    brightness_direction_switch = false;
  }
  // switch D handling
  if(digitalRead(SWITCH_D) == LOW && timeout(&b_timer, 200)) 
  {
    if(backlight) {lcd.noBacklight();}
    else {lcd.backlight();}
    backlight = !backlight;
  }

  // display
  if(update_display)
  {
    Serial.println("LCD update");
    lcd.setCursor(10, 0);
    lcd.print((int)led_mode);
    lcd.setCursor(2, 1);
    lcd.print("   ");
    lcd.setCursor(2, 1);
    lcd.print(led_brightness);
    lcd.setCursor(6, 1);
    if(!brightness_increment_direction) {lcd.print("increasing");}
    else {lcd.print("decreasing");}
    update_display = false;
  }

  // led update
  switch (led_mode)
  {
    case led_off:
      FastLED.clear(true);
      break;
    case led_on:
      led_mode_solid_color(g_LEDs, NUM_LEDS, CRGB::White);
      break;
    case led_rainbow:
      led_mode_rainbow(g_LEDs, NUM_LEDS);
      break;
    case led_comet:
      led_mode_comet(g_LEDs, NUM_LEDS);
      break;
    case led_police:
      led_mode_police(g_LEDs, NUM_LEDS);
      break;
    case led_test:
      // led_mode_test(g_LEDs, NUM_LEDS);
      break;
    default:
      led_mode_solid_color(g_LEDs, NUM_LEDS, CRGB::White);
      break;
  }
  FastLED.show();
}