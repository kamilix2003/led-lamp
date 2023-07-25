#include <FastLED.h>

class Projectile
{
    private:
        int velocity_timer_ = 0;
        int acceleration_timer_ = 0;

    public:
        int ID_;
        int mass_;
        int position_;
        int velocity_;  //milli leds per second
        int velocity_frequency_;
        CRGB color_ = CRGB::White;
        int hue_ = 0;
        int hue_step_ = 0;

        Projectile(int, int, int, int, CRGB);
        Projectile(int, int, int, int, CRGB, int);
        void set_hue(int, int);
        void update(int);
        void print();
};