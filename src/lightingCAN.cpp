// lightingCAN: map CAN bits to two local LED channels (PA0, PA1)
// BOARD macro in the header selects which frames and bits this build uses
#include "lightingCAN.h"

volatile LED leds[2] = {
    {false, false, BLINK1, PA0},
    {false, false, BLINK2, PA1}
};

static void setLED(uint8_t i) {
    volatile LED* led = &leds[i];
    if (led->on) {
        if (led->blinks) {
            // Determines blink phase
            led->blink_val = ((millis() / BLINK_DELAY) % 2) != 0;
            digitalWrite(led->pin, led->blink_val);
        } else {
            digitalWrite(led->pin, HIGH);
        }
    } else {
        digitalWrite(led->pin, LOW);
    }
}

LightingCAN::LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency) {

};

// Static send message for testing
bool LightingCAN::send() {
    uint8_t abc = 12;
    return this->sendMessage(0x700, (void*)&abc, sizeof(uint8_t));
}

void LightingCAN::updateOutputs() {
    // Calls setLED for each LED that is on and blinking (note: bps only, others listen to steering wheel)
    for (uint8_t i = 0; i < 2; i++) {
        if (leds[i].on && leds[i].blinks) {
            setLED(i);
        }
    }
}

void LightingCAN::readHandler(CAN_message_t msg) {
    uint8_t data = msg.buf[0];

#if defined(LEFT_BLINK) || defined(RIGHT_BLINK)
    // 0x300 from steering wheel: blink bits in the digital pack to synchronize
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
    // 0x001 from powertrain: fault bit drives the BPS lamp
    if (msg.id == LED_ID_2) {
        leds[1].on = (data >> BIT_OFF2) & 1;
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
