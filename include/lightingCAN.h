#ifndef __LIGHTING_CAN_H__
#define __LIGHTING_CAN_H__

#include <Arduino.h>
#include "canmanager.h"

// Set BOARD define to the PCB this firmware builds for
#define L_FRONT_LIGHT 1
#define R_FRONT_LIGHT 2
#define L_SIDE_LIGHT 3
#define R_SIDE_LIGHT 4
#define L_REAR_LIGHT 5
#define R_REAR_PLATE 6
#define R_REAR_REVERSE 7

#define BOARD 3

#define BLINK_DELAY 400

// First define = led0, second define = led1
#if BOARD == L_FRONT_LIGHT
    #define LEFT_BLINK
    #define HEADLIGHT
#elif BOARD == R_FRONT_LIGHT
    #define RIGHT_BLINK
    #define HEADLIGHT
#elif BOARD == L_SIDE_LIGHT
    #define LEFT_BLINK
    #define BPS_FAULT
#elif BOARD == R_SIDE_LIGHT
    #define RIGHT_BLINK
    #define BRAKELIGHT
#elif BOARD == L_REAR_LIGHT
    #define LEFT_BLINK
    #define BRAKELIGHT
#elif BOARD == R_REAR_PLATE
    #define RIGHT_BLINK
    #define PLATE_ALWAYS_ON
#elif BOARD == R_REAR_REVERSE
    #define REVERSE
    #define BRAKELIGHT
#endif

// Role macros set CAN ID, bit offset, and blink flag for leds[0] / leds[1]
#ifdef LEFT_BLINK
    #define LED_ID_1 0x300
    #define BIT_OFF1 1
    #define BLINK1 false
#endif

#ifdef RIGHT_BLINK
    #define LED_ID_1 0x300
    #define BIT_OFF1 2
    #define BLINK1 false
#endif

#ifdef BPS_FAULT
    #define LED_ID_2 0x001
    #define BIT_OFF2 0
    #define BLINK2 true
#endif

#ifdef BRAKELIGHT
    #define LED_ID_2 0x207
    #define BIT_OFF2 5
    #define BLINK2 false
#endif

#ifdef HEADLIGHT
    #define LED_ID_2 0x300
    #define BIT_OFF2 0
    #define BLINK2 false
#endif

#ifdef REVERSE
    #define LED_ID_1 0x207
    #define BIT_OFF1 0
    #define BLINK1 false
#endif

#ifndef BLINK1
#define BLINK1 false
#endif
#ifndef BLINK2
#define BLINK2 false
#endif

typedef struct {
    bool on : 1;
    bool blink_val : 1;
    bool blinks : 1;
    uint32_t pin;
} LED;

extern volatile LED leds[2];

class LightingCAN : public CANManager {
    public:
        LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency = DEFAULT_CAN_FREQ);
        void readHandler(CAN_message_t msg);
        void updateOutputs();
        bool send();
};

#endif
