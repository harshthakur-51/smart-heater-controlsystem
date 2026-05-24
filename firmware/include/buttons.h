#pragma once

#include "board.h"

#include <stdint.h>

typedef struct {
    uint8_t pressed;
    uint8_t released;
    uint8_t held;
    uint8_t long_pressed;
} button_events_t;

void buttons_init(void);
button_events_t buttons_update(uint8_t raw_mask);
uint8_t buttons_stable_mask(void);
