# CTX_KEIL_SETUP — Keil uVision Project Setup for A89201 Wiper Firmware

## Source
- Reference project: `github.com/madhunm/A89201_WMD`, folder `Reference/A89201_MotorControl_FOC_ExampleCode_v1.0.1 (Beta)`
- Files examined: `.uvprojx`, `startup_ARMCM4_89201.s`, `system_ARMCM4.c`, `MCAL_128Kb.sct`, `A89201RegisterDefinitions.h`, `Gpio.h/c`, `Spi.h/c`, `Sci.h/c`, `SystemControl.h/c`, `Platform_Types.h`, `Main.c`, driver headers

---

## 1. Project Files to Copy from Reference

Copy these files verbatim into your project tree (do not modify):

| Source in reference | Destination in project | Purpose |
|---------------------|----------------------|---------|
| `Templates/startup_ARMCM4_89201.s` | `Core/` | Vector table + reset handler |
| `Templates/system_ARMCM4.c` | `Core/` | SystemInit, FPU enable, VTOR |
| `Resources/FOC/Core/system_ARMCM4.h` (from Cdrivers) | `Core/` | SystemCoreClock define |
| `Templates/MCAL_128Kb.sct` | `Templates/` | Scatter file for 128KB flash (A89201C) |
| `Templates/A89201.SFR` | `Templates/` | Keil register viewer definitions |
| `Flash/A892XXF128kBv1.2.0.FLM` | `Flash/` | Flash programming algorithm |
| `Cdrivers/Headers/*.h` | `Cdrivers/Headers/` | MCAL headers — all 16 files |
| `Cdrivers/Sources/*.c` | `Cdrivers/Sources/` | MCAL sources — all 14 files |

The Allegro MCAL layer (Cdrivers) is provided as C source, not a binary library. Include all `.c` files in the build.

---

## 2. Keil Project Settings (Target Options)

### Device Tab

No A89201-specific CMSIS device pack exists. Use the generic ARM Cortex-M4 with FP device:

- **Device:** `ARMCM4_FP`
- **Vendor:** ARM
- **CMSIS Pack:** ARM.CMSIS.5.6.0 (install via Pack Installer before creating project)

When creating the project, select "ARM Cortex M4 (with FPU)" from the device database.

### C/C++ (AC6) Tab

| Setting | Value |
|---------|-------|
| Compiler | AC6 (ARMCLANG) — do NOT use AC5 (ARMCC) |
| Optimisation | -O1 (`<Optim>5</Optim>` in XML = level 1) |
| C Language | C99 |
| Enable GNU extensions | Yes (`uGnu=1`) |
| Short enums/wchar | Enabled (`vShortEn=1`, `vShortWch=1`) |
| Plain char is signed | Yes |
| RTTI | Disabled |
| Exceptions | Disabled (`-fno-exceptions`) |
| Debug info | `-g` |
| Misc Controls | `-fno-exceptions -g` |

**Preprocessor defines:** `ARMCM4_FP`

This define is required by `system_ARMCM4.c` to include `ARMCM4_FP.h` (which configures the FPU) and by `system_ARMCM4.h` to set `__FPU_USED = 1`.

**Include paths (adjust to your folder structure):**
```
..\Cdrivers\Headers
.\Drivers\Headers
.\MotorControl\Headers
.\Config\Headers
.\Application\Headers
.\Segger\Headers
```

### Linker Tab

| Setting | Value |
|---------|-------|
| Scatter file | `..\Templates\MCAL_128Kb.sct` (adjust relative path) |
| Use scatter file | Yes |
| Misc controls | `--bestdebug --list_mapping_symbols --symbols --info=summarystack --any_placement=best_fit --diag_suppress L6776W --diag_suppress L6312W --diag_suppress L6314W --diag_suppress L6329W` |

### Output Tab

- Create Executable: Yes
- Create HEX file: Yes
- Debug information: Yes
- Browse information: Yes
- Output path: `.\Objects\`

---

## 3. SFR (Register Viewer) Configuration

In the project Target Options → Device tab, under "SFD File":
- Point to `A89201.SFR` (the file in the Templates folder)
- Tick "Use Custom SFD File"

This enables the Keil Registers window to show A89201 peripheral registers by name during debug sessions.

---

## 4. Flash Download Configuration

In Target Options → Utilities:
- Flash Download: enabled
- Update target before debugging: Yes
- Use target driver for flash programming: Yes
- Add the flash algorithm:
  - Click "Add" → navigate to the `Flash/` folder
  - Select `A892XXF128kBv1.2.0.FLM`
  - Start address: `0x08000000`, size: `0x20000` (128KB)
- Verify on download: Yes

The two other FLM files in the reference (`A892XX-256kBv1.0.4.FLM`, `A892XX_FNX-EX-256K-141A.FLM`) are for 256KB variants. The A89201C is 128KB — use only `A892XXF128kBv1.2.0.FLM`.

---

## 5. Debug Configuration

- Interface: J-Link
- SWD (2-wire), not JTAG
- Target device: Cortex-M4 (the reference J-Link `.ini` incorrectly has "ARM7" — do not copy it)
- Port speed: 4 MHz SWD clock (start conservative; can increase after confirming connection)
- Reset type: VECTRESET (soft reset via NVIC AIRCR)

J-Link settings to configure manually in the Debug tab:
- Interface: SW (Serial Wire Debug)
- Speed: 4000 kHz
- Reset: VECTRESET

**RTT viewer (replaces SWO/ITM):**
The reference project uses SEGGER RTT (`SEGGER_RTT.c`, `SEGGER_RTT_printf.c`) for debug output rather than SWO/ITM. RTT does not require SWO configuration. Open Segger RTT Viewer separately to see debug output — RTT runs over the SWD interface already used for debug.

Include `Segger/Sources/SEGGER_RTT.c` and `Segger/Sources/SEGGER_RTT_printf.c` in the development build. Clock-gate or stub these in production.

---

## 6. Scatter File — Memory Layout (128KB A89201C)

File: `MCAL_128Kb.sct` — exact values extracted from reference:

```
Flash code region:  0x08000000 – 0x0801DFFF  (0x1E000 = 122,880 bytes)
Flash data block:   0x0801E000 – 0x0801FFFF  (0x2000  =   8,192 bytes, read-only data, init_array)

SRAM total:         0x20000000 – 0x20007FFF  (0x8000  =  32,768 bytes)
  Reserved UNINIT:  0x20000000 – 0x20000007  (8 bytes  — DO NOT USE, platform reserved)
  RAM region 1:     0x20000008 – 0x200009B7
  BootROM Flash RAM:0x200009B8 – 0x200009DF  (0x28 = 40 bytes UNINIT — BootROM Flash drivers)
  RAM region 2:     0x200009E0 – 0x20003FFF
  Heap:             0x20004000 – 0x20005FFF  (0x2000 = 8,192 bytes)
  Stack:            0x20006F00 – 0x20007EFF  (0x1000 = 4,096 bytes)
  (unused):         0x20007F00 – 0x20007FFF  (256 bytes)
```

**Critical:** The 40 bytes at `0x200009B8–0x200009DF` are marked `UNINIT` and reserved for the BootROM flash programming subroutines. Never place variables there. Verify that your BSS/ZI regions avoid this range by checking the map file after first build.

Stack pointer on reset: `__initial_sp` = top of Stack area = `0x20006F00 + 0x1000 = 0x20007F00`.

**Calibration flash pages** (CTX_MCU_FLI):
- WiperCalData_t: **page 28** at `0x0801C000`. Runtime log: **page 29** at `0x0801D000`. Both confirmed within 128KB flash range.
- **Resolved in CTX_MCU_FLI:** Page 28 (0x0801_C000) = WiperCalData_t. Page 29 (0x0801_D000) = runtime fault log. Scatter file code region reduced to 0x1C000 (pages 0–27). Data block pages 30–31 are linker-managed const data — do not erase.

---

## 7. Startup File — Vector Table

File: `startup_ARMCM4_89201.s` — use the copy in `Templates/` (identical to `Resources/FOC/Core/`).

Stack size: `0x1000` (4KB). Heap size: `0x2000` (8KB). Both defined in the startup `.s` file. If you change these, also update the scatter file constants `STACK_SIZE` and `HEAP_SIZE`.

**Complete IRQ vector table** (from `Platform_Types.h` — use these enum names with `NVIC_SetPriority`):

| IRQ# | eIRQNUM enum | Handler name in startup | This project |
|------|-------------|------------------------|--------------|
| 0 | `eIRQNUM_CAU` | `Cau_IrqHandler` | 20 kHz PWM/current loop trigger — PRIORITY 0 |
| 1 | `eIRQNUM_DAU` | `Dau_IrqHandler` | IG and analog monitoring |
| 2 | `eIRQNUM_TIMER1` | `Timer1_IrqHandler` | Hall A period (GTU TIMER0 in CTX_MCU_GTU) |
| 3 | `eIRQNUM_TIMER2` | `Timer2_IrqHandler` | Hall B period |
| 4 | `eIRQNUM_TIMER3` | `Timer3_IrqHandler` | Hall C period |
| 5 | `eIRQNUM_TIMER4` | `Timer4_IrqHandler` | 1 ms system tick |
| 6 | `eIRQNUM_TIMER5` | `Timer5_IrqHandler` | Spare |
| 7 | `eIRQNUM_TIMER6` | `Timer6_IrqHandler` | Spare |
| 8 | `eIRQNUM_TIMER7` | `Timer7_IrqHandler` | Spare |
| 9 | `eIRQNUM_TIMER8` | `Timer8_IrqHandler` | Spare |
| 10 | `eIRQNUM_SCI0` | `Sci_IrqHandler0` | SPI0 RX complete — A1333 angle data ready |
| 11 | `eIRQNUM_SCI1` | `Sci_IrqHandler1` | Debug UART (dev only) |
| 12 | `eIRQNUM_LIN` | `Lin_IrqHandler` | Reserved — not used |
| 13 | `eIRQNUM_GDU` | `GduMaster_IrqHandler` | GDU fault reporting |
| 14 | `eIRQNUM_I2C` | `I2c_IrqHandler` | Not used |
| 15 | — | `Interrupt15_Handler` | Reserved |
| 16 | `eIRQNUM_GPIO` | `Gpio_IrqHandler` | Hall commutation + speed commands |
| 17 | `eIRQNUM_AMCT` | `Amct_IrqHandler` | AMCT commutation report |
| 18 | `eIRQNUM_DMA` | `Dma_IrqHandler` | DMA completion (CAU → SRAM, SPI RX → SRAM) |
| 19 | `eIRQNUM_FLASH_OR_SYSTEM_FAULT` | `Scu_FlashOrSystemFaultIrqHandler` | ECC / system fault — PRIORITY 0 |

**Naming note:** The reference code and startup file use 1-indexed timer names (Timer1–Timer8) mapping to IRQ numbers 2–9. The context files CTX_MCU_GTU used 0-indexed names (TIMER0–TIMER7). Mapping: TIMER0 in CTX_MCU_GTU = `Timer1_IrqHandler` / `eIRQNUM_TIMER1`.

All IRQ handlers in the startup file are declared `WEAK` — implement them in your C files by defining a function with the exact name shown in the "Handler name" column.

---

## 8. NVIC Priority Configuration

Cortex-M4 on A89201 has 4-bit priority fields (16 priority levels, 0 = highest). Configure in `main()` before enabling interrupts:

```c
/* CRITICAL — set before any interrupt is enabled */
NVIC_SetPriority(eIRQNUM_CAU, 0);                    /* 20 kHz current loop — cannot be preempted */
NVIC_SetPriority(eIRQNUM_FLASH_OR_SYSTEM_FAULT, 0);  /* ECC / system fault — safety critical */
NVIC_SetPriority(eIRQNUM_GPIO, 1);                   /* Hall commutation interrupts */
NVIC_SetPriority(eIRQNUM_AMCT, 1);                   /* AMCT commutation report */
NVIC_SetPriority(eIRQNUM_GDU, 1);                    /* Gate driver fault */
NVIC_SetPriority(eIRQNUM_SCI0, 2);                   /* A1333 SPI angle read */
NVIC_SetPriority(eIRQNUM_TIMER1, 3);                 /* Hall A period measurement */
NVIC_SetPriority(eIRQNUM_TIMER2, 3);                 /* Hall B period measurement */
NVIC_SetPriority(eIRQNUM_TIMER3, 3);                 /* Hall C period measurement */
NVIC_SetPriority(eIRQNUM_TIMER4, 4);                 /* 1 ms system tick */
NVIC_SetPriority(eIRQNUM_DMA, 4);                    /* DMA completion */
NVIC_SetPriority(eIRQNUM_DAU, 5);                    /* IG / analog monitoring */
```

Then enable each IRQ with `NVIC_EnableIRQ(eIRQNUM_xxx)` after the corresponding peripheral is fully initialised.

---

## 9. GPIO Initialisation API

The Allegro MCAL GPIO driver uses enum-based instance IDs. `eGPIO_INSTANCE_IDx` maps directly to pin PDx.

**Enum mapping:**
| Instance ID | Physical pin |
|-------------|-------------|
| `eGPIO_INSTANCE_ID0` | PD0 |
| `eGPIO_INSTANCE_ID1` | PD1 |
| ... | ... |
| `eGPIO_INSTANCE_ID7` | PD7 |
| `eGPIO_INSTANCE_ID8` – `eGPIO_INSTANCE_ID15` | PD8–PD15 (internal die-to-die only) |

**GPIO modes (`enum GpioMode`):**
- `eGPIO_MODE_DISABLED = 0` — high-impedance input
- `eGPIO_MODE_DIGITAL_INPUT = 1` — digital input, no pull
- `eGPIO_MODE_DIGITAL_INPUT_PULLUP = 2` — digital input with internal pull-up
- `eGPIO_MODE_DIGITAL_INPUT_PULLDOWN = 3` — digital input with internal pull-down
- `eGPIO_MODE_ANALOG_INPUT_SOURCE = 4` — analog input (for DAU)
- `eGPIO_MODE_OUTPUT_PUSH_PULL = 5` — digital output push-pull
- `eGPIO_MODE_OUTPUT_PULLDOWN = 6` — open-drain with pull-down
- `eGPIO_MODE_OUTPUT_PULLUP = 7` — open-drain with pull-up

**Output source (`enum GpioSource`):**
- `eGPIO_SOURCE_DOUT = 16` — manually driven output (software control)
- `eGPIO_ASRC0 = 0` → `GPIO_CONTROL_ASRC_SEL_SCIO_TX` = SPI0 MOSI
- `eGPIO_ASRC2 = 2` → `GPIO_CONTROL_ASRC_SEL_SPIO_SCKOUT` = SPI0 SCK
- `eGPIO_ASRC4 = 4` → `GPIO_CONTROL_ASRC_SEL_SPIO_CSOUT` = SPI0 CS

**Project GPIO init for wiper firmware:**

```c
void GPIO_Init_WiperProject(void) {

    /* --- Hall sensor inputs (PD0, PD1, PD2) --- */
    /* APS12202 is open-drain → pull-up required.                               */
    /* NO GPIO interrupt on these pins. Hall transitions are handled by:         */
    /*   - AMCT (commutation, IRQ17): reads PD0-2 via AMCT_CONFIG_HALLxSELECT   */
    /*   - GTU TIMER1-3 (period capture, IRQ2-4): measures inter-edge intervals  */
    /* Enabling GPIO interrupt on PD0-2 would cause spurious Gpio_IrqHandler     */
    /* invocations at ~500 Hz per phase that serve no purpose.                   */
    Gpio_SetPinActive(eGPIO_INSTANCE_ID0);
    Gpio_SetMode(eGPIO_INSTANCE_ID0, eGPIO_MODE_DIGITAL_INPUT_PULLUP);
    /* No IntConfig / IntEnable call — AMCT and GTU handle Hall directly */

    Gpio_SetPinActive(eGPIO_INSTANCE_ID1);
    Gpio_SetMode(eGPIO_INSTANCE_ID1, eGPIO_MODE_DIGITAL_INPUT_PULLUP);

    Gpio_SetPinActive(eGPIO_INSTANCE_ID2);
    Gpio_SetMode(eGPIO_INSTANCE_ID2, eGPIO_MODE_DIGITAL_INPUT_PULLUP);

    /* --- Low speed command (PD3) --- */
    /* This is the ONLY user GPIO pin with GPIO interrupt enabled.               */
    /* The shared Gpio_IrqHandler (IRQ16) fires only for PD3 changes.           */
    Gpio_SetPinActive(eGPIO_INSTANCE_ID3);
    Gpio_SetMode(eGPIO_INSTANCE_ID3, eGPIO_MODE_DIGITAL_INPUT_PULLDOWN);
    Gpio_SetPinIntConfig(eGPIO_INSTANCE_ID3, eGPIO_INTCFG_RISING_FALLING);
    Gpio_SetPinIntEnable(eGPIO_INSTANCE_ID3, True);

    /* --- SPI0 CS output (PD4) — hardware managed by SPI peripheral --- */
    Gpio_SetPinActive(eGPIO_INSTANCE_ID4);
    Gpio_SetMode(eGPIO_INSTANCE_ID4, eGPIO_MODE_OUTPUT_PUSH_PULL);
    Gpio_SetSource(eGPIO_INSTANCE_ID4, eGPIO_ASRC4);  /* eGPIO_ASRC4 = SPI0_CSOUT */

    /* --- SPI0 SCK output (PD5) --- */
    Gpio_SetPinActive(eGPIO_INSTANCE_ID5);
    Gpio_SetMode(eGPIO_INSTANCE_ID5, eGPIO_MODE_OUTPUT_PUSH_PULL);
    Gpio_SetSource(eGPIO_INSTANCE_ID5, eGPIO_ASRC2);  /* eGPIO_ASRC2 = SPI0_SCKOUT */

    /* --- SPI0 MOSI output (PD6) --- */
    Gpio_SetPinActive(eGPIO_INSTANCE_ID6);
    Gpio_SetMode(eGPIO_INSTANCE_ID6, eGPIO_MODE_OUTPUT_PUSH_PULL);
    Gpio_SetSource(eGPIO_INSTANCE_ID6, eGPIO_ASRC0);  /* eGPIO_ASRC0 = SCI0_TX = SPI0 MOSI */

    /* --- SPI0 MISO input (PD7) --- */
    /* Configure the SCU mux to point SPI0 RX to PD7 */
    /* SCU base = 0x4000D000, SEL_SPI0 offset = 0x40, value = 7 for PD7 */
    volatile uint32_t* pScuSelSpi0 = (volatile uint32_t*)(0x4000D000U + 0x40U);
    *pScuSelSpi0 = 7U;
    Gpio_SetPinActive(eGPIO_INSTANCE_ID7);
    Gpio_SetMode(eGPIO_INSTANCE_ID7, eGPIO_MODE_DIGITAL_INPUT);
    /* Note: do NOT call Gpio_SetSource() on PD7 — it is an input */

    /* --- IG pin (high speed command) --- */
    /* IG is a dedicated HV input — no GPIO_CONTROLx configuration needed */
    /* Read IG digital status via DAU or GDU status register at 100 Hz in main loop */
    /* IG: VIH ≥ 4.5V, VIL ≤ 2.1V, 300 kΩ pull-down, −4V to 50V rated */
}

/* Gpio_IrqHandler — shared single entry point for ALL PD0-PD7 GPIO interrupts */
/* Only PD3 has interrupt enabled in this project, so only PD3 should ever fire */
void Gpio_IrqHandler(void) {
    volatile uint32_t* pEvt = (volatile uint32_t*)0x40008068U;  /* GPIO_EVENT_STATUS */
    uint32_t events = *pEvt;
    *pEvt = events;    /* write-1-to-clear */

    if (events & (1U << 3)) {   /* PD3: low speed command */
        uint8_t lvl = (*(volatile uint32_t*)GPIO_PDIN_ADDR >> 3) & 1U;
        SpeedCommand_ProcessChange(eSPEED_LOW, lvl);
    }
    /* Safety check: PD0-PD2 should never fire */
    if (events & 0x07U) SetFault(FAULT_GPIO_SPURIOUS_HALL_IRQ);
}
```

**Interrupt config enum (used with `Gpio_SetPinIntConfig`):**
These match the `GPIO_CONTROL_INTCFG_*` constants in `Gpio.h`:
- `eGPIO_INTCFG_RISING_EDGE` — rising edge only
- `eGPIO_INTCFG_RISING_FALLING` — both edges (use for Hall sensors and speed commands)
- `eGPIO_INTCFG_LEVEL_HIGH` — level high
- `eGPIO_INTCFG_LEVEL_LOW` — level low

If these API enum names differ in your version of Gpio.h, the raw register bit is `GPIO_CONTROL_INTCFG_RISING_FALLING_EDGE = 0x0400`.

---

## 10. SPI0 Initialisation API

```c
void SPI0_Init_A1333(void) {
    /* 1. Select SPI0 port — also calls Sci_Use(eSPI, 0) internally */
    /*    This sets SCU_CTRL bit SCU_CONTROL_SCI0_MODE (bit 5) to SPI mode */
    Spi_SetPort(eSPI_PORTID_0);

    /* 2. Configure SPI operating mode */
    Spi_SetMode(eMASTER_DEFAULT);            /* SPI master */
    Spi_SetWordSize(e16_BITS);               /* 16-bit word */
    Spi_SetBitOrdering(eMSB_DEFAULT);        /* MSB first */
    Spi_SetBitsPerWord(e16_BIT_WORD_TRANSFER); /* 16 bits per SPI frame */

    /* 3. A1333 SPI mode: CPOL=1 (clock idle HIGH), CPHA=1 (sample trailing edge) = Mode 3 */
    /*    "Trailing edge" for CPOL=1 means the rising edge → data sampled on rising edge */
    Spi_SetClockPolarity(eCLOCK_HIGH);          /* CPOL = 1 */
    Spi_SetClockPhase(eSAMPLE_TRAILING_EDGE);   /* CPHA = 1 */

    /* 4. Clock rate: 4 MHz. At 40 MHz CPU: CYCLES_PER_BIT = (40M / (2*4M)) - 1 = 4 */
    /*    The driver accepts baud rate in Hz and converts internally */
    Spi_SetClockGeneration((enum SpiBaudRateIndex)4000000U);

    /* 5. Hardware CS: active-low, automatic mode */
    Spi_SetCsSense(eACTIVE_LOW_DEFAULT);         /* CS active low */
    Spi_SetCsMode(eAUTOMATIC_DEFAULT);           /* Hardware-managed CS */

    /* 6. CS timing for A1333: */
    /*    tCS (setup) ≥ 50 ns: 1 SPI clock period at 4 MHz = 250 ns ✓ */
    /*    tCHD (hold) ≥ 5 ns: 1 clock period = 250 ns ✓              */
    /*    tCS_IDLE (inter-frame) ≥ 200 ns: 1 clock period = 250 ns ✓ */
    Spi_SetCsTiming(eSETUP, 1U);
    Spi_SetCsTiming(eHOLD,  1U);
    Spi_SetCsTiming(eIDLE,  1U);

    /* 7. Enable SPI RX interrupt */
    Spi_SetIrqEnable(eSPI_RX_RECEIVED);

    /* 8. Register ISR callback — implement this function in your application */
    Spi_SetIrqHandler(A1333_SPI_RxISR);

    /* 9. Enable SPI0 */
    Spi_Open();

    /* 10. Enable SCI0 IRQ in NVIC (do after full SPI init) */
    NVIC_EnableIRQ(eIRQNUM_SCI0);
}

/* ISR callback — called from Sci_IrqHandler0 when 16-bit RX complete */
void A1333_SPI_RxISR(void) {
    a1333_raw_result = (uint16_t)Spi_Read();
    /* Set flag for main loop / position controller to process */
    a1333_data_ready = True;
}

/* Trigger an A1333 angle read (call at 1 kHz from CAU ISR or timer) */
void A1333_RequestAngleRead(void) {
    /* A1333 register 0x20 read: bit 15 = 0 (read), bits 14:8 = 0x20, bits 7:0 = don't care */
    Spi_Write(0x2000U);
    /* CS asserted, 16 clocks, CS deasserted — all hardware managed */
    /* RX result arrives in Sci_IrqHandler0 → A1333_SPI_RxISR callback */
}
```

**Note on `Spi_SetClockGeneration` parameter:** The enum `SpiBaudRateIndex` has no named 4 MHz entry. Pass the value directly as a cast: `(enum SpiBaudRateIndex)4000000U`. The MCAL driver computes `CYCLES_PER_BIT = SystemCoreClock / (2 * rate) - 1` internally.

---

## 11. AMCT Hall Sensor Configuration

Configure the AMCT subsystem to read Hall inputs from PD0/PD1/PD2:

```c
void AMCT_Init_Hall(void) {
    /* Select Hall mode (not BEMF) */
    ReadModifyWriteField(AMCT_BASE_ADDRESS + AMCT_CONFIG,
        AMCT_CONFIG_BEMF, 0, 0);        /* clear BEMF bit */
    SetRegisterField(AMCT_BASE_ADDRESS + AMCT_CONFIG,
        AMCT_CONFIG_HALL);              /* set HALL bit = 1 */

    /* Route Hall inputs: A=PD0, B=PD1, C=PD2 */
    ReadModifyWriteField(AMCT_BASE_ADDRESS + AMCT_CONFIG,
        AMCT_CONFIG_HALLASELECT_MASK, 4, 0);  /* GPIO_0 = PD0 */
    ReadModifyWriteField(AMCT_BASE_ADDRESS + AMCT_CONFIG,
        AMCT_CONFIG_HALLBSELECT_MASK, 7, 1);  /* GPIO_1 = PD1 */
    ReadModifyWriteField(AMCT_BASE_ADDRESS + AMCT_CONFIG,
        AMCT_CONFIG_HALLCSELECT_MASK, 10, 2); /* GPIO_2 = PD2 */

    /* Forward direction — change if motor runs backwards */
    SetRegisterField(AMCT_BASE_ADDRESS + AMCT_CONFIG, AMCT_CONFIG_FWD);
}
```

---

## 12. main() Startup Sequence

Match the pattern from the reference `Main.c`:

```c
int main(void) {
    /* 1. Disable watchdog (re-enable after init with correct timeout) */
    Scu_WatchdogDisable();

    /* 2. Enable flash prefetch */
    Scu_SetFlashControlPreFetchEnable(True);

    /* 3. Set NVIC priorities BEFORE enabling any peripheral */
    NVIC_SetPriority(eIRQNUM_CAU, 0);
    NVIC_SetPriority(eIRQNUM_FLASH_OR_SYSTEM_FAULT, 0);
    NVIC_SetPriority(eIRQNUM_GPIO, 1);
    NVIC_SetPriority(eIRQNUM_AMCT, 1);
    NVIC_SetPriority(eIRQNUM_GDU, 1);
    NVIC_SetPriority(eIRQNUM_SCI0, 2);
    NVIC_SetPriority(eIRQNUM_TIMER1, 3);
    NVIC_SetPriority(eIRQNUM_TIMER2, 3);
    NVIC_SetPriority(eIRQNUM_TIMER3, 3);
    NVIC_SetPriority(eIRQNUM_TIMER4, 4);
    NVIC_SetPriority(eIRQNUM_DMA, 4);

    /* 4. GPIO init (before any peripheral that uses GPIO outputs) */
    GPIO_Init_WiperProject();

    /* 5. GDU init (gate driver) */
    Gdu_Initialization();

    /* 6. PGU init (PWM generator) */
    PwmGen_Initialization();

    /* 7. CAU init (current sense) */
    CAU_Initialization();

    /* 8. AMCT init (Hall commutation) */
    AMCT_Init_Hall();

    /* 9. GTU init (Hall period timers) */
    Timer_Initialization();

    /* 10. SPI0 init (A1333 angle sensor) */
    SPI0_Init_A1333();

    /* 11. Load calibration data from flash */
    Flash_LoadCalibration(&calData);

    /* 12. A1333 startup: wait tPO, then first SPI read */
    SystemTimer_DelayMs(35);   /* ≥30 ms tPO + margin */
    A1333_RequestAngleRead();  /* First absolute angle read */

    /* 13. Enable watchdog with appropriate timeout */
    /* Scu_WatchdogEnable(WDT_PERIOD); */

    /* 14. Main loop */
    while (1) {
        /* Background: fault monitoring, A1333 diagnostics, speed command poll */
        /* High-rate control runs entirely in ISRs (CAU, GPIO, AMCT) */
    }
}
```

---

## 13. Project File Group Structure

Replicate this group layout in the Keil project window (Groups):

```
Core
    Main.c
    startup_ARMCM4_89201.s
    system_ARMCM4.c

Config
    motor_config.h          (project-specific parameters, not from reference)

Application
    wiper_control.c / .h    (speed command, position control, safe state)
    a1333_driver.c / .h     (SPI angle read, diagnostic parsing, EEPROM)

Drivers - Peripheral
    driver_gdu.c / .h       (gate driver wrapper — from reference, adapt)
    driver_pwm.c / .h       (PWM wrapper)
    driver_gpio.c / .h      (GPIO init wrapper)
    driver_timer.c / .h     (GTU timer wrapper)
    driver_cau.c / .h       (current sense wrapper)

Drivers - MCAL
    (all 14 Cdrivers sources/*.c files — do not modify)
    Amct.c, Cau.c, Dau.c, Dma.c, FlashMemory.c, GduMaster.c
    Gpio.c, Gtu.c, Lin.c, PwmGen.c, Sci.c, Spi.c
    SystemControl.c, SystemTimer.c

MotorControl
    (speed/position/current PI controllers — project-specific)

Segger
    SEGGER_RTT.c
    SEGGER_RTT_printf.c
    (development builds only — exclude from production target)
```

---

## 14. Build Configurations

Create two build targets in the project:

**Debug target:**
- Optimisation: -O0 (override reference -O1 to preserve all variables in watch window)
- Include Segger RTT
- Define: `DEBUG`, `ARMCM4_FP`
- Scatter file: `MCAL_128Kb.sct`
- Download and debug enabled

**Release target:**
- Optimisation: -O1 (as in reference)
- Exclude Segger RTT from build
- Define: `RELEASE`, `ARMCM4_FP`
- Scatter file: `MCAL_128Kb.sct`
- Create HEX for production programming

---

## 15. Common Build Issues and Fixes

**"Error: L6218E: Undefined symbol SystemCoreClock"**
→ `system_ARMCM4.c` is not in the build. Add it to the Core group.

**"Error: device not specified" in system_ARMCM4.c**
→ `ARMCM4_FP` not defined in preprocessor. Add it in C/C++ → Define.

**FPU not enabled (hardfault on float operations)**
→ `__FPU_USED` is 0. Verify `ARMCM4_FP` define is present and that the device is set to `ARMCM4_FP` (not `ARMCM4`). Check CPACR setting in `SystemInit`.

**"Warning: L6776W: Could not find mapping for..."**
→ Suppressed by `--diag_suppress L6776W` in linker misc controls. Safe to suppress.

**Flash not erased before program**
→ Ensure `UpdateFlashBeforeDebugging = 1` in Utilities. Confirm the FLM algorithm start address matches `LR_IROM1_START = 0x08000000`.

**J-Link "No Cortex-M device found"**
→ The reference `JLinkSettings.ini` has `Device="ARM7"`. Change this to `Cortex-M4` in the J-Link debug settings or configure via the Keil Debug tab.

**"Error: L6200E: Symbol Timer1_IrqHandler multiply defined"**
→ You have defined `Timer1_IrqHandler` in two compilation units. Each IRQ handler must be defined exactly once. Check both the MCAL and your application code.

---

## 16. Key Facts That Differ from Context Files

The context files (CTX_MCU_*) used different naming conventions from what the reference code implements:

| Context file name | Reference code name | Notes |
|-------------------|--------------------|----|
| TIMER0 | `Timer1` / `eIRQNUM_TIMER1` | +1 offset throughout |
| TIMER1 | `Timer2` / `eIRQNUM_TIMER2` | |
| SCI0 SPI (base 0x4000A000) | `SPI0` — both `Spi.h` and `Sci.h` use same base address | SPI and UART share hardware and interrupt; `Spi_SetPort()` calls `Sci_Use(eSPI)` internally |
| GPIO_CONTROLx ASRCSEL field | `eGPIO_ASRCx` enum (0–15) + `eGPIO_SOURCE_DOUT` (16) | Use the enum, not raw register values |
| `sel_spi0` write | Direct: `*(0x4000D040) = 7` for PD7 as MISO | No named API for SPI MISO pin selection was found; write SCU_SEL_SPI0 directly |
| "DAG=10" current gain | Verify in `GduMaster.h` or `driver_gdu.c` — exact register name TBD | Cross-reference with CTX_MCU_GDU |
