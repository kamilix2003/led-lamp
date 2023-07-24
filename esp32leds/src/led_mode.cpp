#include <Arduino.h>
#include <FastLED.h>
#include "../include/led_mode.h"
#include "../include/timeout.h"
#include "../include/projectile.h"

void led_mode_solid_color(CRGB* LEDs, int NUM_LEDS, CRGB color)
{
    fill_solid(LEDs, NUM_LEDS, color);
}

// rainbow declarations
int hue_step = 3 ;
int hue_change_speed = 50;
int rainbow_timer = 0;

void led_mode_rainbow(CRGB* LEDs, int NUM_LEDS)
{
    if(!timeout(&rainbow_timer, hue_change_speed)) {return; }
    static int current_hue = 0;
    fill_rainbow(LEDs, NUM_LEDS, current_hue, hue_step);
    current_hue += hue_step;
}

// comet declarations
int led_step = 1;
int comet_speed = 50;
int fade_speed = 100;
int comet_timer = 0;
int comet_hue = 0;
int comet_hue_step = 5;
int comet_size = 5;

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
}

// police declarations
int police_speed = 200;
int police_timer = 0;
bool police_cycle = true;

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
}

// test declarations
const int PROJECTILES_NUMBER = 2;
Projectile projectiles[PROJECTILES_NUMBER] =
    {Projectile(1, 1, 10, 60, CRGB::Blue),
     Projectile(2, 1, 50, 30, CRGB::Red)};

void collision(Projectile projectiles[], int PROJECTILES_NUMBER)
{
    if(projectiles[0].position_ == projectiles[1].position_)
    {
        projectiles[0].velocity_ *= -1;
        projectiles[1].velocity_ *= -1;
    }
}

void led_mode_test(CRGB* LEDs, int NUM_LEDS)
{
    fadeToBlackBy(LEDs, NUM_LEDS, 50);
    // FastLED.clear();
    for(int i = 0; i < PROJECTILES_NUMBER; i++)
    {
        collision(projectiles, PROJECTILES_NUMBER);
        projectiles[i].update(NUM_LEDS);
        LEDs[projectiles[i].position_] = projectiles[i].color_;
        // projectiles[i].print();
    }
    // FastLED.show();
}