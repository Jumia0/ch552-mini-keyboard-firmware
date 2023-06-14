
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

unsigned char KEY_CODE[H_CNT * L_CNT] = {
  'a','b','c','d',  'e', \
  'f','g','h','i',  'j', \
  'k','l','m','n',  'o', \
  'p','q','r','s',  't'  \
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
    set_pixel_for_GRB_LED(ledData, i, 10, 10, 10); //Choose the color order depending on the LED you use. 
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
          Keyboard_press(KEY_CODE[i * L_CNT + j]);

          if (j < 4){
            set_pixel_for_GRB_LED(ledData, i * 4 + j, 100, 100, 100);
            neopixel_show_P1_5(ledData, NUM_BYTES);
          }
        } else {
          // button released
          Keyboard_release(KEY_CODE[i * L_CNT + j]);
          if (j < 4){
            set_pixel_for_GRB_LED(ledData, i * 4 + j, 10, 10, 10);
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
