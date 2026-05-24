#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    HEATER_MODE_OFF = 0,
    HEATER_MODE_PREHEAT,
    HEATER_MODE_REGULATE,
    HEATER_MODE_FAULT
} heater_mode_t;

typedef enum {
    HEATER_FAULT_NONE = 0,
    HEATER_FAULT_NTC_OPEN,
    HEATER_FAULT_NTC_SHORT,
    HEATER_FAULT_OVERTEMP
} heater_fault_t;

typedef struct {
    int16_t temperature_c_x10;
    bool ntc_valid;
    bool user_enable;
    bool thermal_fuse_ok;
} heater_inputs_t;

typedef struct {
    heater_mode_t mode;
    heater_fault_t fault;
    bool heater_output;
    bool motor_output;
} heater_outputs_t;

void heater_controller_init(void);
heater_outputs_t heater_controller_update(heater_inputs_t inputs);
const char *heater_mode_name(heater_mode_t mode);
const char *heater_fault_name(heater_fault_t fault);
