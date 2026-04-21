#include "lightingCAN.h"

volatile LED leds[2] = {
    {false, false, BLINK1, PA0},
    {false, false, BLINK2, PA1}
};

void setLED(uint8_t i);

LightingCAN::LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency) {

};

bool LightingCAN::send() {
    // send function is mostly for testing that CAN communication works
    // it should NOT be used in the final product
    uint8_t abc = 12;
    bool ret = this->sendMessage(0x700, (void*)&abc, sizeof(uint8_t));
    return ret;
}

void LightingCAN::readHandler(CAN_message_t msg) {
    uint8_t data = msg.buf[0];

    // Extra handling for brake light if it also needs to blink, can comment out for normal function
    if (BOARD == 7) {
        leds[0].blinks = (msg.id == 0x300);
        if (msg.id == 0x300) {
            leds[0].on = (data >> 2) & 1;
            setLED(0);
            return;
        }
        else if (msg.id == LED_ID_1) {
            leds[0].on = ((data >> BIT_OFF1) & 1) | leds[0].on;
            setLED(0);
            return;
        }
    }
    else if (BOARD == 5) {
        leds[0].blinks = (msg.id == 0x300);
        if (msg.id == 0x300) {
            leds[0].on = (data >> 1) & 1;
            setLED(0);
            return;
        }
        else if (msg.id == LED_ID_1) {
            leds[0].on = ((data >> BIT_OFF1) & 1) | leds[0].on;
            setLED(0);
            return;
        }
    }
    ////////////////////////////////////////////////////////////////////////


    if (msg.id == LED_ID_1) {
        leds[0].on = (data >> BIT_OFF1) & 1;
        #ifdef PLATE
            leds[0].on = 1;
        #endif
        #ifdef BPS_FAULT
            leds[0].on = msg.buf[0] != 0 || msg.buf[2] != 0 || msg.buf[4] != 0 || msg.buf[5] != 0;
        #endif
        setLED(0);
    }
    else if (msg.id == LED_ID_2) {
        leds[1].on = (data >> BIT_OFF2) & 1;
        setLED(1);
    }
}

void setLED(uint8_t i) {
    volatile LED* led = &leds[i];
    if (led->on) {
        if (led->blinks) {
        led->blink_val = !led->blink_val;
        digitalWrite(led->pin, led->blink_val);
        } else {
        digitalWrite(led->pin, HIGH);
        }
    } else {
        digitalWrite(led->pin, LOW); // turn off
    }
}
