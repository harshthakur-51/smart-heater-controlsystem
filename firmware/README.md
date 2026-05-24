# STM8 Firmware Starter

This firmware is a public, sanitized STM8 control skeleton for the smart heater
project. It models the product behavior requested for the portfolio repository:

- Power on/off.
- Tea mode.
- Coffee mode.
- Keep-warm mode.
- Buzzer plus haptic acknowledgement patterns.
- Display driver refresh through a serial shift-register interface.
- Motor control output.
- Main heater control through the optotriac/triac drive.
- NTC temperature supervision.
- Four capacitive buttons.

## Module Map

| Module | Purpose |
| --- | --- |
| `app.c` | Wires buttons, NTC, control state, outputs, display, feedback |
| `heater_controller.c` | Heater mode state machine, targets, hysteresis, faults |
| `buttons.c` | Debounce and long-press detection for four capacitive keys |
| `ntc.c` | ADC-to-temperature conversion using a replaceable calibration table |
| `display.c` | Four-digit 7-segment buffer and 74HC595-style refresh |
| `feedback.c` | Non-blocking buzzer/haptic click, mode, power-off, and fault patterns |
| `board.c` | STM8S003F3P6 pin/register adaptation layer |

## Pin Mapping Work Still Needed

`board.c` intentionally contains stubs. Replace them with STM8 SPL, Cosmic, IAR,
or SDCC register code after confirming the final schematic/PCB pinout:

- NTC ADC channel.
- Four capacitive button inputs.
- MOC3063/triac drive output.
- Motor MOSFET output.
- Buzzer output.
- Haptic output, or map haptic pulses to the motor if no separate actuator exists.
- 74HC595 data, clock, latch lines.
- Display digit-enable transistors.
- 1 ms timer interrupt calling `board_tick_1ms_isr()`.
- Watchdog refresh.

## Default Targets

| Mode | Target |
| --- | --- |
| Tea | 85 C |
| Coffee | 92 C |
| Keep warm | 60 C |
| Over-temperature fault | 105 C |

Tune these values in `include/config.h` after product testing.

## Safety Behavior

The firmware defaults to heater-off on:

- NTC open/invalid reading.
- NTC short/invalid reading.
- over-temperature.
- thermal fuse fault input.
- excessive active heating timeout.

Fault reset is modeled as a long press on the power key.
