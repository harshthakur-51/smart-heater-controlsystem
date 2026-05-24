#pragma once

#include "heater_controller.h"

void display_init(void);
void display_set_status(heater_status_t status);
void display_refresh_tick(void);
