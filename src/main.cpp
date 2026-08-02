#include <Arduino.h>

#include "board_config.h"
#include "can_lighting.h"
#include "debug.h"

// ------------- LOCAL -------------

static CanLighting canBus(CAN1, DEF);

// ------------- PUBLIC FUNCTIONS -------------

void setup() {
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
#ifdef PLATE_ALWAYS_ON
    digitalWrite(LED_PIN_2, HIGH);
#endif

    debugInit();
}

void loop() {
    debugUpdate();

    canBus.runQueue(1);
    canBus.updateOutputs();
}
