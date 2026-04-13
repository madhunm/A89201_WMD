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

| Timer  | Mode                    | Input             | Purpose                                               |
|--------|-------------------------|-------------------|-------------------------------------------------------|
| TIMER0 | Period Measurement      | PD0 (Hall A)      | Hall A signal period → motor speed estimate           |
| TIMER1 | Period Measurement      | PD1 (Hall B)      | Hall B signal period → motor speed estimate           |
| TIMER2 | Period Measurement      | PD2 (Hall C)      | Hall C signal period → motor speed estimate           |
| TIMER3 | Period Measurement      | BEMF tacho (AMCT) | Motor speed via BEMF zero-crossing (sensorless backup)|
| TIMER4 | Free-Run (1 ms tick)    | Internal clock    | System time base: ramp, timeouts, debounce            |
| TIMER5 | High-Time Measurement   | PD7 (A1333 PWM)   | A1333 PWM pulse high time (duty cycle numerator)      |
| TIMER6 | Period Measurement      | PD7 (A1333 PWM)   | A1333 PWM carrier period (duty cycle denominator)     |
| TIMER7 | Free-Run (10 ms tick)   | Internal clock    | Secondary watchdog kick / background task scheduler   |

### A1333 PWM Absolute Angle (TIMER5 + TIMER6 on PD7)

The A1333 PWM output (pin 4) encodes absolute shaft angle as duty cycle: 5% = 0°, 95% = 360°, linear. Carrier frequency is configured at EOL to **1 kHz** (1 ms period).

TIMER5 (high-time) and TIMER6 (period) share the same PD7 input. TIMER5 ISR fires after each high pulse capture; it reads both captures to compute duty cycle:

```c
// TIMER5: high-time on PD7, 1 µs resolution (div=40)
// TIMER6: period  on PD7, 1 µs resolution (div=40), no separate interrupt
void GTU_Timer5_ISR(void) {
    uint16_t t_high   = TIMER5->capture;
    uint16_t t_period = TIMER6->capture;
    if (t_period == 0) return;

    float duty      = (float)t_high / (float)t_period;
    duty            = CLAMP(duty, 0.05f, 0.95f);
    float angle_deg = (duty - 0.05f) / 0.90f * 360.0f;

    pwm_absolute_angle_deg = angle_deg;
    SetFlag(FLAG_PWM_ANGLE_VALID);
}
```

**PWM angle resolution:** At 1 kHz carrier, 1 µs GTU resolution → period = 1000 counts. LSB = 0.1% duty = 0.4°. Coarser than A1333 12-bit SPI (0.088°/count) but entirely adequate as an ABI drift anchor. ABI provides fine resolution (0.044°/step); PWM provides the absolute reference every 1 ms.

**ABI + PWM fusion (runs in background at 100 Hz):**

```c
void ABI_PWM_Fusion(void) {
    if (!flag_pwm_angle_valid) return;

    float abi_angle = ABI_ToAngleDeg();
    float pwm_angle = pwm_absolute_angle_deg;

    float diff = pwm_angle - abi_angle;
    if (diff >  180.0f) diff -= 360.0f;
    if (diff < -180.0f) diff += 360.0f;

    /* Only correct if drift exceeds 2 ABI steps (0.088 deg) */
    if (fabsf(diff) > 0.088f) {
        int32_t correction = CLAMP((int32_t)(diff / ABI_DEG_PER_COUNT), -10, 10);
        abi_count += correction;
        if (fabsf(diff) > 2.0f) SetFlag(FLAG_ABI_LARGE_DRIFT);
    }
}
```

**Startup:** At power-on, PWM gives a valid absolute angle within the first 1 ms. No homing routine is required. Firmware reads `pwm_absolute_angle_deg`, seeds `abi_count = (int32_t)(pwm_absolute_angle_deg / ABI_DEG_PER_COUNT)`, then enables ABI interrupts. The wiper is immediately position-aware without any shaft movement.

### Speed Calculation from Hall Period (TIMER0–TIMER2)

- **Hall period measurement accuracy:** At 40 RPM output (motor 1560 RPM), T_hall ≈ 9.6 ms. With 1 µs GTU resolution and 16-bit counter (max 65.535 ms), this is fine. At very low RPM (below ~1 RPM motor, during ramp-up), the period exceeds 65 ms and the counter overflows. Handle overflow by accumulating: count the number of overflows plus the final capture value.
- **Three Hall timers (TIMER0–TIMER2) vs. one:** Using all three and averaging gives a better speed estimate than one alone because it detects Hall sensor dropout (if one Hall fails, its period goes to infinity, identifiable by counter overflow). This is an ASIL-B relevant diagnostic.
- **Speed estimate for control loop:** The period measurement from GTU gives instantaneous speed. Use a simple moving average over the last 6 Hall events (one full electrical cycle) to smooth the speed estimate before feeding it to the PI speed controller.
- **TIMER5/TIMER6 for A1333 PWM:** Both timers use PD7 as input. TIMER5 captures high-time and TIMER6 captures period. Do not enable separate interrupts on TIMER6 — the TIMER5 ISR reads both captures in one shot. Configure both with the same clock divider (div=40, 1 µs resolution) so their capture units are directly comparable.
- **All GTU counters are 16-bit.** At 40 MHz with no divider, overflow occurs every 1.638 ms. Always set an appropriate clock divider before enabling the timer for measurement.
