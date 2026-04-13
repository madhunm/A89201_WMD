# CTX_MCU_DAU — A89201 Data Acquisition Unit (DAU)

## Source
- Document: DAU.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The DAU is a general-purpose 12-bit ADC with a 16-channel multiplexer. It is distinct from the CAU (which is dedicated to phase current measurement). The DAU is used for low-priority, non-real-time monitoring of voltages inside and external to the device.

**Do not use DAU for phase current measurement.** That function belongs to CAU.

---

## 2. Features

- 16 independently configurable measurement slots
- 16 selectable analog sources (one per slot, independently assigned)
- 12-bit resolution
- Configurable conversion sequence
- Each slot independently configured for channel, sample time, and trigger
- Conversion results stored in individual result registers
- DMA interface available
- Interrupt generation on sequence complete

---

## 3. Available Analog Sources (Channel Assignments)

| Channel     | Source Description                          |
|-------------|---------------------------------------------|
| AOUT        | Analog output mux (BIAS pin, internal nodes)|
| VTEMP       | On-chip temperature sensor                  |
| PD0–PD7     | External GPIO pin voltages (analog input)   |
| VBG         | Internal bandgap reference                  |
| GND         | Ground reference (for offset calibration)   |
| VBB         | Battery voltage sense (internal)            |
| VD50        | 5 V regulator output                        |
| VD33        | 3.3 V regulator output                      |

---

## 4. Operational Description

1. Configure each slot register: set channel source and sample time
2. Set `DAU_EN` bit in `DAU_CONTROL` register to 1
3. After ADC wake-up time (tAW), DAU begins conversion sequence
4. Conversions proceed through enabled slots in sequence
5. Each result stored in corresponding `DAU_RESULT[slot]` register
6. On sequence completion, DAU interrupt fires (if enabled) or DMA transfers results

Slots can be disabled individually. Only enabled slots are converted. This minimises conversion time for applications needing fewer channels.

---

## 5. Start-Up Constraint

Registers can be programmed while DAU is disabled. The module does not become active until `DAU_EN = 1`. After setting `DAU_EN = 1`, wait for ADC wake-up time before reading results.

---

## 6. Project-Specific Usage

The DAU is used for **non-critical background monitoring** tasks. These run asynchronously to the main control loop.

### Assigned DAU Channels

| Slot | Channel | Purpose                                    | Sample time |
|------|---------|--------------------------------------------|-------------|
| 0    | VTEMP   | Die temperature monitoring                 | 5 µs        |
| 1    | VBG     | Bandgap reference (ADC self-check)         | 1 µs        |
| 2    | GND     | Zero offset calibration reference           | 25 µs       |
| 3    | PD3     | Speed command (analog, 3-op-amp circuit)   | 1 µs        |
| 4–15 | Disabled| Reserved for future use                    | —           |

**PD3 analog mode:** Configured as `eGPIO_MODE_ANALOG_INPUT_SOURCE`. Driven by Buffer C output (G=4 non-inverting op-amp) of the external speed command circuit. Voltage represents a 4-level encoded BCM command (see Section 9). No GPIO interrupt configured on PD3 — polled by DAU at 100 Hz.

---

## 7. Temperature Monitoring

The DAU VTEMP channel provides on-chip temperature. Use with ADC calibration reference to compute actual temperature:
- Read VBG at slot 1 → compare to known bandgap value → compute ADC scale factor
- Read VTEMP at slot 0 → compute temperature in °C

Temperature monitoring should flag a warning at 115°C and a fault (safe state) at 125°C (A89201 Grade 1 limit).

---

## 8. Project-Specific Notes

- **DAU runs in background** — triggered by software at 10 Hz. Do not run DAU at the same rate as the control loop.
- **DAU VTEMP channel** gives junction temperature. Not calibrated to absolute accuracy but useful for trend monitoring. The SCU also has a hardware over-temperature interrupt — DAU temperature is a secondary check.
- **VBG channel** is a built-in calibration reference. If VBG reading deviates more than ±3% from nominal, DAU is unreliable — flag FAULT_DAU_UNRELIABLE.
- **Do not use DAU results in the motor control algorithm** (no latency guarantees). All control-loop inputs come from CAU (current) and SCI0 (angle).
- DAU results are 12-bit right-aligned in the result register. Conversion formula: V = (result / 4096) × VREF, where VREF = VIO = 5V (assumed — verify).

---

## 9. Speed Command Encoding Circuit (External Hardware)

### 9.1 Architecture

Three-op-amp circuit. All op-amps: AEC-Q100, 5V single supply, rail-to-rail I/O (e.g., LM321, MCP6001T, TLV341).

```
BCM low speed  (12V, Ri=0.5Ω) → [R_A1=75kΩ] → mid_A → [R_A2=10kΩ to GND]
                                  → Buffer A (×1) → [R_sumA=100kΩ] ──┐
                                                                      [node] → Buffer C (G=4) → PD3
BCM high speed (12V, Ri=0.5Ω) → [R_B1=160kΩ] → mid_B → [R_B2=10kΩ to GND]
                                  → Buffer B (×1) → [R_sumB=100kΩ] ──┘

Buffer C: non-inverting, G = 1 + Rf/Rg = 4,  Rf = 30 kΩ,  Rg = 10 kΩ
```

All resistors E24 standard values.

### 9.2 Voltage Levels (nominal 12V BCM, VREF = 5V)

| BCM low | BCM high | V_node | V_PD3 (after G=4) | ADC code | State         |
|---------|----------|--------|--------------------|----------|---------------|
| 0V      | 0V       | 0.00V  | 0.00V              | 0        | Park          |
| 0V      | 12V      | 0.353V | 1.41V              | 1156     | High speed    |
| 12V     | 0V       | 0.706V | 2.82V              | 2314     | Low speed     |
| 12V     | 12V      | 1.059V | 4.24V              | 3473     | Invalid/fault |

Decision thresholds in firmware (12-bit codes, VREF=5V):

```c
#define SPEED_ADC_PARK_MAX      572U   /* < 0.70V  → Park  */
#define SPEED_ADC_HIGH_MAX      1736U  /* < 2.12V  → High speed (60 RPM) */
#define SPEED_ADC_LOW_MAX       2892U  /* < 3.54V  → Low speed (40 RPM) */
                                       /* ≥ 2893   → FAULT_SPEED_CMD_INVALID */
```

### 9.3 Load Dump Protection (40V, Ri = 0.5Ω)

Divider attenuation limits op-amp input voltages to below VIO at worst-case load dump:

| Point        | Nominal (12V) | Load dump (40V, Ri=0.5Ω) | Limit (VIO) | Status |
|--------------|---------------|---------------------------|-------------|--------|
| Buffer A in  | 1.41V         | 4.68V                     | 5.00V       | ✓      |
| Buffer B in  | 0.71V         | 2.35V                     | 5.00V       | ✓      |
| Buffer C in  | 0–1.06V       | ≤ 3.52V (both channels)   | 5.00V       | ✓      |
| PD3 (V_out)  | 0–4.24V       | clips to ~4.9V (VCC-0.1)  | 5.00V       | ✓      |

Ri = 0.5Ω is negligible vs. divider impedance (85kΩ / 170kΩ). The dominant protection is the resistive attenuation ratio of each divider.

During load dump, Buffer C output clips to ~4.9V (op-amp supply rail). ADC reads code ~4010, which exceeds SPEED_ADC_LOW_MAX → firmware enters safe state. No component damage. System returns to normal operation after load dump clears.

### 9.4 Firmware Decode

```c
typedef enum {
    eSPEED_CMD_PARK      = 0,
    eSPEED_CMD_HIGH      = 1,   /* 60 RPM */
    eSPEED_CMD_LOW       = 2,   /* 40 RPM */
    eSPEED_CMD_FAULT     = 3,
} eSpeedCmd_t;

eSpeedCmd_t SpeedCommand_UpdateFromADC(void) {
    uint16_t code = DAU_RESULT[3];  /* Slot 3 = PD3 */

    if (code <= SPEED_ADC_PARK_MAX)  return eSPEED_CMD_PARK;
    if (code <= SPEED_ADC_HIGH_MAX)  return eSPEED_CMD_HIGH;
    if (code <= SPEED_ADC_LOW_MAX)   return eSPEED_CMD_LOW;

    /* Both BCM lines active, or load dump clipping */
    EnterSafeState(FAULT_SPEED_CMD_INVALID);
    return eSPEED_CMD_FAULT;
}
```

### 9.5 VREF Dependency

All voltage levels and ADC thresholds above assume VREF = VIO = 5V. If the DAU uses an internal reference lower than 3.2V, the 2.82V and 4.24V levels will saturate the ADC. In that case, rescale all four divider resistors to reduce V_PD3_max below VREF, and recalculate thresholds.
