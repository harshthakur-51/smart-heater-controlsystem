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
| `board.c` | STM8S003F3P6 final-schematic adaptation layer |

## Final Schematic Pinout Basis

`board.c` is now documented against the final controller schematic you shared.
The high-confidence public mapping is:

| Function | STM8 / net basis |
| --- | --- |
| Heater command | `MCU_HEATER`, routed to MOC3063 optotriac and main triac heater path |
| NTC input | I2 / NTC divider, ADC input candidate `PD5/AIN5` |
| Motor output | J3 / MOTOR through AO3400A low-side MOSFET path |
| Buzzer feedback | LS1/Q6 output stage, `PD4/BEEP` candidate |
| Display segments | U3 74HC595 serial driver |
| Display digits | Q2-Q5 common-anode digit-enable transistors |
| Buttons | Four capacitive-button GPIO inputs on the controller sheet |

Before replacing stubs with register writes, confirm the visually dense display
and button traces against the Altium netlist or native schematic export. The
heater, NTC, motor, buzzer, and 74HC595 functional blocks are already reflected
in the board layer.

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
