#include <Arduino.h>
#include <FastLED.h>
#include "../include/led_mode.h"
#include "../include/timeout.h"

// rainbow declarations
int hue_step = 3 ;
int hue_change_speed = 50;
int rainbow_timer = 0;

// comet declarations
int led_step = 1;
int comet_speed = 50;
int fade_speed = 100;
int comet_timer = 0;
int comet_hue = 0;
int comet_hue_step = 5;
int comet_size = 5;

// police declarations
int police_speed = 200;
int police_timer = 0;
bool police_cycle = true;


void led_mode_solid_color(CRGB* LEDs, int NUM_LEDS, CRGB color)
{
    fill_solid(LEDs, NUM_LEDS, color);
    FastLED.show();
}

void led_mode_rainbow(CRGB* LEDs, int NUM_LEDS)
{
    if(!timeout(&rainbow_timer, hue_change_speed)) {return; }
    static int current_hue = 0;
    fill_rainbow(LEDs, NUM_LEDS, current_hue, hue_step);
    current_hue += hue_step;
    FastLED.show();
}

void led_mode_comet(CRGB* LEDs, int NUM_LEDS)
{
    if(!timeout(&comet_timer, comet_speed)) {return; }
    static int current_led = 0;
    if(current_led < 0 || (current_led + comet_size) > NUM_LEDS)
    {
        led_step *= -1;
    }
    // LEDs[current_led] = CRGB::Black;
    if(random(3) > 0) {fadeToBlackBy(LEDs, NUM_LEDS, fade_speed);}
    current_led += led_step;
    comet_hue += comet_hue_step;
    for(int i = 0; i < comet_size; i++)
    {
        LEDs[current_led+i].setHue(comet_hue);
    }
    FastLED.show();
}

void led_mode_police(CRGB* LEDs, int NUM_LEDS)
{
    if(!timeout(&police_timer, police_speed)) {return; }
    FastLED.clear();
    for(int i = 0; i < NUM_LEDS/2; i++)
    {
        if(police_cycle) {LEDs[i] = CRGB::Red;}
        else {LEDs[i + NUM_LEDS/2] = CRGB::Blue;}
    }
    police_cycle = !police_cycle;
    FastLED.show();
}