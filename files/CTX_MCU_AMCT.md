# CTX_MCU_AMCT — A89201 Advanced Motor Control Timer (AMCT)

## Source
- Document: AMCT.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The AMCT offloads time-critical motor control timing from the CPU. It provides dedicated hardware for Hall-sensor-based commutation, sensorless BEMF-based commutation, initial position detection (IPD), and current polarity detection. It generates interrupt signals that go directly to the NVIC for minimal latency response.

In this project, the AMCT's **Hall-based subsystem is the primary commutation path** (three APS12202 Hall latches on the rotor). The **BEMF subsystem acts as a cross-check** and fallback for windmilling detection.

---

## 2. Features

### Hall Sensor Subsystem
- Configurable debounce filter (removes Hall signal noise/chatter)
- Phase shift generation (compensates mechanical Hall misalignment or applies phase advance/delay)
- Dedicated timer to measure Hall sensor signal frequency (→ speed)
- Commutation sector report (tells firmware which phase pair to energise)
- Interrupt triggers for Hall state changes

### BEMF Subsystem
- Degauss filter (removes blanking period noise after PWM switching)
- Debounce filter (removes BEMF comparator chatter around zero-crossing)
- Phase disable signal generation (opens BEMF window at correct time)
- BEMF zero-crossing error measurement (error between expected and actual ZC position)
- BEMF tacho signal to GTU (for speed measurement during sensorless/windmilling)
- Automatic phase-disable window generation for BEMF sampling

### IPD (Initial Position Detection) Subsystem
- Hardware that samples BEMF comparators and phase comparators during current injection pulses
- Assists with sensorless startup (not needed if Hall sensors are always present)

### Current Polarity Detection (CPD)
- Samples phase comparator outputs at correct timing
- Reports current direction per phase
- Used in some FOC implementations for current sign recovery

---

## 3. Hall Sensor Input Configuration

Hall sensor inputs are GPIO pins PD0–PD7. The AMCT uses register settings to select which GPIO pins are connected to the Hall sensor inputs.

For this project:
- Hall A → PD0
- Hall B → PD1
- Hall C → PD2

Configure AMCT Hall input selection registers to map PD0/PD1/PD2 to Hall_A/B/C.

---

## 4. Hall Commutation Sector Table

For a standard 3-phase BLDC with APS12202 hall latches (active-LOW output when south field present):

| Hall A | Hall B | Hall C | Sector | Energise         | Description           |
|--------|--------|--------|--------|------------------|-----------------------|
| 1      | 0      | 1      | 1      | A+, B−           | Phase A high, B low   |
| 1      | 0      | 0      | 2      | A+, C−           | Phase A high, C low   |
| 1      | 1      | 0      | 3      | B+, C−           | Phase B high, C low   |
| 0      | 1      | 0      | 4      | B+, A−           | Phase B high, A low   |
| 0      | 1      | 1      | 5      | C+, A−           | Phase C high, A low   |
| 0      | 0      | 1      | 6      | C+, B−           | Phase C high, B low   |
| 0      | 0      | 0      | Fault  | All off           | Invalid               |
| 1      | 1      | 1      | Fault  | All off           | Invalid               |

**Note:** This table assumes a specific motor winding/magnet orientation. Verify rotation direction at commissioning. If motor runs in reverse, swap any two phase connections or invert the table index.

The AMCT commutation section register reports the current sector based on the Hall state. Firmware reads this register to determine which PGU output configuration to apply.

---

## 5. Hall Debounce Filter

The debounce filter removes glitches on Hall sensor outputs. The filter requires the Hall signal to be stable for a programmable duration before accepting the transition as valid.

Configure debounce period to:
- Minimum: 10 µs (removes spikes from PWM switching coupling)
- Maximum: should not exceed 1/10 of minimum expected Hall period

At 60 RPM output (motor 2340 RPM):
- Hall period ≈ 6.4 ms
- Maximum debounce = 640 µs

At 40 RPM output (motor 1560 RPM):
- Hall period ≈ 9.6 ms
- Maximum debounce = 960 µs

Recommended debounce: **50 µs** (2 counts at 40 MHz × 1250 clock cycles). This filters PWM-frequency noise without affecting commutation timing at target speeds.

---

## 6. Hall Phase Shift

If the Hall sensors are not perfectly aligned mechanically (±5° is typical tolerance), the AMCT can apply a programmable phase shift to the Hall timing signals to correct the commutation advance/retard.

Phase shift register: angle in electrical degrees. Positive = advance commutation. Negative = delay.

For maximum efficiency and torque at target speeds, a small phase advance (5–15° electrical) is beneficial. Start at 0° and optimise empirically at target loads.

---

## 7. BEMF Subsystem Configuration

The BEMF comparators are connected via GDU → AMCT signal path. The AMCT receives:
- BEMF comparator outputs for phases A, B, C
- PGU phase disable signals (to know when BEMF window is open)
- PGU PWM_Centre signal (timing reference)

AMCT output to GTU: `bemf_comp_tacho` — this signal changes state at each BEMF zero-crossing and is used by TIMER3 to measure motor speed during sensorless/windmilling.

Degauss filter: blanks the BEMF comparator output for a programmable period after each PWM switching edge. This prevents false zero-crossing detection from ringing on the motor phase voltage.

Degauss period should be set to the longer of:
- MOSFET switching ringing decay time (typically 2–5 µs for automotive FETs)
- Dead time duration

Recommended: **5 µs** (400 counts at 80 MHz fPWM clock).

---

## 8. BEMF Zero-Crossing Error

The AMCT hardware measures the angular error between the expected BEMF zero-crossing position (based on the internal speed counter) and the actual zero-crossing event. This error is available in an AMCT register and can be used to:
1. Correct the internal commutation counter (closed-loop BEMF speed tracking)
2. Detect loss-of-synchronisation (if error exceeds ±30° electrical, motor has lost sync)

---

## 9. AMCT Interrupts

AMCT generates interrupt triggers sent to NVIC:

| Interrupt         | Event                                                       |
|-------------------|-------------------------------------------------------------|
| Hall state change | Any Hall sensor transition (debounce-filtered)              |
| BEMF zero-crossing| BEMF comparator crosses zero during open-phase window       |
| Speed counter     | Internal commutation counter overflow                       |
| Commutation sector| New sector determined from Hall or BEMF state              |

Hall state change interrupt is the highest priority motor control interrupt in this project (must result in commutation within <10 µs of the Hall edge).

---

## 10. Commutation Timing Budget

At 60 RPM output (motor 2340 RPM):
- Electrical period = (60 / N_pole_pairs) / 2340 RPM × 60 s = varies by motor
- For 2 pole pairs: electrical period = 60 / (2 × 2340) = 12.8 ms
- One sector = 12.8 ms / 6 = 2.13 ms

The Hall ISR must complete within 100 µs of the Hall edge (< 5% of sector period) to avoid visible torque ripple.

---

## 11. Project-Specific Notes

- **Hall A, B, C are connected to AMCT via GPIO PD0–PD2.** The AMCT Hall input selection registers must be set before enabling the AMCT. If left at default (no pin assigned), AMCT will not receive Hall transitions.
- **AMCT commutation sector output** is a register the firmware reads in the Hall ISR. The firmware then writes the appropriate compare values to PGU (which phases are active and their duty cycles). Do not compute the sector from GPIO reads in the ISR — use the AMCT hardware output for lowest latency.
- **Invalid Hall states (all-0 or all-1):** If AMCT reports an invalid sector, the firmware must immediately disable all gate outputs and enter fault mode. Invalid Hall = sensor failure or broken wire. This is an ASIL-B safety requirement.
- **Phase advance vs. phase delay for direction reversal:** To reverse wiper direction, do not swap hardware wires. Instead, change the sector table offset in AMCT (apply a 180° phase advance, equivalent to using the opposite-direction commutation sequence). This keeps the current-sensing polarity consistent.
- **Windmilling detection:** If the ignition is cycled while the wiper is in mid-sweep, the motor may be coasting. BEMF tacho → GTU TIMER3 will detect residual rotation. Firmware must synchronise to the spinning motor before applying PWM (open-loop ramp-up into a spinning motor causes current spike). The A89201 BEMF comparator provides this synchronisation capability — use it.
- **ASIL-B: Hall plausibility check:** The AMCT should never report two consecutive identical sectors (a Hall transition always changes the sector). If two consecutive sector reads are identical for >1 ms during commanded motion, one Hall sensor is stuck. Assert diagnostic fault, stop motor, log fault code.
