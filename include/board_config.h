#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

// ------------- BOARD TYPE -------------
#define L_FRONT_LIGHT 1
#define R_FRONT_LIGHT 2
#define L_SIDE_LIGHT 3
#define R_SIDE_LIGHT 4
#define L_REAR_LIGHT 5
#define R_REAR_PLATE 6
#define R_REAR_REVERSE 7

#define BOARD 3

// ------------- TIMING -------------
#define BLINK_DELAY 400  // ms

// ------------- DEBUG -------------
// 0 off (not in flash) | 1 human
#define SC2_DEBUG 0

// ------------- LED PINS -------------
#define LED_PIN_1 PA0
#define LED_PIN_2 PA1

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

#ifdef LEFT_BLINK
#define BIT_OFF1 1
#define BLINK1 false
#endif

#ifdef RIGHT_BLINK
#define BIT_OFF1 2
#define BLINK1 false
#endif

#ifdef BPS_FAULT
#define BIT_OFF2 0
#define BLINK2 true
#endif

#ifdef BRAKELIGHT
#define BIT_OFF2 5
#define BLINK2 false
#endif

#ifdef HEADLIGHT
#define BIT_OFF2 0
#define BLINK2 false
#endif

#ifdef REVERSE
#define BIT_OFF1 0
#define BLINK1 false
#endif

#ifndef BLINK1
#define BLINK1 false
#endif
#ifndef BLINK2
#define BLINK2 false
#endif

#endif  // __BOARD_CONFIG_H__
