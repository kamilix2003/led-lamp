#include <../include/brightness.h>

void check_min_max_brightness(int* current_brightness)
{
    if(*current_brightness > MAX_BRIGHTNESS) {*current_brightness = MAX_BRIGHTNESS;}
    if(*current_brightness < MIN_BRIGHTNESS) {*current_brightness = MIN_BRIGHTNESS;}
}
void increment_brightness(int* current_brightness, bool direction)
{
    if(direction) // increase
    {
        *current_brightness+=10;
    }
    else // decrease
    {
        *current_brightness-=10;
    }
    check_min_max_brightness(current_brightness);
}