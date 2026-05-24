#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    BUTTON_POWER = 0,
    BUTTON_TEA,
    BUTTON_COFFEE,
    BUTTON_KEEP_WARM,
    BUTTON_COUNT
} button_id_t;

#define BUTTON_MASK_POWER       (1U << BUTTON_POWER)
#define BUTTON_MASK_TEA         (1U << BUTTON_TEA)
#define BUTTON_MASK_COFFEE      (1U << BUTTON_COFFEE)
#define BUTTON_MASK_KEEP_WARM   (1U << BUTTON_KEEP_WARM)

void board_init(void);
void board_tick_1ms_isr(void);
uint32_t board_millis(void);
void board_watchdog_kick(void);

uint16_t board_ntc_adc_read(void);
uint8_t board_buttons_raw_mask(void);
bool board_thermal_fuse_ok(void);

void board_heater_set(bool enabled);
void board_motor_set(bool enabled);
void board_buzzer_set(bool enabled);
void board_haptic_set(bool enabled);

void board_shift_data_set(bool high);
void board_shift_clock_pulse(void);
void board_shift_latch_pulse(void);
void board_display_digit_enable(uint8_t digit, bool enabled);
