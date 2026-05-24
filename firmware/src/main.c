#include "app.h"

#include "board.h"
#include "config.h"

int main(void) {
    uint32_t last_tick_ms;

    app_init();
    last_tick_ms = board_millis();

    while (1) {
        const uint32_t now = board_millis();
        if ((uint32_t)(now - last_tick_ms) >= APP_TICK_MS) {
            last_tick_ms += APP_TICK_MS;
            app_tick_10ms();
        }
    }
}
