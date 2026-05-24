#include "display.h"

#include "board.h"

#define DISPLAY_DIGITS 4U
#define SEG_BLANK 0x00U
#define SEG_MINUS 0x40U

static uint8_t buffer[DISPLAY_DIGITS];
static uint8_t active_digit;

static uint8_t encode_digit(uint8_t digit) {
    static const uint8_t map[10] = {
        0x3FU, 0x06U, 0x5BU, 0x4FU, 0x66U,
        0x6DU, 0x7DU, 0x07U, 0x7FU, 0x6FU
    };
    return digit < 10U ? map[digit] : SEG_BLANK;
}

static uint8_t encode_letter(char letter) {
    switch (letter) {
        case 'C': return 0x39U;
        case 'E': return 0x79U;
        case 'F': return 0x71U;
        case 'H': return 0x76U;
        case 'O': return 0x3FU;
        case 'P': return 0x73U;
        case 'T': return 0x78U;
        case 'A': return 0x77U;
        case 'r': return 0x50U;
        default: return SEG_BLANK;
    }
}

static void shift_byte(uint8_t value) {
    for (uint8_t i = 0; i < 8U; ++i) {
        board_shift_data_set((value & 0x80U) != 0U);
        board_shift_clock_pulse();
        value <<= 1U;
    }
    board_shift_latch_pulse();
}

static void set_number(int16_t value_c_x10) {
    int16_t value_c = (int16_t)(value_c_x10 / 10);
    if (value_c < 0) {
        buffer[0] = SEG_MINUS;
        value_c = (int16_t)(-value_c);
    } else {
        buffer[0] = value_c >= 100 ? encode_digit((uint8_t)(value_c / 100)) : SEG_BLANK;
    }
    buffer[1] = value_c >= 10 ? encode_digit((uint8_t)((value_c / 10) % 10)) : SEG_BLANK;
    buffer[2] = encode_digit((uint8_t)(value_c % 10));
    buffer[3] = encode_letter('C');
}

void display_init(void) {
    for (uint8_t i = 0; i < DISPLAY_DIGITS; ++i) {
        buffer[i] = SEG_BLANK;
        board_display_digit_enable(i, false);
    }
    active_digit = 0;
}

void display_set_status(heater_status_t status) {
    if (status.mode == HEATER_MODE_FAULT) {
        buffer[0] = encode_letter('E');
        buffer[1] = encode_letter('r');
        buffer[2] = encode_letter('r');
        buffer[3] = encode_digit((uint8_t)status.fault);
        return;
    }

    if (status.mode == HEATER_MODE_OFF) {
        buffer[0] = SEG_BLANK;
        buffer[1] = encode_letter('O');
        buffer[2] = encode_letter('F');
        buffer[3] = encode_letter('F');
        return;
    }

    if (status.at_temperature && status.mode == HEATER_MODE_KEEP_WARM) {
        buffer[0] = encode_letter('H');
        buffer[1] = encode_letter('O');
        buffer[2] = encode_letter('T');
        buffer[3] = SEG_BLANK;
        return;
    }

    set_number(status.temperature_c_x10);
}

void display_refresh_tick(void) {
    for (uint8_t i = 0; i < DISPLAY_DIGITS; ++i) {
        board_display_digit_enable(i, false);
    }
    shift_byte(buffer[active_digit]);
    board_display_digit_enable(active_digit, true);
    active_digit++;
    if (active_digit >= DISPLAY_DIGITS) {
        active_digit = 0;
    }
}
