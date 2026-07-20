#include "lightingCAN.h"

volatile LED leds[2] = {
    {false, false, BLINK1, PA0},
    {false, false, BLINK2, PA1}
};

#if defined(LEFT_BLINK) || defined(RIGHT_BLINK)
static volatile bool blinkRequest = false;
static volatile bool hazardRequest = false;
#endif

void setLED(uint8_t i);
#if defined(LEFT_BLINK) || defined(RIGHT_BLINK)
void applyBlinkOutput();
#endif

LightingCAN::LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency) {

};

bool LightingCAN::send() {
    // send function is mostly for testing that CAN communication works
    // it should NOT be used in the final product
    return false;
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
    if (msg.id == HAZARD_ID) {
        hazardRequest = (data >> HAZARD_BIT) & 1;
        applyBlinkOutput();
        return;
    }

    if (msg.id == LED_ID_1) {
        blinkRequest = (data >> BIT_OFF1) & 1;
        applyBlinkOutput();
        return;
    }
#endif

    if (msg.id == LED_ID_1) {
        leds[0].on = (data >> BIT_OFF1) & 1;
        #ifdef PLATE
            leds[0].on = 1;
        #endif
        setLED(0);
    }
    #ifdef LED_ID_2
    else if (msg.id == LED_ID_2) {
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
        digitalWrite(led->pin, LOW); // turn off
    }
}

#if defined(LEFT_BLINK) || defined(RIGHT_BLINK)
void applyBlinkOutput() {
    leds[0].on = hazardRequest || blinkRequest;
    setLED(0);
}
#endif
