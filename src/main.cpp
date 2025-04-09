#include <Arduino.h>
#include "STM32_CAN.h"
#include "lightingCAN.h"


LightingCAN Can1(CAN1, DEF );

void setup() {

}

void loop() {
  Can1.runQueue(1000);
  Can1.send();
  // delay(1000);
  // digitalWrite(PA0, LOW);
  // delay(1000);
  // digitalWrite(PA0, HIGH);
}