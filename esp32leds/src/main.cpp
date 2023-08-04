#include <Arduino.h>
#include <FastLED.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

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

#define ENCODER_BUTTON 3
#define ENCODER_A 6
#define ENCODER_B 4

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

// Encoder
enum Encoder_mode{
    ENCODER_BEGIN,
    ENCODER_mode = 0,
    ENCODER_brightness,
    ENCODER_temperature,
    ENCODER_lcd_backlight,
    ENCODER_led_off,
    ENCODER_END,
}encoder_display;

enum Encoder_rotation{
    ENCODER_stop,
    ENCODER_right,
    ENCODER_left,
}encoder_rotation;



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
    led_mode = (Mode)((int)led_mode+1);
    if(led_mode == Mode::END) {led_mode = (Mode)((int)Mode::BEGIN+1);}
    update_display = true;
  }

  // display
  if(update_display)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    switch(encoder_display)
      {
      case ENCODER_mode:
        lcd.print("Led Mode: ");
        lcd.print((int)led_mode);
        break;
      case ENCODER_brightness:
        lcd.print("Brightness: ");
        lcd.print(led_brightness);
        break;
      case ENCODER_temperature:
        lcd.print("Temperature: ");
        break;
      case ENCODER_lcd_backlight:
        lcd.print("Lcd backlight: ");
        break;
      case ENCODER_led_off:
        lcd.print("Led off: ");
        break;
      }
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