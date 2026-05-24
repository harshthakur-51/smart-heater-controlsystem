#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    FEEDBACK_NONE = 0,
    FEEDBACK_CLICK,
    FEEDBACK_MODE_SELECTED,
    FEEDBACK_POWER_OFF,
    FEEDBACK_FAULT
} feedback_pattern_t;

void feedback_init(void);
void feedback_start(feedback_pattern_t pattern);
void feedback_update(uint16_t elapsed_ms);
bool feedback_active(void);
