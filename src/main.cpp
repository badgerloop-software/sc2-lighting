// sc2-lighting main: small STM32 node for blinkers, brake, headlight, BPS lamp
// Set BOARD in lightingCAN.h to pick which lamp roles this PCB runs
#include <Arduino.h>
#include "STM32_CAN.h"
#include "lightingCAN.h"

LightingCAN Can1(CAN1, DEF);

void setup() {
  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
#ifdef PLATE_ALWAYS_ON
  // Rear plate lamp stays on always
  digitalWrite(PA1, HIGH);
#endif
}

void loop() {
  Can1.runQueue(1);
  Can1.updateOutputs();
}
