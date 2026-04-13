# CTX_MCU_GPIO — A89201 GPIO Module

## Source
- Document: GPIO.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

- 16 total GPIO pins: PD0–PD15
- PD0–PD7: externally accessible, bidirectional, configurable as input or output
- PD8–PD15: internal die-to-die connections, unidirectional, not available for external use
- All 8 external GPIOs are individually and independently configurable

---

## 2. Electrical Characteristics

Valid at TJ = TBD, 4.0 V ≤ VD50 ≤ 5.5 V, 1.08 V ≤ VDD ≤ 1.32 V (VIO shorted to VD50 = 5 V for this project).

| Parameter          | Symbol  | Min         | Typ    | Max         | Unit |
|--------------------|---------|-------------|--------|-------------|------|
| Input low voltage  | VIL     | —           | —      | 0.3 × VIO  | V    |
| Input high voltage | VIH     | 0.7 × VIO  | —      | —           | V    |
| Input hysteresis   | VHYS    | 200 mV      | —      | —           | mV   |
| Pull-up resistor   | RPU     | 30 kΩ       | 50 kΩ  | 70 kΩ       | kΩ   |
| Pull-down resistor | RPD     | 30 kΩ       | 50 kΩ  | 70 kΩ       | kΩ   |

With VIO = 5 V: VIL ≤ 1.5 V, VIH ≥ 3.5 V.

---

## 3. Pin Multiplexing

GPIOs share function with peripheral interfaces. Mux is controlled by SCU registers:
- `sel_uart0`, `sel_uart1`: selects which PD pin is used as UART RX for SCI0/SCI1
- `sel_spi0`, `sel_spi1`: selects which PD pin is used as SPI RX (MISO) for SCI0/SCI1
- `sel_lin`, `sel_lctx`: LIN physical layer mux

PWM outputs (HA/LA/HB/LB/HC/LC from PGU) are routed through GPIO driver to GDU.

---

## 4. Interrupt Capability

Each of PD0–PD7 can generate an interrupt:
- Trigger selectable per pin: rising edge, falling edge, both edges, high level, low level, or disabled
- All route to NVIC as GPIO interrupt sources

---

## 5. Project Pin Assignment

This is the **project-mandated pin assignment** for this firmware. Do not deviate without updating this document.

| Pin | Direction | Function                        | Connected to         | Notes                                    |
|-----|-----------|----------------------------------|----------------------|------------------------------------------|
| PD0 | Input     | Hall latch A (rotor position)   | APS12202 #1 OUT      | Internal pull-up enabled                 |
| PD1 | Input     | Hall latch B (rotor position)   | APS12202 #2 OUT      | Internal pull-up enabled                 |
| PD2 | Input     | Hall latch C (rotor position)   | APS12202 #3 OUT      | Internal pull-up enabled                 |
| PD3 | Input     | Low speed command (vehicle)     | Vehicle BCM GPIO     | Internal pull-down; active-high          |
| PD4 | Input     | High speed command (vehicle)    | Vehicle BCM GPIO     | Internal pull-down; active-high          |
| PD5 | Input     | A1333 ABI channel A             | A1333 A/U (pin 14)   | GPIO interrupt, both edges               |
| PD6 | Input     | A1333 ABI channel B             | A1333 B/V (pin 13)   | GPIO interrupt, both edges               |
| PD7 | Input     | A1333 PWM angle output          | A1333 PWM (pin 4)    | GTU6 input, duty cycle measurement       |

**A1333 interface rationale:** The A89201 has 8 external GPIO pins. With 5 consumed by Hall latches and speed commands, only 3 remain (PD5–PD7). SPI requires 4 pins (CS + SCLK + MOSI + MISO) — one more than available. The chosen interface uses ABI A/B (PD5/PD6) for high-resolution incremental tracking plus PWM output (PD7) for absolute angle reference. The A1333 ABI Index output (pin 12) is not connected — the PWM output serves as the continuous absolute reference, making periodic index resyncing unnecessary.

PWM advantage over Index: PWM provides an absolute angle at every carrier period (1 ms at 1 kHz carrier). Index fires only once per 360° revolution. For a wiper operating over a ~100–120° arc, the index pulse position may never be crossed during normal operation. PWM has no such constraint.

ABI A/B edge rate at maximum speed (60 RPM output, 2048 PPR): 60/60 × 2048 × 4 edges/sec = 8192 edges/sec = 1 edge per 122 µs → well within ISR budget at 40 MHz.

**Speed command pin logic:**
- PD3 LOW and PD4 LOW = motor stopped (park)
- PD3 HIGH and PD4 LOW = low speed (40 RPM ±2 RPM)
- PD3 LOW and PD4 HIGH = high speed (60 RPM ±2 RPM)
- PD3 HIGH and PD4 HIGH = **INVALID** — must never occur; treat as fault

**Hall latch inputs:** APS12202 output is open-drain. GPIO internal pull-up is sufficient. No external pull-up required provided GPIO RPU (30–70 kΩ) is acceptable given APS12202 output leakage ≤ 10 µA. Verify voltage at PD0–PD2 when APS12202 output is OFF: VOH = VIO − (IOFF × RPU) ≈ 5 V − (10 µA × 70 kΩ) = 4.3 V > VIH = 3.5 V. ✓

---

## 6. Configuration Registers (Base Address: 0x40000000 region, per CPU address map)

Each GPIO has a control register:
- Direction bit: 0 = input, 1 = output
- Pull-up enable, pull-down enable (mutually exclusive for inputs)
- Output data register
- Input data register (read current pin state)
- Interrupt trigger select (per pin)
- Interrupt enable/disable

Refer to GPIO register map in GPIO.pdf for exact offsets.

---

## 7. Project-Specific Notes

- **PD3 and PD4 must have internal pull-downs enabled.** The vehicle-side circuitry may tristate these lines when no command is active. A floating input must read as logic 0 (no command). Without pull-downs a floating pin registers as a false speed command.
- **APS12202 is open-drain.** Do not configure PD0–PD2 as outputs. Enabling output drive on these pins while the latch is conducting (output LOW) creates a bus conflict.
- **Interrupt on PD0–PD2** (Hall latches) must trigger on both edges to capture every Hall state transition. The ISR notifies the AMCT commutation logic. For ASIL-B, a GTU timer must monitor for absence of Hall transitions at expected intervals during commanded motion — no edge within the expected inter-sector window is a sensor fault.
- **Interrupt on PD3/PD4** (speed command) triggers on rising and falling edges. Debounce in software: minimum 5 ms stable hold before accepting new command state.
- **PD5–PD7 are pure digital inputs (no alternate function).** Configure as GPIO input, no pull-up, no pull-down (A1333 ABI and PWM outputs are push-pull).
- **Interrupt on PD5/PD6** (ABI A/B): trigger on both edges. The ISR increments or decrements the software angle counter based on the quadrature state machine. See CTX_MCU_SCI for the quadrature decode implementation.
- **PD7 (A1333 PWM output) connects to GTU TIMER6**, not a GPIO interrupt. GTU measures the PWM duty cycle, which encodes absolute angle: duty 5% = 0°, duty 95% = 360°, linear. GTU TIMER6 uses High-Time Measurement + Period Measurement pair to compute duty cycle. A1333 PWM carrier frequency is configured at EOL to 1 kHz (1 ms period = 1 ms absolute angle update rate). See CTX_MCU_GTU for TIMER6 configuration.
- **PD7 must not have GPIO interrupt enabled.** GTU takes the input directly. Enabling a GPIO interrupt on PD7 simultaneously would cause spurious interrupts at 2 kHz (1 kHz carrier × 2 edges) — unnecessary CPU load.
- **A1333 SPI is not routed to A89201 GPIO at runtime.** SPI (CS, SCLK, MOSI, MISO) is routed to PCB test pads for end-of-line (EOL) configuration only. A1333 ABI Index output (pin 12) is not connected — leave floating or connect to test pad.
- **ABI A/B drive capability:** A1333 ABI outputs source 1.5 mA and sink 4.5 mA at 5 V. VOH ≈ 5 V (no load) > VIH = 3.5 V ✓. VOL ≈ 0.35 V < VIL = 1.5 V ✓.
- At 5 V VIO, APS12202 VOUT(SAT) with 50 kΩ pull-up: IOUT = 100 µA → VOUT ≈ 0 V < VIL = 1.5 V ✓. APS12202 off: VOH ≈ 4.3 V > VIH = 3.5 V ✓.
