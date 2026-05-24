#include "heater_controller.h"

#include "config.h"

static heater_status_t status;
static uint32_t active_ms;
static uint32_t keep_warm_ms;

static int16_t target_for_mode(heater_mode_t mode) {
    switch (mode) {
        case HEATER_MODE_TEA: return TEMP_TEA_C_X10;
        case HEATER_MODE_COFFEE: return TEMP_COFFEE_C_X10;
        case HEATER_MODE_KEEP_WARM: return TEMP_KEEP_WARM_C_X10;
        default: return 0;
    }
}

static void enter_mode(heater_mode_t mode) {
    status.mode = mode;
    status.target_c_x10 = target_for_mode(mode);
    status.heater_output = false;
    status.motor_output = (mode != HEATER_MODE_OFF && mode != HEATER_MODE_FAULT);
    status.at_temperature = false;
    active_ms = 0;
    keep_warm_ms = 0;
}

static void latch_fault(heater_fault_t fault) {
    status.mode = HEATER_MODE_FAULT;
    status.fault = fault;
    status.fault_latched = true;
    status.heater_output = false;
    status.motor_output = false;
    status.target_c_x10 = 0;
}

void heater_controller_init(void) {
    status.mode = HEATER_MODE_OFF;
    status.fault = HEATER_FAULT_NONE;
    status.target_c_x10 = 0;
    status.temperature_c_x10 = 0;
    status.heater_output = false;
    status.motor_output = false;
    status.at_temperature = false;
    status.fault_latched = false;
    active_ms = 0;
    keep_warm_ms = 0;
}

void heater_controller_clear_fault(void) {
    if (status.mode == HEATER_MODE_FAULT) {
        heater_controller_init();
    }
}

heater_status_t heater_controller_update(heater_inputs_t inputs, heater_command_t command, uint16_t elapsed_ms) {
    status.temperature_c_x10 = inputs.temperature_c_x10;

    if (!inputs.thermal_fuse_ok) {
        latch_fault(HEATER_FAULT_THERMAL_FUSE);
    } else if (inputs.ntc_open || !inputs.ntc_valid) {
        latch_fault(HEATER_FAULT_NTC_OPEN);
    } else if (inputs.ntc_short) {
        latch_fault(HEATER_FAULT_NTC_SHORT);
    } else if (inputs.temperature_c_x10 >= TEMP_OVERTEMP_C_X10) {
        latch_fault(HEATER_FAULT_OVERTEMP);
    }

    if (status.mode == HEATER_MODE_FAULT) {
        return status;
    }

    if (command.power_toggle) {
        if (status.mode == HEATER_MODE_OFF) {
            enter_mode(HEATER_MODE_KEEP_WARM);
        } else {
            enter_mode(HEATER_MODE_OFF);
        }
    }

    if (command.requested_mode != HEATER_MODE_OFF && command.requested_mode != HEATER_MODE_FAULT) {
        enter_mode(command.requested_mode);
    }

    if (status.mode == HEATER_MODE_OFF) {
        status.heater_output = false;
        status.motor_output = false;
        status.at_temperature = false;
        return status;
    }

    active_ms += elapsed_ms;
    if (active_ms > HEATER_MAX_ON_TIME_MS) {
        latch_fault(HEATER_FAULT_TIMEOUT);
        return status;
    }

    if (status.mode == HEATER_MODE_KEEP_WARM) {
        keep_warm_ms += elapsed_ms;
        if (keep_warm_ms > KEEP_WARM_TIMEOUT_MS) {
            enter_mode(HEATER_MODE_OFF);
            return status;
        }
    }

    status.motor_output = true;
    status.at_temperature = inputs.temperature_c_x10 >= status.target_c_x10;

    if (inputs.temperature_c_x10 <= status.target_c_x10 - TEMP_HYSTERESIS_C_X10) {
        status.heater_output = true;
    } else if (inputs.temperature_c_x10 >= status.target_c_x10 + TEMP_HYSTERESIS_C_X10) {
        status.heater_output = false;
    }

    return status;
}

const char *heater_mode_name(heater_mode_t mode) {
    switch (mode) {
        case HEATER_MODE_OFF: return "OFF";
        case HEATER_MODE_TEA: return "TEA";
        case HEATER_MODE_COFFEE: return "COFFEE";
        case HEATER_MODE_KEEP_WARM: return "KEEP_WARM";
        case HEATER_MODE_FAULT: return "FAULT";
    }
    return "UNKNOWN";
}

const char *heater_fault_name(heater_fault_t fault) {
    switch (fault) {
        case HEATER_FAULT_NONE: return "NONE";
        case HEATER_FAULT_NTC_OPEN: return "NTC_OPEN";
        case HEATER_FAULT_NTC_SHORT: return "NTC_SHORT";
        case HEATER_FAULT_OVERTEMP: return "OVERTEMP";
        case HEATER_FAULT_THERMAL_FUSE: return "THERMAL_FUSE";
        case HEATER_FAULT_TIMEOUT: return "TIMEOUT";
    }
    return "UNKNOWN";
}
