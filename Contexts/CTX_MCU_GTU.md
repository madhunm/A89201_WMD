# CTX_MCU_GTU — A89201 General Purpose Timer Unit (GTU)

## Source
- Document: GTU.pdf (A89201 Automotive SoC Motor Driver, Preliminary User Manual, Allegro Microsystems)

---

## 1. Overview

The GTU hosts eight 16-bit independent timer-counters. Each timer is fully configurable and operates independently of the others. Timers can measure external signal characteristics (high time, low time, period, event count) or act as software timers (free-running on internal clock).

---

## 2. Features

- 8 independent 16-bit timer-counters (TIMER0–TIMER7)
- Each timer: independently configurable mode, clock source, interrupt trigger
- Configurable interrupt triggers per timer
- 10 operating modes
- Input signals: any GPIO (PD0–PD7) or internal peripheral signals
- AMCT generates a BEMF tacho signal routed to GTU for windmilling speed measurement

---

## 3. Operating Modes (10 total)

| Mode | Name                    | Description                                             |
|------|-------------------------|---------------------------------------------------------|
| 1    | High-Time Measurement   | Measures duration of HIGH state on input signal        |
| 2    | Low-Time Measurement    | Measures duration of LOW state on input signal         |
| 3    | Period Measurement      | Measures full period (rising edge to rising edge)       |
| 4    | Event Count             | Counts pulses on input signal                           |
| 5    | Free-Run (timer)        | Counts clock ticks, interrupt on overflow or compare    |
| 6–10 | Additional modes        | Combinations of capture, compare, PWM input decode     |

Timers configured for external signal input use GPIO pin as source (selected per timer via register setting).

---

## 4. Clock Source

Each timer has an independently scalable clock derived from the system clock. Clock divider is programmable per timer. With 40 MHz system clock:
- Divider = 1: 25 ns resolution, max period = 65535 × 25 ns = 1.638 ms
- Divider = 40: 1 µs resolution, max period = 65535 µs ≈ 65.5 ms
- Divider = 400: 10 µs resolution, max period = 655.35 ms

---

## 5. Project Timer Assignments

| Timer  | Mode                | Input            | Purpose                                              |
|--------|---------------------|------------------|------------------------------------------------------|
| TIMER0 | Period Measurement  | PD0 (Hall A)     | Measure Hall A signal period → motor speed estimate  |
| TIMER1 | Period Measurement  | PD1 (Hall B)     | Measure Hall B signal period → motor speed estimate  |
| TIMER2 | Period Measurement  | PD2 (Hall C)     | Measure Hall C signal period → motor speed estimate  |
| TIMER3 | Period Measurement  | BEMF tacho (AMCT)| Motor speed via BEMF zero-crossing (sensorless backup)|
| TIMER4 | Free-Run (1 ms tick)| Internal clock   | System time base for speed ramp, timeout, debounce   |
| TIMER5 | High-Time Measurement| PD3/PD4         | Speed command debounce (optional, can use TIMER4 + SW)|
| TIMER6 | Free-Run            | Internal clock   | A1333 SPI polling interval timer (10 ms period)      |
| TIMER7 | Reserved            | —                | Available for future use                             |

### Speed Calculation from Hall Period (TIMER0–TIMER2)

Hall period measurement gives the time between two consecutive rising edges of one Hall sensor signal.

```
For a 3-phase BLDC with N pole-pairs:
  - 6 Hall transitions per electrical cycle
  - Period of one Hall signal (rising-to-rising) = T_hall

  Electrical frequency (Hz) = 1 / T_hall
  Mechanical RPM = (60 / N_pole_pairs) / T_hall

If the motor has 2 pole pairs (4-pole motor, typical for wiper BLDC):
  RPM = 30 / (N_pole_pairs × T_hall)
      = 15 / T_hall  [with T_hall in seconds]

At 40 RPM output (after 39:1 gearbox):
  Motor RPM = 40 × 39 = 1560 RPM
  T_hall = 15 / 1560 = 9.615 ms
  GTU counts at 1 µs resolution: 9615 counts

At 60 RPM output:
  Motor RPM = 60 × 39 = 2340 RPM
  T_hall = 15 / 2340 = 6.41 ms
  GTU counts: 6410 counts
```

**These are starting estimates. Actual pole-pair count must be confirmed from motor spec sheet.**

---

## 6. System Time Base (TIMER4)

Configure TIMER4 as 1 ms free-running timer for all software timing needs:

```c
// Timer4: free-run, 1 ms interrupt
// At 40 MHz, divider = 40 → 1 µs tick
// Period register = 1000 → interrupt every 1 ms
TIMER4->clock_div = 40;
TIMER4->period    = 1000;
TIMER4->mode      = FREE_RUN_COMPARE;
TIMER4->int_en    = 1;
TIMER4->enable    = 1;
```

This 1 ms tick drives:
- Speed command debounce counter (5 ms hold)
- Watchdog kick interval (every 5 ms, watchdog period = 10 ms)
- A1333 SPI poll rate (every 5 ms = 200 Hz angle read)
- Speed ramp rate controller (RPM change per ms)
- Fault timeout counters (stall detection, loss-of-signal timeout)

---

## 7. BEMF Tacho (TIMER3)

The AMCT module generates a `bemf_comp_tacho` signal to GTU TIMER3. This signal toggles at each BEMF zero-crossing event and allows motor speed measurement during sensorless / windmilling conditions.

Configure TIMER3 as period measurement on the BEMF tacho signal. Compare measured period against expected period for target speed to detect speed error.

---

## 8. Interrupt Handling

Each timer has a configurable interrupt trigger:
- Timer overflow (counter rolls over)
- Compare match (counter reaches programmed compare value)
- Capture event (external signal edge detected)

Timer interrupts are lower priority than CAU and AMCT interrupts. TIMER4 (1 ms tick) runs at medium priority.

---

## 9. Project-Specific Notes

- **Hall period measurement accuracy:** At 40 RPM output (motor 1560 RPM), T_hall ≈ 9.6 ms. With 1 µs GTU resolution and 16-bit counter (max 65.535 ms), this is fine. At very low RPM (below ~1 RPM motor, during ramp-up), the period exceeds 65 ms and the counter overflows. Handle overflow by accumulating: count the number of overflows plus the final capture value.
- **Three Hall timers (TIMER0–TIMER2) vs. one:** Using all three and averaging gives a better speed estimate than one alone because it detects Hall sensor dropout (if one Hall fails, its period goes to infinity, identifiable by counter overflow). This is an ASIL-B relevant diagnostic.
- **Speed estimate for control loop:** The period measurement from GTU gives instantaneous speed. Use a simple moving average over the last 6 Hall events (one full electrical cycle) to smooth the speed estimate before feeding it to the PI speed controller.
- **TIMER6 for SPI polling:** At 10 ms period, the A1333 is polled at 100 Hz. This is more than adequate for 40–60 RPM positioning control. Do not poll faster than 5 ms (200 Hz) — this saturates the SPI bus and consumes unnecessary CPU cycles.
- **All GTU counters are 16-bit.** At 40 MHz with no divider, overflow occurs every 1.638 ms. Always set an appropriate clock divider before enabling the timer for measurement.
