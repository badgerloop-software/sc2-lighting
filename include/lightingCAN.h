#ifndef __LIGHTING_CAN_H__
#define __LIGHTING_CAN_H__

#include <Arduino.h>
#include "canmanager.h"

#define L_FRONT_LIGHT 0
#define R_FRONT_LIGHT 1
#define L_SIDE_LIGHT 2
#define R_SIDE_LIGHT 3
#define MID_LIGHT 4
#define L_REAR_LIGHT 5
#define R_REAR_LIGHT 6

#define BOARD 0

#define BLINK_DELAY 400

#if BOARD == L_FRONT_LIGHT
    #define LEFT_BLINK
    #define HEADLIGHT
#elif BOARD == R_FRONT_LIGHT
    #define RIGHT_BLINK
    #define HEADLIGHT
#elif BOARD == L_SIDE_LIGHT
    #define LEFT_BLINK
    #define LED_ID_2 0x9999
    #define BIT_OFF2 0
    #define BLINK2 0
#elif BOARD == R_SIDE_LIGHT
    #define RIGHT_BLINK
    #define LED_ID_2 0x9999
    #define BIT_OFF2 0
    #define BLINK2 0
#elif BOARD == MID_LIGHT
    #define BPS_FAULT
    #define BRAKELIGHT
#elif BOARD == L_REAR_LIGHT
    #define LEFT_BLINK
    #define BRAKELIGHT
#elif BOARD == R_REAR_LIGHT
    #define RIGHT_BLINK
    #define BRAKELIGHT
#endif

#ifdef LEFT_BLINK
    #define LED_ID_1 0x300
    #define BIT_OFF1 1
    #define BLINK1 true
#endif

#ifdef RIGHT_BLINK
    #define LED_ID_1 0x300
    #define BIT_OFF1 2
    #define BLINK1 true
#endif

#ifdef BPS_FAULT
    #define LED_ID_1 0x100
    #define BLINK1 true
#endif

#ifdef BRAKELIGHT
    #define LED_ID_2 0x207
    #define BIT_OFF2 4
    #define BLINK2 false
#endif

#ifdef HEADLIGHT
    #define LED_ID_2 0x300
    #define BIT_OFF2 0
    #define BLINK2 0
#endif

typedef struct {
    bool on : 1;            // whether the light is on or off
    bool blink_val : 1;     // if the light is on, is it blinking on or off
    bool blinks : 1;        // whether the light is a blinking light or not
    uint32_t pin;
} LED; 

extern volatile LED leds[2];

class LightingCAN : public CANManager {
    public:
        LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency = DEFAULT_CAN_FREQ);
        void readHandler(CAN_message_t msg);
        bool send();
};

#endif