#include <FastLED.h>
#include <Arduino.h>

const int MAX_BRIGHTNESS = 255;
const int MIN_BRIGHTNESS = 0;

void check_min_max_brightness(int* current_brightness);
void increment_brightness(int* current_brightness, bool direction);
