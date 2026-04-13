# CTX_MCU_CAU — A89201 Current Acquisition Unit (CAU)

## Source
- Document: CAU.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The CAU contains three dedicated 12-bit SAR ADCs for phase current measurement in three-phase motor control. These ADCs are not general-purpose — they are hardwired to the current-sense amplifier outputs and are the only correct path for current measurement. This is the real-time, safety-relevant measurement path.

**This project uses a single 2 mΩ common shunt.** All three phases share one shunt. Time-multiplexed current sampling is used: only one phase conducts through the shunt at any valid measurement instant during the PWM cycle.

---

## 2. Features

- 3 × 12-bit SAR ADCs (ADC_A, ADC_B, ADC_C)
- Configurable conversion speed: up to 1 µs minimum conversion time
- Configurable track-and-hold (T/H) trigger source and period
- 14 selectable trigger sources per channel
- Zero-current offset calibration with IIR filtering
- Automatic calibration subtraction from normal conversion results
- DMA handshake signals per channel
- Interrupt on conversion complete and error conditions

---

## 3. Trigger Sources (14 available per channel)

| Source Type     | Description                                   |
|-----------------|-----------------------------------------------|
| ADCANT_A/B/C    | PWM normal trigger for phase A/B/C (from PGU) |
| ADCACT_A/B/C    | PWM calibration trigger for phase A/B/C (from PGU) |
| Software trigger| `sw_trig` bit in CAU_TRIGx register            |
| GTU timer       | Any of 8 GTU timer compare/overflow events     |
| GPIO PD0–PD7    | External edge on any GPIO                      |

Selected via `NTSSEL[3:0]` (normal trigger) and `CTSSEL[3:0]` (calibration trigger) fields in `CAU_TRIGx` registers.

**In this project:** PGU generates `ADCANT` (normal) and `ADCACT` (calibration) triggers at the correct sampling instant within each PWM period. CAU_TRIG registers must be set to use PGU triggers.

---

## 4. Conversion Sequence

On trigger:
1. ADC conversion control signals T/H to capture the sense amplifier output
2. ADC sequences through conversion (12-bit, ≤1 µs)
3. Result stored in `CAU_RESULTx` register (12-bit, right-aligned)
4. Offset stored separately in `CAU_OFFSETx` register
5. Automatic calibration: `NORMRES = raw_result − CALRESx` (if auto-cal enabled)
6. Interrupt fires (if enabled) or DMA transfer occurs

---

## 5. Zero-Current Offset Calibration

The current sense amplifiers (CSP/CSM differential amp and CSO amp) have non-zero output at zero current due to offset voltage. CAU has automatic calibration:

1. A calibration trigger (`ADCACT` from PGU, fired when all PWM outputs are OFF — zero current phase) captures the amplifier output at zero current
2. This value is stored as `CAU_OFFSETx`
3. Optional IIR filter smooths the calibration over multiple cycles
4. In automatic mode, the CAU subtracts offset from every normal conversion result

**Calibration sequence in firmware (one-time at startup):**
```c
// Force all gate outputs LOW (zero current through shunt)
GDU_DisableAllPhases();
// Trigger calibration ADC on all three channels
CAU->CAU_TRIG_A |= SW_TRIG_CAL;
CAU->CAU_TRIG_B |= SW_TRIG_CAL;
CAU->CAU_TRIG_C |= SW_TRIG_CAL;
// Wait for calibration complete flag
while (!(CAU->CAU_STATUS & CAL_DONE));
// Offset now in CAU_OFFSET_A/B/C
// Enable automatic offset subtraction
CAU->CAU_CTRL |= AUTO_CAL_EN;
```

---

## 6. ADC Result Registers

| Register      | Address Offset | Description                      |
|---------------|----------------|----------------------------------|
| CAU_RESULT_A  | per base addr  | Phase A current (12-bit)         |
| CAU_RESULT_B  | per base addr  | Phase B current (12-bit)         |
| CAU_RESULT_C  | per base addr  | Phase C current (12-bit)         |
| CAU_OFFSET_A  | per base addr  | Phase A zero-current offset      |
| CAU_OFFSET_B  | per base addr  | Phase B zero-current offset      |
| CAU_OFFSET_C  | per base addr  | Phase C zero-current offset      |

---

## 7. Current Calculation

With 2 mΩ shunt, CSO amplifier (default gain = 20 V/V, pedestal = 1.2 V), VLR = 3.3 V (verify), 12-bit ADC:

```
V_shunt     = I_phase × 0.002
V_amp_out   = V_shunt × gain + V_pedestal
            = I_phase × 0.002 × 20 + 1.2
            = I_phase × 0.04 + 1.2

ADC_count   = (V_amp_out / VLR) × 4096
            = ((I × 0.04 + 1.2) / 3.3) × 4096

I_phase [A] = ((ADC_count / 4096 × VLR) − V_pedestal) / (gain × R_shunt)
            = ((ADC_count × 3.3 / 4096) − 1.2) / (20 × 0.002)
            = ((ADC_count × 0.000806) − 1.2) / 0.04
```

**At 6 A continuous:** V_out = 6 × 0.04 + 1.2 = 1.44 V → ADC = 1786 counts (43.6% of full scale)
**At 30 A stall:** V_out = 30 × 0.04 + 1.2 = 2.4 V → ADC = 2979 counts (72.7% of full scale). No saturation. ✓

**If differential amplifier is used instead (DAG=10, gain=20 V/V, no pedestal, full-scale ±1.2 V):**
- At 30 A: V_diff = 30 × 0.002 × 20 = 1.2 V (exactly at full scale). Use DAG=5 (gain=10) for 30 A headroom.

---

## 8. Interrupt Sources

- Conversion complete (normal)
- Calibration complete
- ADC overflow error
- Track-and-hold error

All routed to NVIC. Normal conversion complete interrupt triggers the control ISR.

---

## 9. Single-Shunt Measurement Strategy

With one common shunt, only one phase current can be measured at a time. Three valid measurement windows exist within each PWM cycle. PGU generates three separate `ADCANT` triggers (one per phase) timed to each active phase interval.

The three ADC channels (ADC_A, ADC_B, ADC_C) are triggered sequentially:
- ADC_A triggered when phase A is active (high-side A ON, low-side B/C conducting through shunt)
- ADC_B triggered for phase B
- ADC_C triggered for phase C

The third phase current is computed from Kirchhoff's current law: I_C = −(I_A + I_B), reducing measurement error accumulation. Firmware should use this to cross-check the measured third channel.

---

## 10. Project-Specific Notes

- **Gain selection for single shunt (2 mΩ):** For 30 A stall, CSO gain of 20 V/V is acceptable. With gain=20 and pedestal=1.2 V, the output at 30 A is 2.4 V (72% of 3.3 V ADC input range). This provides headroom. For the differential amp path, use DAG=5 (gain=10) if routing to differential ADC input; DAG=10 (gain=20) saturates at 30 A.
- **CAU_RESULT registers must be read (or DMA-transferred) before the next PWM trigger fires** — the registers may be overwritten. At 20 kHz PWM, there are 50 µs per cycle.
- **IIR filter on calibration** reduces noise in the offset estimate. Enable it. Recommended IIR coefficient: 0.125 (1/8 averaging). This means the calibration offset tracks slow thermal drift but ignores single-cycle noise spikes.
- **ASIL-B requirement:** The CAU result must be checked for plausibility on every cycle. An instantaneous current jump from 5 A to 35 A in one cycle is physically impossible given motor inductance. If such a jump occurs, the reading is likely corrupt — discard and use previous value. After 3 consecutive implausible readings, enter safe state.
- **Stall detection:** If `CAU_RESULT > stall_threshold` for more than `N_stall_cycles`, disable gate outputs. Stall threshold corresponds to 30 A: ADC ≈ 2979 counts (using CSO amp). Set `stall_threshold = 2900` (with 2% margin). `N_stall_cycles` should correspond to 100 ms (2000 cycles at 20 kHz).
