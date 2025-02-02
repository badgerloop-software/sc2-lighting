#include "lightingCAN.h"

LightingCAN::LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency) {
    pinMode(PA0, OUTPUT);
    pinMode(PA1, OUTPUT);
};

void LightingCAN::readHandler(CAN_message_t msg) {
    uint8_t* data = msg.buf;
    if (data) 
    switch (msg.id) {
        case 0x050:
            digitalWrite(PA0, data[0]);
            break;
        case 0x051:
            digitalWrite(PA1, data[0]);
            break;
        default:
            break;
    }
}

