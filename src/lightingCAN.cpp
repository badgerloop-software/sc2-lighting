#include "lightingCAN.h"

volatile LED leds[2] = {
    {false, false, BLINK1, PA0},
    {false, false, BLINK2, PA1}
};

static volatile bool sharedBlinkRequest = false;
static volatile bool sharedBrakeRequest = false;

void setLED(uint8_t i);
void applySharedRearLight();

LightingCAN::LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency) {

};

bool LightingCAN::send() {
    // send function is mostly for testing that CAN communication works
    // it should NOT be used in the final product
    uint8_t abc = 12;
    bool ret = this->sendMessage(0x700, (void*)&abc, sizeof(uint8_t));
    return ret;
}

void LightingCAN::updateOutputs() {
    for (uint8_t i = 0; i < 2; i++) {
        if (leds[i].on && leds[i].blinks) {
            setLED(i);
        }
    }
}

void LightingCAN::readHandler(CAN_message_t msg) {
    uint8_t data = msg.buf[0];

    // Boards 5 and 7 combine blink and brake requests onto one physical LED.
    if (BOARD == 7) {
        if (msg.id == 0x300) {
            sharedBlinkRequest = (data >> 2) & 1;
            applySharedRearLight();
            return;
        }
        else if (msg.id == LED_ID_1) {
            sharedBrakeRequest = (data >> BIT_OFF1) & 1;
            applySharedRearLight();
            return;
        }
    }
    else if (BOARD == 5) {
        if (msg.id == 0x300) {
            sharedBlinkRequest = (data >> 1) & 1;
            applySharedRearLight();
            return;
        }
        else if (msg.id == LED_ID_1) {
            sharedBrakeRequest = (data >> BIT_OFF1) & 1;
            applySharedRearLight();
            return;
        }
    }
    ////////////////////////////////////////////////////////////////////////


    if (msg.id == LED_ID_1) {
        leds[0].on = (data >> BIT_OFF1) & 1;
        #ifdef PLATE
            leds[0].on = 1;
        #endif
        setLED(0);
    }
    else if (msg.id == LED_ID_2) {
        leds[1].on = (data >> BIT_OFF2) & 1;
        #ifdef BPS_FAULT
            leds[1].on = msg.buf[0] != 0 || msg.buf[2] != 0 || msg.buf[4] != 0 || msg.buf[5] != 0;
        #endif
        setLED(1);
    }
}

void setLED(uint8_t i) {
    volatile LED* led = &leds[i];
    if (led->on) {
        if (led->blinks) {
        led->blink_val = ((millis() / BLINK_DELAY) % 2) != 0;
        digitalWrite(led->pin, led->blink_val);
        } else {
        digitalWrite(led->pin, HIGH);
        }
    } else {
        digitalWrite(led->pin, LOW); // turn off
    }
}

void applySharedRearLight() {
    // Blink has priority for boards 5/7 when both requests are active.
    leds[0].on = sharedBlinkRequest || sharedBrakeRequest;
    leds[0].blinks = sharedBlinkRequest;
    setLED(0);
}
