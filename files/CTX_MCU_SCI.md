# CTX_MCU_SCI — A89201 Serial Communication Interface (SCI) Module

## Source
- Document: SCI.pdf (A89201 SoC Motor Driver, Preliminary, Allegro Microsystems)

---

## 1. Overview

The A89201 has two independent SCI units: SCI0 and SCI1. Each supports:
- Full SPI controller (master) or peripheral (slave)
- Full UART

The two instances are independent and can be configured differently (e.g., SCI0 as SPI, SCI1 as UART simultaneously).

---

## 2. Features

- Two independent channels (SCI0, SCI1)
- Full SPI support (controller and peripheral mode)
- Full UART support
- Independent interrupt request per channel
- DMA flow-control interface per channel
- GPIO pin mux via SCU registers

---

## 3. Pin Multiplexing (SCU Dependency)

The SCI pins are shared with GPIO PD0–PD7. **Pin assignment for SPI and UART is controlled by SCU mux registers, not the SCI module itself.**

SCU mux registers:
| SCU Register | Offset  | Function                          |
|--------------|---------|-----------------------------------|
| sel_uart0    | 0x0038  | Selects PD pin for UART0 RX       |
| sel_uart1    | 0x003C  | Selects PD pin for UART1 RX       |
| sel_spi0     | 0x0040  | Selects PD pin for SPI0 MISO/RX   |
| sel_spi1     | 0x0044  | Selects PD pin for SPI1 MISO/RX   |

**The SCU mux must be configured before SCI is initialised.** If not set, the SCI peripheral will not receive any serial data.

---

## 4. Project Assignment

### SCI0 — Not used for A1333 runtime

**A1333 angle data is received at runtime via GPIO interrupts on ABI A/B (PD5/PD6) and GTU duty-cycle measurement on PWM output (PD7).** SPI was the initial design intent but cannot fit within the 3 remaining GPIO pins after Hall (PD0–PD2) and speed commands (PD3–PD4): SPI requires 4 pins (CS + SCLK + MOSI + MISO), while ABI A/B + PWM requires exactly 3. See CTX_MCU_GPIO for the full rationale.

**SCI0 is available for debug UART** during development. In production firmware it remains unconfigured (clock-gated via SCU to save power).

A1333 SPI access is available at end-of-line (EOL) configuration only via PCB test pads (not connected to A89201 GPIO at runtime). An external programmer performs:
- LBIST result check at first power-on
- ABI resolution configuration (RESOLUTION_PAIRS = 3 → 2048 PPR, 0.044°/quadrature state)
- ZERO_OFFSET calibration write
- EEPROM lock after calibration

### SCI0 — UART (Debug, development only)

- Mode: UART
- Purpose: Debug output during development (printf via UART, fault log dumps)
- Baud rate: 115200 bps
- Pins: assign via SCU `sel_uart0` to available debug header pins on PCB
- **Disable in production build** (clock-gate SCI0 via SCU to prevent spurious transitions on unpopulated debug header)

### SCI1 — Reserved / Unused

- Not configured in current design
- Available for future use (e.g., LIN bypass, factory test interface)

---

## 5. ABI Quadrature Decoding (replaces SPI section)

A1333 ABI outputs (PD5 = A, PD6 = B) are decoded in software using GPIO interrupts. PD7 (A1333 PWM output) connects to GTU TIMER5/TIMER6 — not a GPIO interrupt. The ABI counter is periodically re-anchored to the PWM absolute angle.

```c
/* Quadrature state machine */
/* Call from PD5 or PD6 interrupt ISR */
/* A1333 configured at RESOLUTION_PAIRS=3: 2048 PPR, 8192 CPR */
/* One count = 360 / 8192 = 0.044 degrees */

static int32_t abi_count = 0;   /* Running quadrature count, signed */
static uint8_t abi_prev = 0;    /* Previous AB state */

void ABI_EdgeISR(void) {
    uint8_t A = GPIO_ReadPin(PD5);
    uint8_t B = GPIO_ReadPin(PD6);
    uint8_t curr = (A << 1) | B;  /* 2-bit state: A=MSB, B=LSB */

    /* Quadrature decode table: abi_prev → curr → direction */
    static const int8_t decode[4][4] = {
    /*       curr: 00  01  10  11  */
    /* 00 */ {  0, -1, +1,  0 },
    /* 01 */ { +1,  0,  0, -1 },
    /* 10 */ { -1,  0,  0, +1 },
    /* 11 */ {  0, +1, -1,  0 },
    };

    abi_count += decode[abi_prev][curr];
    abi_prev = curr;
}

/* PD7 is NOT a GPIO interrupt source — it feeds GTU TIMER5/TIMER6 directly */
/* for hardware-timed PWM duty cycle measurement. See CTX_MCU_GTU for      */
/* the TIMER5 ISR that produces pwm_absolute_angle_deg every 1 ms.         */
/* ABI drift correction is performed in ABI_PWM_Fusion() at 100 Hz.        */

float ABI_ToAngleDeg(void) {
    /* Convert count to degrees, wrap to 0–360 */
    float deg = (float)abi_count * ABI_DEG_PER_COUNT;  /* 0.044 deg/count */
    deg = fmodf(deg, 360.0f);
    if (deg < 0.0f) deg += 360.0f;
    return deg;
}
```

### ABI Edge Rate at Maximum Speed

At 60 RPM output shaft, 2048 PPR ABI:
- Edge rate = 60/60 × 2048 × 4 = 8192 edges/sec = 1 edge every 122 µs
- At 40 MHz CPU, 122 µs = 4880 clock cycles per edge → ISR overhead is negligible

### ASIL-B Note on ABI

ABI provides no diagnostic flags (no equivalent of EF/UV/parity from SPI). Angle validity must be inferred by:
1. Index pulse periodicity — index should appear once per revolution; missing index during expected rotation = sensor fault
2. Maximum angular rate plausibility — ABI count change per interrupt should not exceed physically possible rate
3. Hall-based speed vs ABI-based speed cross-check — at steady speed, speeds should agree within tolerance

---

## 6. Interrupt and DMA

- ABI decoding uses GPIO interrupts (PD5, PD6 only) — no DMA required. PD7 is routed to GTU directly.
- SCI1 UART TX uses DMA2 if debug logging is enabled in development build

---

## 7. Project-Specific Notes

- **ABI A and B must both have interrupts enabled on both edges.** Missing one edge direction causes the quadrature counter to drift.
- **PD7 connects to GTU TIMER5/TIMER6, not a GPIO interrupt.** The GTU measures A1333 PWM duty cycle and produces an absolute angle every 1 ms. Do not configure PD7 as a GPIO interrupt source. ABI drift is corrected continuously via ABI_PWM_Fusion() at 100 Hz. See CTX_MCU_GTU for TIMER5/TIMER6 configuration.
- **SCI0 SCU clock gate:** In production firmware, set the SCI0 clock gate bit in SCU to disable SCI0. This prevents the unused SCI0 from consuming power and generating noise on the unconnected UART pins.
- **ABI resolution must be confirmed at EOL.** RESOLUTION_PAIRS=3 in A1333 EEPROM gives 2048 PPR (0.044°/quadrature state). If EEPROM is not programmed, the factory default resolution applies (unknown — do not assume). Verify by counting edges per full rotation during EOL test. At EOL also configure PWM carrier frequency (1 kHz recommended).
- **ABI_DEG_PER_COUNT constant:** = 360.0 / (2048 × 4) = 0.04394531°. Store as a compile-time constant in `motor_config.h`.
- The quadrature decode table handles all four valid transitions and returns 0 for illegal transitions (A and B change simultaneously). Illegal transitions should increment a fault counter; if cumulative illegal transitions exceed a threshold per second, declare an ABI noise fault.

