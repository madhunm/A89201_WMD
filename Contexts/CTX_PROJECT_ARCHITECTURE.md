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
A1333 ─────────────────►  │ SCI0 SPI → Angle Register Poll                 │
  (output shaft angle)   │      └─► 12-bit shaft angle (0–360°)            │
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
- Position accuracy at park and reversal points: **±0.5°** (requires A1333 ≥600 G magnet, see CTX_A1333)

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
| 7        | SCI0 RX done        | Arm next A1333 SPI read                 | < 1 µs       |
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
    Sensor_Init();          /* A1333 startup wait, BIST check, Hall latch check */
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
            A1333_ReadDiagnostics();      /* Poll ERR/WARN registers */
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
   ├── Set GPIO mux: sel_spi0 → PD6(SCK)/PD7(MOSI/MISO)
   ├── Clear intstat/diag (POR flags)
   └── Enable SCU interrupts (VD50 UV, WDT, GDU fault, ECC error)

2. GPIO init
   ├── PD0–PD2: input, pull-up enabled (Hall latches)
   ├── PD3–PD4: input, pull-down enabled (speed commands)
   ├── PD5: output HIGH (A1333 CS idle state)
   ├── PD6: alternate (SPI0 SCK)
   └── PD7: alternate (SPI0 MOSI/MISO)

3. SCI0 init (SPI master, Mode 3, 4 MHz, 16-bit)

4. DMA init
   ├── DMA1: CAU result registers → SRAM current_samples[3]
   └── DMA2: SCI0 RX register → SRAM a1333_spi_rx_buf

5. A1333 init
   ├── Wait ≥30 ms (power-on self-test)
   ├── Read ERR register — verify LBIST passed (ERR[3]=0)
   ├── Read and clear RST flag (ERR[0])
   ├── Read and store initial angle as reference
   └── Verify EF=0, UV=0 before accepting angle data

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
| A1333 EF flag or parity fail ×3    | Stop position control, freeze angle   |
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
| Sensor fault detection               | A1333 EF+parity check, Hall 000/111 check  |
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

/* A1333 SPI */
#define A1333_ANGLE_REG       0x20
#define A1333_SCLK_HZ         4000000     /* 4 MHz */
#define A1333_CS_SETUP_NS     50
#define A1333_CS_IDLE_NS      200

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
│   ├── a1333.c / .h        — A1333 SPI protocol, angle read, diagnostics
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
| A1333 package | LE (14-pin, single die) | Confirm BOM |
| A1333 magnet field | ≥600 G at sensor surface | Verify magnet selection |
| MOSFET selection | Not specified | Required for GDU dead-time and drive-strength config |
| A1333 VCC source | External LDO from VBB | Verify schematic |
| SPI mode A1333 | Mode 3 (CPOL=1, CPHA=1) | Verify against A1333 App Note or scope measurement |
| CAU gain setting | DAG=10 | Verify no saturation at actual stall current |
| Hall placement offset | Zero (no AMCT phase shift) | Verify mechanically at assembly |
| PID gains | TBD — must be tuned on hardware | Cannot pre-calculate without motor inertia/back-EMF data |
| Speed command debounce | 5 ms | Tune based on actual signal quality from vehicle BCM |
