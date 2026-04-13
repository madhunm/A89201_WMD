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

| Parameter          | Symbol  | Min               | Typ    | Max               |
|--------------------|---------|-------------------|--------|-------------------|
| Input low voltage  | VIL     | —                 | —      | 0.3 × VIO (1.5 V) |
| Input high voltage | VIH     | 0.7 × VIO (3.5 V) | —      | —                 |
| Input hysteresis   | VHYS    | 200 mV            | —      | —                 |
| Pull-up resistor   | RPU     | 30 kΩ             | 50 kΩ  | 70 kΩ             |
| Pull-down resistor | RPD     | 30 kΩ             | 50 kΩ  | 70 kΩ             |

---

## 3. GPIO Output Multiplexer (ASRCSEL)

Each GPIO pin has a `GPIO_CONTROLx` register with the field `ASRCSEL[3:0]` (bits 7:4), which selects which hardware peripheral output drives that pin:

| ASRCSEL     | Source signal    | Use in this project            |
|-------------|-----------------|-------------------------------|
| 0000b       | SCI0_Tx         | SPI0 MOSI (PD6)               |
| 0001b       | SCI1_Tx         | —                             |
| 0010b       | SPI0_SCKOUT     | SPI0 SCK (PD5)                |
| 0011b       | SPI1_SCKOUT     | —                             |
| 0100b       | SPI0_CSOUT      | SPI0 CS hardware output (PD4) |
| 0101b       | SPI1_CSOUT      | —                             |
| 1000b       | LIN_Tx          | —                             |
| 1001b       | BEMF_COMP_TACHO | —                             |
| 1010b–1101b | T1–T4 triggers  | —                             |
| 1110b       | PWM_Centre      | —                             |
| 1111b       | Any Interrupt   | —                             |

Setting `AOSEL` (bit 13 of GPIO_CONTROLx) = 1 enables the ASRCSEL source to drive the pin instead of the manual PDOUT register.

SCI0 SPI outputs (CS, SCK, MOSI) can be assigned to any of PD0–PD7. No fixed alternate-function pin constraints exist. MISO (input) is selected via SCU register `sel_spi0`, which indexes gpi[7:0] = PD0–PD7.

---

## 4. SCI0 Hardware CS Controller

SCI0 SPI master has dedicated hardware CS management with timing registers:

| Register              | Offset | Description                               |
|-----------------------|--------|-------------------------------------------|
| spi0_cs_control       | 0x2C   | CS mode (auto / manual)                   |
| spi0_cs_assert        | 0x30   | CS assert control                         |
| spi0_cs_assert_bits   | 0x34   | Clock cycles of CS setup before first SCK |
| spi0_cs_deassert_bits | 0x38   | Clock cycles of CS hold after last SCK    |
| spi0_cs_idle_bits     | 0x3C   | Idle cycles (CS HIGH) between frames      |

Configure for A1333 timing requirements:
- `cs_assert_bits`: tCS >= 50 ns; at 4 MHz SCK, 1 SCK = 250 ns; set to 1
- `cs_deassert_bits`: tCHD >= 5 ns; set to 1
- `cs_idle_bits`: tCS_IDLE >= 200 ns; 1 SCK = 250 ns; set to 1

Hardware CS removes all software timing burden for A1333 SPI transactions.

---

## 5. Project Pin Assignment

| Pin | Direction | Mode                                    | Pull     | Function               | Connected to            |
|-----|-----------|-----------------------------------------|----------|------------------------|-------------------------|
| PD0 | Input     | Digital input                           | Pull-UP  | Hall A (rotor)         | APS12202 #1 OUT         |
| PD1 | Input     | Digital input                           | Pull-UP  | Hall B (rotor)         | APS12202 #2 OUT         |
| PD2 | Input     | Digital input                           | Pull-UP  | Hall C (rotor)         | APS12202 #3 OUT         |
| PD3 | Input     | Analog (eGPIO_MODE_ANALOG_INPUT_SOURCE) | None     | Speed command ADC      | Buffer C output (ext.)  |
| PD4 | Output    | Peripheral (ASRCSEL=0100b, AOSEL=1)     | None     | SPI0_CSOUT             | A1333 CS/SA0 (pin 8)    |
| PD5 | Output    | Peripheral (ASRCSEL=0010b, AOSEL=1)     | None     | SPI0_SCKOUT            | A1333 SCLK (pin 9)      |
| PD6 | Output    | Peripheral (ASRCSEL=0000b, AOSEL=1)     | None     | SCI0_Tx (SPI MOSI)     | A1333 MOSI/SA1 (pin 10) |
| PD7 | Input     | Digital input                           | None     | SPI0 MISO              | A1333 MISO (pin 11)     |
| IG  | —         | Not used                                | —        | Not connected          | —                       |
| LIN | —         | Reserved                                | —        | Future LIN use         | —                       |

**No GPIO interrupts are configured on any pin in this project.**

### Pull selection rationale

**PD0–PD2 (Hall, APS12202): Pull-UP.**
APS12202 is an open-drain NPN output. It actively pulls LOW when the Hall field exceeds the operate threshold; the pin floats when the device is off. The internal pull-up holds the line HIGH (no-field state). Signal integrity check at VIO = 5V, RPU_max = 70 kΩ, APS12202 IOUT_max = 10 µA:
- OFF state: V_OH = 5V - (10 µA x 70 kΩ) = 4.3 V > VIH = 3.5 V ✓
- ON state: V_OL(sat) < 0.4 V < VIL = 1.5 V ✓
Pull-down would prevent the pin from reaching VIH in the off state — do not use pull-down on PD0–PD2.

**PD3 (Speed command, analog): None.**
Pin is driven by the low-impedance output of Buffer C (external op-amp, Zout < 100 Ω). The `eGPIO_MODE_ANALOG_INPUT_SOURCE` mode disables the digital input buffer and the pull resistors in hardware. Confirm in silicon bring-up that pull bits in GPIO_CONTROL3 have no effect in analog mode.

**PD7 (SPI MISO): None.**
A1333 drives MISO actively during every SPI frame. A pull would fight the A1333 output during transmission and draw continuous current. MISO is high-impedance between CS assertions; it is never sampled in that window, so no pull is needed to guarantee a valid idle level.

### Speed command decode summary

Speed commands decoded from DAU ADC on PD3 (Slot 3, polled at 100 Hz). See CTX_MCU_DAU Section 9 for full circuit description and VREF dependency.

| ADC code (12-bit, VREF=5V) | V_PD3       | State          | Speed target |
|----------------------------|-------------|----------------|--------------|
| 0–572                      | 0–0.70 V    | Park           | 0 RPM        |
| 573–1736                   | 0.70–2.12 V | High speed     | 60 RPM       |
| 1737–2892                  | 2.12–3.54 V | Low speed      | 40 RPM       |
| >= 2893                    | >= 3.54 V   | Invalid / fault| SAFE STATE   |

---

## 6. Configuration Register Reference

Base Address: **0x40008000**

| Register          | Offset | Description                                                   |
|-------------------|--------|---------------------------------------------------------------|
| GPIO_CONTROL0     | 0x00   | PD0 control (direction, ASRCSEL, AOSEL, pull, interrupt cfg)  |
| GPIO_CONTROL1     | 0x04   | PD1 control                                                   |
| GPIO_CONTROL2     | 0x08   | PD2 control                                                   |
| GPIO_CONTROL3     | 0x0C   | PD3 control                                                   |
| GPIO_CONTROL4     | 0x10   | PD4 control                                                   |
| GPIO_CONTROL5     | 0x14   | PD5 control                                                   |
| GPIO_CONTROL6     | 0x18   | PD6 control                                                   |
| GPIO_CONTROL7     | 0x1C   | PD7 control                                                   |
| GPIO_PDOUT        | varies | Output data register                                          |
| GPIO_PDIN         | varies | Input data register                                           |
| GPIO_INTSTAT      | varies | Interrupt status                                              |
| GPIO_INTEN        | varies | Interrupt enable — all bits must be 0 in this project         |
| GPIO_EVENT_STATUS | 0x68   | Bit n = PDn caused interrupt; write 1 to clear                |

Key fields in GPIO_CONTROLx:
- Bits [1:0]: Direction (input/output)
- Bits [3:2]: Pull enable (pull-up / pull-down)
- Bits [7:4]: ASRCSEL — peripheral output source selection
- Bit 8: DRVS — drive strength (normal/boost)
- Bits [11:9]: GPI_INTCFG — interrupt trigger (rising/falling/both/level)
- Bit 12: GPI_INTEN — interrupt enable (must be 0 on ALL pins in this project)
- Bit 13: AOSEL — 0 = manual GPIO, 1 = use ASRCSEL source

---

## 7. Initialisation Code

```c
void GPIO_Init_WiperProject(void) {

    /* PD0: Hall A — digital input, pull-up, no interrupt */
    Gpio_SetMode(eGPIO_INSTANCE_ID0, eGPIO_MODE_DIGITAL_INPUT_PULLUP);

    /* PD1: Hall B — digital input, pull-up, no interrupt */
    Gpio_SetMode(eGPIO_INSTANCE_ID1, eGPIO_MODE_DIGITAL_INPUT_PULLUP);

    /* PD2: Hall C — digital input, pull-up, no interrupt */
    Gpio_SetMode(eGPIO_INSTANCE_ID2, eGPIO_MODE_DIGITAL_INPUT_PULLUP);

    /* PD3: Speed command — analog input, no pull */
    /* eGPIO_MODE_ANALOG_INPUT_SOURCE disables digital buffer and pull resistors */
    Gpio_SetMode(eGPIO_INSTANCE_ID3, eGPIO_MODE_ANALOG_INPUT_SOURCE);

    /* PD4: SPI0_CSOUT — peripheral output, ASRCSEL = 0100b */
    Gpio_SetMode(eGPIO_INSTANCE_ID4, eGPIO_MODE_OUTPUT_PUSH_PULL);
    Gpio_SetAsrcSel(eGPIO_INSTANCE_ID4, eGPIO_ASRC4);   /* SPI0_CSOUT */
    Gpio_SetAoSel(eGPIO_INSTANCE_ID4, 1U);

    /* PD5: SPI0_SCKOUT — peripheral output, ASRCSEL = 0010b */
    Gpio_SetMode(eGPIO_INSTANCE_ID5, eGPIO_MODE_OUTPUT_PUSH_PULL);
    Gpio_SetAsrcSel(eGPIO_INSTANCE_ID5, eGPIO_ASRC2);   /* SPI0_SCKOUT */
    Gpio_SetAoSel(eGPIO_INSTANCE_ID5, 1U);

    /* PD6: SCI0_Tx / SPI MOSI — peripheral output, ASRCSEL = 0000b */
    Gpio_SetMode(eGPIO_INSTANCE_ID6, eGPIO_MODE_OUTPUT_PUSH_PULL);
    Gpio_SetAsrcSel(eGPIO_INSTANCE_ID6, eGPIO_ASRC0);   /* SCI0_Tx */
    Gpio_SetAoSel(eGPIO_INSTANCE_ID6, 1U);

    /* PD7: SPI0 MISO — digital input, no pull */
    Gpio_SetMode(eGPIO_INSTANCE_ID7, eGPIO_MODE_DIGITAL_INPUT);

    /* Route PD7 to SCI0 SPI RX — no Gpio API for this; direct SCU register write */
    *(volatile uint32_t*)0x4000D040U = 7U;   /* SCU_SEL_SPI0 = 7 -> PD7 as MISO */

    /* Confirm: GPI_INTEN (bit 12) = 0 in GPIO_CONTROL0 through GPIO_CONTROL7 */
    /* No GPIO interrupts used in this project */
}
```

**API enum mapping (from Gpio.h):**
- `eGPIO_INSTANCE_IDx` maps to PDx
- `eGPIO_MODE_DIGITAL_INPUT` = input, no pull
- `eGPIO_MODE_DIGITAL_INPUT_PULLUP` = input, pull-up enabled
- `eGPIO_MODE_DIGITAL_INPUT_PULLDOWN` = input, pull-down enabled
- `eGPIO_MODE_ANALOG_INPUT_SOURCE` = analog input, disables digital buffer and pull
- `eGPIO_MODE_OUTPUT_PUSH_PULL` = push-pull output
- `eGPIO_ASRC0` = SCI0_Tx (MOSI), `eGPIO_ASRC2` = SPI0_SCKOUT, `eGPIO_ASRC4` = SPI0_CSOUT

---

## 8. Gpio_IrqHandler — Shared ISR

No GPIO interrupts are configured in this project. `Gpio_IrqHandler` (IRQ16) should never be invoked during normal operation.

- PD0–PD2 (Hall): transitions captured by AMCT (commutation) and GTU TIMER1–3 (period). GPIO interrupt controller not involved.
- PD3 (speed command): analog input polled at 100 Hz via DAU. GPIO interrupts not possible in analog mode.
- PD4–PD7 (SPI): peripheral-driven or SCI0 MISO input. No GPIO interrupt.

If `Gpio_IrqHandler` fires unexpectedly, log the fault and clear the pending bits to prevent re-entry:

```c
void Gpio_IrqHandler(void) {
    /* Should never execute — no GPIO interrupts configured */
    volatile uint32_t* pEvt = (volatile uint32_t*)0x40008068U;
    uint32_t events = *pEvt;
    *pEvt = events;                   /* write-1-to-clear; prevent re-entry */
    SetFault(FAULT_GPIO_SPURIOUS);    /* log only — do not enter safe state */
}
```

---

## 9. Project-Specific Notes

- **PD0–PD2 pull-ups are required.** Without them the APS12202 open-drain output cannot produce a valid HIGH. Do not configure pull-down or no-pull on Hall pins.
- **PD3 analog mode disables GPIO_PDIN bit 3.** Reading PDIN[3] in analog mode returns undefined data. Speed command state must be obtained exclusively from DAU_RESULT[3].
- **PD4–PD6 AOSEL=1 overrides PDOUT.** Direction must still be set to output, but the signal driven onto the pin comes from the SCI0 peripheral. Software writes to PDOUT[4:6] have no effect while AOSEL=1.
- **PD7 MISO assignment has no GPIO API.** The Gpio_SetAsrcSel/Gpio_SetAoSel path is for output muxing only. MISO source selection is exclusively via SCU_SEL_SPI0 (0x4000D040); write 7 to select PD7.
- **IG pin is not used.** IG is a dedicated HV input with its own comparator. It has no role in the speed command decode path. Do not read IG status registers in firmware.
