#include "app.h"

#include "board.h"
#include "buttons.h"
#include "config.h"
#include "display.h"
#include "feedback.h"
#include "heater_controller.h"
#include "ntc.h"

static heater_status_t last_status;
static uint16_t display_refresh_accum_ms;
static uint16_t display_status_accum_ms;

static heater_command_t command_from_buttons(button_events_t events) {
    heater_command_t command = {HEATER_MODE_OFF, false};

    if ((events.pressed & BUTTON_MASK_POWER) != 0U) {
        command.power_toggle = true;
    } else if ((events.pressed & BUTTON_MASK_TEA) != 0U) {
        command.requested_mode = HEATER_MODE_TEA;
    } else if ((events.pressed & BUTTON_MASK_COFFEE) != 0U) {
        command.requested_mode = HEATER_MODE_COFFEE;
    } else if ((events.pressed & BUTTON_MASK_KEEP_WARM) != 0U) {
        command.requested_mode = HEATER_MODE_KEEP_WARM;
    }

    if ((events.long_pressed & BUTTON_MASK_POWER) != 0U && last_status.mode == HEATER_MODE_FAULT) {
        heater_controller_clear_fault();
        feedback_start(FEEDBACK_CLICK);
    }

    return command;
}

static void update_feedback(heater_status_t previous, heater_status_t current, heater_command_t command) {
    if (current.mode == HEATER_MODE_FAULT && previous.mode != HEATER_MODE_FAULT) {
        feedback_start(FEEDBACK_FAULT);
    } else if (current.mode == HEATER_MODE_FAULT) {
        return;
    } else if (command.power_toggle && current.mode == HEATER_MODE_OFF) {
        feedback_start(FEEDBACK_POWER_OFF);
    } else if (command.power_toggle || command.requested_mode != HEATER_MODE_OFF) {
        feedback_start(FEEDBACK_MODE_SELECTED);
    }
}

void app_init(void) {
    board_init();
    buttons_init();
    ntc_init();
    display_init();
    feedback_init();
    heater_controller_init();
    display_refresh_accum_ms = 0;
    display_status_accum_ms = 0;
    {
        heater_inputs_t inputs = {250, true, false, false, true};
        heater_command_t command = {HEATER_MODE_OFF, false};
        last_status = heater_controller_update(inputs, command, 0);
    }
}

void app_tick_10ms(void) {
    const uint8_t raw_buttons = board_buttons_raw_mask();
    const button_events_t button_events = buttons_update(raw_buttons);
    const ntc_reading_t ntc = ntc_from_adc(board_ntc_adc_read());

    const heater_inputs_t inputs = {
        ntc.temperature_c_x10,
        ntc.valid,
        ntc.open_circuit,
        ntc.short_circuit,
        board_thermal_fuse_ok()
    };
    const heater_command_t command = command_from_buttons(button_events);
    const heater_status_t previous = last_status;
    const heater_status_t current = heater_controller_update(inputs, command, APP_TICK_MS);

    board_heater_set(current.heater_output);
    board_motor_set(current.motor_output);
    update_feedback(previous, current, command);
    feedback_update(APP_TICK_MS);

    display_refresh_accum_ms += APP_TICK_MS;
    while (display_refresh_accum_ms >= DISPLAY_REFRESH_PERIOD_MS) {
        display_refresh_tick();
        display_refresh_accum_ms -= DISPLAY_REFRESH_PERIOD_MS;
    }

    display_status_accum_ms += APP_TICK_MS;
    if (display_status_accum_ms >= DISPLAY_STATUS_PERIOD_MS) {
        display_set_status(current);
        display_status_accum_ms = 0;
    }

    board_watchdog_kick();
    last_status = current;
}
