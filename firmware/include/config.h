#pragma once

#include <stdint.h>

#define APP_TICK_MS                 10U

#define TEMP_TEA_C_X10              850
#define TEMP_COFFEE_C_X10           920
#define TEMP_KEEP_WARM_C_X10        600
#define TEMP_HYSTERESIS_C_X10       20
#define TEMP_OVERTEMP_C_X10         1050
#define TEMP_SENSOR_MIN_C_X10       (-100)
#define TEMP_SENSOR_MAX_C_X10       1250

#define HEATER_MAX_ON_TIME_MS       900000UL
#define KEEP_WARM_TIMEOUT_MS        1800000UL
#define DISPLAY_REFRESH_PERIOD_MS   2U
#define DISPLAY_STATUS_PERIOD_MS    500U

#define BUTTON_DEBOUNCE_TICKS       4U
#define BUTTON_LONG_PRESS_TICKS     120U

#define FEEDBACK_CLICK_MS           45U
#define FEEDBACK_MODE_ON_MS         70U
#define FEEDBACK_MODE_GAP_MS        50U
#define FEEDBACK_FAULT_ON_MS        120U
#define FEEDBACK_FAULT_GAP_MS       120U
