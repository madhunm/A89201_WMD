# CTX_MCU_LIN — A89201 LIN Module

## Source
- Document: LIN.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The A89201 integrates a full LIN controller (physical layer + data-link layer). In this project, the LIN interface is **not used for primary speed command reception** — speed commands arrive via GPIO (PD3/PD4). LIN is retained as an available interface for diagnostics, calibration commands, or fleet management integration if required.

---

## 2. Features

- Compliant: ISO 17987-3:2016, LIN 1.3, LIN 2.2
- Rate: 1 kHz to 20 kHz (typically 10.417 kbps for automotive LIN)
- Full frame processing at data-link layer
- Peripheral wake request support
- Automatic bus idle timeout detection
- Comprehensive error detection (framing, checksum, parity)
- Indicators: frame header received, TX/RX complete, wakeup detect
- Classic (LIN 1.3) and enhanced (LIN 2.0) checksum support
- Independent RX and TX paths
- Up to ±14% clock deviation tolerance

---

## 3. LIN Physical Interface

- LIN pin: rated −40 V to 50 V (automotive bus tolerance)
- LIN transceiver integrated (LIN PHY on-chip)
- LGND: LIN ground reference
- LRX: internal signal to CPU (logic-level RX)
- LTX: internal signal from CPU (logic-level TX)

In dominant state (bus pulled low), LRX is low. In recessive state, LRX is high. In sleep mode, LRX is not active.

---

## 4. LIN as PWM Input (Alternative Use)

The LIN pin can optionally be configured as a PWM input by routing LRX to a GTU timer input. This allows:
- Duty cycle measurement of a controlling PWM signal on the LIN wire
- Interpretation by CPU and appropriate action

In this project, this function is **not used** (speed commands are GPIO-based). This note is retained for completeness.

---

## 5. LIN Wake

From sleep/minimum-power state, the A89201 can be woken by a LIN break on the bus. Wake detection does not require the main oscillator to be active.

---

## 6. Register Set

- LIN register map: 18 × 16-bit registers
- Starting address: per SCU LIN mux control; LIN pins selected via `sel_lin` register in SCU

Key LIN registers (refer to LIN.pdf for full definitions):
- Control register: enable/disable LIN, mode select (master/slave)
- Status register: break detect, sync detect, PID received, TX/RX complete, error flags
- Data registers: frame data bytes (up to 8 bytes payload)
- Baud rate register: derived from system clock

---

## 7. LIN Frame Structure (Reference)

LIN frame = Break + Sync byte (0x55) + Protected ID + Data (1–8 bytes) + Checksum

Checksum types:
- Classic (LIN 1.3): data bytes only
- Enhanced (LIN 2.0): ID byte + data bytes

---

## 8. Project-Specific Notes

- **LIN is passive in the current design.** The A89201 operates as a LIN peripheral (responder), not as master. Speed commands come from GPIO, not LIN frames.
- **If LIN commands are added in future**, the firmware must arbitrate between GPIO speed commands and LIN speed commands with a defined priority. Recommend: GPIO commands take precedence for safety (hardware-level override possible without firmware); LIN commands are advisory only.
- **The LIN pin must be connected to the vehicle LIN bus** (even if not used for speed commands) if the vehicle bus master polls the wiper module. Leaving LIN open may generate bus errors at the BCM level.
- **LIN sleep current:** A89201 sleep mode draws ≤15 µA from VBB when LIN is at 12 V. If the system design requires standby below this, verify LIN physical layer configuration.
- **Mux control:** The `sel_lin` register in SCU must be configured to assign the LIN TX line to the correct GPIO pin if LIN TX is used. The LIN RX is hardwired to the LIN PHY.
- **For ASIL-B**, if LIN is used to transmit safety-relevant data (e.g., position feedback to BCM), the firmware must implement LIN checksum verification and frame timeout detection. Do not use LIN for safety-critical control inputs without these safeguards.
