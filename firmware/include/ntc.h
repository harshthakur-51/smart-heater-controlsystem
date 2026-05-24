#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int16_t temperature_c_x10;
    bool valid;
    bool short_circuit;
    bool open_circuit;
} ntc_reading_t;

void ntc_init(void);
ntc_reading_t ntc_from_adc(uint16_t adc_count);
