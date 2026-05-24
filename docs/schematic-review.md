# Schematic Review Notes

These notes summarize the public-facing information visible in the shared schematic screenshots. They are not a substitute for production schematic review, PCB inspection, or safety certification.

## SMPS Sheet

Observed functional blocks:

- AC input with fuse, MOV surge protection, and X2 capacitor across line/neutral.
- MB10F bridge rectifier generating high-voltage DC.
- Bulk capacitor on the high-voltage bus.
- VIPer22AS-E offline flyback controller/switcher.
- Primary clamp/snubber network around the transformer primary.
- Transformer with isolated secondary winding.
- SR360 secondary rectifier, output filter capacitors, and 22 uH inductor.
- TL431 reference and PC817 optocoupler feedback loop.
- 12 V / 2 A output rail, later regulated for 5 V logic.

Engineering points to validate before any real hardware use:

- fuse rating and breaking capacity;
- MOV energy rating;
- X2 capacitor safety class;
- transformer insulation system;
- creepage and clearance on PCB layout;
- optocoupler isolation rating;
- output ripple and load transient behavior;
- thermal performance of the flyback switch and secondary rectifier.

## MCU Controller Sheet

Observed functional blocks:

- STM8S003F3P6 MCU.
- AMS1117-5.0 regulator from 12 V to 5 V logic rail.
- 74HC595 serial-to-parallel output expansion for display segment drive.
- LTC-4627G-style multi-digit 7-segment display module.
- Digit switching transistors and segment resistors.
- NTC connector and resistor network for temperature measurement.
- SS49E Hall sensor block.
- Motor connector driven through AO3400A MOSFET with flyback diode.
- SWIM/programmer header for STM8 programming/debug.

Engineering points to validate:

- regulator power dissipation from 12 V input;
- ADC scaling for NTC range and fault detection;
- display current per segment and per digit;
- motor current rating, diode rating, and MOSFET thermal margin;
- reset/programming accessibility;
- firmware fail-safe defaults on reset and brownout.

## Heater Switching Sheet

Observed functional blocks:

- MCU heater command drives MOC3063 optotriac through a series resistor.
- Optotriac output drives the power triac gate.
- Triac switches the AC heater path.
- Thermal fuse appears in series with heater/load path.
- Gate resistor and optotriac isolation provide low-voltage/high-voltage separation.

Engineering points to validate:

- triac current, surge, thermal, and dv/dt ratings;
- optotriac zero-cross behavior suitability for the load;
- heater load current under cold and hot conditions;
- thermal fuse placement and trip rating;
- PCB spacing between MCU-side and mains-side circuits;
- enclosure, grounding, strain relief, and production test requirements.
