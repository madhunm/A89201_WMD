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

| Pin | Direction | Function                        | Connected to         | Notes                            |
|-----|-----------|----------------------------------|----------------------|----------------------------------|
| PD0 | Input     | Hall latch A (rotor position)   | APS12202 #1 OUT      | Internal pull-up enabled          |
| PD1 | Input     | Hall latch B (rotor position)   | APS12202 #2 OUT      | Internal pull-up enabled          |
| PD2 | Input     | Hall latch C (rotor position)   | APS12202 #3 OUT      | Internal pull-up enabled          |
| PD3 | Input     | Low speed command (vehicle)     | Vehicle CAN/BCM GPIO | Internal pull-down; active-high   |
| PD4 | Input     | High speed command (vehicle)    | Vehicle CAN/BCM GPIO | Internal pull-down; active-high   |
| PD5 | Output    | A1333 SPI chip select (CS)      | A1333 CS/SA0         | Active-low; driven by software    |
| PD6 | Alternate | SPI0 SCK                        | A1333 SCLK           | Configured via SCU sel_spi0      |
| PD7 | Alternate | SPI0 MOSI / SPI0 MISO           | A1333 MOSI/MISO      | Configured via SCU sel_spi0      |

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

- **PD3 and PD4 must have internal pull-downs enabled.** The vehicle-side driving circuitry may tristate or float these lines when no command is active. A floating input on PD3 or PD4 must be read as logic 0 (no command). Without pull-downs, a floating pin could register as a false speed command.
- **APS12202 is open-drain.** Do not configure PD0–PD2 as outputs. Enabling output drive on these pins while the hall latch is in conduction (output LOW) creates a bus conflict.
- **PD5 (CS) must be initialised HIGH** before SCI0 SPI is enabled. If PD5 is low during SPI init, the A1333 will receive spurious clock edges and may latch garbage.
- **Interrupt on PD0–PD2** (Hall latches) should trigger on both edges to capture each Hall state transition. The ISR should invoke the AMCT state machine or notify the commutation logic. For ASIL-B, a watchdog timer should detect absence of Hall transitions at expected intervals during motion.
- **Interrupt on PD3/PD4** (speed command) should trigger on rising and falling edges to detect command changes. Debounce in software (minimum 5 ms hold time before accepting new command state).
- **PD6 and PD7** pin function must be set to alternate function (SPI) via SCU mux registers before SCI0 SPI is initialised. If SCU mux is not set, SPI0 will not function.
- At 5 V VIO, APS12202 output saturation VOUT(SAT) = 185 mV (typ) at 20 mA load. With 50 kΩ pull-up and VCC = 5 V, IOUT = 5 V / 50 kΩ = 100 µA. VOUT(SAT) at 100 µA ≈ negligible. Input LOW voltage ≈ 0.1 V, well within VIL = 1.5 V. ✓
