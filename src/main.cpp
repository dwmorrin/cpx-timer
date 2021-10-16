#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN_NEO 8
#define N_PIXELS 10
#define BUTTON_A 4
#define BUTTON_B 5

enum DurationMS {
  sixteenth = 125,
  eighth = 250,
  eighth_triplet = 333,
  dotted_eighth = 375,
  quarter = 500,
  dotted_quarter = 750,
  half = 1000,
};

enum Interval {
  rest = -255,
  unison = 0,
  m2,
  M2,
  m3,
  M3,
  P4,
  aug4,
  P5,
  m6,
  M6,
  m7,
  M7,
  octave,
};

struct Note {
  int interval;
  int duration;
};

const int MELODY_LENGTH = 51;
struct Note melody[] = {
  {P5, quarter},
  {unison, dotted_quarter},
  {unison, eighth},
  {unison, sixteenth},
  {M2, sixteenth},
  {M3, sixteenth},
  {P4, sixteenth},
  {P5, half},
  {rest, eighth},
  {P5, eighth},
  {P5, eighth_triplet},
  {m6, eighth_triplet},
  {m7, eighth_triplet},
  {octave, half},
  {rest, eighth},
  {octave, eighth},
  {octave, sixteenth},
  {m7, sixteenth},
  {m6, eighth},
  {m7, dotted_eighth},
  {m6, sixteenth},
  {P5, half},
  {P5, quarter},
  {P4, eighth},
  {P4, sixteenth},
  {P5, sixteenth},
  {m6, half},
  {P5, eighth},
  {P4, eighth},
  {m3, eighth},
  {m3, sixteenth},
  {P4, sixteenth},
  {P5, half},
  {P4, eighth},
  {m3, eighth},
  {M2, eighth},
  {M2, sixteenth},
  {M3, sixteenth},
  {aug4, half},
  {M6, quarter},
  {P5, eighth},
  {P5, sixteenth},
  {P5, sixteenth},
  {P5, eighth},
  {P5, sixteenth},
  {P5, sixteenth},
  {P5, eighth},
  {P5, sixteenth},
  {P5, sixteenth},
  {P5, eighth},
  {P5, eighth},
};

// helper function to map integers to musical frequencies
uint32_t n2freq(uint8_t n) {
  // leading constant is the base frequency of which n is n steps away from result
  return uint32_t(220.0 * pow(2, n/12.0));
}

// global NeoPixel object to share across program
static Adafruit_NeoPixel *pixels;

uint32_t note2rgb(struct Note note) {
  uint8_t r = map(note.interval, 0, 12, 170, 0);
  uint8_t g = map(note.interval, 0, 12, 30, 170);
  uint8_t b = map(note.duration, 125, 1000, 30, 170);
  return pixels->Color(r, g, b);
}

uint16_t _random_pixel() {
  return random(0, N_PIXELS);
}

uint16_t random_pixel() {
  static uint16_t last_pixel = _random_pixel();
  uint16_t next_pixel = _random_pixel();
  while (next_pixel == last_pixel) {
    next_pixel = _random_pixel();
  }
  last_pixel = next_pixel;
  return next_pixel;
}

void play_melody_for_n_seconds(uint32_t s) {
  uint32_t ms = s * 1000;
  uint32_t start = millis();
  uint8_t melody_index = 0;
  // melody stops after s seconds or button B is pressed
  while (millis() - start < ms && !digitalRead(BUTTON_B)) {
    struct Note note = melody[melody_index];
    melody_index = (melody_index + 1) % MELODY_LENGTH;
    pixels->clear();
    if (note.interval > rest) {
      pixels->setPixelColor(random_pixel(), note2rgb(note));
      tone(PIN_A0, n2freq(note.interval), note.duration);
    }
    else {
      noTone(PIN_A0);
    }
    pixels->show();
    delay(note.duration);
  }
  noTone(PIN_A0);
  pixels->clear();
  pixels->show();
}

void play_melody_for_n_minutes(uint32_t m) {
  play_melody_for_n_seconds(m * 60);
}

void setup() {
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pixels = new Adafruit_NeoPixel(N_PIXELS, PIN_NEO, NEO_GRB + NEO_KHZ800);
  pixels->begin();
  pixels->clear();
  pixels->show();
}

void loop() {
  // debounce button A and play melody if pressed
  static int button_a_count = 0;
  if (digitalRead(BUTTON_A)) {
    ++button_a_count;
    if (button_a_count > 10) {
      button_a_count = 0;
      play_melody_for_n_minutes(2);
    }
  }
}