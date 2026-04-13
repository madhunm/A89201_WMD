# CTX_MCU_DMA — A89201 DMA Module

## Source
- Document: DMA.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The DMA module offloads data movement from the CPU, enabling peripheral-to-memory and memory-to-peripheral transfers without CPU involvement. This is particularly useful for burst current-sample readout (CAU → SRAM) and periodic angle readout (SCI0 SPI ↔ SRAM).

---

## 2. Features

- 3 DMA channels (DMA1, DMA2, DMA3)
- Transfer types:
  - Memory-to-Memory
  - Peripheral-to-Memory
  - Memory-to-Peripheral
  - Peripheral-to-Peripheral
- Memory-based linked-list transfer descriptors
- 3 descriptor sizes (balance between functionality and setup overhead)
- Multiple data-width transfers

---

## 3. Register Map

| Channel | Base Address   |
|---------|----------------|
| DMA1    | 0x40070000     |
| DMA2    | 0x40070080     |
| DMA3    | 0x40070100     |

Per-channel registers (offsets from channel base):

| Register | Access | Offset | Reset  | Description                |
|----------|--------|--------|--------|----------------------------|
| status   | R/W    | 0x40   | 0x0003 | Channel status flags       |
| control  | R/W    | 0x44   | 0x0000 | Channel control            |
| current  | R/W    | 0x48   | 0x0000 | Current descriptor pointer |

---

## 4. DMA Flow Control

- CAU and DAU both include DMA handshake signals for flow-controlled peripheral-to-memory transfers
- SCI0/SCI1 include DMA flow-control signals (scix_rx_ack) for receive buffer transfers
- DMA trigger sources are peripheral-generated; no polling required from CPU

---

## 5. Project-Specific Usage

### DMA1 — CAU Current Results → SRAM

Configured as Peripheral-to-Memory, triggered by CAU conversion-complete signal.

Purpose: Move three phase current ADC results (CAU_RESULT_A, CAU_RESULT_B, CAU_RESULT_C) into a SRAM buffer after each PWM cycle sample event. This keeps the control ISR lean: it only reads from SRAM rather than polling CAU registers.

Configuration:
- Source: CAU result registers (3 × 16-bit)
- Destination: `current_samples[3]` array in SRAM (0x20000000 region)
- Transfer width: 16-bit
- Burst length: 3 transfers per trigger
- Trigger: CAU conversion complete

### DMA2 — SCI0 SPI RX → SRAM (A1333 angle reads)

Configured as Peripheral-to-Memory, triggered by SCI0 RX data register full.

Purpose: Receive A1333 SPI angle response (16-bit packet) into a dedicated SRAM location without CPU involvement. CPU reads the SRAM buffer at its own pace (once per control cycle).

Configuration:
- Source: SCI0 RX data register
- Destination: `a1333_spi_rx_buf` in SRAM
- Transfer width: 16-bit
- Transfer count: 1 (single angle word per SPI transaction)
- Trigger: SCI0 RX complete (DMA flow-control ack)

### DMA3 — Reserved

Available for DAU multi-channel voltage scan results → SRAM if needed for diagnostic monitoring.

---

## 6. Project-Specific Notes

- **DMA1 and DMA2 must be initialised before the main control loop starts.** If DMA is not armed before the first PWM trigger, the first current sample interrupt will find the result registers un-transferred.
- **DMA does not disable interrupts.** The CAU ISR must still fire to signal the control loop that new data is available, but the actual register readout should be via DMA, not inline register reads.
- **Linked-list descriptors** are not required for this project (no scatter-gather needed). Simple single-descriptor transfers are sufficient.
- **DMA status register** should be polled or interrupt-driven to detect transfer errors (bus error, etc.) as part of ASIL-B runtime diagnostics.
- **Do not configure DMA3 for any safety-relevant transfer** without adding error detection. DMA memory corruption is a latent fault for ASIL-B.
- The NVIC has a DMA error interrupt that must be enabled and handled. On DMA error, the safe state is to disable gate outputs (ENABLE pin LOW) and assert a fault.
