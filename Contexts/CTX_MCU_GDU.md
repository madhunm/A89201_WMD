# CTX_MCU_GDU — A89201 Gate Driver Unit (GDU)

## Source
- Document: GDU.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The GDU is the analog power stage of the A89201. It drives external N-channel power MOSFETs arranged in a 3-phase bridge. The MCU core communicates with the GDU over a dedicated internal serial bus (separate from the main AHB/APB bus). The GDU is a distinct die within the A89201 package communicating with the MCU die via this internal serial interface.

---

## 2. Features

- 3-phase gate driver for external N-channel MOSFETs
- Integrated gate slew rate control (programmable drive strength)
- Dedicated VREG charge pump for low-battery gate drive (VBB ≥ 7 V)
- VCP charge pump: enables full 0–100% duty cycle without restriction
- Per-phase high-side and low-side VDS monitoring (overcurrent protection)
- Bootstrap charge management
- On-chip 5 V LDO (VD50) for MCU power supply
- Battery-voltage-compliant external sensor terminal (IG — ignition input)
- Q&A (question-and-answer) watchdog between MCU and GDU
- Dedicated internal serial communication interface to MCU register set
- Open-drain fault output driver (MCLRn) to SCU

---

## 3. Gate Drive Architecture

For each of three phases (A, B, C):

| Signal | Terminal | Description                                      |
|--------|----------|--------------------------------------------------|
| GHx    | GHA/B/C  | High-side gate drive output (above Sx)           |
| Sx     | SA/SB/SC | Motor phase node (source of high-side FET)       |
| Cx     | CA/CB/CC | Bootstrap capacitor node (above Sx)              |
| GLx    | GLA/B/C  | Low-side gate drive output                       |
| LSS    | LSS      | Low-side common source / shunt kelvin return     |

Bootstrap supply: Cx is charged each time the low-side FET conducts. The VCP charge pump maintains bootstrap charge at high duty cycles.

---

## 4. Power Supply Chains

```
VBB (6.5–50 V)
  └── VREG charge pump → Gate drive supply (≥5 V relative to each Sx)
  └── VD50 LDO (5 V) → VIO → GPIO, MCU logic
  └── VD33 LDO (3.3 V) → Internal analog
  └── VCP charge pump → Bootstrap supply for 100% duty cycle
```

All internal supplies are generated from VBB. The MCU can also be powered independently from a 5 V external supply on VD50.

---

## 5. MCU–GDU Communication Interface

The MCU accesses GDU configuration and diagnostics through a dedicated serial communication peripheral mapped in the APB bus (not the main SCI0/SCI1). This is distinct from the SPI used for A1333.

GDU register map base address: per APB peripheral map (refer to CPU address map). Registers accessed via APB at the GDU block address.

Key GDU MCU-side registers:

| Register | Offset | Access | Description                              |
|----------|--------|--------|------------------------------------------|
| ADDR     | 0x00   | R/W    | GDU internal register address (write triggers transfer) |
| TX       | 0x04   | R/W    | 16-bit transmit data to GDU              |
| RX       | 0x08   | R      | 16-bit received data from GDU            |
| CTRL     | 0x0C   | R/W    | Mode select (disabled / user operation)  |
| INTEN    | 0x10   | R/W    | Interrupt enables: tx, rx, ack, crc, ovf, ff |
| INTSTAT  | 0x14   | R/W1C  | Interrupt status: tx, rx, ack, crc, ovf, ff, busy, polling |

**CTRL mode field:**
- 00: Disabled
- 01: User operation enabled (firmware-initiated transfers)
- Other: Automatic polling mode (GDU autonomously sends Thermal Condition Frame to MCU)

---

## 6. Automatic Polling (TCF)

In automatic polling mode, the GDU periodically sends a Thermal Condition Frame (TCF) to the MCU containing temperature and status data without CPU initiation. The MCU can initiate a user transfer; if the auto-read of TCF is in progress, the transfer is deferred until TCF completes.

---

## 7. Diagnostics and Protection

The GDU monitors and reports the following faults:

| Fault Type               | Detection Method           | Action                          |
|--------------------------|----------------------------|---------------------------------|
| High-side VDS overcurrent| VDS > threshold per phase  | Latch fault, assert MCLRn       |
| Low-side VDS overcurrent | VDS > threshold per phase  | Latch fault, assert MCLRn       |
| Bootstrap undervoltage   | Cx below VREG threshold    | Inhibit high-side drive         |
| Charge pump fault        | VREG or VCP out of range   | Inhibit all gate drives         |
| Over-temperature         | GDU die temperature        | MCLRn assertion, SCU flag       |
| Short to battery (phase) | SA/SB/SC above threshold   | Overcurrent detection           |
| Short to ground (phase)  | SA/SB/SC below threshold   | VDS monitoring                  |

MCLRn: GDU asserts this signal to SCU to trigger a system reset or fault interrupt. Firmware must read GDU INTSTAT to determine fault cause.

**ENABLE pin:** Active-high. Pulling ENABLE low immediately disables all gate outputs regardless of software state. Connect to a GPIO or a hardware logic gate for emergency shutdown.

---

## 8. Gate Drive Strength (Slew Rate Control)

Programmable drive strength controls gate current during MOSFET switching, which controls dV/dt and switch-node ringing. Stronger drive = faster switching = higher EMI.

For automotive wiper applications, moderate slew rate is preferred (balance between switching loss and EMI). Program this via GDU CTRL register after initial characterisation.

---

## 9. Dead Time

Cross-conduction protection: the GDU enforces a dead time between turning off one FET and turning on the complementary FET. Dead time is adjustable via GDU register.

**Dead time must be long enough to guarantee the outgoing FET is fully off before the incoming FET turns on.** For typical automotive power MOSFETs (rise/fall time ~100 ns), set dead time ≥ 200 ns (8 counts at 40 MHz). The PGU register for dead time must match GDU programming.

Dead time that is too short → shoot-through (catastrophic). Dead time that is too long → increases BEMF zero-crossing detection difficulty.

---

## 10. Q&A Watchdog

The GDU implements a question-and-answer watchdog protocol between the GDU and the MCU firmware. The MCU must respond to a challenge from the GDU within a defined timeout. Failure to respond correctly:
- Asserts MCLRn to SCU
- Disables gate outputs

For ASIL-B, the Q&A watchdog provides hardware monitoring of MCU liveness independent of the software watchdog. It must be serviced within the main control loop.

**Q&A service location in firmware:** Inside the 20 kHz control ISR or inside the 1 ms TIMER4 ISR. Whichever is chosen, the timeout must be configured accordingly. Recommended: service every 1 ms via TIMER4 ISR.

---

## 11. ENABLE Pin Management

ENABLE must be driven HIGH to activate gate outputs. Recommended logic:

```c
// ENABLE is controlled by a dedicated GPIO (external to A89201, or via MCU signal)
// Safe state: ENABLE = LOW
// Normal operation: ENABLE = HIGH

void MotorStart(void) {
    // Clear all faults before enabling
    GDU_ClearFaults();
    ClearSafeState();
    ENABLE_HIGH();
}

void MotorStop(void) {
    // Controlled stop: ramp down PWM duty cycle first, then disable
    PWM_RampDown();
    ENABLE_LOW();
}

void FaultStop(void) {
    // Immediate: no ramp
    ENABLE_LOW();
}
```

---

## 12. Project-Specific Notes

- **MCLRn assertion by GDU:** When GDU asserts MCLRn, the SCU receives a fault signal. The firmware SCU ISR must read GDU INTSTAT to identify the fault (VDS overcurrent, OTP, etc.) before clearing. Log the fault type to EEPROM for field diagnostics.
- **Bootstrap capacitor sizing:** CBC must be sized per A89201 datasheet Section 15. A large value increases charge time (limits minimum duty cycle at startup). A small value causes insufficient gate drive at high duty cycles. Typical: 100 nF. Verify against MOSFET Qgate.
- **VDS threshold programming:** The overcurrent threshold is set via GDU registers. Set the VDS threshold to correspond to 30 A peak × R_DS(on) of selected MOSFET. This threshold must be set before the ENABLE pin is asserted.
- **Gate drive frequency:** The PWM frequency is set by PGU (80 MHz clock, 14-bit counter). The GDU must be configured for gate strength compatible with the selected MOSFET at this switching frequency.
- **Phase voltage measurement (VBRG):** The VBRG terminal senses the drain-side of the high-side FETs. Used internally by GDU for VDS measurement. Ensure VBRG is connected to the positive bus rail (after the reverse-battery diode) with a 10 kΩ filter resistor as recommended.
- **ASIL-B diagnostic requirement:** The firmware must read GDU diagnostic registers at least once per 10 ms and check for latched faults. Do not wait for the MCLRn interrupt alone — some fault conditions may require active polling.
