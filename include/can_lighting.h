#ifndef __CAN_LIGHTING_H__
#define __CAN_LIGHTING_H__

#include "board_config.h"
#include "canmanager.h"

// ------------- TYPES -------------

typedef struct {
    bool on : 1;
    bool blink_val : 1;
    bool blinks : 1;
    uint32_t pin;
} LED;

// ------------- GLOBALS -------------

extern volatile LED leds[2];

// ------------- CLASS -------------

class CanLighting : public CANManager {
   public:
    CanLighting(CAN_TypeDef* canPort, CAN_PINS pins, int frequency = DEFAULT_CAN_FREQ);
    void readHandler(CAN_message_t msg) override;
    void updateOutputs();
    bool send();
};

#endif  // __CAN_LIGHTING_H__
