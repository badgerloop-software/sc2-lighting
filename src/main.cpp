#include <Arduino.h>
#include "STM32_CAN.h"
#include "lightingCAN.h"


LightingCAN Can1(CAN1, DEF );

void setup() {
  Serial.begin(115200); // TX = PA2, RX = PA3
}

void loop() {
  Can1.runQueue(1000);
}