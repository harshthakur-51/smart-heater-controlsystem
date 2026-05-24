#include "buttons.h"

#include "config.h"

typedef struct {
    uint8_t stable;
    uint8_t last_raw;
    uint8_t counters[BUTTON_COUNT];
    uint16_t held_ticks[BUTTON_COUNT];
    uint8_t long_sent;
} button_state_t;

static button_state_t state;

void buttons_init(void) {
    state.stable = 0;
    state.last_raw = 0;
    state.long_sent = 0;
    for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
        state.counters[i] = 0;
        state.held_ticks[i] = 0;
    }
}

button_events_t buttons_update(uint8_t raw_mask) {
    button_events_t events = {0, 0, state.stable, 0};

    for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
        const uint8_t bit = (uint8_t)(1U << i);
        const bool raw_pressed = (raw_mask & bit) != 0U;
        const bool stable_pressed = (state.stable & bit) != 0U;

        if (((state.last_raw & bit) != 0U) == raw_pressed) {
            if (state.counters[i] < BUTTON_DEBOUNCE_TICKS) {
                state.counters[i]++;
            }
        } else {
            state.counters[i] = 0;
        }

        if (state.counters[i] >= BUTTON_DEBOUNCE_TICKS && raw_pressed != stable_pressed) {
            if (raw_pressed) {
                state.stable |= bit;
                events.pressed |= bit;
                state.held_ticks[i] = 0;
            } else {
                state.stable &= (uint8_t)~bit;
                events.released |= bit;
                state.held_ticks[i] = 0;
                state.long_sent &= (uint8_t)~bit;
            }
        }

        if ((state.stable & bit) != 0U) {
            if (state.held_ticks[i] < UINT16_MAX) {
                state.held_ticks[i]++;
            }
            if (state.held_ticks[i] >= BUTTON_LONG_PRESS_TICKS && (state.long_sent & bit) == 0U) {
                events.long_pressed |= bit;
                state.long_sent |= bit;
            }
        }
    }

    state.last_raw = raw_mask;
    events.held = state.stable;
    return events;
}

uint8_t buttons_stable_mask(void) {
    return state.stable;
}
