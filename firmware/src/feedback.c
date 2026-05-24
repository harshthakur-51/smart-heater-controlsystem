#include "feedback.h"

#include "board.h"
#include "config.h"

typedef struct {
    feedback_pattern_t pattern;
    uint16_t timer_ms;
    uint8_t step;
    bool output_on;
} feedback_state_t;

static feedback_state_t state;

static void set_outputs(bool enabled) {
    board_buzzer_set(enabled);
    board_haptic_set(enabled);
}

void feedback_init(void) {
    state.pattern = FEEDBACK_NONE;
    state.timer_ms = 0;
    state.step = 0;
    state.output_on = false;
    set_outputs(false);
}

void feedback_start(feedback_pattern_t pattern) {
    state.pattern = pattern;
    state.timer_ms = 0;
    state.step = 0;
    state.output_on = false;
}

bool feedback_active(void) {
    return state.pattern != FEEDBACK_NONE;
}

void feedback_update(uint16_t elapsed_ms) {
    if (state.pattern == FEEDBACK_NONE) {
        set_outputs(false);
        return;
    }

    state.timer_ms += elapsed_ms;

    switch (state.pattern) {
        case FEEDBACK_CLICK:
            set_outputs(state.timer_ms < FEEDBACK_CLICK_MS);
            if (state.timer_ms >= FEEDBACK_CLICK_MS) {
                state.pattern = FEEDBACK_NONE;
            }
            break;

        case FEEDBACK_POWER_OFF:
            set_outputs(state.timer_ms < FEEDBACK_MODE_ON_MS);
            if (state.timer_ms >= FEEDBACK_MODE_ON_MS) {
                state.pattern = FEEDBACK_NONE;
            }
            break;

        case FEEDBACK_MODE_SELECTED:
            if (state.step == 0) {
                set_outputs(state.timer_ms < FEEDBACK_MODE_ON_MS);
                if (state.timer_ms >= FEEDBACK_MODE_ON_MS + FEEDBACK_MODE_GAP_MS) {
                    state.step = 1;
                    state.timer_ms = 0;
                }
            } else {
                set_outputs(state.timer_ms < FEEDBACK_MODE_ON_MS);
                if (state.timer_ms >= FEEDBACK_MODE_ON_MS) {
                    state.pattern = FEEDBACK_NONE;
                }
            }
            break;

        case FEEDBACK_FAULT:
            set_outputs(state.timer_ms < FEEDBACK_FAULT_ON_MS);
            if (state.timer_ms >= FEEDBACK_FAULT_ON_MS + FEEDBACK_FAULT_GAP_MS) {
                state.timer_ms = 0;
                state.step++;
                if (state.step >= 3U) {
                    state.pattern = FEEDBACK_NONE;
                }
            }
            break;

        case FEEDBACK_NONE:
        default:
            state.pattern = FEEDBACK_NONE;
            set_outputs(false);
            break;
    }
}
