#include "../include/projectile.h"
#include "../include/timeout.h"

Projectile::Projectile(int ID, int mass, int pos, int vel, CRGB color)
{
    ID_ = ID;
    mass_ = mass;
    position_ = pos;
    velocity_ = vel;
    velocity_frequency_ = 1000/velocity_;
    color_ = color;
}

void Projectile::update(int NUM_LEDS)
{
    if(timeout(&velocity_timer_, velocity_frequency_))
    {
        if(signbit(velocity_)) {position_--;}
        else {position_++;}
        if(position_ + 1 > NUM_LEDS || position_ < 1) {velocity_ *= -1;}
    }
    velocity_frequency_ = abs(1000/velocity_);
}

void Projectile::print()
{
    Serial.printf("ID: %i; mass: %i; position: %i; velcity: %i\n",
     ID_, mass_, position_, velocity_);
}