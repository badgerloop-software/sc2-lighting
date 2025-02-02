#ifndef __LIGHTING_CAN_H__
#define __LIGHTING_CAN_H__

#include <Arduino.h>
#include "canmanager.h"

class LightingCAN : public CANManager {
    public:
        LightingCAN(CAN_TypeDef* canPort, CAN_PINS pins, int frequency = DEFAULT_CAN_FREQ);
        void readHandler(CAN_message_t msg);
};

#endif