# CTX_PROJECT_ARCHITECTURE — BLDC Wiper Controller Firmware Architecture

## Project
- Target: ASIL-B compliant, closed-loop three-phase BLDC motor controller
- Application: Automotive windscreen wiper (single motor, one axis)
- Toolchain: Keil uVision (ARM Compiler 6)
- Hardware: Allegro A89201C SoC (ARM Cortex-M4F @ 40 MHz)

---

## 1. System Block Diagram

```
Vehicle BCM
   │  │
   │  └── High-Speed CMD ──► PD4 (GPIO input, pull-down)
   └───── Low-Speed CMD  ──► PD3 (GPIO input, pull-down)

                         ┌──────────────── A89201C ────────────────────────┐
PD3/PD4 ─────────────►  │ GPIO                                             │
                         │  └─► Speed Command Decoder                      │
APS12202 ×3 ──────────►  │ PD0/PD1/PD2 → AMCT (Hall subsystem)            │
  (rotor position)       │      └─► Commutation Sector, Speed Estimate     │
                         │                                                  │
A1333 ─────────────────►  │ PD5/PD6 (ABI A/B) → GPIO interrupts → quadrature count  │
  (output shaft angle)   │ PD7 (PWM out) → GTU TIMER5/6 duty-cycle → absolute angle │
                         │      └─► ABI: 0.044°/step + PWM absolute anchor (0.4°)  │
                         │                                                  │
                         │ CAU ← PGU triggers (3× per PWM cycle)           │
CSP/CSM (2 mΩ shunt) ──► │     └─► Phase current samples                  │
                         │                                                  │
                         │ PGU → 6× PWM signals (HA/LA/HB/LB/HC/LC)       │
                         │ GDU → Gate drive → External MOSFETs             │
                         └────────────────────────────────────────────────-┘
                                         │
                               3-Phase MOSFET Bridge
                                         │
                                    BLDC Motor
                                         │
                               39:1 Worm Gearbox
                                         │
                              A1333 (shaft angle measurement)
                                         │
                                   Wiper Arm
```

---

## 2. Control Architecture

### 2.1 Overall Structure

The firmware implements a **three-loop cascade controller**:

```
Speed Command (PD3/PD4)
        │
        ▼
[1] Position/Angle Controller   (outer loop, 1 kHz)
        │   Set-point: target angle
        │   Process var: A1333 shaft angle
        │   Output: speed set-point (RPM)
        ▼
[2] Speed Controller            (middle loop, 1 kHz)
        │   Set-point: speed set-point from outer loop
        │   Process var: speed from AMCT Hall timer
        │   Output: Vq (torque-generating voltage demand)
        ▼
[3] Current Controller          (inner loop, 20 kHz)
            Set-point: Iq demand from speed controller
            Process var: phase current from CAU
            Output: PWM duty cycle (via PGU)
```

All three loops are PI controllers. The position controller additionally has a feed-forward path to reduce steady-state angle error.

### 2.2 Speed Targets

| Command State       | PD3 | PD4 | Speed Target | Tolerance |
|---------------------|-----|-----|--------------|-----------|
| Park / Stop         | 0   | 0   | 0 RPM        | Park position |
| Low speed           | 1   | 0   | 40 RPM       | ±2 RPM    |
| High speed          | 1   | 0   | 60 RPM       | ±2 RPM    |
| Invalid (fault)     | 1   | 1   | SAFE STATE   | —         |

Wait — corrected from design specification:
| Command State       | PD3 | PD4 | Speed Target | Tolerance |
|---------------------|-----|-----|--------------|-----------|
| Park / Stop         | 0   | 0   | 0 RPM        | Park position |
| Low speed           | 1   | 0   | 40 RPM       | ±2 RPM    |
| High speed          | 0   | 1   | 60 RPM       | ±2 RPM    |
| Invalid (fault)     | 1   | 1   | SAFE STATE   | —         |

### 2.3 Angle Target

- Wiper operates within a defined mechanical arc (e.g., 0° park to 120° maximum sweep)
- At any speed command, the controller regulates speed while maintaining awareness of the end-stop positions
- At park command (PD3=PD4=0): position controller drives wiper to park angle (stored in flash as `PARK_ANGLE_DEG`)
- Position accuracy at park and reversal points: **±1°** (confirmed requirement; end-of-line calibration + optional temperature compensation implemented in firmware)

### 2.4 Current Limits

| Limit         | Value | Enforcement mechanism                    |
|---------------|-------|------------------------------------------|
| Continuous    | 6 A   | Current controller Iq clamp             |
| Stall (peak)  | 30 A  | Immediate: ENABLE=0, fault latch        |
| Hardware OC   | GDU VDS| GDU autonomous response + MCU IRQ      |

---

## 3. Interrupt Priority Table

| Priority | Source              | Handler purpose                          | Max duration |
|----------|---------------------|------------------------------------------|--------------|
| 0 (highest) | GDU Fault        | Disable ENABLE pin, set fault flag       | < 1 µs       |
| 1        | CAU conversion done | Read current samples, run current PI    | < 5 µs       |
| 2        | AMCT Hall event     | Update sector, speed estimate           | < 2 µs       |
| 3        | PGU period event    | Update PWM duty, run speed/pos PI       | < 10 µs      |
| 4        | SCU fault           | Log fault, enter safe state             | < 5 µs       |
| 5        | GPIO (PD3/PD4)      | Decode speed command, debounce          | < 2 µs       |
| 6        | GTU overflow        | Watchdog tick, background timers        | < 2 µs       |
| 7        | GTU7 (PWM period)   | Capture PWM high-time + period, update absolute angle  | < 2 µs       |
| 8 (lowest)| LIN / DAU / DMA    | Background diagnostics                  | < 10 µs      |

Total worst-case ISR execution budget within one 50 µs PWM period:
- CAU ISR: 5 µs
- PGU ISR: 10 µs
- AMCT ISR: 2 µs
- Margin: 33 µs for background tasks ✓

---

## 4. Task Structure

### 4.1 ISR Context (real-time, interrupt-driven)

```c
/* 20 kHz — triggered by PGU centre-aligned PWM period event */
void PGU_Period_ISR(void) {
    /* 1. Read current samples from SRAM (placed there by DMA1 from CAU) */
    /* 2. Run current PI controller → update Iq duty cycle demand */
    /* 3. Every 10th call (2 kHz): run speed PI → update speed set-point */
    /* 4. Every 20th call (1 kHz): run position PI → update angle demand */
    /* 5. Write new duty cycle to PGU compare registers */
    /* 6. Kick watchdog */
}

/* Triggered by AMCT Hall transition */
void AMCT_Hall_ISR(void) {
    /* 1. Update commutation sector */
    /* 2. Apply new commutation vector to PGU (enable correct phase pair) */
    /* 3. Compute instantaneous speed from AMCT timer */
}

/* Triggered by GDU fault line */
void GDU_Fault_ISR(void) {
    /* 1. Immediately clear ENABLE pin (hardware safe state) */
    /* 2. Set global fault flag */
    /* 3. Log fault type from GDU register */
}
```

### 4.2 Background Loop (main(), runs in WFI loops)

```c
void main(void) {
    System_Init();          /* SCU, clocks, WDT */
    Peripheral_Init();      /* GPIO, SCI0, PGU, CAU, AMCT, GDU, DMA */
    Sensor_Init();          /* A1333 startup wait (≥30 ms), ABI resolution verify, Hall latch check */
    CalibrationRun();       /* CAU offset calibration, A1333 zero angle */
    EnableInterrupts();
    MotorController_Init(); /* Load PID gains from flash */
    BridgeEnable();         /* Set ENABLE pin HIGH */

    while (1) {
        __WFI();                         /* Sleep until next interrupt */

        /* Background tasks (all non-time-critical): */
        if (bg_tick_10Hz) {
            DAU_ReadTemperature();        /* Die temp monitor */
            DAU_ReadSupplyVoltages();     /* VD50 / VD33 sanity check */
            ABI_ValidatePlausibility();   /* Rate check, index periodicity, Hall speed vs ABI speed cross-check */
            FaultManager_Process();       /* Escalate latched faults */
        }
        if (bg_tick_1Hz) {
            SpeedPID_AdaptGains();        /* Gain scheduling if implemented */
            Flash_WriteRuntime();         /* Log odometer, fault counts */
            WatchdogKick();               /* Belt-and-suspenders WDT kick */
        }
    }
}
```

---

## 5. Peripheral Initialisation Sequence

```
1. SCU init
   ├── Set mcuclk: CPU = 40 MHz
   ├── Configure WDT period = 10 ms
   ├── Set GPIO mux: sel_uart0 → assign UART RX pin for debug UART (development only)
   ├── sel_spi0 → leave at reset value (SPI0 not used at runtime)
   ├── Clear intstat/diag (POR flags)
   └── Enable SCU interrupts (VD50 UV, WDT, GDU fault, ECC error)

2. GPIO init
   ├── PD0–PD2: input, pull-up enabled (Hall latches)
   ├── PD3–PD4: input, pull-down enabled (speed commands)
   ├── PD5: input, no pull (A1333 ABI A — push-pull from A1333)
   ├── PD6: input, no pull (A1333 ABI B — push-pull from A1333)
   └── PD7: input, no pull (A1333 PWM output — routed to GTU timer for duty-cycle measurement)

3. SCI0 init (UART debug only — disabled in production build; clock-gated via SCU)
   SCI1: not configured

4. DMA init
   ├── DMA1: CAU result registers → SRAM current_samples[3]
   └── DMA2: Reserved (SCI0 UART TX buffer if debug logging enabled)

5. A1333 init
   ├── Wait ≥30 ms (A1333 power-on self-test period — tPO)
   ├── At EOL only (external programmer via SPI test pads, not at runtime):
   │     Verify LBIST passed (ERR[3]=0)
   │     Configure RESOLUTION_PAIRS = 3 (2048 PPR ABI)
   │     Configure PWM carrier frequency = 1 kHz
   │     Write ZERO_OFFSET calibration value
   │     Lock EEPROM after calibration
   ├── At runtime (firmware):
   │     Configure PD5/PD6 as GPIO inputs, no pull (ABI A/B)
   │     Configure PD7 as GTU TIMER5/TIMER6 input (PWM) — not GPIO interrupt
   │     Enable GPIO interrupts on PD5/PD6 (both edges each)
   │     Wait for TIMER5 first capture (≤1 ms) → seed abi_count from pwm_absolute_angle_deg
   └── No homing routine required — absolute angle available immediately from PWM

6. CAU init
   ├── Set gain: DAG=10, SAG=20
   ├── Set trigger: ADC_A ← ADCANT, ADC_B ← ADCBNT, ADC_C ← ADCCNT
   ├── Set sample time: 1 µs
   ├── Enable auto-calibration with IIR filter
   └── Run initial offset calibration (requires bridge disabled)

7. AMCT init
   ├── Assign Hall inputs: PD0→Hall_A, PD1→Hall_B, PD2→Hall_C
   ├── Set debounce period: 10 µs
   ├── Configure hall timer for speed measurement
   └── Enable Hall event interrupts

8. PGU init
   ├── Clock: 80 MHz, centre-aligned mode
   ├── Period register: 80,000,000 / 20,000 / 2 = 2000 counts (20 kHz)
   ├── Dead time: set per GDU requirements (verify with MOSFET gate charge)
   ├── All duty cycles: 0 (outputs off)
   └── Enable PGU period interrupt

9. GDU init
   ├── Configure drive strength (slew rate)
   ├── Enable VDS monitoring per phase
   ├── Enable Q&A watchdog
   └── ENABLE pin: LOW (bridge disabled until after calibration)

10. Enable NVIC priorities (as per Section 3 table above)

11. Set ENABLE pin HIGH → bridge active
```

---

## 6. Motor Control Algorithm Detail

### 6.1 Commutation (Six-Step Trapezoidal)

The BLDC motor uses six-step commutation based on Hall sector from AMCT:

```c
/* Commutation table: [sector] → {active_phase_pos, active_phase_neg} */
/* Applied to PGU: positive phase gets (duty/2 + 50%) high-side PWM */
/* Negative phase gets complementary low-side */
const uint8_t COMMUTATION_TABLE[8][2] = {
    {0, 0},  /* Sector 0: invalid */
    {A, B},  /* Sector 1: A+, B- */
    {A, C},  /* Sector 2: A+, C- */
    {B, C},  /* Sector 3: B+, C- */
    {B, A},  /* Sector 4: B+, A- */
    {C, A},  /* Sector 5: C+, A- */
    {C, B},  /* Sector 6: C+, B- */
    {0, 0},  /* Sector 7: invalid */
};
```

### 6.2 Speed Measurement

Motor speed (output shaft RPM) computed from Hall timer in AMCT:

```c
/* AMCT provides time between Hall transitions (in timer counts) */
/* Motor RPM (output shaft) = 60 / (6 × P × T_hall_seconds × gear_ratio) */
/* Where P = pole-pairs, gear_ratio = 39 */
/* For P=4: RPM_output = 60 / (6 × 4 × T_hall_sec × 39) */
float ComputeOutputRPM(uint32_t hall_timer_counts, uint32_t timer_freq_hz) {
    float T_hall_sec = (float)hall_timer_counts / (float)timer_freq_hz;
    return 60.0f / (6.0f * POLE_PAIRS * T_hall_sec * GEAR_RATIO);
}
```

### 6.3 Angle Control (Position Loop)

```c
/* Position controller: PI with anti-windup */
/* Runs at 1 kHz. Input: shaft_angle_deg (from A1333) */
/* Output: speed_setpoint_rpm (clamped to 0–MAX_SPEED) */

float PositionController(float target_angle_deg, float current_angle_deg) {
    float error = target_angle_deg - current_angle_deg;
    /* Wrap error to ±180° for continuous rotation handling */
    if (error > 180.0f)  error -= 360.0f;
    if (error < -180.0f) error += 360.0f;

    pos_integrator += KI_POS * error * DT_POS;
    pos_integrator = CLAMP(pos_integrator, -MAX_SPEED, MAX_SPEED);

    float output = KP_POS * error + pos_integrator;
    return CLAMP(output, -MAX_SPEED, MAX_SPEED);
}
```

### 6.4 Current Limiting

Current controller sets a maximum Iq demand based on measured current:

```c
/* Hard limit: disable bridge immediately at 30 A */
if (fabsf(I_phase_amps) >= STALL_CURRENT_A) {
    GPIO_ENABLE_LOW();       /* Hardware fast path */
    SetFault(FAULT_OVERCURRENT_STALL);
    return;
}

/* Soft limit: clamp Iq demand at 6 A equivalent */
Iq_demand = CLAMP(Iq_demand, -CONTINUOUS_CURRENT_A, CONTINUOUS_CURRENT_A);
```

---

## 7. Flash Memory Layout

| Region              | Address               | Size   | Content                         |
|---------------------|----------------------|--------|---------------------------------|
| Vector table        | 0x0800_0000          | 256 B  | ARM interrupt vectors           |
| Application code    | 0x0800_0100          | ~100 kB| Main firmware                   |
| ISR code (IRAM copy)| 0x0001_0000          | 8 kB   | Time-critical ISR handlers      |
| Calibration data    | 0x0803_C000          | 4 kB   | Page 60: angle cal, PID gains   |
| Fault log           | 0x0803_D000          | 4 kB   | Page 61: fault history          |
| Reserved (protected)| 0x0803_8000 (page 54)| 4 kB   | Allegro reserved                |
| Reserved (protected)| 0x0803_F000 (page 63)| 4 kB   | Allegro reserved                |

**Calibration data page** stores:
- `PARK_ANGLE_DEG` (float, 4 bytes): A1333 angle reading at wiper park position
- `MAX_ANGLE_DEG` (float, 4 bytes): A1333 angle at maximum sweep position
- `KP_POS`, `KI_POS`, `KP_SPD`, `KI_SPD`, `KP_CUR`, `KI_CUR`: PID gains
- CRC32 of all calibration data (for integrity check at startup)

---

## 8. Safe State Definition

The system enters safe state when any of the following occur:

| Trigger                            | Response                              |
|------------------------------------|---------------------------------------|
| GDU fault (overcurrent, VDS)       | ENABLE = 0, latch fault, await reset  |
| CAU stall current (≥30 A)          | ENABLE = 0, latch fault               |
| ABI plausibility fail (rate, index miss) | Stop position control, freeze angle, log fault |
| Hall state = 0b000 or 0b111        | ENABLE = 0, latch fault               |
| SCU WDT timeout                    | Hardware reset                        |
| SCU ECC multi-bit error            | ENABLE = 0 via fault handler, reset   |
| PD3 and PD4 both HIGH              | ENABLE = 0, latch fault               |
| VD50 undervoltage                  | ENABLE = 0, fault                     |
| Die temperature ≥ 125°C            | ENABLE = 0, wait for cool-down        |

Safe state: all PWM outputs off (ENABLE pin LOW), wiper holds position, fault code available.

Recovery from safe state requires a power cycle or explicit external reset (unless fault is thermal, in which case auto-recovery after cool-down is permitted).

---

## 9. ASIL-B Compliance Evidence Points

| Requirement                          | Implementation                              |
|--------------------------------------|---------------------------------------------|
| Hardware watchdog                    | SCU WDT, 10 ms timeout, kicked in main loop |
| Memory integrity                     | SRAM/IRAM/Flash ECC via SCU                |
| Sensor fault detection               | ABI rate/index plausibility, Hall 000/111 check, Hall vs ABI speed cross-check |
| Actuator fault detection             | GDU VDS monitoring, CAU stall current       |
| Safe state on fault                  | ENABLE pin LOW, PWM disabled                |
| Single-point fault prevention        | Two independent speed sensors (Hall for speed, A1333 for position) |
| Stack overflow protection            | MPU configured for stack region             |
| Invalid input detection              | PD3=PD4=1 treated as fault                 |
| Calibration data integrity           | CRC32 check at every startup               |
| Software execution monitoring        | Watchdog + execution flow checkpoints       |

**Remaining ASIL-B gap:** The A1333 Safety Manual must be obtained from Allegro and followed. The A89201 does not have a published functional safety manual — contact Allegro for ASIL-B application support.

---

## 10. Key Configuration Constants

```c
/* Motor and mechanical parameters */
#define POLE_PAIRS          4       /* Motor pole pairs — verify against motor spec */
#define GEAR_RATIO          39      /* Worm gearbox ratio */
#define PWM_FREQ_HZ         20000   /* 20 kHz PWM */
#define CPU_FREQ_HZ         40000000

/* Speed targets (output shaft RPM) */
#define SPEED_LOW_RPM       40.0f
#define SPEED_HIGH_RPM      60.0f
#define SPEED_TOLERANCE_RPM 2.0f

/* Angle targets */
#define ANGLE_TOLERANCE_DEG 0.5f
#define PARK_ANGLE_DEG      (read from flash at startup)
#define MAX_SWEEP_ANGLE_DEG (read from flash at startup)

/* Current limits */
#define CONTINUOUS_CURRENT_A  6.0f
#define STALL_CURRENT_A       30.0f
#define OVERCURRENT_WARN_A    25.0f

/* Shunt and amplifier */
#define SHUNT_RESISTANCE_OHM  0.002f   /* 2 mΩ */
#define CSA_GAIN              10        /* DAG = 10 */
#define ADC_FULL_SCALE_V      1.2f      /* ±1.2 V differential */

/* Timing */
#define DT_CURRENT_S          (1.0f / PWM_FREQ_HZ)          /* 50 µs */
#define DT_SPEED_S            (DT_CURRENT_S * 10)            /* 500 µs = 2 kHz */
#define DT_POSITION_S         (DT_CURRENT_S * 20)            /* 1 ms = 1 kHz */

/* A1333 ABI + PWM */
#define ABI_RESOLUTION_PPR    2048         /* RESOLUTION_PAIRS=3 in EEPROM */
#define ABI_CPR               (ABI_RESOLUTION_PPR * 4)  /* 8192 counts/rev */
#define ABI_DEG_PER_COUNT     (360.0f / ABI_CPR)        /* 0.04394531 deg */
#define A1333_PWM_CARRIER_HZ  1000         /* 1 kHz configured at EOL */
#define A1333_PWM_DUTY_MIN    0.05f        /* 5% = 0 deg */
#define A1333_PWM_DUTY_MAX    0.95f        /* 95% = 360 deg */
#define ABI_FUSION_THRESHOLD_DEG  0.088f   /* 2 ABI steps before correction */
#define ABI_LARGE_DRIFT_DEG   2.0f         /* Drift worth logging */

/* Watchdog */
#define WDT_PERIOD_MS         10
```

---

## 11. File and Module Structure (Keil uVision Project)

```
WiperController/
├── main.c                  — System init, background loop
├── startup_A89201.s        — Vector table, stack, IRAM copy
├── A89201_Registers.h      — All peripheral register definitions (manual, no CMSIS pack)
│
├── hal/
│   ├── hal_gpio.c / .h     — GPIO init, read, write wrappers
│   ├── hal_sci.c / .h      — SCI0 SPI init and transfer
│   ├── hal_pgu.c / .h      — PWM period, duty cycle, dead time
│   ├── hal_cau.c / .h      — ADC trigger, result read, calibration
│   ├── hal_amct.c / .h     — Hall input config, speed timer read
│   ├── hal_gdu.c / .h      — Gate driver enable, fault read
│   ├── hal_scu.c / .h      — Clock, WDT kick, fault flags
│   ├── hal_dma.c / .h      — DMA channel init
│   └── hal_fli.c / .h      — Flash read/write for calibration data
│
├── drivers/
│   ├── a1333.c / .h        — A1333 ABI quadrature decode, PWM duty-cycle angle, plausibility checks
│   └── aps12202.c / .h     — Hall latch read (thin wrapper over GPIO)
│
├── control/
│   ├── current_ctrl.c / .h — Inner PI current controller
│   ├── speed_ctrl.c / .h   — Middle PI speed controller
│   ├── position_ctrl.c / .h— Outer PI position controller
│   ├── commutation.c / .h  — Six-step commutation table, sector logic
│   └── pid.c / .h          — Generic PI implementation with anti-windup
│
├── safety/
│   ├── fault_manager.c / .h— Fault code definitions, logging, escalation
│   ├── safe_state.c / .h   — EnterSafeState(), RecoverFromFault()
│   └── diagnostics.c / .h  — Periodic self-tests, ECC checks, sensor validation
│
└── config/
    ├── motor_config.h      — Pole pairs, gear ratio, current limits, speed targets
    ├── pid_gains.h         — Default PID gains (loaded from flash at runtime)
    └── pinmap.h            — GPIO pin assignments, peripheral base addresses
```

---

## 12. Open Questions / Assumptions Made

The following assumptions were made in the absence of explicit design decisions. These must be confirmed:

| Item | Assumption | Must confirm |
|------|-----------|--------------|
| Motor pole pairs | 4 pole-pairs | Verify against actual motor datasheet |
| Wiper arc | 0–120° at output shaft | Verify mechanical design |
| A1333 package | LE (14-pin, single die) | **Confirmed** |
| A1333 ABI resolution | RESOLUTION_PAIRS=3 (2048 PPR) | Verify EOL programmer configures this before production |
| A1333 angle accuracy | ±1° over temperature | **Confirmed acceptable** |
| A1333 calibration | End-of-line offset cal + optional temp comp | Design decision confirmed |
| MOSFET selection | Not specified | Required for GDU dead-time and drive-strength config |
| A1333 VCC source | External LDO from VBB | Verify schematic |
| CAU gain setting | DAG=10 | Verify no saturation at actual stall current with full PCB parasitics |
| Hall placement offset | Zero (no AMCT phase shift) | Verify mechanically at assembly |
| PID gains | TBD — must be tuned on hardware | Cannot pre-calculate without motor inertia/back-EMF data |
| Speed command debounce | 5 ms | Tune based on actual signal quality from vehicle BCM |
| Startup angle acquisition | PWM duty-cycle read via GTU — immediate on power-up, no homing needed | **Resolved — design confirmed** |

---

## 13. Startup Angle Acquisition

With PD7 connected to A1333 PWM output and GTU measuring duty cycle, absolute angle is available within one PWM carrier period (≤1 ms at 1 kHz carrier, configured at EOL). No homing routine or index search is required.

### Startup Sequence

```
Power-on
   │
   ├── Wait ≥30 ms (A1333 tPO + LBIST)
   │
   ├── GTU5: period measurement on PD7 → T_period (counts)
   │   GTU6: high-time measurement on PD7 → T_high (counts)
   │   Both start simultaneously; first valid capture ≤ 1 ms
   │
   ├── Compute absolute angle:
   │     duty = (float)T_high / (float)T_period
   │     if (duty < 0.05f || duty > 0.95f) → A1333_FAULT (out of range)
   │     angle_deg = (duty - 0.05f) / 0.90f * 360.0f
   │     angle_deg += PARK_ANGLE_OFFSET_DEG   /* from flash calibration */
   │
   ├── Seed ABI counter:
   │     abi_count = (int32_t)(angle_deg / ABI_DEG_PER_COUNT)
   │     abi_prev  = (GPIO_ReadPin(PD5) << 1) | GPIO_ReadPin(PD6)
   │
   ├── Enable GPIO interrupts: PD5 both edges, PD6 both edges
   │
   └── absolute_angle_valid = true → control loop may start
```

### PWM Angle Formula

```c
/* A1333 PWM: 5% duty = 0°, 95% duty = 360° */
/* angle = (duty - 0.05) / 0.90 * 360 */
float PWM_ToAngleDeg(uint16_t t_high, uint16_t t_period) {
    if (t_period == 0) return -1.0f;              /* invalid */
    float duty = (float)t_high / (float)t_period;
    if (duty < 0.04f || duty > 0.96f) return -1.0f; /* out of range */
    return (duty - 0.05f) / 0.90f * 360.0f;
}
```

### Periodic PWM Resync (Runtime)

The PWM output continues running while ABI is active. Use GTU7 in period+high-time mode to capture the current PWM angle at 10 Hz and compare to the ABI counter. If they diverge by more than 2° (after accounting for ABI resolution), resync the ABI counter to the PWM value and flag a `WARN_ABI_RESYNC`. More than 3 resyncs per minute indicates accumulated ABI edge loss — escalate to `FAULT_ABI_DEGRADED`.

### Hall-Only Fallback

If both ABI and PWM fail (A1333 VCC lost, GTU misconfiguration), the Hall sector gives a fallback angle estimate:

For P=4 pole-pairs, gear ratio=39: one Hall sector = 60° electrical = 7.5° motor shaft = **0.19° output shaft**. Sufficient for safe park manoeuvre — drive Hall-only at slow speed to mechanical end-stop, then declare sensor fault and hold position.
