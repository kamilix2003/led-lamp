#include <Arduino.h>
#include <FastLED.h>
#include "../include/led_mode.h"
#include "../include/utility.h"
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
int comet_size = 1;

int led_mode_comet(CRGB* LEDs, int NUM_LEDS)
{
    static int current_led = 0;
    if(!timeout(&comet_timer, comet_speed)) {return current_led; }
    if(current_led < 0 || (current_led + comet_size + 1) > NUM_LEDS)
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
    return current_led;
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

// // test declarations
// const int PROJECTILES_NUMBER = 3;
// Projectile projectiles[PROJECTILES_NUMBER] =
//     {Projectile(1, 1, 10, 2000, CRGB::Blue),
//      Projectile(2, 2, 20, 500, CRGB::Red),
//      Projectile(3, 4, 30, 1000, CRGB::Green)};
// int print_timer = 0;
// int print_frequency = 500;
// int collision_timer = 0;

// // bool collision_check(Projectile projectiles[], int PROJECTILES_NUMBER)
// // {
// //     if()
// // }

// int max_velocity(Projectile projectiles[], int PROJECTILES_NUMBER)
// {
//     int max_vel = 0;
//     for(int i = 0; i < PROJECTILES_NUMBER; i++)
//     {
//         if(max_vel < projectiles[i].velocity_) {max_vel = projectiles[i].velocity_;}
//     }
//     return max_vel;
// }

// void collision(Projectile projectiles[], int PROJECTILES_NUMBER)
// {
//     for(int i = 0; i < PROJECTILES_NUMBER; i++)
//     {
//         for(int j = 0; j < PROJECTILES_NUMBER; j++)
//         {
//             if((projectiles[i].position_ - projectiles[j].position_)
//             *(projectiles[i].position_ - projectiles[j].position_) == 1
//             && timeout(&collision_timer, max_velocity(projectiles, PROJECTILES_NUMBER)))
//             {
//                 // Serial.println("collision!");
//                 int initial_velocity_0 = projectiles[i].velocity_;
//                 int initial_velocity_1 = projectiles[j].velocity_;
//                 int mass_sum = projectiles[i].mass_ + projectiles[j].mass_;
//                 // projectiles[i].velocity_ *= -1;
//                 // projectiles[j].velocity_ *= -1;
//                 projectiles[i].velocity_ = 
//                     initial_velocity_0*(projectiles[i].mass_ - projectiles[j].mass_)/mass_sum
//                     + initial_velocity_1*2*projectiles[j].mass_/mass_sum;
//                 projectiles[j].velocity_ = 
//                     initial_velocity_0*2*projectiles[i].mass_/mass_sum
//                     + initial_velocity_1*(projectiles[j].mass_ - projectiles[i].mass_)/mass_sum;
//             }
//         }
//     }
// }

// void led_mode_test(CRGB* LEDs, int NUM_LEDS)
// {
//     fadeToBlackBy(LEDs, NUM_LEDS, 10);
//     // FastLED.clear();
//     for(int i = 0; i < PROJECTILES_NUMBER; i++)
//     {
//         collision(projectiles, PROJECTILES_NUMBER);
//         projectiles[i].update(NUM_LEDS);
//         if(projectiles[i].hue_step_ == 0) {LEDs[projectiles[i].position_] = projectiles[i].color_;}
//         else {LEDs[projectiles[i].position_].setHue(projectiles[i].hue_);}
//         // projectiles[i].print();
//     }
//     // if(timeout(&print_timer, print_frequency)) 
//     // {
//     //     projectiles[0].print();
//     //     projectiles[1].print();
//     //     Serial.println("");
//     // }
//     // FastLED.show();
// }