#include "life_types.h"
#include <FastLED.h>

#define NUM_LEDS 30
#define ANIMATION_FRAMES 64
#define DATA_PIN 6
CHSV born_color = CHSV(64, 255, 200);
CHSV alive_color = CHSV(96, 255, 255);
CHSV died_color = CHSV(160, 200, 100);
CHSV dead_color = CHSV(0, 0, 0); //CRGB::Black;
CHSV extinction_color = CHSV(0, 255, 255);

CRGB leds[NUM_LEDS];
CRGB old_colors[NUM_LEDS];
CRGB new_colors[NUM_LEDS];


void setup() {
  Serial.begin(9600);

  //chosen by fair die roll
  //guaranteed to be random
  randomSeed(analogRead(9));
  create_life(cur_gen, CELLS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    old_colors[i] = 0;
  }
  memset(last_patterns, 0UL, sizeof(last_patterns));
}

void loop() {
  bool extinction = false;
  for (int i = 0; i < CELLS; i++) {
    next_gen[i] = update_cell(i, cur_gen, CELLS);
  }

  if (check_pattern(next_gen, CELLS)) {
    create_life(next_gen, CELLS);
    memset(last_patterns, 0UL, sizeof(last_patterns));
    extinction = true;
  }
  
  for (int i = 0; i < CELLS; i++) {
    cur_gen[i] = next_gen[i];
  }
  
  for (int i = 0; i < CELLS; i++) {
    CRGB new_color;
    if (extinction) {
      new_color = extinction_color;
    } else if (cur_gen[i] == born) {
      new_color = born_color;
    } else if (cur_gen[i] == alive) {
      new_color = alive_color;
    } else if (cur_gen[i] == died) {
      new_color = died_color;
    } else if (cur_gen[i] == dead) {
      new_color = dead_color;
    }
    new_colors[i] = new_color;
  }
  for (int i = 0; i < ANIMATION_FRAMES; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j] = blend(old_colors[j], new_colors[j], 4 * i);
    }
    //Serial.println();
    delay(5);
    FastLED.show();
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    old_colors[i] = new_colors[i];
    leds[i] = new_colors[i];
  }

  FastLED.show();
  delay(500);
}



