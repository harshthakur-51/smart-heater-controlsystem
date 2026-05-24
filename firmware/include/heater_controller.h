#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    HEATER_MODE_OFF = 0,
    HEATER_MODE_TEA,
    HEATER_MODE_COFFEE,
    HEATER_MODE_KEEP_WARM,
    HEATER_MODE_FAULT
} heater_mode_t;

typedef enum {
    HEATER_FAULT_NONE = 0,
    HEATER_FAULT_NTC_OPEN,
    HEATER_FAULT_NTC_SHORT,
    HEATER_FAULT_OVERTEMP,
    HEATER_FAULT_THERMAL_FUSE,
    HEATER_FAULT_TIMEOUT
} heater_fault_t;

typedef struct {
    int16_t temperature_c_x10;
    bool ntc_valid;
    bool ntc_open;
    bool ntc_short;
    bool thermal_fuse_ok;
} heater_inputs_t;

typedef struct {
    heater_mode_t requested_mode;
    bool power_toggle;
} heater_command_t;

typedef struct {
    heater_mode_t mode;
    heater_fault_t fault;
    int16_t target_c_x10;
    int16_t temperature_c_x10;
    bool heater_output;
    bool motor_output;
    bool at_temperature;
    bool fault_latched;
} heater_status_t;

void heater_controller_init(void);
heater_status_t heater_controller_update(heater_inputs_t inputs, heater_command_t command, uint16_t elapsed_ms);
void heater_controller_clear_fault(void);
const char *heater_mode_name(heater_mode_t mode);
const char *heater_fault_name(heater_fault_t fault);
