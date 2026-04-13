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
   │  └── High-Speed CMD ──► 160kΩ/10kΩ divider → Buffer B (×1) ──┐
   └───── Low-Speed CMD  ──► 75kΩ/10kΩ  divider → Buffer A (×1) ──┤
                                100kΩ+100kΩ sum → Buffer C (G=4) → PD3

                         ┌──────────────── A89201C ────────────────────────┐
PD3 (analog) ────────►  │ DAU ADC (PD3, analog mode)                       │
                         │  └─► Speed Command Decoder (4 voltage levels)   │
APS12202 ×3 ──────────►  │ PD0/PD1/PD2 → AMCT (Hall subsystem)            │
  (rotor position)       │      └─► Commutation Sector, Speed Estimate     │
                         │                                                  │
A1333 ──── SPI ────────►  │ SCI0 SPI (PD4=CS, PD5=SCK, PD6=MOSI, PD7=MISO)          │
  (output shaft angle)   │ SCI0 SPI (PD4-7) → angle + EF + UV + parity (0.088°)     │
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
Speed Command (DAU ADC on PD3, 4-level analog)
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

Speed command decoded from DAU ADC on PD3 (analog). Two BCM outputs combine via a three-op-amp conditioning circuit (dual per-channel dividers + unity-gain buffers + G=4 summing buffer). IG pin not used for speed command.

| Command State  | V_PD3 nominal | ADC code (12-bit, VREF=5V) | Speed target | Tolerance   |
|----------------|---------------|----------------------------|--------------|-------------|
| Park / Stop    | 0.00 V        | 0–572                      | 0 RPM (park) | ±1° at park |
| High speed     | 1.41 V        | 573–1736                   | 60 RPM       | ±2 RPM      |
| Low speed      | 2.82 V        | 1737–2892                  | 40 RPM       | ±2 RPM      |
| Invalid (fault)| 4.24 V        | 2893–4095                  | SAFE STATE   | —           |

Decision thresholds: code < 573 = park; 573–1736 = high speed; 1737–2892 = low speed; ≥ 2893 = fault.

BCM low-speed output active → 2.82V (higher code); BCM high-speed active → 1.41V (lower code). Both BCM active simultaneously → fault state ~4.24V.

**VREF assumption: VREF = VIO = 5V.** Verify against DAU documentation before committing resistor values. If VREF < 3.2V rescale dividers (see CTX_MCU_DAU, Section 9).

### 2.3 Angle Target

- Wiper operates within a configurable mechanical arc — MIN_ANGLE_DEG (park) and MAX_ANGLE_DEG (reversal) stored in flash calibration page and set at EOL
- At any speed command, the controller regulates speed while maintaining awareness of the end-stop positions
- At park command (DAU ADC code < 573, ~0V at PD3): position controller drives wiper to `calData.parkAngleDeg` (stored in flash calibration page)
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
| 5        | GPIO (all PD)       | Not used — PD3 analog, Hall via AMCT, SPI via SCI0 | N/A          |
| 6        | GTU overflow        | Watchdog tick, background timers        | < 2 µs       |
| 7        | SCI0 RX complete    | Store A1333 angle result, check EF/UV/parity           | < 2 µs       |
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

/* Triggered by AMCT Hall sector change (IRQ17 — Amct_IrqHandler) */
void Amct_IrqHandler(void) {
    /* 1. Update commutation sector from AMCT_COMU_SECTION register */
    /* 2. Apply new six-step vector to PGU phase outputs */
    /* 3. Compute instantaneous speed from AMCT_HALL_FREQ register */
}

/* Triggered by GDU fault (IRQ13 — GduMaster_IrqHandler) */
void GduMaster_IrqHandler(void) {
    /* 1. Immediately clear ENABLE pin (hardware safe state) */
    /* 2. Set global fault flag */
    /* 3. Log fault type from GDU STATUS register */
}

/* GPIO ISR (IRQ16 — Gpio_IrqHandler): NOT configured in this project.
 * PD0–PD2: Hall inputs routed through AMCT hardware (no GPIO IRQ needed).
 * PD3: analog DAU ADC input — speed command polled at 100 Hz in background.
 * PD4–PD7: SPI peripheral signals — no GPIO IRQ.
 * Do NOT enable GPIO interrupts on any pin. This handler should never fire.
 * If it does fire unexpectedly, log FAULT_GPIO_SPURIOUS and do nothing else. */
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
            SpeedCommand_UpdateFromADC(); /* DAU slot 3 (PD3 analog) → decode 4-level voltage → update speed setpoint */
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
   ├── Disable WDT immediately on entry (Scu_WatchdogDisable) — re-enable after all init
   ├── Enable flash instruction prefetch: Scu_SetFlashControlPreFetchEnable(True)
   ├── Set mcuclk: CPU = 40 MHz
   ├── Configure WDT period = 10 ms
   ├── Set sel_spi0 = 7 (routes PD7 to SCI0 SPI RX/MISO)
   ├── Set sel_uart1 → assign UART RX pin for debug UART (development only, SCI1)
   ├── Clear intstat/diag (POR flags)
   └── Enable SCU interrupts (VD50 UV, WDT, GDU fault, ECC error)

2. GPIO init
   ├── PD0–PD2: input, pull-up enabled (Hall latches)
   ├── PD3: analog input, no pull (eGPIO_MODE_ANALOG_INPUT_SOURCE) — speed command, driven by external 3-op-amp circuit
   ├── PD4: SPI0_CSOUT output (ASRCSEL=0100b, AOSEL=1)
   ├── PD5: SPI0_SCKOUT output (ASRCSEL=0010b, AOSEL=1)
   ├── PD6: SCI0_Tx output/MOSI (ASRCSEL=0000b, AOSEL=1)
   ├── PD7: input, no pull (SPI0 MISO, sel_spi0=7)
   └── LIN: not configured (reserved for future use)
   NOTE: IG pin not used. GPIO interrupts not enabled on any pin.

3. SCI0 init (UART debug only — disabled in production build; clock-gated via SCU)
   SCI1: not configured

4. DMA init
   ├── DMA1: CAU result registers → SRAM current_samples[3]
   └── DMA2: Reserved (SCI0 UART TX buffer if debug logging enabled)

5. A1333 init
   ├── Wait ≥30 ms (A1333 power-on self-test period — tPO)
   ├── At EOL only (external programmer via SPI test pads):
   │     Verify LBIST passed (ERR[3]=0)
   │     Configure RESOLUTION_PAIRS = 3 (2048 PPR ABI)
   │     Write ZERO_OFFSET = 0 (park calibration done in firmware, stored in flash)
   │     Lock EEPROM after confirmed good
   ├── At runtime (firmware):
   │     SCI0 SPI enabled (PD4=CS, PD5=SCK, PD6=MOSI, PD7=MISO)
   │     Wait for A1333 tPO (30 ms already elapsed)
   │     First SPI read register 0x20 → verify EF=0, UV=0, parity
   │     Read ERR register (0x24) → verify LBIST bit = 0
   │     angle_deg = raw × (360/4096) − calData.zeroOffsetDeg
   └── Absolute reference established at first park event — see Section 13

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
/* For P=2: RPM_output = 60 / (6 × 2 × T_hall_sec × 39) */
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
| (IRAM — unused)     | 0x0001_0000          | 8 kB   | Not used — flash prefetch sufficient |
| Calibration data    | 0x0801_C000          | 4 kB   | Page 28: WiperCalData_t (A89201C 128KB) |
| Fault log           | 0x0801_D000          | 4 kB   | Page 29: fault history / park log |
| Reserved (protected)| 0x0803_8000 (page 54)| 4 kB   | Allegro reserved                |
| Reserved (protected)| 0x0803_F000 (page 63)| 4 kB   | Allegro reserved                |

**Calibration data page** stores:
- `parkAngleDeg` (float): ABI angle at mechanical park position
- `reversalAngleDeg` (float): ABI angle at maximum sweep position (configurable at EOL)
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
| DAU PD3 code ≥ 2893 (both BCM active → ~4.24V) | ENABLE = 0, latch FAULT_SPEED_CMD_INVALID |
| VD50 undervoltage                  | ENABLE = 0, fault                     |
| Die temperature ≥ 125°C            | ENABLE = 0, wait for cool-down        |

Safe state: all PWM outputs off (ENABLE pin LOW), wiper holds position, fault code available.

Recovery from safe state requires a power cycle or explicit external reset (unless fault is thermal, in which case auto-recovery after cool-down is permitted).

---

## 9. ASIL-B Compliance Evidence Points

| Requirement                          | Implementation                              |
|--------------------------------------|---------------------------------------------|
| Hardware watchdog                    | SCU WDT, 10 ms timeout, kicked in main loop |
| Memory integrity                     | SRAM/Flash ECC via SCU                     |
| Sensor fault detection               | ABI rate/index plausibility, Hall 000/111 check, Hall vs ABI speed cross-check |
| Actuator fault detection             | GDU VDS monitoring, CAU stall current       |
| Safe state on fault                  | ENABLE pin LOW, PWM disabled                |
| Single-point fault prevention        | Two independent speed sensors (Hall for speed, A1333 for position) |
| Stack overflow protection            | MPU configured for stack region             |
| Invalid input detection              | DAU PD3 code ≥ 2893 treated as fault (both BCM lines active simultaneously) |
| Calibration data integrity           | CRC32 check at every startup               |
| Software execution monitoring        | Watchdog + execution flow checkpoints       |

**Remaining ASIL-B gap:** The A1333 Safety Manual must be obtained from Allegro and followed. The A89201 does not have a published functional safety manual — contact Allegro for ASIL-B application support.

---

## 10. Key Configuration Constants

```c
/* Motor and mechanical parameters */
#define POLE_PAIRS          2       /* Motor pole pairs — confirmed 2-pole-pair motor */
#define GEAR_RATIO          39      /* Worm gearbox ratio */
#define PWM_FREQ_HZ         20000   /* 20 kHz PWM */
#define CPU_FREQ_HZ         40000000

/* Speed targets (output shaft RPM) */
#define SPEED_LOW_RPM       40.0f
#define SPEED_HIGH_RPM      60.0f
#define SPEED_TOLERANCE_RPM 2.0f

/* Angle targets — loaded from WiperCalData_t in flash page 28 */
#define ANGLE_TOLERANCE_DEG  1.0f           /* ±1° position accuracy requirement */
#define OVERRUN_TOLERANCE_DEG 2.0f          /* Extra travel before hard fault */
/* calData.parkAngleDeg     — park position (flash page 28)   */
/* calData.reversalAngleDeg — reversal position (flash page 28) */

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

/* A1333 SPI interface (PD4=CS, PD5=SCK, PD6=MOSI, PD7=MISO) */
#define A1333_SPI_CLK_HZ        4000000           /* 4 MHz, A1333 max = 10 MHz */
#define A1333_ANGLE_REG         0x2000            /* Register 0x20, R/W=read, 12-bit angle */
#define A1333_ERR_REG           0x2400            /* Register 0x24, error flags */
#define A1333_WARN_REG          0x2600            /* Register 0x26, warning flags */
#define A1333_TSEN_REG          0x2800            /* Register 0x28, temperature sensor */
#define A1333_DEG_PER_COUNT     (360.0f / 4096.0f) /* 0.0879°/count, 12-bit */
#define A1333_FAULT_THRESHOLD   3                 /* consecutive read failures before fault */
#define A1333_DIAG_RATE_HZ      10                /* diagnostic register poll rate */

/* Watchdog */
#define WDT_PERIOD_MS         10

/* Speed command ADC decode — DAU slot 3, PD3 analog input, VREF = VIO = 5V assumed */
/* External circuit: dual BCM dividers (75kΩ/10kΩ low, 160kΩ/10kΩ high) → */
/*   Buffer A (×1) + Buffer B (×1) → 100kΩ+100kΩ sum → Buffer C (G=4, Rf=30kΩ, Rg=10kΩ) → PD3 */
/* 40V load dump (Ri=0.5Ω): BufA_in=4.68V, BufB_in=2.35V, both ≤ VIO=5V, BufC output clips gracefully */
#define SPEED_ADC_PARK_MAX      572U   /* 0–0.70 V  → Park  */
#define SPEED_ADC_HIGH_MAX      1736U  /* 0.70–1.73 V → High speed (60 RPM) */
#define SPEED_ADC_LOW_MAX       2892U  /* 1.73–3.54 V → Low speed (40 RPM) */
                                       /* ≥ 2893     → Invalid / fault  */
```

---

## 11. File and Module Structure (Keil uVision Project)

```
WiperController/
├── main.c                  — System init, background loop
├── startup_A89201.s        — Vector table, stack init (no IRAM copy)
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
│   ├── a1333.c / .h        — A1333 SPI read/write, angle read, diagnostic polling, EEPROM access
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
| Motor pole pairs | 2 pole-pairs | **Confirmed** |
| Wiper arc | Configurable — stored in flash (MIN_ANGLE_DEG, MAX_ANGLE_DEG) | Set at EOL via calibration procedure |
| A1333 package | LE (14-pin, single die) | **Confirmed** |
| A1333 SPI mode | Mode 3 (CPOL=1, CPHA=1) inferred from timing | Verify on first bring-up with scope |
| A1333 angle accuracy | ±1° over temperature | **Confirmed acceptable** |
| A1333 calibration | End-of-line offset cal + optional temp comp | Design decision confirmed |
| MOSFET selection | Not specified | Required for GDU dead-time and drive-strength config |
| A1333 VCC source | External LDO from VBB | Verify schematic |
| CAU gain setting | DAG=10 | Verify no saturation at actual stall current with full PCB parasitics |
| Hall placement offset | Zero (no AMCT phase shift) | Verify mechanically at assembly |
| PID gains | TBD — must be tuned on hardware | Cannot pre-calculate without motor inertia/back-EMF data |
| Speed command VREF | Assumed VIO = 5V | Verify from DAU documentation — if VREF < 3.2V, rescale dividers per CTX_MCU_DAU §9 |
| Speed command poll rate | 100 Hz (10ms bg tick) | Adequate for human-time-scale BCM changes; increase if BCM transitions faster |
| Startup angle acquisition | SPI read of register 0x20 after 30 ms tPO — absolute, no homing | **Confirmed** |

---

## 13. Startup Angle Acquisition

A1333 connected via SPI (PD4=CS, PD5=SCK, PD6=MOSI, PD7=MISO). SPI angle register 0x20 provides absolute shaft angle immediately after A1333 power-on time (tPO ≥ 30 ms). No homing routine required.

### Startup Sequence

```
Power-on
   │
   ├── Wait ≥30 ms (A1333 tPO + LBIST)
   │
   ├── First SPI read: send 0x2000 to register 0x20
   │     Check EF=0, UV=0, parity valid
   │     angle_deg = raw * (360.0 / 4096.0) − calData.zeroOffsetDeg
   │
   ├── absolute_angle_valid = true → position controller may start
   │
   └── Read ERR register (0x24): verify LBIST bit (ERR[3]) = 0
         If LBIST failed → FAULT_A1333_LBIST, enter safe state
```

### Configurable Wiper Arc

Arc limits stored in flash page 28 (calibration), set at EOL:

```c
typedef struct {
    float     parkAngleDeg;        /* SPI angle at mechanical park position */
    float     reversalAngleDeg;    /* SPI angle at maximum sweep position */
    float     zeroOffsetDeg;       /* Offset applied to every SPI angle reading */
    float     calPoints_raw[8];    /* Optional linearity correction table */
    float     calPoints_ref[8];
    uint8_t   calPointCount;
    uint32_t  crc32;
} WiperCalData_t;
```

Position limits enforced every control cycle:
```c
if (angle_deg < calData.parkAngleDeg    - OVERRUN_DEG) EnterSafeState(FAULT_UNDER_TRAVEL);
if (angle_deg > calData.reversalAngleDeg + OVERRUN_DEG) EnterSafeState(FAULT_OVER_TRAVEL);
```

EOL procedure:
1. First SPI read → immediate absolute angle (no drive needed)
2. Drive to mechanical park stop → write `parkAngleDeg` = current SPI angle
3. Drive to reversal stop → write `reversalAngleDeg`
4. Write CRC, program flash page 28

### Fallback: Hall-Only Mode

If A1333 SPI fails at startup (3 consecutive read errors):
- P=2, gear ratio=39: Hall sector = 0.385° output shaft ≈ ±0.19° uncertainty
- Enable Hall-only commutation, drive to park at ≤15 RPM
- Set FAULT_A1333_COMM, hold position, await reset
