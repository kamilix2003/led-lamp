#include "../include/projectile.h"
#include "../include/timeout.h"

Projectile::Projectile(int ID, int mass, int pos, int vel, CRGB color)
{
    ID_ = ID;
    mass_ = mass;
    position_ = pos;
    velocity_ = vel;
    velocity_frequency_ = 100000/velocity_;
    color_ = color;
}

void Projectile::set_hue(int hue_inital, int hue_step)
{
    hue_ = hue_inital;
    hue_step_ = hue_step;
}

Projectile::Projectile(int ID, int mass, int pos, int vel, CRGB color, int hue_step)
{
    ID_ = ID;
    mass_ = mass;
    position_ = pos;
    velocity_ = vel;
    velocity_frequency_ = 100000/velocity_;
    color_ = color;
    set_hue(0, hue_step);
}

void Projectile::update(int NUM_LEDS)
{
    if(timeout(&velocity_timer_, velocity_frequency_))
    {
        if(signbit(velocity_)) {position_--;}
        else {position_++;}
        hue_ += hue_step_;
    }
    if(position_ > NUM_LEDS -1) 
        {
            position_ = NUM_LEDS - 1;
            velocity_ *= -1;
        }
        else if(position_ < 0)
        {
            position_ = 0;
            velocity_ *= -1;
    }
    velocity_frequency_ = abs(100000/velocity_);
}

void Projectile::print()
{
    Serial.printf("ID: %i; mass: %i; position: %i; velcity: %i\n",
     ID_, mass_, position_, velocity_);
}