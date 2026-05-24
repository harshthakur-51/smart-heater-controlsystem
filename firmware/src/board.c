#include "board.h"

/*
 * Board adaptation layer.
 *
 * Replace these stubs with STM8S003F3P6 register or SPL calls after final pin
 * mapping is confirmed from the schematic/PCB:
 * - capacitive button inputs
 * - NTC ADC channel
 * - optotriac output
 * - motor MOSFET output
 * - buzzer and haptic outputs
 * - 74HC595 data/clock/latch and digit-enable lines
 * - 1 ms timer tick
 */

static volatile uint32_t system_ms;
static uint8_t simulated_buttons;
static uint16_t simulated_adc = 580U;

void board_init(void) {
    system_ms = 0;
    simulated_buttons = 0;
}

void board_tick_1ms_isr(void) {
    system_ms++;
}

uint32_t board_millis(void) {
    return system_ms;
}

void board_watchdog_kick(void) {
}

uint16_t board_ntc_adc_read(void) {
    return simulated_adc;
}

uint8_t board_buttons_raw_mask(void) {
    return simulated_buttons;
}

bool board_thermal_fuse_ok(void) {
    return true;
}

void board_heater_set(bool enabled) {
    (void)enabled;
}

void board_motor_set(bool enabled) {
    (void)enabled;
}

void board_buzzer_set(bool enabled) {
    (void)enabled;
}

void board_haptic_set(bool enabled) {
    (void)enabled;
}

void board_shift_data_set(bool high) {
    (void)high;
}

void board_shift_clock_pulse(void) {
}

void board_shift_latch_pulse(void) {
}

void board_display_digit_enable(uint8_t digit, bool enabled) {
    (void)digit;
    (void)enabled;
}
