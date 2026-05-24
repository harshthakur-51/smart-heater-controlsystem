#include "heater_controller.h"

#define TARGET_TEMP_C_X10 850
#define HYSTERESIS_C_X10 25
#define OVERTEMP_C_X10 1050
#define NTC_MIN_VALID_C_X10 (-200)
#define NTC_MAX_VALID_C_X10 1300

static heater_mode_t mode = HEATER_MODE_OFF;
static heater_fault_t fault = HEATER_FAULT_NONE;

static void latch_fault(heater_fault_t reason) {
    fault = reason;
    mode = HEATER_MODE_FAULT;
}

void heater_controller_init(void) {
    mode = HEATER_MODE_OFF;
    fault = HEATER_FAULT_NONE;
}

heater_outputs_t heater_controller_update(heater_inputs_t inputs) {
    heater_outputs_t outputs = {mode, fault, false, false};

    if (!inputs.thermal_fuse_ok) {
        latch_fault(HEATER_FAULT_OVERTEMP);
    } else if (!inputs.ntc_valid || inputs.temperature_c_x10 < NTC_MIN_VALID_C_X10) {
        latch_fault(HEATER_FAULT_NTC_OPEN);
    } else if (inputs.temperature_c_x10 > NTC_MAX_VALID_C_X10) {
        latch_fault(HEATER_FAULT_NTC_SHORT);
    } else if (inputs.temperature_c_x10 >= OVERTEMP_C_X10) {
        latch_fault(HEATER_FAULT_OVERTEMP);
    }

    if (mode == HEATER_MODE_FAULT) {
        outputs.mode = mode;
        outputs.fault = fault;
        outputs.heater_output = false;
        outputs.motor_output = false;
        return outputs;
    }

    if (!inputs.user_enable) {
        mode = HEATER_MODE_OFF;
    } else if (inputs.temperature_c_x10 < TARGET_TEMP_C_X10 - HYSTERESIS_C_X10) {
        mode = HEATER_MODE_PREHEAT;
    } else {
        mode = HEATER_MODE_REGULATE;
    }

    outputs.mode = mode;
    outputs.fault = fault;
    outputs.motor_output = inputs.user_enable;
    outputs.heater_output = inputs.user_enable &&
        inputs.temperature_c_x10 < TARGET_TEMP_C_X10 + HYSTERESIS_C_X10;
    return outputs;
}

const char *heater_mode_name(heater_mode_t value) {
    switch (value) {
        case HEATER_MODE_OFF: return "OFF";
        case HEATER_MODE_PREHEAT: return "PREHEAT";
        case HEATER_MODE_REGULATE: return "REGULATE";
        case HEATER_MODE_FAULT: return "FAULT";
    }
    return "UNKNOWN";
}

const char *heater_fault_name(heater_fault_t value) {
    switch (value) {
        case HEATER_FAULT_NONE: return "NONE";
        case HEATER_FAULT_NTC_OPEN: return "NTC_OPEN";
        case HEATER_FAULT_NTC_SHORT: return "NTC_SHORT";
        case HEATER_FAULT_OVERTEMP: return "OVERTEMP";
    }
    return "UNKNOWN";
}
