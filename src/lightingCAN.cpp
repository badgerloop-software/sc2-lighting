#include "lightingCAN.h"

#define L_FRONT_LIGHT 0
#define R_FRONT_LIGHT 1
#define L_SIDE_LIGHT 2
#define R_SIDE_LIGHT 3
#define MID_LIGHT 4
#define L_REAR_LIGHT 5
#define R_REAR_LIGHT 6

#define BOARD 0

#if BOARD == L_FRONT_LIGHT
#define LED_ID_1 0x300
#define BIT_OFF 1
#define BLINK1 1
#define LED_ID_2 0x0//Headlight ID
#define BLINK2 0
#elif BOARD == R_FRONT_LIGHT
#define LED_ID_1 0x300
#define BIT_OFF 2
#define BLINK1 1
#define LED_ID_2 0x0//Headlight ID
#define BLINK2 0
#elif BOARD == L_SIDE_LIGHT
#define LED_ID_1 0x300
#define BIT_OFF 1
#define BLINK1 1
#elif BOARD == R_SIDE_LIGHT
#define LED_ID_1 0x300
#define BIT_OFF 2
#define BLINK1 1
#elif BOARD == MID_LIGHT 
#define LED_ID_1 0x0//BPS fault ID
#define LED_ID_2 0x20A
#define BLINK1 0
#define BLINK2 0
#elif BOARD == L_REAR_LIGHT
#define LED_ID_1 0x300
#define BIT_OFF 1
#define BLINK1 1
#define LED_ID_2 0x20A
#define BLINK2 0
#elif BOARD == R_REAR_LIGHT
#define LED_ID_1 0x300
#define BIT_OFF 2
#define BLINK1 1
#define LED_ID_2 0x20A
#define BLINK2 0

int bitVal;

LightingCAN::LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency) {
    pinMode(PA0, OUTPUT);
    pinMode(PA1, OUTPUT);
};

void LightingCAN::readHandler(CAN_message_t msg) {
    uint8_t* data = msg.buf;
    if (data)
    switch (msg.id) {
        case LED_ID_1:
            // digitalWrite(PA0, data[0]);
            bitVal = (LED_ID_1 >> BIT_OFF) & 1;
            setLED(PA0, bitVal, BLINK1);
            break;
        case LED_ID_2:
            // digitalWrite(PA1, data[0]);
            bitVal = (LED_ID_2 >> BIT_OFF) & 1;
            setLED(PA1, bitBal, BLINK2);
            break;
        default:
            break;
    }
}

void setLED(uint32_t pin, int bitVal, int blink){
    if(!bitVal){ //LED off
        digitalWrite(pin, LOW);
    }
    else if(bitVal){ //LED on/blink
        if(!blink){
            digitalWrite(pin, HIGH);
        }
        else if(blink == 1){
            //blink LED
            digitalWrite(pin, HIGH);
            delay(1000);
            digitalWrite(pin, LOW);
            delay(1000);
        }
    }   
}
