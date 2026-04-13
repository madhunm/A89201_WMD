# CTX_A89201_DATASHEET — Allegro A89201C Motor Driver SoC

## Source
- Document: DS_A89201C_LIMITED_FINAL_R5 (Rev. 5, January 22, 2026)
- Part number in use: A89201KEVSR-C (128 kB flash, 5 V regulator, LIN + 2 SCI, Grade 1)
- Classification: Allegro Confidential / Limited Distribution

---

## 1. Device Overview

The A89201C is an automotive SoC integrating an ARM Cortex-M4F CPU with a three-phase N-channel MOSFET gate driver, current-sense amplifiers, and LIN transceiver in a single 48-pin QFN package. It is the sole IC running firmware in this project. All peripheral module documents (CPU, GPIO, DMA, SCI, LIN, SCU, DAU, CAU, GTU, GDU, FLI, PGU, AMCT) are A89201C internal peripheral user manuals.

**Not an external MCU with a separate driver.** The gate driver, charge pumps, current sense amplifiers, and CPU are all on the same die.

---

## 2. Package
- 48-terminal QFN, 7 mm × 7 mm, 0.9 mm nominal height
- Exposed thermal pad, wettable flank
- Suffix: EV

---

## 3. Supply Voltages

| Rail       | Min  | Typ  | Max  | Description                                  |
|------------|------|------|------|----------------------------------------------|
| VBB        | 6.5 V| —    | 50 V | Main supply (connected through reverse-battery protection) |
| VBRG       | 5.5 V| 13.6 V| 50 V| Sense input to top of MOSFET bridge         |
| VD50       | 5 V  | —    | —    | 5 V logic supply (LDO output)               |
| VD33       | 3.3 V| —    | —    | 3.3 V supply (LDO output)                  |
| VIO        | —    | —    | —    | GPIO logic supply; short to VD50 for 5 V I/O |
| VDD        | 1.08 V|1.2 V| 1.32 V| Core digital supply                        |
| VLR        | —    | —    | —    | Sense-amplifier output reference supply     |

Quiescent sleep current (VBB): ≤15 µA at VBB = 12 V, LIN = 12 V.

---

## 4. CPU Core
- ARM Cortex-M4F @ 40 MHz (up to 50 MIPS)
- FPU: IEEE 754 compliant
- ARMv7-M Thumb instruction set with DSP extensions
- 32-bit hardware divider
- NVIC: 20 interrupt vectors, 16 priority levels
- Memory Protection Unit (MPU)
- SWD 2-wire debug (TEST/SWDIO pin)
- 8 interrupt priority levels

---

## 5. Memory Map

| Address Range          | Size    | Description          |
|------------------------|---------|----------------------|
| 0x0000_0000–0x0000_7FFF| 32 kB   | Boot ROM             |
| 0x0001_0000–0x0001_1FFF| 8 kB    | Instruction RAM (IRAM)|
| 0x0800_0000–0x0803_FFFF| 128 kB  | Flash memory         |
| 0x2000_0000–0x2000_7FFF| 32 kB   | Data SRAM            |
| 0x4000_0000–0x4000_FFFF| 64 kB   | APB peripheral region|
| 0x4001_0000–0x4001_FFFF| 64 kB   | PWM generator region |

EEPROM: 64 bytes (non-volatile, on-chip).

---

## 6. Peripheral Units Summary

| Module | Function                                          |
|--------|---------------------------------------------------|
| GPIO   | 8 external bidirectional pins (PD0–PD7), 8 internal |
| DMA    | 3-channel DMA                                     |
| SCI    | 2× SPI/UART (SCI0, SCI1)                         |
| LIN    | LIN 1.3/2.2 / ISO17987-3 transceiver + DLL       |
| SCU    | Clock, ECC, watchdog, fault management, pin mux   |
| DAU    | 16-channel general-purpose ADC                    |
| CAU    | 3× 12-bit SAR ADC dedicated to phase current      |
| GTU    | 8× 16-bit general-purpose timers                  |
| GDU    | 3-phase gate driver unit + diagnostics            |
| FLI    | Flash interface with ECC and prefetch             |
| PGU    | 3-channel PWM generator (80 MHz clock)            |
| AMCT   | Advanced motor control timer (Hall + BEMF)        |

---

## 7. Gate Driver

- Drives 3-phase N-channel MOSFET bridge (external MOSFETs required)
- Bootstrap gate drive for high-side N-channel MOSFETs
- Charge pump (VREG) for gate supply at VBB ≥ 7 V
- VCP charge pump: enables 0–100% duty cycle without restriction
- Programmable gate drive strength and slew rate
- Adjustable cross-conduction (dead time) protection
- Per-phase VDS monitoring
- ENABLE pin: active-high, enables gate outputs

Gate output voltage range:
- High-side: VCX − 16 V to VCX + 0.3 V (transient: −18 V to VCX + 0.3 V)
- Low-side: VREG − 16 V to 18 V

---

## 8. Current Sense (Single Shunt)

Project uses a 2 mΩ common shunt (CSP/CSM inputs).

Two amplifiers are available:
1. **Differential amplifier** (CSP/CSM → differential output VOD): Full-scale ±1.2 V, gain 10–40 V/V (configurable), input offset ±1 mV (after calibration), 1 µs settling time. Output goes to CAU ADC.
2. **CSO amplifier** (single-ended, ground-referenced): Default gain 20 V/V, pedestal voltage 1.2 V by default.

Gain settings (DAG / SAG pairs):
| DAG | SAG | VIDMAX     |
|-----|-----|------------|
| 5   | 10  | ±240 mV    |
| 10  | 20  | ±120 mV    |
| 20  | 40  | ±60 mV     |
| 25  | 50  | ±48 mV     |

For 6 A continuous / 30 A stall through 2 mΩ shunt:
- V at 6 A = 12 mV → use DAG = 20 (SAG = 40, VIDMAX = ±60 mV) — 6 A maps to 240 mV differential output
- V at 30 A = 60 mV → same gain saturates at exactly 30 A. **Recommendation: verify gain selection to prevent saturation at stall. Consider DAG = 10 for 30 A headroom (±120 mV → 60 A full-scale).**

---

## 9. GPIO Electrical Characteristics

| Parameter        | Min         | Typ    | Max         |
|------------------|-------------|--------|-------------|
| Input low (VIL)  | —           | —      | 0.3 × VIO  |
| Input high (VIH) | 0.7 × VIO  | —      | —           |
| Hysteresis       | 250 mV      | 550 mV | —           |
| Pull-up (RPU)    | 25 kΩ       | 50 kΩ  | 80 kΩ       |
| Pull-down (RPD)  | 25 kΩ       | 50 kΩ  | 80 kΩ       |

---

## 10. BEMF and Phase Comparators

- Integrated BEMF comparator for sensorless pre-rotation detection
- AMCT block processes BEMF signals for frequency, angle and direction detection
- Phase comparators available on all three phases
- degauss and debounce filters in AMCT

---

## 11. LIN Interface

- LIN2.x / ISO 17987-3 / J2602 compatible
- Supports physical layer + data link layer
- Can also receive LIN pin as PWM input via GTU
- LIN wake from sleep supported
- LIN pin rated ±40 V to 50 V (bus)

---

## 12. Data Acquisition ADC (DAU)

- 16 configurable channels including: GPIO pins (PD0–PD7), VTEMP, AOUT, VBG, GND
- Used for general monitoring: supply voltage, temperature, sensor biases
- Not used for phase current — that is the CAU

---

## 13. Diagnostics (GDU)

- Per-phase VDS over-current detection
- Phase short to battery / GND detection
- Bootstrap undervoltage
- Charge pump fault
- Over-temperature
- Q&A watchdog between MCU and GDU
- All faults reported via GDU register set over internal serial bus; IRQ to CPU

---

## 14. Absolute Maximum Ratings (key)

| Terminal         | Limit           |
|------------------|-----------------|
| ENABLE, PD0–PD7  | −0.3 V to 6 V  |
| LIN              | −40 V to 50 V  |
| CSP, CSM         | −10 V to 10 V  |
| CA, CB, CC (bootstrap) | −0.3 V to 71 V |
| SA, SB, SC (phase) | −18 V to VCX + 0.3 V transient |

---

## 15. Temperature Grade

- AEC-Q100 Grade 1: −40°C to 125°C junction temperature
- Flash: 10,000 program cycles, 15 years retention (up to 125°C)
- EEPROM: 5000 cycles at TJ < 35°C, 500 at 35–85°C, 50 at 85–150°C

---

## 16. External Component Requirements (Summary)

| Component | Value       | Note                              |
|-----------|-------------|-----------------------------------|
| CCP1      | 0.47 µF, 25 V | Charge pump cap                 |
| CCP2      | 2.2 µF, 25 V  | Charge pump cap                 |
| CVCP      | 2.2 µF, 25 V  | VCP decoupling                  |
| CVDD      | 2.2 µF, 6.3 V | Core supply decoupling (≥1 µF)  |
| CVD33     | 4.7 µF, 6.3 V | 3.3 V decoupling (≥2.2 µF)     |
| CVIO      | 2.2 µF, 6.3 V | VIO decoupling (≥1 µF)          |
| REXT      | 24.9 kΩ       | Oscillator reference resistor   |
| CBC (×3)  | Per GDU calc  | Bootstrap capacitors (CA/CB/CC) |

---

## 17. Project-Specific Notes

- **VIO must be shorted to VD50 (5 V)** for GPIO voltage levels compatible with APS12202 and A1333 outputs.
- **BIAS pin** can supply sensor bias (voltage or current mode) to APS12202 or A1333 if required.
- **ENABLE pin** is active-high; must be driven high to enable gate outputs. During fault conditions, pull low to disable the bridge.
- **TEST/SWDIO** must be connected to GND for normal operation; connect to VD33 for SWD debug.
- The **two MOSFET bridges for ASIL-B** require that the firmware implement periodic fault-checking against the GDU diagnostic registers.
- **Single shunt topology** is used (CSP/CSM on common bus return). All three phase currents are measured in time-multiplexed fashion. PGU generates timed CAU triggers to sample at correct point in PWM cycle.
