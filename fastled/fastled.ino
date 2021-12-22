#include <FastLED.h>

const int DATA_PIN = 2;
const int LEDS_AMOUNT = 12;
const int DELAY = 400;

CRGB leds_data[LEDS_AMOUNT];
CRGB leds[LEDS_AMOUNT];
bool state[LEDS_AMOUNT];
int current = 0;

void leds_read()
{
  while (Serial.available() >= 3)
  {
    char r = Serial.read();
    char g = Serial.read();
    char b = Serial.read();
    leds_data[current++] = CRGB(b, g, r);
    Serial.print(current);
    if (current == LEDS_AMOUNT)
    {
      current = 0;
    }
  }
}

void leds_activate(int cycle)
{
  for (int i = 0; i < LEDS_AMOUNT; i++)
  {
    leds_data[i] = CRGB::Black;
  }
  for (int i = 0; i < LEDS_AMOUNT; i += cycle)
  {
    leds[i] = leds_data[i];
    state[i] = true;
  }
}

void leds_next()
{
  bool turn = state[LEDS_AMOUNT - 1];
  for (int i = 0; i < LEDS_AMOUNT; i++)
  {
    if (turn)
    {
      state[i] = true;
      leds[i] = leds_data[i];
      turn = false;
    }
    else
    {
      leds[i] = CRGB::Black;
      if (state[i])
      {
        turn = true;
        state[i] = false;
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, LEDS_AMOUNT);
  leds_activate(3);
}

void loop() {
  leds_read();
  leds_next();
  FastLED.show();
  delay(DELAY);
}
