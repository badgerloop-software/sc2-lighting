#include "lightingCAN.h"

#define L_FRONT_LIGHT 0
#define R_FRONT_LIGHT 1
#define L_SIDE_LIGHT 2
#define R_SIDE_LIGHT 3
#define MID_LIGHT 4
#define L_REAR_LIGHT 5
#define R_REAR_LIGHT 6

#define BOARD 0

#define BLINK_DELAY 400

// BLINK==0 means NOT blinking, BLINK==1 means to blink
#if BOARD == L_FRONT_LIGHT
#define LED_ID_1 0x300
#define BIT_OFF 1
#define BLINK1 0
#define LED_ID_2 0x400 // TODO: replace with headlight ID
#define BLINK2 0
#elif BOARD == R_FRONT_LIGHT
#define LED_ID_1 0x300
#define BIT_OFF 2
#define BLINK1 1
#define LED_ID_2 0x0 // TODO : replace with headlight ID  
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
#define LED_ID_1 0x0  // TODO : replace with BPS fault ID
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
#endif

void setLED(uint32_t, int, int);

int bitVal;

LightingCAN::LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency) {
    pinMode(PA0, OUTPUT);
    pinMode(PA1, OUTPUT);
};

void LightingCAN::send() {
    uint8_t abc = 12;
    bool ret = this->sendMessage(0x100, (void*)&abc, sizeof(uint8_t));
    // if (!ret) {
    //     digitalWrite(PA0, LOW);
    //     this->reset();
    // } else {
    //     digitalWrite(PA0, HIGH);
    // }
}

void LightingCAN::readHandler(CAN_message_t msg) {
    uint8_t data = msg.buf[0];
    switch (msg.id) {
        case LED_ID_1:
            bitVal = (data >> BIT_OFF) & 1;
            setLED(PA0, bitVal, BLINK1);
            break;
        case LED_ID_2:
            bitVal = (data >> BIT_OFF) & 1;
            setLED(PA1, bitVal, BLINK2);
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
            delay(BLINK_DELAY);
            digitalWrite(pin, LOW);
            delay(BLINK_DELAY);
        }
    }   
}
