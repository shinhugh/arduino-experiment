#include "common.h"
#include <avr/io.h>
#include <util/delay.h>
#include "text_lcd.h"

// Delay after each instruction
#define DELAY_BUSY 5

// Offset for numbers in character table
#define CHAR_NUMERIC_OFFSET 0x30U

// --------------------------------------------------

// Backlight RGB PWM values (0 ~ 255)
volatile uint8_t pwm_rgb_red, pwm_rgb_green, pwm_rgb_blue;

// Backlight RGB transition values
uint8_t rgb_trans_on;
volatile uint8_t rgb_incr_color;
volatile uint8_t rgb_incr_color_val;

// --------------------------------------------------

void text_lcd_init() {

  // Initialize variables
  pwm_rgb_red = 0;
  pwm_rgb_green = 0;
  pwm_rgb_blue = 0;
  rgb_trans_on = 0;
  rgb_incr_color = 0;
  rgb_incr_color_val = PWM_MAX;

  // ----------------------------------------

  // Set pin 12 to output mode
  DDRB |= (1 << DDB4);
  // Set pin 11 to output mode
  DDRB |= (1 << DDB3);
  // Set pin 10 to output mode
  DDRB |= (1 << DDB2);
  // Set pin 9 to output mode
  DDRB |= (1 << DDB1);
  // Set pin 8 to output mode
  DDRB |= (1 << DDB0);
  // Set pin 7 to output mode
  DDRD |= (1 << DDD7);
  // Set pin 6 to output mode
  DDRD |= (1 << DDD6);
  // Set pin 5 to output mode
  DDRD |= (1 << DDD5);
  // Set pin 3 to output mode
  DDRD |= (1 << DDD3);

  // ----------------------------------------

  // Function set: 4-bit operation
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB1);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB &= ~(1 << PORTB1);

  // Function set: 4-bit operation, 2-line display, 5x8 character font
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB1);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB &= ~(1 << PORTB1);
  PORTB |= (1 << PORTB3);
  PORTD |= (1 << PORTD7);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTD &= ~(1 << PORTD7);

  // Display control: display on, cursor off, blinking off
  PORTB |= (1 << PORTB3);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB |= (1 << PORTB3);
  PORTD |= (1 << PORTD7);
  PORTB |= (1 << PORTB0);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);

  // Entry mode set: increment address, cursor shift right, no display shift
  PORTB |= (1 << PORTB3);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB0);
  PORTB |= (1 << PORTB1);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);

}

// --------------------------------------------------

void text_lcd_clear() {

  PORTB |= (1 << PORTB3);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB2);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB &= ~(1 << PORTB2);

}

// --------------------------------------------------

void text_lcd_write_char(uint8_t code) {

  // Enable: rising edge
  PORTB |= (1 << PORTB3);

  // Register select: data
  PORTB |= (1 << PORTB4);
  // d7
  if(0x80 & code) {
    PORTD |= (1 << PORTD7);
  } else {
    PORTD &= ~(1 << PORTD7);
  }
  // d6
  if(0x40 & code) {
    PORTB |= (1 << PORTB0);
  } else {
    PORTB &= ~(1 << PORTB0);
  }
  // d5
  if(0x20 & code) {
    PORTB |= (1 << PORTB1);
  } else {
    PORTB &= ~(1 << PORTB1);
  }
  // d4
  if(0x10 & code) {
    PORTB |= (1 << PORTB2);
  } else {
    PORTB &= ~(1 << PORTB2);
  }

  // Enable: falling edge
  PORTB &= ~(1 << PORTB3);

  // Delay
  _delay_ms(DELAY_BUSY);

  // Reset registers
  PORTB &= ~(1 << PORTB4);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

  // Enable: rising edge
  PORTB |= (1 << PORTB3);

  // Register select: data
  PORTB |= (1 << PORTB4);
  // d7
  if(0x08 & code) {
    PORTD |= (1 << PORTD7);
  } else {
    PORTD &= ~(1 << PORTD7);
  }
  // d6
  if(0x04 & code) {
    PORTB |= (1 << PORTB0);
  } else {
    PORTB &= ~(1 << PORTB0);
  }
  // d5
  if(0x02 & code) {
    PORTB |= (1 << PORTB1);
  } else {
    PORTB &= ~(1 << PORTB1);
  }
  // d4
  if(0x01 & code) {
    PORTB |= (1 << PORTB2);
  } else {
    PORTB &= ~(1 << PORTB2);
  }

  // Enable: falling edge
  PORTB &= ~(1 << PORTB3);

  // Delay
  _delay_ms(DELAY_BUSY);

  // Reset registers
  PORTB &= ~(1 << PORTB4);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

}

// --------------------------------------------------

void text_lcd_write_number(uint64_t number) {

  uint8_t i;

  uint8_t length = 0;
  uint64_t number_copy = number;
  while(number_copy > 0) {
    length++;
    number_copy /= 10;
  }
  length = length == 0 ? 1 : length;

  uint8_t curr_digit;
  uint64_t divisor = 1;
  for(i = 0; i < length - 1; i++) {
    divisor *= 10;
  }
  for(i = 0; i < length; i++) {
    curr_digit = (uint8_t) ((number / divisor) % 10);
    divisor /= 10;
    text_lcd_write_char(CHAR_NUMERIC_OFFSET + curr_digit);
  }

}

// --------------------------------------------------

void text_lcd_place_cursor(uint16_t row, uint16_t col) {

  uint8_t index = (0x40 * row) + col;

  // Enable: rising edge
  PORTB |= (1 << PORTB3);

  // Register select: instruction
  PORTB &= ~(1 << PORTB4);
  // d7
  PORTD |= (1 << PORTD7);
  // d6
  if(0x40 & index) {
    PORTB |= (1 << PORTB0);
  } else {
    PORTB &= ~(1 << PORTB0);
  }
  // d5
  if(0x20 & index) {
    PORTB |= (1 << PORTB1);
  } else {
    PORTB &= ~(1 << PORTB1);
  }
  // d4
  if(0x10 & index) {
    PORTB |= (1 << PORTB2);
  } else {
    PORTB &= ~(1 << PORTB2);
  }

  // Enable: falling edge
  PORTB &= ~(1 << PORTB3);

  // Delay
  _delay_ms(DELAY_BUSY);

  // Reset registers
  PORTB &= ~(1 << PORTB4);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

  // Enable: rising edge
  PORTB |= (1 << PORTB3);

  // Register select: instruction
  PORTB &= ~(1 << PORTB4);
  // d7
  if(0x08 & index) {
    PORTD |= (1 << PORTD7);
  } else {
    PORTD &= ~(1 << PORTD7);
  }
  // d6
  if(0x04 & index) {
    PORTB |= (1 << PORTB0);
  } else {
    PORTB &= ~(1 << PORTB0);
  }
  // d5
  if(0x02 & index) {
    PORTB |= (1 << PORTB1);
  } else {
    PORTB &= ~(1 << PORTB1);
  }
  // d4
  if(0x01 & index) {
    PORTB |= (1 << PORTB2);
  } else {
    PORTB &= ~(1 << PORTB2);
  }

  // Enable: falling edge
  PORTB &= ~(1 << PORTB3);

  // Delay
  _delay_ms(DELAY_BUSY);

  // Reset registers
  PORTB &= ~(1 << PORTB4);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

}

// --------------------------------------------------

void text_lcd_set_backlight_rgb(uint8_t red, uint8_t green, uint8_t blue) {

  pwm_rgb_red = red;
  pwm_rgb_green = green;
  pwm_rgb_blue = blue;

  OCR0A = pwm_rgb_red;
  OCR0B = pwm_rgb_green;
  OCR2B = pwm_rgb_blue;

}

// --------------------------------------------------

void text_lcd_backlight_rgb_trans() {

  if(!rgb_trans_on) {
    return;
  }

  if(rgb_incr_color == 0) {
    text_lcd_set_backlight_rgb(rgb_incr_color_val, 0,
    PWM_MAX - rgb_incr_color_val);
    if(rgb_incr_color_val == PWM_MAX) {
      rgb_incr_color = 1;
      rgb_incr_color_val = 1;
    } else {
      rgb_incr_color_val++;
    }
  }
  else if(rgb_incr_color == 1) {
    text_lcd_set_backlight_rgb(PWM_MAX - rgb_incr_color_val,
    rgb_incr_color_val, 0);
    if(rgb_incr_color_val == PWM_MAX) {
      rgb_incr_color = 2;
      rgb_incr_color_val = 1;
    } else {
      rgb_incr_color_val++;
    }
  }
  else if(rgb_incr_color == 2) {
    text_lcd_set_backlight_rgb(0, PWM_MAX - rgb_incr_color_val,
    rgb_incr_color_val);
    if(rgb_incr_color_val == PWM_MAX) {
      rgb_incr_color = 0;
      rgb_incr_color_val = 1;
    } else {
      rgb_incr_color_val++;
    }
  }

}

// --------------------------------------------------

void text_lcd_backlight_rgb_trans_on() {

  rgb_trans_on = 1;

}

// --------------------------------------------------

void text_lcd_backlight_rgb_trans_off() {

  rgb_trans_on = 0;

}
