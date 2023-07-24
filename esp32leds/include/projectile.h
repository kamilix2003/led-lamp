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
        int velocity_;  //leds per second
        int velocity_frequency_;
        CRGB color_ = CRGB::White;

        Projectile(int, int, int, int, CRGB);
        void update(int);
        void print();
};