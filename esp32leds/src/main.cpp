#include <Arduino.h>
#include <FastLED.h>
#include <math.h>
#include <WiFi.h>

#include "../include/led_mode.h"
#include "../include/timeout.h"

#define BUILTIN_LED 8
#define NUM_LEDS 60 // Number of leds
#define LED_PIN 5 // Data pin for led strip

#define SWITCH_A 9
#define SWITCH_B 10
#define SWITCH_C 4
#define SWITCH_D 3

// fastled declarations
CRGB g_LEDs[NUM_LEDS] = {0}; // Frame buffer for FastLED
int g_brightness = 180;
const int MAX_BRIGHTNESS = 220;
const int MIN_BRIGHTNESS = 0;
int g_maxpower = 1200;
int g_maxmilliamps = 1500;
int g_maxvolt = 5;

// brightness declarations
bool brightness_increase = false;
bool brightness_direction_switch = false;

// mode declarations
Mode led_mode = Mode::led_on;
Mode led_mode_previous;
std::string led_mode_names[3] = {
  "leds white",
  "leds rainbow",
  "leds off",
};
bool led_mode_switch = false;

// WiFi declarations
char ssid[] = "*^____^*";
char password[] = "Kolej12!";
WiFiServer server(80);

// timeout
int a_timer = 0;
int b_timer = 0;
int c_timer = 0;
int d_timer = 0;

void setup() {

  // pins setup
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_A, INPUT_PULLUP);
  pinMode(SWITCH_B, INPUT_PULLUP);
  pinMode(SWITCH_C, INPUT_PULLUP);
  pinMode(SWITCH_D, INPUT_PULLUP);
  
  // serial communication setup
  Serial.begin(115200);

  // fastled setup
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);
  FastLED.setBrightness(g_brightness);
  // FastLED.setMaxPowerInMilliWatts(g_maxpower);
  FastLED.setMaxPowerInVoltsAndMilliamps(g_maxvolt, g_maxmilliamps);

  // WiFi
  // while(WiFi.status() != WL_CONNECTED)
  // {
  //   WiFi.begin(ssid, password);
  //   Serial.printf("connecting to %s\n", ssid);
  //   Serial.println(WiFi.status());
  //   delay(1000);
  // }
  // server.begin();
}

void loop() {
  // listen for incoming clients
  // WiFiClient client = server.available();
  // if (client) {

  //   if (client.connected()) {
  //     Serial.println("Connected to client");
  //   }

  //   // close the connection:
  //   client.stop();
  // }
  // switch A handling
  if(digitalRead(SWITCH_A) == LOW && timeout(&a_timer, 200)) 
  {
    if(led_mode == Mode::led_off) {led_mode = led_mode_previous;}
    else 
    {
      led_mode_previous = led_mode;
      led_mode = Mode::led_off;
    }
  }
  
  // switch B handling
  if(digitalRead(SWITCH_B) == LOW && timeout(&b_timer, 200)) 
  {
    led_mode = (Mode)((int)led_mode+1);
    if(led_mode == Mode::END) {led_mode = (Mode)((int)Mode::BEGIN+2);}
  }

  // switch C handling
  if(digitalRead(SWITCH_C) == LOW && timeout(&c_timer, 10))
  {
    if(g_brightness < MAX_BRIGHTNESS && brightness_increase)
    {
      if(g_brightness < 2) {g_brightness+1;}
      g_brightness += log(g_brightness+1);
      FastLED.setBrightness(g_brightness);
    }
    if(g_brightness > MIN_BRIGHTNESS && !brightness_increase)
    {
      g_brightness -= log(g_brightness-1);
      FastLED.setBrightness(g_brightness);
    }
    brightness_direction_switch = true;
    Serial.printf("brightness: %i\n", FastLED.getBrightness());
  }
  if(digitalRead(SWITCH_C) == HIGH && brightness_direction_switch)
  {
    brightness_increase = !brightness_increase;
    if(brightness_increase) {digitalWrite(BUILTIN_LED, HIGH);}
    else {digitalWrite(BUILTIN_LED, LOW);}
    brightness_direction_switch = false;
  }
  // switch D handling
  if(digitalRead(SWITCH_D) == LOW && timeout(&b_timer, 200)) 
  {
    Serial.println(WiFi.localIP());
  }


  // led update
  switch (led_mode)
  {
    case led_off:
      FastLED.clear(true);
      break;
    case led_on:
      led_mode_solid_color(g_LEDs, NUM_LEDS, CRGB::White);
      break;
    case led_rainbow:
      led_mode_rainbow(g_LEDs, NUM_LEDS);
      break;
    case led_comet:
      led_mode_comet(g_LEDs, NUM_LEDS);
      break;
    case led_police:
      led_mode_police(g_LEDs, NUM_LEDS);
      break;
    default:
      led_mode_solid_color(g_LEDs, NUM_LEDS, CRGB::White);
      break;
  }
}