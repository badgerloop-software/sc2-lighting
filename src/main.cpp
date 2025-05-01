#include <Arduino.h>
#include "STM32_CAN.h"
#include "lightingCAN.h"

LightingCAN Can1(CAN1, DEF);

void setup() {
  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
}

void loop() {
  Can1.runQueue(1000);
}
