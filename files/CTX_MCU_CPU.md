# CTX_MCU_CPU — A89201 CPU Module

## Source
- Document: CPU.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Core

- **ARM Cortex-M4F**
- Clock: up to 40 MHz (configured via SCU mcuclk register)
- FPU: IEEE 754 compliant (single precision)
- ARMv7-M Thumb-2 instruction set
- DSP extension instructions
- 32-bit hardware divider
- Little-endian
- Harvard architecture: separate instruction (ICode) and data (DCode) buses

---

## 2. NVIC

- 20 interrupt vectors
- 16 priority levels
- Low-latency interrupt service
- Configurable priority per source
- 8 interrupt priority levels visible to software

Interrupt sources include: CAU, DAU, GTU, SCI0, SCI1, LIN, SCU (watchdog, POR, faults), DMA, PGU, AMCT, GDU.

---

## 3. Memory Protection Unit (MPU)

- Present
- Allows partitioning of code, data, and peripheral regions
- Required for ASIL-B firmware partitioning

---

## 4. Debug Interface

- 2-wire SWD (Serial Wire Debug)
- Pin TEST/SWDIO: tie to GND for normal operation, connect to VD33 to enable SWD
- Pin D0/SWCLK: SWD clock in debug mode
- 2 hardware breakpoints
- 1 watchpoint (data value match)
- SysTick timer available

---

## 5. Memory Map

| Address Range          | Size  | Matrix | Description           |
|------------------------|-------|--------|-----------------------|
| 0x0000_0000–0x0000_7FFF| 32 kB | NVM    | Boot ROM              |
| 0x0001_0000–0x0001_1FFF| 8 kB  | NVM    | Instruction RAM (IRAM)|
| 0x0800_0000–0x0803_FFFF| 128 kB| NVM    | Flash memory          |
| 0x2000_0000–0x2000_7FFF| 32 kB | SRAM   | Data RAM              |
| 0x4000_0000–0x4000_FFFF| 64 kB | P1 APB | APB peripheral region |
| 0x4001_0000–0x4001_FFFF| 64 kB | P1     | PWM generator         |
| 0x4002_0000–...        | —     | —      | Additional peripherals|

**Note:** IRAM is separate from SRAM. Time-critical ISR code can be located in IRAM for deterministic execution. SRAM is the main data workspace.

---

## 6. Bus Architecture

- Two independent AHB-Lite buses (multi-layer)
- Each supports Harvard architecture (instruction + data bus)
- Peripherals connected via APB (Advanced Peripheral Bus) bridge
- Two master nodes (CPU and DMA) can access different peripherals simultaneously without penalty
- Bus arbitration occurs only when both masters access the same matrix
- Non-populated AHB/APB address access generates a bus fault IRQ to CPU

---

## 7. Clock

- On-chip resistor oscillator: ±1% accurate (REXT = 24.9 kΩ on REXT pin)
- Programmable clock divider in SCU (mcuclk register)
- Default boot clock: configured by boot ROM
- PLL available (configured via SCU)
- SCU monitors PLL lock, PLL timeout, and PLL source timeout as fault conditions

---

## 8. Startup Sequence

On power-on reset:
1. Boot ROM executes from 0x0000_0000
2. Boot ROM initialises memory ECC (via SCU)
3. Boot ROM checks ENABLE pin and TEST pin state
4. Boot ROM loads user application from flash at 0x0800_0000
5. Vector table must be at flash base (or relocated by firmware)

---

## 9. Keil uVision Configuration

- Device family: ARM Cortex-M4F
- Use CMSIS pack for Cortex-M4 (no vendor-specific CMSIS pack — Allegro does not publish one; peripheral access must use direct register address definitions)
- Startup file: custom, per A89201 memory map (not a standard ST/NXP file)
- Linker scatter file: must define regions for BootROM (RO), IRAM, Flash, SRAM
- FPU must be enabled in project options (Use FPU: yes, ABI: hard float)
- Optimisation: -O1 minimum for timing-critical code; critical ISRs should be in IRAM

Recommended scatter file regions:
```
LOAD_FLASH 0x08000000 0x20000
{
  ER_FLASH 0x08000000 0x20000
  { *(RESET, +First); .ANY (+RO) }

  ER_IRAM  0x00010000 0x2000
  { ISR_handlers.o (+RO) }

  RW_SRAM  0x20000000 0x8000
  { .ANY (+RW, +ZI) }
}
```

---

## 10. Project-Specific Notes

- At 40 MHz, one instruction cycle = 25 ns. The main control ISR (triggered by PGU at 20 kHz) must complete within 50 µs. Budget approximately 1500 instructions for the ISR body.
- The Cortex-M4F FPU eliminates software float emulation overhead. Use `float` (not `double`) for all control variables (PID, angle, speed). Double-precision operations use the FPU inefficiently on M4F.
- Tail-chaining and late-arriving interrupt handling in NVIC significantly reduces ISR latency. Priority must be set correctly: CAU conversion complete > speed command GPIO > AMCT hall event > GTU timer overflow > SCI (A1333 poll) > LIN.
- MPU should be configured to mark stack region as no-execute and SRAM peripheral area as non-cacheable for ASIL-B compliance evidence.
- The hardware divider (SDIV/UDIV instructions) is 32-bit only. For 64-bit division, fall back to software — avoid in ISR.
