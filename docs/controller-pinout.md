# Controller Pinout Mapping

This mapping is derived from the final controller schematic screenshots shared for
the smart-heater controller. It records the firmware-facing intent of each major
net so `firmware/src/board.c` can be converted from stubs to STM8 register code.

## High-Confidence Nets

| Firmware function | Schematic evidence | Firmware API |
| --- | --- | --- |
| Heater output | `MCU_HEATER` net leaves controller sheet and enters heater switching sheet through R21/MOC3063, then power triac | `board_heater_set()` |
| NTC temperature input | I2 connector labelled `NTC`, with divider/pull-up network near MCU analog pins | `board_ntc_adc_read()` |
| Motor output | J3 `MOTOR`, D6 flyback diode, Q7 AO3400A low-side MOSFET, R19/R20 gate network | `board_motor_set()` |
| Buzzer feedback | LS1 sounder driven by Q6 SS8050-G stage | `board_buzzer_set()` |
| Display segments | U3 74HC595 outputs Q0-Q7 drive DS1 cathodes/segments | `board_shift_*()` |
| Display digit enables | Q2-Q5 transistor bank drives DS1 common-anode digit lines | `board_display_digit_enable()` |

## Working STM8 Assignment Notes

| Function | STM8 pin candidate | Notes |
| --- | --- | --- |
| Heater command | `PD3 / TIM2_CH2 / AIN4` | Visible `MCU_HEATER` net route to heater circuit |
| NTC ADC | `PD5 / AIN5` | Connected to I2 NTC divider candidate |
| Motor gate | `PC7 / TIM1_CH2` | Routes toward R20/Q7 motor MOSFET gate network |
| Buzzer/haptic | `PD4 / BEEP / TIM2_CH1` | Natural fit for LS1/Q6 feedback stage |
| 74HC595 DS | `PC6 / SPI_MOSI` | Serial data candidate |
| 74HC595 SHCP | `PC5 / SPI_SCK` | Serial clock candidate |
| 74HC595 STCP | `PC4 / TIM1_CH4` | Latch clock candidate |

## Items To Confirm From Native Schematic/Netlist

The screenshot is enough for the firmware architecture, but these exact GPIOs
should be confirmed before final register-level code:

- four capacitive-button input pins;
- Q2-Q5 digit-enable GPIO order;
- whether haptic is a separate actuator or should share buzzer/motor feedback;
- whether thermal-fuse status is MCU-readable or only a hardware series safety path;
- exact NTC ADC pin if the final netlist shows `PD6/AIN6` instead of `PD5/AIN5`.

Once the netlist is available, `board.c` can be converted to direct STM8 register
writes without changing the higher-level mode/state-machine code.
