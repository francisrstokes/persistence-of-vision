#include <avr/io.h>
#include <stdint-gcc.h>
#include <util/delay.h>

#define FRAME_DELAY     (32) // Milliseconds
#define KERNING_DELAY   (12) // Milliseconds
#define FRAMES_PER_CHAR (5)

// The contents of this array was generated by running the "font-to-array.py" script
const uint8_t letters[][5] = {
  { 0xff, 0x90, 0x90, 0x90, 0xff }, // A [0]
  { 0xff, 0x91, 0x91, 0x91, 0x6e }, // B [1]
  { 0xff, 0x81, 0x81, 0x81, 0x81 }, // C [2]
  { 0xff, 0x81, 0x81, 0x81, 0x7e }, // D [3]
  { 0xff, 0x91, 0x91, 0x91, 0x81 }, // E [4]
  { 0xff, 0x90, 0x90, 0x90, 0x80 }, // F [5]
  { 0x7e, 0x81, 0x89, 0x89, 0x4e }, // G [6]
  { 0xff, 0x10, 0x10, 0x10, 0xff }, // H [7]
  { 0x81, 0x81, 0xff, 0x81, 0x81 }, // I [8]
  { 0x81, 0x81, 0x81, 0x81, 0xfe }, // J [9]
  { 0xff, 0x18, 0x24, 0x42, 0x81 }, // K [10]
  { 0xff, 0x01, 0x01, 0x01, 0x01 }, // L [11]
  { 0xff, 0x40, 0x20, 0x40, 0xff }, // M [12]
  { 0xff, 0x60, 0x18, 0x06, 0xff }, // N [13]
  { 0x7e, 0x81, 0x81, 0x81, 0x7e }, // O [14]
  { 0xff, 0x90, 0x90, 0x90, 0x60 }, // P [15]
  { 0x7e, 0x81, 0x81, 0x82, 0x7d }, // Q [16]
  { 0xff, 0x98, 0x94, 0x92, 0x61 }, // R [17]
  { 0x61, 0x91, 0x91, 0x91, 0x8e }, // S [18]
  { 0x80, 0x80, 0xff, 0x80, 0x80 }, // T [19]
  { 0xfe, 0x01, 0x01, 0x01, 0xfe }, // U [20]
  { 0xfc, 0x02, 0x01, 0x02, 0xfc }, // V [21]
  { 0xff, 0x02, 0x04, 0x02, 0xff }, // W [22]
  { 0xc3, 0x2c, 0x10, 0x2c, 0xc3 }, // X [23]
  { 0xc0, 0x20, 0x1f, 0x20, 0xc0 }, // Y [24]
  { 0x83, 0x85, 0x89, 0x91, 0xe1 }, // Z [25]
  { 0x00, 0x00, 0x00, 0x00, 0x00 }, // " " [26]
};

uint8_t get_char_index(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a';
  } else if (c >= 'A' && c <= 'A') {
    return c - 'A';
  } else {
    return 26; // Space for everything else
  }
}

void show_message(const char* message) {
  uint8_t message_index = 0;

  while (message[message_index]) {
    uint8_t char_index = get_char_index(message[message_index]);
    for (uint8_t i = 0; i < FRAMES_PER_CHAR; i++) {
      PORTD = letters[char_index][i];
      _delay_ms(FRAME_DELAY);
    }

    message_index++;
    PORTD = 0;
    _delay_ms(KERNING_DELAY);
  }
}

int main (void) {
  // Disable the UART receiver and transmitter to avoid RX and TX being pulled high
  UCSR0B = 0;

  // This is designed to run on an arduino nano. Port D is the only port where all 8 signals are
  // broken out and usable. On the hardware I've added a jumper on PD0 so that it can still be easily
  // flashed over UART.
  DDRD |= 0xff; // The entire port is an output

  const char* message = "Low Byte Productions";

  while (1) {
    show_message(message);
    _delay_ms(2200);
  }
}