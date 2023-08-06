#include <Arduino.h>
#include <FastLED.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>

#include "../include/utility.h"
#include "../include/emotes.h"

#define BUILTIN_LED 8
#define NUM_LEDS 60 // Number of leds
#define LED_PIN 5 // Data pin for led strip

#define SWITCH_A 0
#define SWITCH_B 10

LiquidCrystal_I2C lcd(0x27, 16, 2);

enum Led_effects
{
  led_effects_off = 0,
  led_effects_solid,
  led_effects_rainbow,
  led_effects_END
};

char* led_effects_name[] =
{
  "off",
  "solid",
  "rainbow",
};
struct Led
{
  const int pin = LED_PIN;
  const int led_count = NUM_LEDS;
  CRGB buffer[NUM_LEDS] = {0};

  bool pause_update = false;
  Led_effects mode = Led_effects::led_effects_off;
  Led_effects temp = Led_effects::led_effects_solid;
  
  int brightness = 200;
  const int max_brightness = 255;
  const int min_brightness = 0;

  int rainbow_hue = 0;
  const int rainbow_hue_step = 10;
  int rainbow_timer = 0;
  const int rainbow_speed = 50;

  /// @brief updates led strip from the buffer
  void update()
  {
    switch(mode)
    {
      case Led_effects::led_effects_off:
      led_off();
      break;

      case Led_effects::led_effects_solid:
      led_solid(CRGB::White);
      break;

      case Led_effects::led_effects_rainbow:
      led_rainbow();
      break;
    }
    if(!pause_update){FastLED.show();}
  }
  /// @brief switches to led_effect_off and switches back to previous effect when called again
  void off_effect()
  {
    if(mode == Led_effects::led_effects_off)
      {mode = temp; return;}
    temp = mode;
    mode = Led_effects::led_effects_off;
  }
  /// @brief cycles through effects
  void next_effect()
  {
    mode = (Led_effects)((int)mode + 1);
    if(mode == Led_effects::led_effects_END) {mode = (Led_effects)0;}
  }
  void increase_brightness(int multiplier)
  {
    brightness += multiplier;
    if(brightness > max_brightness) {brightness = min_brightness;}
    FastLED.setBrightness(brightness);
  }
  // void decrease_brightness(int multiplier)
  // {
  //   brightness -= multiplier;
  //   if(brightness < min_brightness) {brightness = min_brightness;}
  // }

  // effects
  void led_off()
  {
    FastLED.clear(true);
  }

  void led_solid(CRGB color)
  {
    fill_solid(buffer, led_count, color);
    FastLED.show();
  }

  void led_rainbow()
  {
    if(!timeout(&rainbow_timer, rainbow_speed)) {return; }
    fill_rainbow(buffer, led_count, rainbow_hue, rainbow_hue_step);
    rainbow_hue += rainbow_hue_step;
  }

}led_strip;

// Button
struct Button
{
  int pin;
  int debaunce_timer = 0;
}button_a, button_b;

// Lcd
enum Display_selection
{
  DISPLAY_led_mode = 0,
  DISPLAY_led_brigthnesss,
  DISPLAY_END
};
char* display_selection_names[] =
{
  "Led mode",
  "Brightness"
};
struct Display
{
  LiquidCrystal_I2C core_display = lcd;
  int backlight = 90;
  const  int max_backlight = 255;
  const  int min_backlight = 0;

  bool to_update = false;
  int periodic_update_timer = 0;
  const  int periodic_update_time = 10000;

  Display_selection mode = Display_selection::DISPLAY_led_mode;

  void next_selection()
  {
    mode = (Display_selection)((int)mode + 1);
    if(mode == Display_selection::DISPLAY_END) {mode = (Display_selection)0;}
  }

    void increase_backlight(int multiplier)
  {
    backlight += multiplier;
    if(backlight > max_backlight) {backlight = min_backlight;}
  }

  void update()
  {
    if(!to_update && !timeout(&periodic_update_timer, periodic_update_time)) {return; }
    lcd.clear();
    lcd.home();
    lcd.print(display_selection_names[(int)mode]);
    switch(mode)
    {
      case Display_selection::DISPLAY_led_mode:
      lcd.print(":");
      lcd.setCursor(0, 1);
      lcd.print(led_effects_name[(int)led_strip.mode]);
      break;

      case Display_selection::DISPLAY_led_brigthnesss:
      lcd.print(": ");
      lcd.print(led_strip.brightness);
      break;
    }
    to_update = false;
  }
}display;

void IRAM_ATTR button_a_interrupt()
{
  if(!timeout(&button_a.debaunce_timer, 3)) {return;}
  display.next_selection();
  display.to_update = true;
}

void IRAM_ATTR button_b_interrupt()
{
  if(!timeout(&button_b.debaunce_timer, 3)) {return;}
  switch (display.mode)
  {
    case Display_selection::DISPLAY_led_mode:
    led_strip.next_effect();
    break;

    case Display_selection::DISPLAY_led_brigthnesss:
    led_strip.increase_brightness(10);
    break;
  }
  display.to_update = true;
}

void setup() {
  Serial.begin();

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_A, INPUT_PULLUP);
  pinMode(SWITCH_B, INPUT_PULLUP);

  button_a.pin = SWITCH_A;
  button_b.pin = SWITCH_B;

  lcd.init();
  lcd.setBacklight(150);

  attachInterrupt(button_a.pin, button_a_interrupt, FALLING);
  attachInterrupt(button_b.pin, button_b_interrupt, FALLING);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(led_strip.buffer, NUM_LEDS);
}

void loop() {
  led_strip.update();
  display.update();
}