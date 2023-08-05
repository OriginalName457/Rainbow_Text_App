#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "okcolor.hpp"
#include "galactic_unicorn.hpp"

using namespace pimoroni;

PicoGraphics_PenRGB888 visuals(53, 11, nullptr);
GalacticUnicorn cosmic_unicorn;

void convert_hsv(float hue, float saturation, float value, uint8_t &red, uint8_t &green, uint8_t &blue) {
  float index = floor(hue * 6.0f);
  float fraction = hue * 6.0f - index;
  value *= 255.0f;
  uint8_t p = value * (1.0f - saturation);
  uint8_t q = value * (1.0f - fraction * saturation);
  uint8_t t = value * (1.0f - (1.0f - fraction) * saturation);

  switch (int(index) % 6) {
    case 0: red = value; green = t; blue = p; break;
    case 1: red = q; green = value; blue = p; break;
    case 2: red = p; green = value; blue = t; break;
    case 3: red = p; green = q; blue = value; break;
    case 4: red = t; green = p; blue = value; break;
    case 5: red = value; green = p; blue = q; break;
  }
}

void draw_text(std::string textInput, Point pos, float scale = 1.0f, float alpha = 1.0f) {
  int width = visuals.measure_text(textInput, scale);
  pos.x += (53 / 2) - (width / 2);
  pos.y += (11 / 2);
  visuals.text(textInput, Point(pos.x, pos.y), -1, scale, alpha);
  visuals.text(textInput, Point(pos.x + 1, pos.y), -1, scale, alpha);
  visuals.text(textInput, Point(pos.x + 1, pos.y + 1), -1, scale, alpha);
  visuals.text(textInput, Point(pos.x, pos.y + 1), -1, scale, alpha);
}

int main() {
  cosmic_unicorn.init();
  visuals.set_font("sans");

  uint8_t color_map[53][3];
  for(int index = 0; index < 53; index++) {
    convert_hsv(index / 53.0f, 1.0f, 0.1f, color_map[index][0], color_map[index][1], color_map[index][2]);
  }

  uint counter = 0;

  while(true) {
 
    if(cosmic_unicorn.is_pressed(cosmic_unicorn.SWITCH_BRIGHTNESS_UP)) {
      cosmic_unicorn.adjust_brightness(+0.01);
    }
    if(cosmic_unicorn.is_pressed(cosmic_unicorn.SWITCH_BRIGHTNESS_DOWN)) {
      cosmic_unicorn.adjust_brightness(-0.01);
    }

    counter++;
    visuals.set_pen(0, 0, 0);
    visuals.clear();

    float scale = 0.8f;
    float alpha = 1.0f;

    float xCoord = (sin((counter) / 50.0f) * 90.0f);
    float yCoord = (cos((counter) / 40.0f) * 5.0f);
    visuals.set_pen(255, 255, 255);
    draw_text("Galactic Unicorn", Point(xCoord, yCoord), scale, alpha);

    uint8_t *bufferPtr = (uint8_t *)visuals.frame_buffer;
    for(size_t i = 0; i < 53 * 11; i++) {
      int xPos = i % 53;
      int yPos = i / 53;
      uint r = *bufferPtr++;
      uint g = *bufferPtr++;
      uint b = *bufferPtr++;
      bufferPtr++;

      if(r > 0) {
        r = color_map[xPos][0];
        g = color_map[xPos][1];
        b = color_map[xPos][2]; 
      }

      visuals.set_pen(r, g, b);
      visuals.pixel(Point(xPos, yPos));
    }
  }

  printf("done\n");

  return 0;
}
