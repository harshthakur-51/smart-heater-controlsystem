# Validation Plan

This checklist is written for portfolio documentation and engineering review planning. It is not a substitute for compliance testing.

## Power Supply

| Check | Expected evidence |
| --- | --- |
| AC input protection | Fuse, MOV, and X2 capacitor ratings documented |
| DC bus stability | Bulk capacitor voltage and ripple checked under load |
| 12 V rail | Output voltage, ripple, load step, and thermal data |
| 5 V logic rail | Regulator dissipation and MCU brownout margin |
| Isolation | Transformer and optocoupler isolation ratings verified |

## Controller

| Check | Expected evidence |
| --- | --- |
| Reset behavior | Heater output remains off during reset and boot |
| NTC open fault | Heater disables and display/error state updates |
| NTC short fault | Heater disables and display/error state updates |
| Over-temperature | Heater disables before unsafe thermal rise |
| Watchdog | Firmware recovers or fails safe after lockup |

## Heater Switching

| Check | Expected evidence |
| --- | --- |
| Optotriac drive | MCU pin current within rating |
| Triac gate behavior | Reliable trigger at operating line/load conditions |
| Load current | Triac RMS/surge current margin documented |
| Thermal fuse | Series placement and trip rating reviewed |
| PCB layout | Mains/control separation reviewed on board files |

## User-Facing Behavior

| Check | Expected evidence |
| --- | --- |
| Display startup | Power-on state is deterministic |
| Mode changes | Button/display behavior matches product requirements |
| Fault display | User can distinguish sensor and thermal faults |
| Recovery | Fault reset policy is explicit and safe |
