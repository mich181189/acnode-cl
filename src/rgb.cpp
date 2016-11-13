#include "rgb.h"

RGB::RGB(int pin_r, int pin_g, int pin_b) : colour1(BLACK), colour2(BLACK) {
  _r = pin_r;
  _g = pin_g;
  _b = pin_b;
  is_flashing = false;
  flashing_on = false;
}

void RGB::begin() {
  // Move this into the constructor?
  pinMode(_r, OUTPUT);
  pinMode(_g, OUTPUT);
  pinMode(_b, OUTPUT);
  run();
}

void RGB::run() {
  if (is_flashing) {
    unsigned long now = millis();
    if (now - last_toggled_time > FLASH_INTERVAL) {
      flashing_on ? light_up(colour1) : light_up(colour2);
      flashing_on = !flashing_on;
      last_toggled_time = now;
    }
  } else {
    light_up(colour1);
  }
}

void RGB::solid(Colour &c) {
  is_flashing = false;
  colour1 = c;
  run();
}

void RGB::flashing(Colour &c) {
  flashing(c, BLACK);
}

void RGB::flashing(Colour &c, Colour &d) {
  is_flashing = true;
  colour1 = c;
  colour2 = d;
  run();
}

void RGB::off() {
  solid(BLACK);
}

void RGB::light_up(Colour &c) {
  analogWrite(_r, c.r);
  analogWrite(_g, c.g);
  analogWrite(_b, c.b);
}
