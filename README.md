# Smart Heater Control System

Embedded control and power-electronics documentation for a redesigned smart heater platform. The project captures the public-facing architecture of an AC-powered heater controller: isolated flyback SMPS, STM8 MCU control board, display/shift-register interface, NTC sensing, motor output, and optotriac/triac heater switching.

This repository is a sanitized portfolio version. It documents engineering approach and control structure without publishing client-owned manufacturing files, production firmware, or certification material.

## System Blocks

| Block | Function |
| --- | --- |
| Isolated SMPS | AC input protection, bridge rectification, VIPer22A flyback conversion, optocoupler/TL431 feedback, 12 V rail, 5 V regulation |
| MCU controller | STM8S003F3P6 control logic, display drive, keypad/IO, NTC feedback, motor/heater commands |
| User interface | 7-segment display module driven through 74HC595-style serial expansion and digit switching |
| Temperature sensing | NTC input network for closed-loop heater supervision |
| Heater switching | MCU-driven MOC3063 optotriac and BTA/BTC-style triac AC load control with thermal fuse path |
| Motor output | Low-side MOSFET drive path with flyback protection |

## Repository Contents

- `docs/architecture.md`: block-level system architecture and signal flow.
- `docs/schematic-review.md`: notes extracted from the shared SMPS, MCU, and heater schematic sheets.
- `docs/safety-notes.md`: public documentation boundaries and mains-safety cautions.
- `firmware/`: STM8 embedded-C starter with on/off, tea, coffee, keep-warm modes, NTC supervision, display refresh, motor/heater outputs, and buzzer/haptic feedback.

## Public Scope

The repo is intended to demonstrate:

- embedded product redesign thinking;
- low-cost MCU-based control architecture;
- isolated offline power supply understanding;
- heater safety interlocks and fault state handling;
- documentation discipline for industrial electronics.

It is not a build guide for mains hardware. AC-powered heater products require qualified design review, creepage/clearance checks, thermal validation, fuse coordination, EMC/safety testing, and compliance approval before use.

## Next Steps

- Replace the board-adaptation stubs with reviewed original STM8 pin/register code.
- Add sanitized schematic exports or redrawn block diagrams.
- Add state-machine timing notes for heater, fan/motor, UI, and error display behavior.
- Add a short validation table: power rail checks, NTC response, triac trigger behavior, thermal cutoff, and fault recovery.
