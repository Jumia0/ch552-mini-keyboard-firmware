
//  cli board options: usb_settings=user148


//For windows user, if you ever played with other HID device with the same PID C55D
//You may need to uninstall the previous driver completely        


#ifndef USER_USB_RAM
#error "This example needs to be compiled with a USER USB setting"
#endif

#include "src/userUsbHidKeyboard/USBHIDKeyboard.h"


#define H0_PIN 33
#define H1_PIN 16
#define H2_PIN 17
#define H3_PIN 31

#define L0_PIN 11
#define L1_PIN 34
#define L2_PIN 14
#define L3_PIN 32
#define L4_PIN 30

#define H_CNT 4
uint8_t H_PINS[H_CNT] = {H0_PIN, H1_PIN, H2_PIN, H3_PIN};

#define L_CNT 5
uint8_t L_PINS[L_CNT] = {L0_PIN, L1_PIN, L2_PIN, L3_PIN, L4_PIN};

#define FUNC_COUNT 4
#define FUNC_L 0
#define FUNC_H 0

unsigned char FUNC_COLOR[FUNC_COUNT][3] = {
  {80,0,0},
  {0,80,0},
  {80,0,80},
  {0,0,80}
};

unsigned char FUNC_COLOR_HIGHLIGHT[FUNC_COUNT][3] = {
  {255,0,0},
  {0,255,0},
  {255,0,255},
  {0,0,255}
};

unsigned char KEY_CODE[FUNC_COUNT][H_CNT * L_CNT][8] = {
  // func layer 0
  {
    {0},                                     {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, KEY_END},                                      {1, KEY_PAGE_UP},                         {1, KEY_PAGE_DOWN},                       {1, KEY_ESC}, \
    {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'a'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'b'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'c'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'd'},  {1, KEY_TAB}, \
    {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, '1'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, '2'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, '3'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, '4'},  {1, ' '}, \
    {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, '5'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, '6'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, '7'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, '8'},  {1, KEY_RETURN}, \
  },
  // func layer 1
  {
    {0},      {1, '0'}, {2, KEY_RIGHT_SHIFT, KEY_TAB},  {1, KEY_TAB},         {1, KEY_ESC}, \
    {1, '1'}, {1, '2'}, {1, '3'},                       {1, '?'},             {1, KEY_TAB}, \
    {1, '4'}, {1, '5'}, {1, '6'},                       {1, KEY_BACKSPACE},   {1, ' '}, \
    {1, '7'}, {1, '8'}, {1, '9'},                       {1, '-'},             {1, KEY_RETURN}, \
  },
  // func layer 2
  {
    {0},                                     {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, '0'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'e'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'f'},  {1, KEY_ESC}, \
    {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'g'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'h'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'i'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'j'},  {1, KEY_TAB}, \
    {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'k'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'l'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'm'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'n'},  {1, ' '}, \
    {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'o'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'p'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'q'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'r'},  {1, KEY_RETURN}, \
  },
  // func layer 3
  {
    {0},                                     {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'A'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'B'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'C'},  {1, KEY_ESC}, \
    {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'D'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'E'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'F'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'G'},  {1, KEY_TAB}, \
    {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'H'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'I'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'J'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'K'},  {1, ' '}, \
    {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'L'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'M'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'N'}, {3, KEY_RIGHT_CTRL, KEY_RIGHT_ALT, 'O'},  {1, KEY_RETURN}, \
  },
};

bool btn_state[H_CNT * L_CNT];

// ws2812 led

#include <WS2812.h>

#define NUM_LEDS 16
#define COLOR_PER_LEDS 3
#define NUM_BYTES (NUM_LEDS * COLOR_PER_LEDS)
#define RGB_PIN 15
__xdata uint8_t ledData[NUM_BYTES];

#if NUM_BYTES > 255
#error "NUM_BYTES can not be larger than 255."
#endif

unsigned char func = 0;


void setup() {
  // put your setup code here, to run once:

  // usb init
  USBInit();

  // set input pin Lx
  for (uint8_t i=0; i<L_CNT; i++) {
    pinMode(L_PINS[i], INPUT_PULLUP);
  }
  
  // set output pin Hx
  for (uint8_t i=0; i<H_CNT; i++) {
    pinMode(H_PINS[i], OUTPUT);
  }

  // set init state high
  for (uint8_t i=0; i<H_CNT; i++) {
    digitalWrite(H_PINS[i], HIGH);
  }

  for (uint8_t i=0; i<L_CNT; i++) {
    for (uint8_t j=0; j<H_CNT; j++) {
      btn_state[i * H_CNT + j] = 0;
    }    
  }

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    // set_pixel_for_GRB_LED(ledData, i, 10, 10, 10); //Choose the color order depending on the LED you use. 
    set_pixel_for_GRB_LED(ledData, i, FUNC_COLOR[func][0], FUNC_COLOR[func][1], FUNC_COLOR[func][2]);
    neopixel_show_P1_5(ledData, NUM_BYTES); //Possible to use other pins. 
    delay(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // set init state high
  for (uint8_t i=0; i<H_CNT; i++) {
    
    digitalWrite(H_PINS[i], LOW);

    for (uint8_t j=0; j<L_CNT; j++) {
      bool btn_pressed = !digitalRead(L_PINS[j]);
      if (btn_state[i * L_CNT + j] != btn_pressed) {
        if (btn_pressed) {
          // button pressed
          if (i==FUNC_H && j==FUNC_L) {
            // functinon key pressed

            // release all key if pressed
            for (uint8_t k=0; k < H_CNT * L_CNT; k++ ) {
              if (btn_state[k]) {
                if (KEY_CODE[func][k][0] == '\x01') {
                  Keyboard_release(KEY_CODE[func][k][1]);
                }
                btn_state[k] = 0;
              }
            }

            func = (func+1) % FUNC_COUNT;
            for (uint8_t i = 0; i < NUM_LEDS; i++) {
              // set_pixel_for_GRB_LED(ledData, i, 10, 10, 10); //Choose the color order depending on the LED you use. 
              set_pixel_for_GRB_LED(ledData, i, FUNC_COLOR[func][0], FUNC_COLOR[func][1], FUNC_COLOR[func][2]);
              neopixel_show_P1_5(ledData, NUM_BYTES); //Possible to use other pins. 
              delay(1);
            }
            
          } else {
            // if (KEY_CODE[func][i * L_CNT + j][0] == '\x01') {
              // Keyboard_press(KEY_CODE[func][i * L_CNT + j][1]);
            // } else if (KEY_CODE[func][i * L_CNT + j][0] <= 7) {
              for (uint8_t k=1; k<=KEY_CODE[func][i * L_CNT + j][0]; k++) {
                Keyboard_press(KEY_CODE[func][i * L_CNT + j][k]);
                delay(1);
              // }
            }
          }

          if (j < 4){
            set_pixel_for_GRB_LED(ledData, i * 4 + j, FUNC_COLOR_HIGHLIGHT[func][0], FUNC_COLOR_HIGHLIGHT[func][1], FUNC_COLOR_HIGHLIGHT[func][2]);
            neopixel_show_P1_5(ledData, NUM_BYTES);
          }
        } else {
          // button released
          if (i==FUNC_H && j==FUNC_L) {
            // functinon key pressed
            // func = (func+1) % FUNC_COUNT;
          } else {
            // if (KEY_CODE[func][i * L_CNT + j][0] == '\x01') {
              // Keyboard_release(KEY_CODE[func][i * L_CNT + j][1]);
            // } else if (KEY_CODE[func][i * L_CNT + j][0] <= 7) {
              for (uint8_t k=KEY_CODE[func][i * L_CNT + j][0]; k>=1; k--) {
                Keyboard_release(KEY_CODE[func][i * L_CNT + j][k]);
                delay(1);
              }
            // }
          }
          if (j < 4){
            set_pixel_for_GRB_LED(ledData, i * 4 + j, FUNC_COLOR[func][0], FUNC_COLOR[func][1], FUNC_COLOR[func][2]);
            neopixel_show_P1_5(ledData, NUM_BYTES);
          }
        }
      }
      btn_state[i * L_CNT + j] = btn_pressed;
    }
    digitalWrite(H_PINS[i], HIGH);
    delay(1);
  }

//  for (uint8_t i = 0; i < NUM_LEDS; i++) {
//    set_pixel_for_GRB_LED(ledData, i, 1, 0, 0); //Choose the color order depending on the LED you use. 
//    neopixel_show_P1_5(ledData, NUM_BYTES); //Possible to use other pins. 
//    delay(100);
//  }
//  for (uint8_t i = 0; i < NUM_LEDS; i++) {
//    set_pixel_for_GRB_LED(ledData, i, 0, 1, 0);
//    neopixel_show_P1_5(ledData, NUM_BYTES);
//    delay(100);
//  }
//  for (uint8_t i = 0; i < NUM_LEDS; i++) {
//    set_pixel_for_GRB_LED(ledData, i, 0, 0, 1);
//    neopixel_show_P1_5(ledData, NUM_BYTES);
//    delay(100);
//  }

}


