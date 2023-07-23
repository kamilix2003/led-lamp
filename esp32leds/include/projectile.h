#include <FastLED.h>

class Projectile
{
    public:
        int ID_;
        int mass_;
        int position_;
        int velocity_;
        int acceleration_;
        CRGB color_ = CRGB::White;

        Projectile(int, int, int, int, int);
        Projectile(int, int, int, int, int, CRGB);
        void update();
        void update(int);
        void print();
};