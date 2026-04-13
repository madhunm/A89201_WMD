# CTX_MCU_SCU — A89201 System Control Unit (SCU)

## Source
- Document: SCU.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The SCU is the central housekeeping module. It controls clocking, memory ECC, the watchdog, voltage monitors, peripheral pin-mux, and system fault management. **Every boot sequence must configure SCU before any other peripheral is initialised.**

---

## 2. Features

- Clock controller (CPU clock, peripheral clock gating)
- SRAM ECC control and fault reporting
- IRAM ECC control and fault reporting
- Flash ECC control and fault reporting
- SCI0/SCI1 mux control (which GPIO pins map to SPI/UART)
- LIN physical layer mux control
- Watchdog timer
- Voltage monitors (VD50, VD30, VIO undervoltage)
- Die over-temperature detection
- PLL control (lock, timeout, source timeout)
- Fault management and IRQ routing (IRQ_SCU)
- Boot register
- Reset source identification

---

## 3. Register Map

Base Address: **0x4000D000**

| Register     | Access | Offset  | Reset Value    | Description                            |
|--------------|--------|---------|----------------|----------------------------------------|
| mcu_Info     | R      | 0x0000  | 0x00D4_0000    | MCU mask revision                      |
| ctrl         | R/W    | 0x0008  | 0x0000_0000    | System control register                |
| intstat      | R/W1C  | 0x0010  | 0x0000_0000    | Interrupt status (write 1 to clear)    |
| inten        | R/W    | 0x0014  | 0x0000_0000    | Interrupt enable                       |
| diag         | R/W1C  | 0x0018  | 0x0000_0001    | Diagnostic flags                       |
| mcuclk       | R/W    | 0x001C  | 0x0000_0000    | CPU clock configuration                |
| ecc_dsa      | R      | 0x0020  | 0x0000_0000    | ECC DRAM single-bit error address      |
| ecc_dse      | R      | 0x0024  | 0x0000_0000    | ECC DRAM single-bit error              |
| ecc_isa      | R      | 0x0028  | 0x0000_0000    | ECC IRAM single-bit error address      |
| ecc_ise      | R      | 0x002C  | 0x0000_0000    | ECC IRAM single-bit error              |
| wdt          | W      | 0x0030  | 0x0000_0000    | Watchdog kick register                 |
| wdt_period   | R/W    | 0x0034  | 0x0000_0000    | Watchdog timeout period                |
| sel_uart0    | R/W    | 0x0038  | 0x0000_0000    | GPIO mux for UART0 RX pin              |
| sel_uart1    | R/W    | 0x003C  | 0x0000_0000    | GPIO mux for UART1 RX pin              |
| sel_spi0     | R/W    | 0x0040  | 0x0000_0000    | GPIO mux for SPI0 pins                 |
| sel_spi1     | R/W    | 0x0044  | 0x0000_0000    | GPIO mux for SPI1 pins                 |
| sel_lin      | R/W    | 0x0048  | 0x0000_0000    | LIN mux control                        |
| sel_lctx     | R/W    | 0x0054  | 0x0000_0000    | LIN TX mux control                     |
| debug        | R/W    | 0x0058  | 0x0000_0000    | Debug configuration                    |

---

## 4. CTRL Register Key Bits (Offset 0x0008)

| Field    | Bits | Description                                    |
|----------|------|------------------------------------------------|
| mclrn_en | bit  | Enable MCLRn (external reset from GDU)         |
| ida      | bit  | Idle mode assist                               |
| plloff   | bit  | PLL power off                                  |
| nohlt    | bit  | No-halt: disable CPU WFI low-power mode        |
| viomon   | bit  | VIO voltage monitor enable                     |

---

## 5. Watchdog

- Write-only kick register (wdt at 0x0030)
- Timeout period configured via wdt_period register
- On timeout: generates SCU interrupt (if enabled) and/or system reset
- Must be kicked from main loop at interval shorter than wdt_period
- On watchdog timeout event, SCU sets corresponding bit in intstat and diag

**Watchdog service sequence (must occur periodically):**
```c
SCU->wdt = WATCHDOG_KICK_VALUE;   // Exact kick value — verify from SCU register definition
```

For ASIL-B: watchdog timeout period should be set to the maximum tolerable reaction time for the application. Recommended starting value: 10 ms (200 × PWM cycles at 20 kHz). Review against system safety requirements.

---

## 6. Memory ECC

- SRAM: hardware ECC monitors all writes/reads
- IRAM: hardware ECC monitors all writes/reads
- Flash: hardware ECC monitors reads (via FLI)
- Single-bit errors: corrected automatically, address logged in ecc_*a registers
- Multi-bit errors: uncorrectable, SCU IRQ generated, firmware must treat as fatal fault

On SCU IRQ for ECC error:
1. Read ecc_dsa / ecc_isa / Flash ECC address register to identify failing location
2. Log fault
3. Enter safe state (disable gate outputs, stop motor)

---

## 7. Fault Conditions Monitored

The `diag` register (0x0018) flags these conditions:

| Condition          | Description                                                |
|--------------------|------------------------------------------------------------|
| Power-on Reset     | Normal startup; volatile registers re-initialised         |
| System reset       | Any non-POR reset                                          |
| Watchdog timeout   | Software failed to kick watchdog in time                   |
| CPU lock-up        | Cortex-M4 detected a fault/lock-up condition               |
| GDU fault          | Gate driver reported a fault (overcurrent, VDS, etc.)     |
| PLL lock           | PLL acquired lock                                          |
| PLL timeout        | PLL failed to lock within timeout                          |
| VD50 undervoltage  | 5 V supply below threshold                                 |
| VD30 undervoltage  | 3.3 V supply below threshold                               |
| VIO undervoltage   | GPIO logic supply below threshold                          |
| SRAM ECC           | ECC error in data SRAM                                     |
| IRAM ECC           | ECC error in instruction RAM                               |
| Flash ECC          | ECC error in flash                                         |
| Die over-temp      | Junction temperature exceeded limit                        |

---

## 8. Boot Sequence Role

On POR, the SCU:
1. Applies POR to all peripherals
2. Initialises ECC on all memories
3. Releases CPU from reset

Firmware boot sequence must:
1. Read `mcu_Info` to verify silicon revision
2. Configure `mcuclk` to set desired CPU clock frequency
3. Configure watchdog period (`wdt_period`)
4. Set GPIO mux registers (`sel_spi0`, `sel_uart0`, `sel_lin`, etc.)
5. Enable watchdog by setting wdt_period > 0
6. Enable desired SCU interrupts in `inten`
7. Clear `intstat` and `diag` (write 1 to clear any stale POR flags)
8. Proceed to peripheral initialisation

---

## 9. Project-Specific Notes

- **SCU `sel_spi0`** is not used in this project — SPI0 is not active at runtime. Leave `sel_spi0` at its reset value (0x0000_0000). If SCI0 UART debug mode is enabled during development, configure `sel_uart0` to assign the UART RX pin; `sel_spi0` remains untouched.
- **Watchdog must be kicked** inside the main 20 kHz control loop ISR (or a dedicated software timer ISR). If the main ISR stalls or the CPU hangs, the watchdog must trigger a reset within the configured period.
- **GDU fault flag in SCU** is mirrored from the GDU diagnostic register. When this flag is set, the firmware must immediately set ENABLE = LOW and stop commutation. This is a real-time safety response.
- **VD50 undervoltage** means GPIO logic voltage is lost. In this condition, Hall inputs (PD0–PD2) and speed commands (PD3–PD4) are unreliable. Firmware must treat VD50 UV as a motor stop condition.
- **Die over-temperature:** On A89201, the SCU monitors junction temperature. If triggered, immediately disable the bridge and wait for thermal recovery. Do not re-enable without a cool-down delay.
- **CPU lock-up detection:** The Cortex-M4 generates a hard-fault if it encounters an illegal instruction or access violation. The SCU flags this. Firmware hard-fault handler should log the PC/LR values to a non-volatile location (EEPROM) before resetting.
- The `nohlt` bit in CTRL disables WFI low-power. **Do not disable WFI** — using WFI in the main loop background task reduces idle current and improves EMC. The watchdog will still run during WFI.
