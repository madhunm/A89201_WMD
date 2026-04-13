# CTX_MCU_FLI — A89201 Flash Interface (FLI)

## Source
- Document: FLI.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The FLI manages all access to the on-chip flash memory. It provides read, page erase, and program (burn) operations with hardware ECC generation. An instruction prefetch buffer improves execution speed when running code from flash.

---

## 2. Features

- Flash read with ECC generation and correction
- Instruction prefetch to improve code execution time
- Operations: read, page erase, program (burn)
- Interrupt generation for: flash operation complete, ECC single-bit error, ECC multi-bit error

---

## 3. Flash Memory Map

- Total: 128 kB (64 pages × 4 kB per page)
- AHB base address: **0x0800_0000**
- AHB end address: **0x0803_FFFF**
- Each page: 4 kB
- **Pages 54 and 63: reserved and protected. Cannot be erased or programmed by user firmware.**

```
0x0800_0000  Page 0   (4 kB) — User firmware start (vector table)
0x0800_1000  Page 1   (4 kB)
...
0x0803_5000  Page 53  (4 kB)
0x0803_6000  Page 54  (4 kB) — RESERVED/PROTECTED
...
0x0803_F000  Page 63  (4 kB) — RESERVED/PROTECTED
```

For firmware, pages 0–53 and 55–62 are available: 61 pages × 4 kB = 244 kB usable.

---

## 4. Flash Operations

### 4a. Read
- Accessed via AHB bus like normal memory
- ECC checked automatically on read
- Instruction prefetch enabled by default (improves loop execution in flash)

### 4b. Page Erase

Sequence:
1. Configure `FLTIME` register: erase operation requires 130 ms timing parameter
2. Write page address to flash control register
3. Set ERASE bit in flash control register
4. Wait for operation complete interrupt or poll status flag
5. Verify page is erased (all 0xFF) if required

**Note:** During erase and program operations, flash is not accessible for instruction fetch. If the application is running from flash, the erase/program operation must be executed from RAM. Copy the flash programming routine to SRAM and execute from 0x2000_0000 region.

### 4c. Program (Burn)

Sequence:
1. Configure `FLTIME` register: program operation requires 20 µs timing parameter
2. Write data to flash write buffer
3. Trigger program operation
4. Wait for complete interrupt or poll status
5. Read back and verify against source data

The word "burn" is used throughout FLI documentation to mean "program."

---

## 5. ECC

- Hardware ECC generated on every write
- Single-bit errors: corrected automatically on read, fault flag set in SCU `ecc_*` registers
- Multi-bit errors: uncorrectable, SCU interrupt generated
- Multi-bit flash ECC error = fatal fault → disable bridge, log to EEPROM if possible, enter safe state

---

## 6. FLTIME Register

The FLTIME register must be programmed with correct timing values based on the active CPU clock frequency. Incorrect FLTIME results in:
- Incomplete erase (data retention failure)
- Unreliable programming
- Potential corruption of adjacent pages

At 40 MHz system clock:
- Erase parameter: 130 ms → FLTIME_ERASE = 130,000,000 ns / (1/40MHz) = 5,200,000 counts (example — verify exact formula from FLI register definition)
- Burn parameter: 20 µs → FLTIME_BURN = 800 counts (verify)

---

## 7. Instruction Prefetch

The prefetch buffer reduces flash access latency for sequential code execution. It is beneficial for tight loops in the control algorithm. The prefetch buffer should remain enabled during normal operation.

Disable prefetch only during flash programming or erase operations (mandatory — the flash bus is unavailable during these operations).

---

## 8. Project-Specific Usage

### Flash Memory Layout (Recommended)

```
Page 0    (0x0800_0000): Interrupt vector table + boot stub
Pages 1–40 (0x0800_1000–0x0802_7FFF): Main firmware (160 kB)
Pages 41–50 (0x0802_9000–0x0803_1FFF): Calibration data + parameters (40 kB)
Pages 51–53 (0x0803_3000–0x0803_5FFF): Fault log (12 kB, circular buffer)
Pages 55–62 (0x0803_7000–0x0803_EFFF): Reserved for OTA update staging
Page 63    (0x0803_F000): PROTECTED — do not use
```

### Calibration Data Storage

The A1333 zero-offset calibration and current sense calibration offsets should be stored in flash pages 41–50. These are written once during end-of-line calibration and read at every boot.

```c
// Calibration struct written to flash page 41
typedef struct {
    uint16_t a1333_zero_offset;     // Electrical angle at mechanical 0°
    int16_t  cau_offset_a;          // CAU phase A zero-current offset
    int16_t  cau_offset_b;          // CAU phase B zero-current offset
    int16_t  cau_offset_c;          // CAU phase C zero-current offset
    uint16_t speed_low_rpm_target;  // 40 RPM (nominally 40)
    uint16_t speed_high_rpm_target; // 60 RPM (nominally 60)
    uint32_t crc32;                 // CRC32 of above fields
} CalibrationData_t;
```

### Fault Log (Circular Buffer)

Pages 51–53 hold a persistent fault log. On any fault entry, the firmware appends a fault record (timestamp, fault code, motor state snapshot) to the log. On page full, advance to next page and erase previous. On boot, the last fault log is read and used for diagnostics/debug.

---

## 9. Project-Specific Notes

- **Flash programming from flash is not allowed.** The FLI cannot read instructions from a page while erasing or burning that same page, and the prefetch buffer does not protect against this. All flash write routines must execute from SRAM. Copy the write function to SRAM at startup if calibration writes are expected at runtime.
- **Pages 54 and 63 are hardware-protected.** Attempting to erase or program them has no effect. Do not include them in any memory layout.
- **Flash endurance:** 10,000 program cycles. The fault log must not be written more than 10,000 times per page per device lifetime. At 1 fault log entry per hour over a 15-year vehicle life: 15 × 365 × 24 = 131,400 events. This exceeds the 10,000 cycle limit. Use a multi-page circular buffer and track write counts per page. The 12 kB log region (3 pages) distributes wear: 10,000 cycles × 3 pages × (4 kB / record_size) gives adequate life.
- **EEPROM vs. Flash for calibration:** The A89201 has 64 bytes of EEPROM (on GDU die, 5000 cycles at low temp). For calibration data exceeding 64 bytes, use flash pages 41–50. For the A1333 zero-offset (2 bytes) and system mode flags, EEPROM is suitable.
- **ECC multi-bit error in code pages:** If the CPU fetches an instruction from a multi-bit-error flash location, the CPU will execute corrupted code. The SCU ECC interrupt may not fire fast enough to prevent this. Mitigation: run from IRAM for safety-critical routines, or implement a startup flash integrity check (CRC over all code pages before enabling the bridge).
