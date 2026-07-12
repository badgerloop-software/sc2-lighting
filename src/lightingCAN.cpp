#include "lightingCAN.h"

volatile LED leds[2] = {
    {false, false, BLINK1, PA0},
    {false, false, BLINK2, PA1}
};

void setLED(uint8_t i);

LightingCAN::LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency) {

};

bool LightingCAN::send() {
    uint8_t abc = 12;
    return this->sendMessage(0x700, (void*)&abc, sizeof(uint8_t));
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

#if defined(LEFT_BLINK) || defined(RIGHT_BLINK)
    if (msg.id == LED_ID_1) {
        leds[0].on = (data >> BIT_OFF1) & 1;
        setLED(0);
    }
#endif

#ifdef REVERSE
    if (msg.id == LED_ID_1) {
        leds[0].on = (data >> BIT_OFF1) & 1;
        setLED(0);
    }
#endif

#if defined(BPS_FAULT)
    if (msg.id == LED_ID_2) {
        leds[1].on = msg.buf[0] != 0 || msg.buf[2] != 0 || msg.buf[4] != 0 || msg.buf[5] != 0;
        setLED(1);
    }
#endif

#if defined(BRAKELIGHT) || defined(HEADLIGHT)
    if (msg.id == LED_ID_2) {
        leds[1].on = (data >> BIT_OFF2) & 1;
        setLED(1);
    }
#endif
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
        digitalWrite(led->pin, LOW);
    }
}
