#include "board.h"

/*
 * Board adaptation layer for the final STM8S003F3P6 controller schematic.
 *
 * The firmware above this file is hardware-agnostic. Replace the body of each
 * function with STM8 SPL, Cosmic, IAR, or SDCC register code for the pins below.
 * The net/function names are taken from the final controller schematic images.
 *
 * High-confidence schematic nets:
 * - MCU_HEATER -> MOC3063 optotriac input -> main triac heater path.
 * - I2 / NTC -> ADC divider input for temperature feedback.
 * - J3 / MOTOR -> AO3400A low-side motor MOSFET with flyback diode.
 * - LS1 / Q6 -> buzzer output stage.
 * - U3 74HC595 -> display segment serial driver.
 * - Q2-Q5 -> common-anode digit enables for DS1.
 *
 * Working STM8 pin assignment derived from the visible sheet:
 * - PD3 / TIM2_CH2 / AIN4: heater command net, labelled MCU_HEATER.
 * - PD5 / AIN5: NTC divider input at connector I2.
 * - PC7 / TIM1_CH2: motor MOSFET gate path through R20/Q7.
 * - PD4 / BEEP / TIM2_CH1: buzzer/haptic feedback output candidate.
 * - PC6 / SPI_MOSI: 74HC595 serial data candidate.
 * - PC5 / SPI_SCK: 74HC595 shift clock candidate.
 * - PC4 / TIM1_CH4: 74HC595 latch clock candidate.
 * - Q2-Q5 digit-enable and four capacitive-button GPIOs are routed on the
 *   controller sheet but should be confirmed against the Altium netlist before
 *   hard-coding register writes.
 */

static volatile uint32_t system_ms;
static uint8_t simulated_buttons;
static uint16_t simulated_adc = 580U;

void board_init(void) {
    system_ms = 0;
    simulated_buttons = 0;

    /*
     * Configure here on target hardware:
     * - PD3 output push-pull low: heater optotriac command.
     * - PC7 output push-pull low: motor MOSFET command.
     * - PD4 output push-pull low: buzzer/haptic command.
     * - PC6/PC5/PC4 output push-pull low: 74HC595 data/clock/latch.
     * - digit-enable GPIOs output inactive.
     * - four capacitive-button GPIOs input with the final bias mode.
     * - PD5/AIN5 ADC channel for NTC divider.
     * - timer interrupt at 1 ms calling board_tick_1ms_isr().
     */
}

void board_tick_1ms_isr(void) {
    system_ms++;
}

uint32_t board_millis(void) {
    return system_ms;
}

void board_watchdog_kick(void) {
    /* Refresh STM8 IWDG/WWDG here after enabling watchdog in production. */
}

uint16_t board_ntc_adc_read(void) {
    /* Read PD5/AIN5 NTC divider ADC count here. */
    return simulated_adc;
}

uint8_t board_buttons_raw_mask(void) {
    /* Read the four capacitive-button digital outputs here. */
    return simulated_buttons;
}

bool board_thermal_fuse_ok(void) {
    /* Return thermal-fuse/safety-chain status if routed to MCU; otherwise true. */
    return true;
}

void board_heater_set(bool enabled) {
    /* Drive PD3 / MCU_HEATER. Fail-safe inactive state must be low. */
    (void)enabled;
}

void board_motor_set(bool enabled) {
    /* Drive PC7 -> R20 -> AO3400A motor MOSFET gate. */
    (void)enabled;
}

void board_buzzer_set(bool enabled) {
    /* Drive PD4/BEEP candidate -> Q6/LS1 buzzer stage. */
    (void)enabled;
}

void board_haptic_set(bool enabled) {
    /* If there is no separate haptic driver, map this to buzzer or motor pulse. */
    (void)enabled;
}

void board_shift_data_set(bool high) {
    /* Drive PC6 / SPI_MOSI candidate -> U3 DS. */
    (void)high;
}

void board_shift_clock_pulse(void) {
    /* Pulse PC5 / SPI_SCK candidate -> U3 SHCP. */
}

void board_shift_latch_pulse(void) {
    /* Pulse PC4 candidate -> U3 STCP. */
}

void board_display_digit_enable(uint8_t digit, bool enabled) {
    /* Drive Q2-Q5 common-anode digit-enable transistors after netlist confirm. */
    (void)digit;
    (void)enabled;
}
