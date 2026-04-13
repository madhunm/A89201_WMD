# CTX_MCU_GPIO — A89201 GPIO Module

## Source
- Document: GPIO.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

- 16 total GPIO pins: PD0–PD15
- PD0–PD7: externally accessible, bidirectional, individually configurable
- PD8–PD15: internal die-to-die connections only, not available for external signals

---

## 2. Electrical Characteristics

Valid at TJ = TBD, VIO = 5 V (shorted to VD50).

| Parameter          | Symbol  | Min         | Typ    | Max         |
|--------------------|---------|-------------|--------|-------------|
| Input low voltage  | VIL     | —           | —      | 0.3 × VIO (1.5 V) |
| Input high voltage | VIH     | 0.7 × VIO (3.5 V) | — | —         |
| Input hysteresis   | VHYS    | 200 mV      | —      | —           |
| Pull-up resistor   | RPU     | 30 kΩ       | 50 kΩ  | 70 kΩ       |
| Pull-down resistor | RPD     | 30 kΩ       | 50 kΩ  | 70 kΩ       |

---

## 3. GPIO Output Multiplexer (ASRCSEL)

Each GPIO pin has a `GPIO_CONTROLx` register with the field `ASRCSEL[3:0]` (bits 7:4), which selects which hardware peripheral output drives that pin:

| ASRCSEL | Source signal    | Use in this project            |
|---------|-----------------|-------------------------------|
| 0000b   | SCI0_Tx         | SPI0 MOSI (PD6)               |
| 0001b   | SCI1_Tx         | —                             |
| 0010b   | SPI0_SCKOUT     | SPI0 SCK (PD5)                |
| 0011b   | SPI1_SCKOUT     | —                             |
| 0100b   | SPI0_CSOUT      | SPI0 CS hardware output (PD4) |
| 0101b   | SPI1_CSOUT      | —                             |
| 1000b   | LIN_Tx          | —                             |
| 1001b   | BEMF_COMP_TACHO | —                             |
| 1010b–1101b | T1–T4 triggers | —                           |
| 1110b   | PWM_Centre      | —                             |
| 1111b   | Any Interrupt   | —                             |

Setting `AOSEL` (bit 13 of GPIO_CONTROLx) = 1 enables the ASRCSEL source to drive the pin instead of the manual PDOUT register.

**This means SCI0 SPI outputs (CS, SCK, MOSI) can be assigned to ANY of PD0–PD7.** There are no fixed alternate-function pin constraints. The user selects which GPIO each SPI signal routes to via ASRCSEL.

MISO (input) is selected via SCU register `sel_spi0`, which points to one of gpi[7:0] = PD0–PD7.

---

## 4. SCI0 Hardware CS Controller

SCI0 SPI master has dedicated hardware CS management with timing registers:

| Register              | Offset | Description                               |
|-----------------------|--------|-------------------------------------------|
| spi0_cs_control       | 0x2C   | CS mode (auto / manual)                  |
| spi0_cs_assert        | 0x30   | CS assert control                         |
| spi0_cs_assert_bits   | 0x34   | Clock cycles of CS setup before first SCK |
| spi0_cs_deassert_bits | 0x38   | Clock cycles of CS hold after last SCK    |
| spi0_cs_idle_bits     | 0x3C   | Idle cycles (CS HIGH) between frames      |

Configure for A1333 timing requirements:
- `cs_assert_bits`: tCS ≥ 50 ns → at 4 MHz SCK → 1 SCK period = 250 ns → 1 bit is sufficient, set to 1
- `cs_deassert_bits`: tCHD ≥ 5 ns → 1 bit
- `cs_idle_bits`: tCS_IDLE ≥ 200 ns → at 4 MHz → 1 SCK period = 250 ns → 1 bit

Hardware CS removes all software timing burden for A1333 SPI transactions.

---

## 5. Project Pin Assignment

| Pin | Direction | Function | Connected to | Configuration |
|-----|-----------|----------|--------------|---------------|
| PD0 | Input | Hall A (rotor position) | APS12202 #1 OUT | Pull-up enabled; routed to AMCT + GTU, no GPIO interrupt |
| PD1 | Input | Hall B (rotor position) | APS12202 #2 OUT | Pull-up enabled; routed to AMCT + GTU, no GPIO interrupt |
| PD2 | Input | Hall C (rotor position) | APS12202 #3 OUT | Pull-up enabled; routed to AMCT + GTU, no GPIO interrupt |
| PD3 | Input | Low speed command | Vehicle BCM | Pull-down; active-high; only PD pin using GPIO interrupt |
| PD4 | Output | SPI0_CSOUT (hardware CS) | A1333 CS/SA0 (pin 8) | ASRCSEL=0100b, AOSEL=1 |
| PD5 | Output | SPI0_SCKOUT (hardware SCK) | A1333 SCLK (pin 9) | ASRCSEL=0010b, AOSEL=1 |
| PD6 | Output | SCI0_Tx (SPI MOSI) | A1333 MOSI/SA1 (pin 10) | ASRCSEL=0000b, AOSEL=1 |
| PD7 | Input | SPI0 MISO | A1333 MISO (pin 11) | sel_spi0 = 7; GPIO input, no pull |
| IG | Input | High speed command | Vehicle BCM | Dedicated HV input; see Section 6 |
| LIN | Reserved | — | Preserved for future LIN use | — |

**Speed command logic:**
- PD3 LOW and IG LOW = motor stopped (park)
- PD3 HIGH and IG LOW = low speed (40 RPM ±2 RPM)
- PD3 LOW and IG HIGH = high speed (60 RPM ±2 RPM)
- PD3 HIGH and IG HIGH = **INVALID** — fault condition

**Why IG for high-speed command:**
After Hall×3 (PD0–PD2), low-speed command (PD3), and full SPI (CS+SCK+MOSI+MISO on PD4–PD7), all 8 external GPIO pins are consumed. The A89201 IG pin is a dedicated HV logic input with VIH ≥ 4.5 V (BCM 5V output drives this), VIL ≤ 2.1 V, 300 kΩ internal pull-down, rated −4 V to 50 V. It is not a GPIO pin — it does not consume any PD resource. LIN pin is fully preserved for future use.

---

## 6. IG Pin (High Speed Command Input)

| Parameter | Min | Typ | Max | Unit |
|-----------|-----|-----|-----|------|
| Input low voltage (VIL) | — | — | 2.1 | V |
| Input high voltage (VIH) | 4.5 | — | — | V |
| Hysteresis | 250 | — | — | mV |
| Input current | — | — | 100 | µA |
| Pull-down resistor | — | 300 | — | kΩ |
| Input voltage range | −4 | — | 50 | V |

BCM drives 5 V (high speed active) or 0 V (inactive). IG internal pull-down ensures LOW when BCM is tri-stated (park / no command).

**Reading IG in firmware:**
IG is readable as a logic-level digital status and also measurable via DAU AOUT multiplexer (for analog voltage readback as a diagnostic). The digital status (HIGH/LOW relative to comparator threshold) is available in the A89201 status register — verify exact register and bit position against DAU/GDU documentation.

---

## 7. Configuration Register Reference

Base Address: **0x40008000**

| Register | Offset | Description |
|----------|--------|-------------|
| GPIO_CONTROL0 | 0x00 | PD0 control (direction, ASRCSEL, AOSEL, interrupt config) |
| GPIO_CONTROL1 | 0x04 | PD1 control |
| ... | ... | ... |
| GPIO_CONTROL7 | 0x1C | PD7 control |
| GPIO_PDOUT | varies | Output data register |
| GPIO_PDIN | varies | Input data register |
| GPIO_INTSTAT | varies | Interrupt status |
| GPIO_INTEN | varies | Interrupt enable |
| GPIO_EVENT_STATUS | 0x68 | Bit n set = PDn caused pending interrupt; write 1 to clear |

Key fields in GPIO_CONTROLx:
- Bits [1:0]: Direction (input/output)
- Bits [3:2]: Pull-up/pull-down enable
- Bits [7:4]: ASRCSEL — peripheral output source selection
- Bit 8: DRVS — drive strength (normal/boost)
- Bits [11:9]: GPI_INTCFG — interrupt trigger (rising/falling/both/level)
- Bit 12: GPI_INTEN — interrupt enable
- Bit 13: AOSEL — 0=manual GPIO, 1=use ASRCSEL source

---

## 8. Project-Specific Notes

- **PD0–PD2: pull-ups enabled, GPIO interrupt disabled.** APS12202 is open-drain. Do not configure as outputs and do not enable GPIO interrupt controller on these pins. Hall transitions are detected by AMCT (commutation, IRQ17) and GTU TIMER1–3 (period capture, IRQ2–4) which connect to PD0–PD2 independently of the GPIO interrupt controller.
- **PD3 must have pull-down enabled.** BCM line may tri-state when no command active. Pull-down ensures LOW (park state) as default. PD3 is the ONLY user GPIO pin with the interrupt controller enabled.
- **PD4–PD6 are SPI outputs.** Set AOSEL=1 and the appropriate ASRCSEL for each. Also configure direction as output (though when AOSEL=1 and ASRCSEL drives a hardware peripheral output, the direction bit may be overridden — verify against silicon).
- **PD7 is MISO input.** Configure as input (no pull). Point sel_spi0 in SCU to PD7 (field value = 7 or as per SCU register definition — verify from SCI/SCU documentation).
- **IG pin (high speed command): poll at 100 Hz from background loop.** IG is not connected to the GPIO interrupt controller — it is a dedicated HV input with its own comparator. Read IG digital status via the DAU/GDU status register in the main loop. Speed commands change at human time scales; 10 ms poll latency is negligible.
- **APS12202 input levels at 5V VIO:** OFF state: VOH ≈ 5V − (10µA × 70kΩ) = 4.3V > VIH = 3.5V ✓. ON state: IOUT = 5V/30kΩ ≈ 167µA, VOUT(SAT) ≈ 0V < VIL = 1.5V ✓.
- **SPI CS timing** is managed by the SCI0 hardware CS controller (cs_assert_bits, cs_deassert_bits, cs_idle_bits registers). No software toggling of PD4 is required during normal SPI operation.

---

## 9. Gpio_IrqHandler — Shared Single-Entry ISR

**Critical:** `Gpio_IrqHandler` (IRQ16) is a SINGLE shared entry point for ALL PD0–PD7 interrupt sources. The Allegro MCAL does not provide per-pin ISR handlers. The handler must read `GPIO_EVENT_STATUS` (offset 0x68 from GPIO base) to identify which pin(s) fired, then dispatch accordingly.

```c
/* GPIO_EVENT_STATUS base address: 0x40008000 + 0x68 = 0x40008068 */
#define GPIO_EVENT_STATUS_ADDR  (0x40008068U)

void Gpio_IrqHandler(void) {
    volatile uint32_t* pEvt = (volatile uint32_t*)GPIO_EVENT_STATUS_ADDR;
    uint32_t events = *pEvt;
    *pEvt = events;               /* write-1-to-clear each pending bit */

    /* PD3: low speed command changed */
    if (events & (1U << 3)) {
        uint8_t cmd_state = (GPIO_PDIN >> 3) & 0x01U;  /* read current PD3 level */
        SpeedCommand_ProcessChange(eSPEED_LOW, cmd_state);
    }

    /* PD0–PD2: Hall — should NOT fire (no GPIO interrupt enabled on these pins) */
    /* If this branch executes, a configuration error exists */
    if (events & 0x07U) {
        SetFault(FAULT_GPIO_SPURIOUS_HALL_IRQ);
    }
}
```

**Debounce:** Implement in `SpeedCommand_ProcessChange()`. Minimum stable hold before accepting new state: 5 ms. Use a GTU free-run timer counter snapshot at the interrupt event; accept if stable for ≥5 ms.

**IG (high speed command):** Not handled in `Gpio_IrqHandler`. Poll at 100 Hz from main loop background task.
