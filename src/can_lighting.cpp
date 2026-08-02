#include "can_lighting.h"

#include "can_ids.h"

// ------------- GLOBALS -------------

volatile LED leds[2] = {
    {false, false, BLINK1, LED_PIN_1},
    {false, false, BLINK2, LED_PIN_2},
};

// ------------- LOCAL FUNCTIONS -------------

static void setLED(uint8_t i);

// ------------- PUBLIC FUNCTIONS -------------

CanLighting::CanLighting(CAN_TypeDef* canPort, CAN_PINS pins, int frequency)
    : CANManager(canPort, pins, frequency) {}

bool CanLighting::send() {
    uint8_t heartbeat = 12;
    return sendMessage(SC2_CAN_LIGHTING_HEARTBEAT_ID, (void*)&heartbeat, sizeof(uint8_t));
}

void CanLighting::updateOutputs() {
    for (uint8_t i = 0; i < 2; i++) {
        if (leds[i].on && leds[i].blinks) {
            setLED(i);
        }
    }
}

void CanLighting::readHandler(CAN_message_t msg) {
    uint8_t data = msg.buf[0];

#if defined(LEFT_BLINK) || defined(RIGHT_BLINK)
    if (msg.id == SC2_CAN_STEERING_DIGITAL_ID) {
        leds[0].on = (data >> BIT_OFF1) & 1;
        setLED(0);
    }
#endif

#ifdef REVERSE
    if (msg.id == SC2_CAN_PDC_DIGITAL_ID) {
        leds[0].on = (data >> BIT_OFF1) & 1;
        setLED(0);
    }
#endif

#ifdef BPS_FAULT
    if (msg.id == SC2_CAN_PT_FAULT_STATUS_ID) {
        leds[1].on = (data >> BIT_OFF2) & 1;
        setLED(1);
    }
#endif

#ifdef BRAKELIGHT
    if (msg.id == SC2_CAN_PDC_DIGITAL_ID) {
        leds[1].on = (data >> BIT_OFF2) & 1;
        setLED(1);
    }
#endif

#ifdef HEADLIGHT
    if (msg.id == SC2_CAN_STEERING_DIGITAL_ID) {
        leds[1].on = (data >> BIT_OFF2) & 1;
        setLED(1);
    }
#endif
}

static void setLED(uint8_t i) {
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
