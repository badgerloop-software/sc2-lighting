#include "debug.h"

#if SC2_DEBUG

#include <Arduino.h>

// ------------- PUBLIC FUNCTIONS -------------

void debugInit() {
    Serial.begin(115200);
}

void debugUpdate() {}

void debugError(const char* msg) {
    printf("%s\n", msg);
}

#endif  // SC2_DEBUG
