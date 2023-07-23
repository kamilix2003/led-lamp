#include "../include/projectile.h"

Projectile::Projectile(int ID, int mass, int pos, int vel, int acc)
{
    ID_ = ID;
    mass_ = mass;
    position_ = pos;
    velocity_ = vel;
    acceleration_ = acc;
}

Projectile::Projectile(int ID, int mass, int pos, int vel, int acc, CRGB color)
{
    ID_ = ID;
    mass_ = mass;
    position_ = pos;
    velocity_ = vel;
    acceleration_ = acc;
    color_ = color;
}

void Projectile::update()
{
    position_ += velocity_;
    velocity_ += acceleration_;
}

void Projectile::update(int NUM_LEDS)
{
    if(position_ + velocity_ > NUM_LEDS || 
    position_ + velocity_ < 0)
    {velocity_ *= -1;}
    update();
}

void Projectile::print()
{
    Serial.printf("ID: %i; mass: %i; position: %i; velcity: %i; acceleration: %i\n",
     ID_, mass_, position_, velocity_, acceleration_);
}