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
| 3    | PD3     | Speed command pin voltage (diagnostic)     | 1 µs        |
| 4    | PD4     | Speed command pin voltage (diagnostic)     | 1 µs        |
| 5–15 | Disabled| Reserved for future use                    | —           |

**Why measure PD3/PD4 with DAU?** The GPIO digital reading gives only HIGH/LOW. DAU gives the actual voltage, which can detect marginal signal levels (e.g., a PD3 sitting at 2.0 V when it should be 0 V or 5 V — indicating a degraded pull-down or a stuck-at condition). This supports ASIL-B diagnostic coverage.

---

## 7. Temperature Monitoring

The DAU VTEMP channel provides on-chip temperature. Use with ADC calibration reference to compute actual temperature:
- Read VBG at slot 1 → compare to known bandgap value → compute ADC scale factor
- Read VTEMP at slot 0 → compute temperature in °C

Temperature monitoring should flag a warning at 115°C and a fault (safe state) at 125°C (A89201 Grade 1 limit).

---

## 8. Project-Specific Notes

- **DAU runs in background** — triggered by software at 10 Hz or slower. Do not run DAU at the same rate as the control loop; this wastes bus bandwidth.
- **DAU VTEMP channel** gives a reading that represents junction temperature. It is not calibrated to absolute accuracy but is useful for trend monitoring and over-temperature detection. The SCU also has a hardware over-temperature interrupt — the DAU temperature reading is a secondary check.
- **VBG channel** is a built-in calibration reference. Reading it allows the firmware to verify that the ADC is functioning correctly. If the VBG reading deviates more than ±3% from its nominal value, the DAU is producing unreliable results and should be flagged as a diagnostic fault.
- **Do not use DAU results in the motor control algorithm** (no latency guarantees). All control-loop inputs come from CAU (current) and SCI0 (angle).
- DAU results are 12-bit right-aligned in the result register. Conversion formula: V = (result / 4096) × VREF, where VREF = VLR or internal reference (verify from GDU section).
