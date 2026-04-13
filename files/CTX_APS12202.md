# CTX_APS12202 — Allegro APS12202 Chopper-Stabilised Hall-Effect Latch

## Source
- Document: APS1220212Datasheet.pdf (APS12202-12-DS, Rev. 3, October 27, 2025, Allegro MicroSystems)

---

## 1. Device Overview

The APS12202 is a chopper-stabilised Hall-effect latch for automotive, industrial, and consumer applications. Three APS12202 devices are used in this project as rotor position sensors, mounted 120° apart around the BLDC motor stator to provide 6-step Hall-sector commutation feedback to the A89201 AMCT module.

The APS12202 and APS12212 are functionally identical except for switch-point sensitivity (APS12202 has lower BOP/BRP thresholds, making it suitable for weaker magnetic fields).

---

## 2. Features

- Chopper-stabilised (dynamic quadrature offset cancellation) — superior thermal stability
- Symmetrical switch points (BOP and BRP equal magnitude, opposite polarity)
- Output short-circuit protection
- Reverse battery protection (down to −20 V)
- AEC-Q100 automotive qualified
- Operating temperature: −40°C to 150°C
- Open-drain output — requires external or GPIO pull-up

---

## 3. Package Options

| Suffix | Package         | Mounting       |
|--------|-----------------|----------------|
| LH     | 3-pin SOT23W    | Surface mount  |
| UA     | 3-pin SIP       | Through-hole   |

Recommended for automotive PCB: **LH (SOT23W)**.

---

## 4. Pin Description

| Pin (LH) | Pin (UA) | Name | Function          |
|----------|----------|------|-------------------|
| 1        | 1        | VCC  | Power supply      |
| 2        | 3        | OUT  | Open-drain output |
| 3        | 2        | GND  | Ground            |

---

## 5. Electrical Characteristics

### Supply

| Parameter             | Symbol | Min  | Typ  | Max  | Unit |
|-----------------------|--------|------|------|------|------|
| Supply voltage        | VCC    | 3.0  | —    | 24   | V    |
| Reverse battery voltage | VRCC | −20  | —    | —    | V    |
| Supply current        | ICC    | —    | 2.0  | 3.0  | mA   |

### Output

| Parameter                  | Symbol     | Conditions             | Min | Typ  | Max  | Unit |
|----------------------------|------------|------------------------|-----|------|------|------|
| Output saturation voltage  | VOUT(SAT)  | IOUT = 20 mA, B > BOP  | —   | 185  | 500  | mV   |
| Output leakage current     | IOFF       | VOUT = 24 V, B < BRP   | —   | —    | 10   | µA   |
| Output current limit       | ION        | B > BOP                | 30  | —    | 60   | mA   |
| Continuous sink current    | IOUT       | Max rated              | —   | —    | 25   | mA   |
| Output rise time           | tR         | RLOAD=820 Ω, CL=20 pF  | —   | 0.2  | 2.0  | µs   |
| Output fall time           | tF         | RLOAD=820 Ω, CL=20 pF  | —   | 0.1  | 2.0  | µs   |
| Power-on time              | tPO        | VCC > 3.0 V            | —   | —    | 25   | µs   |
| Chopping frequency         | fC         | —                      | —   | 800  | —    | kHz  |

---

## 6. Magnetic Characteristics (APS12202)

South polarity (+) turns output ON. North polarity turns output OFF.

| Parameter         | Symbol | Conditions                   | Min  | Typ  | Max  | Unit |
|-------------------|--------|------------------------------|------|------|------|------|
| Operate point     | BOP    | −40°C to 150°C               | 5    | 25   | 50   | Gauss|
| Release point     | BRP    | −40°C to 150°C               | −50  | −25  | −5   | Gauss|
| Hysteresis (BOP−BRP) | BHYS | 25°C to TA(max)             | 10   | 50   | 100  | Gauss|
| Hysteresis (BOP−BRP) | BHYS | −40°C                       | —    | —    | 100  | Gauss|

Polarity convention: positive value = south pole field on branded face. Negative = north pole.

**Switching behaviour (latch):**
- Output turns ON (LOW) when B transitions through and exceeds +BOP (south field)
- Output stays ON until B decreases through BRP (north field threshold)
- Hysteresis prevents chatter at the threshold
- At power-on: if ambient field is between BOP and BRP, initial output state is indeterminate until the field crosses a threshold

---

## 7. Application Circuit

```
VCC (3–24 V)
     │
     ├── 0.1 µF ── GND   (bypass cap, close to device)
     │
  APS12202
     │  VCC pin (pin 1)
     │  OUT pin (pin 2) ──── PD0/PD1/PD2 (A89201 GPIO)
     │                  │
     │              (pull-up provided by A89201 GPIO internal RPU 30–70 kΩ)
     │  GND pin (pin 3) ── GND
```

No external pull-up resistor is required for this project. The A89201 GPIO internal pull-up (30–70 kΩ) is used.

**Output voltage when APS12202 is OFF (B < BRP):**
- VOUT(off) = VCC − (IOFF × RPU) = 5 V − (10 µA × 70 kΩ) = 4.3 V > VIH = 3.5 V ✓

**Output voltage when APS12202 is ON (B > BOP):**
- IOUT = VCC / RPU = 5 V / 30 kΩ = 167 µA (worst case)
- VOUT(SAT) at 167 µA ≈ negligible (spec is at 20 mA) — essentially 0 V < VIL = 1.5 V ✓

---

## 8. Thermal Characteristics

| Package | PCB                                    | RθJA    |
|---------|----------------------------------------|---------|
| LH      | 1-layer, copper limited to pads        | 228 °C/W|
| LH      | 2-layer, 0.463 in² copper each side    | 110 °C/W|
| UA      | 1-layer, copper limited to pads        | 165 °C/W|

At ICC = 2 mA, VCC = 5 V: PD = 10 mW. Negligible thermal rise.

---

## 9. Hall Sector Table (Three Latches, 120° Spacing)

With three APS12202 devices mounted 120° mechanically apart, the combined output pattern produces 6 unique sectors per electrical period. For a motor with P pole-pairs, one electrical period = 360°/P mechanical. For a typical 4-pole-pair motor, one electrical period = 90° mechanical.

| Sector | Hall A (PD0) | Hall B (PD1) | Hall C (PD2) | Motor state                |
|--------|--------------|--------------|--------------|----------------------------|
| 1      | 0            | 0            | 1            | Phase A+, Phase B−         |
| 2      | 0            | 1            | 1            | Phase A+, Phase C−         |
| 3      | 0            | 1            | 0            | Phase B+, Phase C−         |
| 4      | 1            | 1            | 0            | Phase B+, Phase A−         |
| 5      | 1            | 0            | 0            | Phase C+, Phase A−         |
| 6      | 1            | 0            | 1            | Phase C+, Phase B−         |
| —      | 0            | 0            | 0            | **FAULT** — all off        |
| —      | 1            | 1            | 1            | **FAULT** — all on         |

Output 0 = Hall output LOW (south field present, device conducting). Output 1 = Hall output HIGH (no/north field, device off, pulled up).

**Note:** The exact sector-to-phase mapping depends on the physical orientation of the magnets relative to the coils. The above table is a reference; actual commutation table must be verified by rotating the motor and measuring Hall transitions.

---

## 10. Timing Constraints

| Constraint                  | Value                         |
|-----------------------------|-------------------------------|
| Max switching speed         | Limited by tR/tF = 2 µs max  |
| At 60 RPM, 4-pole-pair motor| Hall edge every 4.17 ms → no speed concern |
| At 40 RPM, 4-pole-pair motor| Hall edge every 6.25 ms → no speed concern |
| Power-on time               | 25 µs max — ignore Hall inputs for first 25 µs after VCC applied |

At 60 RPM output (maximum speed), the motor-side shaft rotates at 60 × 39 = 2340 RPM. With P=4 pole pairs:
- Electrical frequency = 2340/60 × 4 = 156 Hz
- Time per sector = 1/(156 × 6) = 1.07 ms >> 2 µs rise time ✓

---

## 11. Project-Specific Notes

- **Three devices required:** PD0 = Hall A, PD1 = Hall B, PD2 = Hall C. All inputs to AMCT module via GPIO.
- **120° placement:** Physical placement accuracy matters. Misalignment beyond ±2° mechanical causes uneven torque ripple. The AMCT module can apply a software phase shift to compensate (see CTX_MCU_AMCT).
- **ASIL-B fault detection for Hall inputs:**
  - States 0b000 and 0b111 are invalid — assert Hall fault immediately
  - No transition during expected motion window = sensor stuck fault
  - Unexpected rapid transitions = noise or magnet problem
  - AMCT debounce filter removes glitch transitions — configure debounce period appropriately (recommend 10–50 µs at motor-side speeds in this application)
- **Power-on indeterminate state:** In the first 25 µs after VCC is applied to the APS12202, the output state is undefined (tPO). The A89201 firmware must not use Hall inputs until after tPO. The boot sequence must include a 50 µs hold after APS12202 VCC is stable before reading Hall state.
- **Bypass capacitor:** 0.1 µF ceramic placed within 2 mm of each APS12202 VCC/GND pins. Without this, the 800 kHz chopper clock can couple noise into adjacent circuits.
- **Hall sensor supply voltage:** The APS12202 can run from 3–24 V. In this design, connect VCC to the A89201 VD50 (5 V) rail. This is the same rail as VIO, maintaining consistent logic levels between Hall outputs and GPIO inputs.
- **No software debouncing required** if AMCT hardware debounce is enabled. If reading Hall inputs via GPIO interrupts (bypassing AMCT), software debouncing of ≥5 µs is required.
