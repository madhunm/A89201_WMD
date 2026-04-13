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

### SCI0 — SPI Master → A1333 Angle Sensor

- Mode: SPI Controller (master)
- Target: A1333 (14-pin LE package, single die)
- Purpose: Read 12-bit angle register (0x20) at configurable rate
- CS management: Software-driven via GPIO PD5 (active-low)
- SCLK: up to 10 MHz (A1333 max); recommend 4 MHz for margin
- SPI Mode: CPOL=1, CPHA=1 (Mode 3) — inferred from A1333 timing (SCLK idle high, MISO valid after falling edge, MOSI captured on rising edge). **Verify against A1333 application note before final implementation.**
- Data width: 16-bit
- Bit order: MSB first
- DMA: Yes (DMA2 handles RX result → SRAM)

SCI0 pin mapping (via SCU sel_spi0):
| Signal   | GPIO Pin | Description                       |
|----------|----------|-----------------------------------|
| SPI0 SCK | PD6      | Clock output to A1333 SCLK        |
| SPI0 MOSI| PD7      | Data output to A1333 MOSI         |
| SPI0 MISO| PD7 alt  | Data input from A1333 MISO        |
| SPI0 CS  | PD5      | GPIO output, software-controlled  |

**Note:** MISO and MOSI are separate pins on A1333. The A89201 SPI MISO input pin assignment is controlled by sel_spi0. PD7 may need to be split if MOSI and MISO share the same PD pin mux slot — verify against SCI register definition for dual-wire vs. separate-pin SPI configuration.

### SCI1 — UART (Debug or LIN-bypass)

- Mode: UART
- Purpose: Debug UART during development (Keil ITM printf-style output or raw UART)
- Baud rate: 115200 bps
- Not used in production firmware unless a diagnostic serial port is required

---

## 5. SCI Configuration Sequence (SPI Mode, SCI0)

```c
// 1. Configure SCU mux for SPI0 pins
SCU->sel_spi0 = (PD6_SCK | PD7_MOSI | PD_MISO_pin);  // exact field values from SCI/SCU register map

// 2. Configure GPIO
GPIO->PD6_ctrl = ALT_FUNCTION;   // SCK output
GPIO->PD7_ctrl = ALT_FUNCTION;   // MOSI output
GPIO->PD5_ctrl = OUTPUT;         // CS as GPIO output
GPIO->PD5_out  = 1;              // CS idle HIGH

// 3. Configure SCI0 for SPI
SCI0->mode   = SPI_CONTROLLER;
SCI0->cpol   = 1;                // Clock idle high
SCI0->cpha   = 1;                // Data captured on rising edge
SCI0->width  = 16;               // 16-bit frames
SCI0->brate  = CLK_40MHz / (2 * 4MHz);  // Baud rate divider for 4 MHz SCLK
SCI0->msb    = 1;                // MSB first
SCI0->enable = 1;
```

---

## 6. A1333 SPI Transaction Sequence

To read angle register 0x20 (12-bit angle):

```c
void A1333_ReadAngle(uint16_t *angle_raw) {
    GPIO_PD5_LOW();              // Assert CS (active low)
    delay_ns(50);                // tCS: CS to first SCLK edge ≥ 50 ns
    uint16_t tx = 0x2000;        // Address 0x20 with R/W = read (MSB=0 = read for direct regs)
    uint16_t rx = SPI_Transfer16(tx);
    GPIO_PD5_HIGH();             // Deassert CS
    delay_ns(200);               // tCS_IDLE: ≥ 200 ns between frames

    // Extract 12-bit angle and status bits
    uint8_t  error_flag = (rx >> 14) & 0x01;   // EF bit
    uint8_t  uv_flag    = (rx >> 13) & 0x01;   // UV bit
    uint8_t  parity     = (rx >> 12) & 0x01;   // P bit
    uint16_t angle      = rx & 0x0FFF;          // Bits [11:0]

    // Verify parity (even parity over bits [14:0])
    // Verify EF and UV flags before using angle
    *angle_raw = angle;
}
```

The 12-bit angle maps linearly: 0x000 = 0°, 0xFFF = 359.912°. Resolution = 360 / 4096 = 0.0879°/count.

---

## 7. Interrupt and DMA

- `sci0_rx_complete` interrupt: fires when 16-bit SPI receive shift register is full
- DMA flow-control: `sci0_rx_ack` signal triggers DMA2 to pull RX register into SRAM
- On interrupt (if not using DMA): read SCI0 RX register within ISR

---

## 8. Project-Specific Notes

- **A1333 SPI parity bit (P)** must be verified on every read. If parity fails, discard the angle value and retain the previous valid reading. Count consecutive parity errors; if >3, assert a sensor fault.
- **A1333 error flag (EF)** must be checked on every angle read. EF = 1 indicates an internal diagnostic fault inside the A1333. This is a safety-relevant signal for ASIL-B.
- **A1333 UV flag (UV)** = 1 indicates undervoltage inside A1333. If UV = 1, angle data is invalid.
- **CS idle time (tCS_IDLE ≥ 200 ns)** must be enforced between consecutive SPI frames. At 40 MHz CPU, 200 ns = 8 NOP instructions minimum.
- **Do not use hardware CS** (hardware-driven chip select from SCI0). Use GPIO PD5 software-driven CS. Reason: hardware CS timing may not satisfy A1333 tCS and tCHD requirements without explicit configuration and verification.
- The A1333 SCLK maximum is 10 MHz. Running at 4 MHz provides >2× timing margin. Do not increase clock rate to >5 MHz without re-verifying PCB trace lengths and capacitive loading (CL ≤ 20 pF spec on A1333 MISO).
- **SCI0 must be reinitialised after a watchdog reset.** The SCU reset clears SCI registers. Boot code must re-run SCI0 init before the control loop starts.
