#include "heater_controller.h"

/*
 * Integration stub for the public portfolio repository.
 * Replace these adapter functions with STM8 GPIO, ADC, timer, display,
 * watchdog, and SWIM-debug project code when importing the original firmware.
 */

static heater_inputs_t read_inputs(void) {
    heater_inputs_t inputs;
    inputs.temperature_c_x10 = 780;
    inputs.ntc_valid = true;
    inputs.user_enable = true;
    inputs.thermal_fuse_ok = true;
    return inputs;
}

static void write_outputs(heater_outputs_t outputs) {
    (void)outputs;
    /* Map heater_output to optotriac drive and motor_output to MOSFET drive. */
}

int main(void) {
    heater_controller_init();
    while (1) {
        heater_outputs_t outputs = heater_controller_update(read_inputs());
        write_outputs(outputs);
    }
}
