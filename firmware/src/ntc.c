#include "ntc.h"

#include "config.h"

typedef struct {
    uint16_t adc;
    int16_t temp_c_x10;
} ntc_point_t;

/*
 * Placeholder table for a 10 k NTC divider into the STM8 ADC.
 * Replace with measured values from the final resistor network.
 */
static const ntc_point_t table[] = {
    {960, -100},
    {900, 0},
    {820, 200},
    {710, 400},
    {580, 600},
    {430, 800},
    {300, 1000},
    {210, 1200}
};

void ntc_init(void) {
}

ntc_reading_t ntc_from_adc(uint16_t adc_count) {
    ntc_reading_t reading;
    reading.temperature_c_x10 = 0;
    reading.valid = true;
    reading.short_circuit = false;
    reading.open_circuit = false;

    if (adc_count <= 5U) {
        reading.valid = false;
        reading.short_circuit = true;
        return reading;
    }

    if (adc_count >= 1018U) {
        reading.valid = false;
        reading.open_circuit = true;
        return reading;
    }

    if (adc_count >= table[0].adc) {
        reading.temperature_c_x10 = table[0].temp_c_x10;
    } else if (adc_count <= table[(sizeof(table) / sizeof(table[0])) - 1U].adc) {
        reading.temperature_c_x10 = table[(sizeof(table) / sizeof(table[0])) - 1U].temp_c_x10;
    } else {
        for (uint8_t i = 0; i < (uint8_t)((sizeof(table) / sizeof(table[0])) - 1U); ++i) {
            const ntc_point_t high = table[i];
            const ntc_point_t low = table[i + 1U];
            if (adc_count <= high.adc && adc_count >= low.adc) {
                const int16_t adc_span = (int16_t)(high.adc - low.adc);
                const int16_t temp_span = (int16_t)(low.temp_c_x10 - high.temp_c_x10);
                const int16_t adc_delta = (int16_t)(high.adc - adc_count);
                reading.temperature_c_x10 = (int16_t)(high.temp_c_x10 + ((int32_t)temp_span * adc_delta) / adc_span);
                break;
            }
        }
    }

    if (reading.temperature_c_x10 < TEMP_SENSOR_MIN_C_X10 || reading.temperature_c_x10 > TEMP_SENSOR_MAX_C_X10) {
        reading.valid = false;
    }

    return reading;
}
