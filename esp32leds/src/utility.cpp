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
