# CTX_MCU_PGU — A89201 Pulse Width Modulation Unit (PGU/PWMU)

## Source
- Document: PGU.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The PGU generates complementary PWM signals for the three-phase bridge. It is the timing heart of the motor drive. All gate drive signals, current sample triggers, and BEMF measurement windows originate from the PGU. It runs on an 80 MHz clock independent of the 40 MHz CPU clock.

---

## 2. Features

- 3 independent PWM generators (one per phase: A, B, C)
- 2 complementary outputs per channel: Hx (high-side) and Lx (low-side)
- 2 CAU trigger signals per channel: ADCANT (normal) and ADCACT (calibration)
- Independent control per channel
- Centre-aligned and edge-aligned modulation
- Dead time generation (integrated, programmable)
- Phase shift in centre-aligned mode
- Phase disable signal (Phasex_Dis) for sensorless BEMF sampling window
- PWM_Centre signal (high at centre of PWM period, low at start) for DAU, AMCT, GPIO
- Compatible with single and multiple shunt systems

---

## 3. Clock and Timer

- Clock source: fPWM = 80 MHz (fixed, independent of CPU clock)
- Timer TPWM: 14-bit counter (0 to 16383)
- Two modes: centre-aligned and edge-aligned

### Frequency Calculation

**Centre-aligned mode:**
```
f_PWM = fPWM / (2 × PERIOD_register)
```

For 20 kHz:
```
PERIOD_register = 80,000,000 / (2 × 20,000) = 2000
```

The counter counts up from 0 to PERIOD, then back down to 0. Each full up-down cycle is one PWM period.

**Edge-aligned mode:**
```
f_PWM = fPWM / PERIOD_register
```

**Resolution at 20 kHz (centre-aligned):**
```
Duty cycle steps = PERIOD_register = 2000
Resolution = 1/2000 = 0.05% per step ≈ 10.8 bits (effective)
```

For 12-bit resolution (4096 steps) at 20 kHz, the formula gives:
```
f_PWM = 80,000,000 / (2 × 4096) = 9765 Hz
```

At PERIOD = 2000 (20 kHz), duty cycle is set in 2000 steps (not 4096). The A89201 datasheet states "12-bit PWM at 20 kHz" — this uses the DAG/SAG gain chain with 80 MHz clock for sub-cycle precision control, not 4096 counter steps. The effective resolution is better than 11 bits at 20 kHz.

---

## 4. PWM Resolution

| fPWM setting | PERIOD register | Frequency | Effective duty steps |
|-------------|-----------------|-----------|----------------------|
| 20 kHz      | 2000            | 20 kHz    | 2000 (≈11 bit)       |
| 10 kHz      | 4000            | 10 kHz    | 4000 (≈12 bit)       |
| 5 kHz       | 8000            | 5 kHz     | 8000 (≈13 bit)       |

Project uses **20 kHz, PERIOD = 2000** in centre-aligned mode.

---

## 5. Outputs

| Signal      | Destination | Description                                         |
|-------------|-------------|-----------------------------------------------------|
| HA          | GDU GHA     | Phase A high-side gate signal                       |
| LA          | GDU GLA     | Phase A low-side gate signal (complementary to HA)  |
| HB, LB      | GDU GHB/GLB | Phase B gate signals                                |
| HC, LC      | GDU GHC/GLC | Phase C gate signals                                |
| ADCANT_A/B/C| CAU         | Normal ADC trigger per phase (sampling instant)     |
| ADCACT_A/B/C| CAU         | Calibration ADC trigger per phase (zero-current)    |
| Phasex_Dis  | AMCT, GPIO  | Phase disable (opens BEMF window)                   |
| PWM_Centre  | DAU, AMCT   | High at centre of PWM period                        |
| PWM_Compx   | GPIO, AMCT  | PWM compare output signal                           |

---

## 6. Centre-Aligned Mode (Used in This Project)

In centre-aligned mode the TPWM counter counts 0 → PERIOD → 0. The compare registers (one per phase, high-side and low-side) set the switching points symmetrically around the period centre. This has two advantages:
1. Current measurement at the counter peak or trough coincides with maximum or minimum instantaneous current — ideal sampling point for single-shunt current reconstruction
2. Symmetrical switching reduces harmonic content in the motor current

PWM_Centre goes HIGH when the counter reaches PERIOD (peak), and LOW when counter reaches 0 (trough). This signal is used by CAU and AMCT to synchronise sampling.

---

## 7. Dead Time

The dead time control unit inserts a delay between the Hx turn-off and Lx turn-on (and vice versa). This prevents shoot-through.

Dead time register value = dead time in fPWM clock counts.
```
Dead time counts = dead_time_ns × 80 MHz / 1e9
For 500 ns dead time: counts = 500 × 80 / 1000 = 40
```

Minimum recommended dead time: 200 ns (16 counts at 80 MHz). Start with 500 ns (40 counts) and reduce after characterisation.

---

## 8. ADCANT Trigger Timing (Single Shunt)

For single-shunt current reconstruction, each ADCANT trigger must fire at the instant when the target phase is the only active phase conducting through the shunt (all other phases off).

In centre-aligned mode, ADCANT for each phase is positioned:
- ADCANT_A: fires when only Phase A is switching (midpoint of Phase A's active window)
- ADCANT_B: fires at Phase B's active window midpoint
- ADCANT_C: fires at Phase C's active window midpoint

The PGU COMPARE register for ADCANT must be set to: `PERIOD − compare_value_for_phase + offset`. The exact offset depends on motor current and must be tuned during commissioning.

The ADCACT (calibration) trigger must fire when all three phases are OFF simultaneously (dead-time instant). Configure this to fire when TPWM = 0 (counter trough, before any phase turns on).

---

## 9. Phase Disable Signal

`Phasex_Dis`: when asserted, this signal removes the gate drive for the selected phase. Used in two ways:
1. BEMF sensorless: disable one phase to allow its BEMF to be measured (opens the freewheeling window)
2. AMCT block commutation: AMCT can automatically generate phase disable signals for trapezoidal commutation

---

## 10. Register Configuration Sequence

```c
void PGU_Init(void) {
    // Set PWM frequency: centre-aligned, 20 kHz
    PGU->PERIOD = 2000;            // 80 MHz / (2 × 2000) = 20 kHz
    PGU->MODE   = CENTRE_ALIGNED;

    // Dead time: 500 ns = 40 counts at 80 MHz
    PGU->DEADTIME = 40;

    // Initialise duty cycles to 0 (all off)
    PGU->COMPARE_HA = 0;
    PGU->COMPARE_HB = 0;
    PGU->COMPARE_HC = 0;

    // CAU trigger: ADCACT at counter trough (TPWM = 0)
    PGU->ADCACT_OFFSET = 0;

    // CAU trigger: ADCANT per phase — set after first commutation
    PGU->ADCANT_A_OFFSET = 100;    // Initial estimate; tune during commissioning
    PGU->ADCANT_B_OFFSET = 100;
    PGU->ADCANT_C_OFFSET = 100;

    // Enable PGU
    PGU->CTRL |= PGU_ENABLE;
}
```

---

## 11. Updating Duty Cycles (Control Loop)

PWM compare registers are updated by the CPU on each PWM cycle (in the PGU period interrupt or the CAU conversion-complete interrupt). The PGU latches new compare values at the start of each PWM period to prevent mid-cycle glitches.

```c
void ControlLoop_ISR(void) {
    // Read current (from CAU via DMA)
    float i_a = CurrentFromADC(dma_cau_buf[0]);
    float i_b = CurrentFromADC(dma_cau_buf[1]);
    float i_c = CurrentFromADC(dma_cau_buf[2]);

    // Read angle (from SCI0 DMA buffer, or poll A1333)
    float angle_deg = AngleFromA1333(dma_spi_buf);

    // Read speed (from GTU Hall period measurement)
    float speed_rpm = SpeedFromHallPeriod(gtu_hall_period);

    // Run control algorithm (FOC or trapezoidal)
    // ... outputs Va, Vb, Vc (0 to 1 duty cycle)

    // Write duty cycles to PGU
    PGU->COMPARE_HA = (uint16_t)(Va * PGU->PERIOD);
    PGU->COMPARE_HB = (uint16_t)(Vb * PGU->PERIOD);
    PGU->COMPARE_HC = (uint16_t)(Vc * PGU->PERIOD);
}
```

---

## 12. Project-Specific Notes

- **20 kHz PWM frequency** is appropriate for a wiper motor (typical wiper BLDC motors operate well at 15–25 kHz). Above 20 kHz is outside audible range, reducing acoustic noise complaints.
- **Centre-aligned mode is mandatory** for single-shunt current reconstruction. Edge-aligned mode does not provide a symmetrical sampling window.
- **The dead time register in PGU must be consistent with GDU dead time configuration.** If PGU inserts 500 ns software dead time but GDU is also adding hardware dead time, the total dead time doubles. Verify which unit controls dead time (usually PGU, with GDU providing a minimum hardware floor). Set GDU dead time to the minimum hardware value; set PGU to the total desired value.
- **Modulation limit at 20 kHz:** Maximum duty cycle is PERIOD−DEADTIME/PERIOD = (2000−40)/2000 = 98%. Minimum is DEADTIME/PERIOD = 2%. For the charge pump (VCP) to maintain 100% duty capability, the VCP must be configured in GDU — this avoids bootstrap starvation at high duty cycles.
- **Phase currents at 20 kHz:** The A89201 CAU specifies ≤1 µs conversion time. At 20 kHz (50 µs period), the ADC has adequate time to complete conversion before the next cycle. Trigger the ADCANT at mid-active-pulse, allow 2 µs for settling + conversion, read result before next period.
