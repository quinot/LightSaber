#include <FastLED.h>


#define NUM_LEDS 30
#define DATA_PIN PIN_PB3
#define POT_PIN A1
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 10

CRGB leds[NUM_LEDS];
const CRGB colors[] = { CRGB::Red, CRGB::Green, CRGB::Blue };
const int NUM_COLORS = sizeof colors / sizeof(CRGB);

void setup() {
  pinMode(POT_PIN, INPUT);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  for (int j = 0; j < NUM_LEDS; j++)
    leds[j] = colors[j / (NUM_LEDS / NUM_COLORS)];
  FastLED.show();
  FastLED.delay(2000);
  for (int j = 0; j < NUM_LEDS; j++)
     leds[j] = CRGB::Black;
}

#define WAVE_WIDTH (NUM_LEDS / 4)
#define CYCLE_LENGTH (WAVE_WIDTH + NUM_LEDS)

int curr_step = 0;
int curr_led = 0;
int curr_color = 0;

void loop() {
  int hue = analogRead(A1) / 4;
  CHSV dim_hue = CHSV(hue, 255, 128);
  CHSV bright_hue = CHSV(hue, 255, 255);
  CRGB wave[WAVE_WIDTH];
  int wave_offset, first_led_in_wave, wave_j;
  
  fill_solid(leds, NUM_LEDS, dim_hue);
  fill_gradient(wave, WAVE_WIDTH / 2, dim_hue, bright_hue);
  fill_gradient(wave + WAVE_WIDTH / 2, WAVE_WIDTH - WAVE_WIDTH / 2, bright_hue, dim_hue);
  
  wave_offset = (curr_step < WAVE_WIDTH) ? WAVE_WIDTH - curr_step : 0;
  first_led_in_wave = curr_step - WAVE_WIDTH + wave_offset;
  
  for (wave_j = wave_offset; wave_j < min(WAVE_WIDTH, NUM_LEDS - first_led_in_wave); wave_j++) {
     leds[first_led_in_wave + wave_j - wave_offset] = wave[wave_j];
  }
  
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  curr_step = (curr_step + 1) % CYCLE_LENGTH;
}
