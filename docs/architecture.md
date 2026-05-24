# Architecture

The design separates high-voltage power conversion, low-voltage control, sensing, user interface, and AC load switching.

```mermaid
flowchart LR
  AC[AC mains input] --> PROTECT[Fuse, MOV, X2 capacitor]
  PROTECT --> RECT[Bridge rectifier]
  RECT --> HV[High-voltage DC bus]
  HV --> FLYBACK[VIPer22A flyback converter]
  FLYBACK --> ISO[Transformer isolation]
  ISO --> RAIL12[12 V output rail]
  RAIL12 --> REG5[5 V regulator]
  REG5 --> MCU[STM8S003F3P6 MCU]
  MCU --> DISPLAY[Shift register and 7-segment display]
  MCU --> NTC[NTC temperature input]
  MCU --> MOTOR[Motor MOSFET driver]
  MCU --> OPTO[MOC3063 optotriac]
  OPTO --> TRIAC[Power triac]
  TRIAC --> HEATER[Heater load and thermal fuse]
```

## Control Flow

1. The offline SMPS generates an isolated low-voltage rail for the controller.
2. The STM8 controller reads temperature feedback through the NTC input network.
3. Display outputs are multiplexed through a serial shift register and digit driver transistors.
4. Heater enable is isolated through the optotriac before driving the power triac.
5. Motor output is handled separately through a low-side MOSFET and flyback diode.
6. Any invalid temperature state or over-temperature condition should force heater output off and latch a fault until recovery policy is satisfied.

## Design Intent

The public architecture emphasizes cost-aware redesign while keeping major safety boundaries visible: isolated power conversion, separated sensing/control logic, optically isolated AC switching, and thermal protection in the load path.
