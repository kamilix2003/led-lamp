#include <Arduino.h>
#include "../include/utility.h"

bool timeout(int* p_timer, int timeout_time)
{
  if(*p_timer == 0) 
  {
    *p_timer = millis();
    return true;
  }
  if((millis() - *p_timer) > timeout_time)
  {
    *p_timer = 0;
  }
  return false;
}

/// @brief takes pointer to int(enum) and cycles through it by one step. enum must start with `BEGIN` and end with `END`
/// @param enum_mode 
void cycle_enum(int* enum_mode)
{
  *enum_mode += 1;
  // led_mode = (Mode)((int)led_mode+1);
  // if(led_mode == Mode::END) {led_mode = (Mode)((int)Mode::BEGIN+1);}
}