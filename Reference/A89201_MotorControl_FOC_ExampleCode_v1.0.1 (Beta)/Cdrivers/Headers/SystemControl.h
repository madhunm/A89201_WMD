#ifndef HEADER_SYSTEM_CONTROL_COMMON_H
#define HEADER_SYSTEM_CONTROL_COMMON_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file       SystemControl.h
 * @author     G. Sloan
 * @version    1.0
 *
 * @brief      MCU system control common register interface
 *
 * @copyright (c) 2024 Allegro MicroSystems, Inc. All Rights Reserved.
 * The information contained in this file is confidential and proprietary.
 *
 * The information contained in this file is governed by Software License
 * acknowledged during the download from Allegro's Software Download
 * Portal. You may obtain a copy of the Software License at
 * https://www.allegromicro.com/en/about-allegro/legal.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the Software License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the Software License for the specific language
 * governing permissions and limitations under the License.
 *
 * You understand that the Software is subject to export control laws and
 * regulations. YOU MAY NOT DOWNLOAD OR OTHERWISE EXPORT
 * OR RE-EXPORT THE SOFTWARE OR ANY UNDERLYING
 * INFORMATION OR TECHNOLOGY EXCEPT IN FULL COMPLIANCE
 * WITH ALL UNITED STATES AND OTHER APPLICABLE LAWS AND
 * REGULATIONS.
 *
 */
 /* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
#include "Std_Types.h"
#include "A89201RegisterDefinitions.h"

// ScuDiagnostic cached RAM mask
#define SCU_DIAG_POR_CACHE     ( 0b0000000000000001U )
#define SCU_DIAG_SYS_CACHE     ( 0b0000000000000010U )
#define SCU_DIAG_WDT_CACHE     ( 0b0000000000000100U )
#define SCU_DIAG_LKUP_CACHE    ( 0b0000000000001000U )
#define SCU_DIAG_MCLR_CACHE    ( 0b0000000000010000U )
#define SCU_DIAG_PLLOK_CACHE   ( 0b0000000000100000U )
#define SCU_DIAG_PLLTO_CACHE   ( 0b0000000001000000U )
#define SCU_DIAG_SRCTO_CACHE   ( 0b0000000010000000U )

// ScuRegisters
#define SCU_BASE_ADDRESS  0x4000D000U
#define SCU_CTRL          0x08U
#define SCU_INTSTAT       0x10U
#define SCU_INTEN         0x14U
#define SCU_DIAG          0x18U
#define SCU_MCUCLK        0x1CU
#define SCU_ECC_DSA       0x20U
#define SCU_ECC_DSE       0x24U
#define SCU_ECC_ISA       0x28U
#define SCU_ECC_ISE       0x2CU
#define SCU_WDT           0x30U
#define SCU_WDT_PRD       0x34U
#define SCU_SEL_UART0     0x38U
#define SCU_SEL_UART1     0x3CU
#define SCU_SEL_SPI0      0x40U
#define SCU_SEL_SPI1      0x44U
#define SCU_SEL_LIN       0x48U
#define SCU_SEL_LCTX      0x54U
#define SCU_DEBUG         0x58U


// ScuControl
#define SCU_CONTROL_DDA               ( 0b0000000000000001U )
#define SCU_CONTROL_LOCK_EN           ( 0b0000000000000010U )
#define SCU_CONTROL_WDT_EN            ( 0b0000000000000100U )
#define SCU_CONTROL_WDT_RESET_EN      ( 0b0000000000001000U )
#define SCU_CONTROL_SWD_EN            ( 0b0000000000010000U )
#define SCU_CONTROL_SCI0_MODE         ( 0b0000000000100000U )
#define SCU_CONTROL_SCI1_MODE         ( 0b0000000001000000U )
#define SCU_CONTROL_IDA               ( 0b0000001000000000U )
#define SCU_CONTROL_PLL_OFF           ( 0b0000010000000000U )
#define SCU_CONTROL_NO_HLT            ( 0b0000100000000000U )
#define SCU_CONTROL_VIO_MONITOR       ( 0b0001000000000000U )


// ScuIntStat
#define SCU_INTSTAT_VD50UV      ( 0b0000000000000001U )
#define SCU_INTSTAT_VD33UV      ( 0b0000000000000010U )
#define SCU_INTSTAT_VIOUV       ( 0b0000000000000100U )
#define SCU_INTSTAT_OVTEMP      ( 0b0000000000001000U )
#define SCU_INTSTAT_DSSEC       ( 0b0000000000010000U )
#define SCU_INTSTAT_DSDED       ( 0b0000000000100000U )
#define SCU_INTSTAT_ISSECT      ( 0b0000000001000000U )
#define SCU_INTSTAT_ISDED       ( 0b0000000010000000U )
#define SCU_INTSTAT_WDT         ( 0b0000000100000000U )
#define SCU_INTSTAT_LOSTLOCK    ( 0b0000001000000000U )
#define SCU_INTSTAT_VD50OV      ( 0b0000010000000000U )

#define SCU_INTSTAT_VD50UV_BITPOS   (0U)
#define SCU_INTSTAT_VD33UV_BITPOS   (1U)
#define SCU_INTSTAT_VIOUV_BITPOS    (2U)
#define SCU_INTSTAT_OVTEMP_BITPOS   (3U)
#define SCU_INTSTAT_DSSEC_BITPOS    (4U)
#define SCU_INTSTAT_DSDED_BITPOS    (5U)
#define SCU_INTSTAT_ISSEC_BITPOS    (6U)
#define SCU_INTSTAT_ISDED_BITPOS    (7U)
#define SCU_INTSTAT_WDT_BITPOS      (8U)
#define SCU_INTSTAT_LOSTLOCK_BITPOS (9U)
#define SCU_INTSTAT_VD50OV_BITPOS   (10U)

// ScuIntEnable
#define SCU_INTENABLE_VD50UV     ( 0b0000000000000001U )
#define SCU_INTENABLE_VD33UV     ( 0b0000000000000010U )
#define SCU_INTENABLE_VDIOUV     ( 0b0000000000000100U )
#define SCU_INTENABLE_OVTEMP     ( 0b0000000000001000U )
#define SCU_INTENABLE_DSSEC      ( 0b0000000000010000U )
#define SCU_INTENABLE_DSDED      ( 0b0000000000100000U )
#define SCU_INTENABLE_ISSEC      ( 0b0000000001000000U )
#define SCU_INTENABLE_ISDED      ( 0b0000000010000000U )
#define SCU_INTENABLE_WDT        ( 0b0000000100000000U )
#define SCU_INTENABLE_LOSTLOCK   ( 0b0000001000000000U )
#define SCU_INTENABLE_VD50OV     ( 0b0000010000000000U )

// ScuMcuClk
#define SCU_MCUCLK_40MHZ     ( 0b0000000000000000U )
#define SCU_MCUCLK_20MHZ     ( 0b0000000000000001U )
#define SCU_MCUCLK_10MHZ     ( 0b0000000000000010U )
#define SCU_MCUCLK_5MHZ      ( 0b0000000000000011U )
#define SCU_MCUCLK_2P5MHZ    ( 0b0000000000000100U )
#define SCU_MCUCLK_1P25MHZ   ( 0b0000000000000101U )
#define SCU_MCUCLK_625KHZ    ( 0b0000000000000110U )
#define SCU_MCUCLK_128KHZ    ( 0b0000000000000111U )
#define SCU_MCUCLK_CLEAR     ( 0b0000000000000111U )

#define SCU_MCUCLK_SELWD     ( 0b0000000000001000U )
#define SCU_MCUCLK_PWM       ( 0b0000000000010000U )
#define SCU_MCUCLK_AMCT      ( 0b0000000000100000U )
#define SCU_MCUCLK_DMA       ( 0b0000000010000000U )
#define SCU_MCUCLK_CAU       ( 0b0000000100000000U )
#define SCU_MCUCLK_GDU       ( 0b0000001000000000U )
#define SCU_MCUCLK_DAU       ( 0b0000010000000000U )
#define SCU_MCUCLK_GPIO      ( 0b0000100000000000U )
#define SCU_MCUCLK_GTU       ( 0b0001000000000000U )
#define SCU_MCUCLK_SCI0      ( 0b0010000000000000U )
#define SCU_MCUCLK_SCI1      ( 0b0100000000000000U )
#define SCU_MCUCLK_LIN       ( 0b1000000000000000U )

// ScuWdt
#define SCU_WDT_RSTRT        ( 0b0000000000000001U )


// ScuWdtPeriod
#define SCU_WDT_PERIOD       ( 0b0000011111111111U )

// ScuSelUartRx
#define SCU_SELUARTRX_MASK   ( 0b0000000000001111U )
#define SCU_SELUARTRX_BITPOS ( 0U )
#define SCU_SELUARTRX_D0  0
#define SCU_SELUARTRX_D1  1
#define SCU_SELUARTRX_D2  2
#define SCU_SELUARTRX_D3  3
#define SCU_SELUARTRX_D4  4
#define SCU_SELUARTRX_D5  5
#define SCU_SELUARTRX_D6  6
#define SCU_SELUARTRX_D7  7
#define SCU_SELUARTRX_D8  8
#define SCU_SELUARTRX_D9  9
#define SCU_SELUARTRX_D10  10
#define SCU_SELUARTRX_D11  11
#define SCU_SELUARTRX_D12  12
#define SCU_SELUARTRX_D13  13
#define SCU_SELUARTRX_D14  14
#define SCU_SELUARTRX_D15  15

// ScuSelSpiRx
#define SCU_SELSPIRX_MASK   ( 0b0000000000001111U )
#define SCU_SELSPIRX_BITPOS ( 0U )
#define SCU_SELSPIRX_D0  0
#define SCU_SELSPIRX_D1  1
#define SCU_SELSPIRX_D2  2
#define SCU_SELSPIRX_D3  3
#define SCU_SELSPIRX_D4  4
#define SCU_SELSPIRX_D5  5
#define SCU_SELSPIRX_D6  6
#define SCU_SELSPIRX_D7  7
#define SCU_SELSPIRX_D8  8
#define SCU_SELSPIRX_D9  9
#define SCU_SELSPIRX_D10  10
#define SCU_SELSPIRX_D11  11
#define SCU_SELSPIRX_D12  12
#define SCU_SELSPIRX_D13  13
#define SCU_SELSPIRX_D14  14
#define SCU_SELSPIRX_D15  15
    
// ScuSelSpiSckIn
#define SCU_SELSPISCKIN_MASK   ( 0b0000000011110000U )
#define SCU_SELSPISCKIN_BITPOS ( 4U )
#define SCU_SELSPISCKIN_D0   0x00
#define SCU_SELSPISCKIN_D1   0x10
#define SCU_SELSPISCKIN_D2   0x20
#define SCU_SELSPISCKIN_D3   0x30
#define SCU_SELSPISCKIN_D4   0x40
#define SCU_SELSPISCKIN_D5   0x50
#define SCU_SELSPISCKIN_D6   0x60
#define SCU_SELSPISCKIN_D7   0x70
#define SCU_SELSPISCKIN_D8   0x80
#define SCU_SELSPISCKIN_D9   0x90
#define SCU_SELSPISCKIN_D10  0xA0
#define SCU_SELSPISCKIN_D11  0xB0
#define SCU_SELSPISCKIN_D12  0xC0
#define SCU_SELSPISCKIN_D13  0xD0
#define SCU_SELSPISCKIN_D14  0xE0
#define SCU_SELSPISCKIN_D15  0xF0

// ScuSelSpiCsIn
#define SCU_SELSPICSIN_MASK   ( 0b0000111100000000U )
#define SCU_SELSPICSIN_BITPOS ( 8U )
#define SCU_SELSPICSIN_D0   0x000
#define SCU_SELSPICSIN_D1   0x100
#define SCU_SELSPICSIN_D2   0x200
#define SCU_SELSPICSIN_D3   0x300
#define SCU_SELSPICSIN_D4   0x400
#define SCU_SELSPICSIN_D5   0x500
#define SCU_SELSPICSIN_D6   0x600
#define SCU_SELSPICSIN_D7   0x700
#define SCU_SELSPICSIN_D8   0x800
#define SCU_SELSPICSIN_D9   0x900
#define SCU_SELSPICSIN_D10  0xA00
#define SCU_SELSPICSIN_D11  0xB00
#define SCU_SELSPICSIN_D12  0xC00
#define SCU_SELSPICSIN_D13  0xD00
#define SCU_SELSPICSIN_D14  0xE00
#define SCU_SELSPICSIN_D15  0xF00

// ScuSelLcTx
#define SCU_SELLCTX_DIRECT  ( 0b0000000000000001U )
#define SCU_SELLCTX_DOUT    ( 0b0000000000000010U )

// ScuDebug
#define SCU_DEBUG_DISABLE   ( 0b0000000000000001U )

// Declared and deliberately undefined 8 byte array for the reserved RAM start address.
// Clear on write diagnostic register values are cached by the boot ROM here. 
 /* polyspace-begin DEFECT:HARD_CODED_BUFFER_SIZE [Justified:Not a defect] "Necessary reserved RAM allocation." */
static uint8 spau8ReservedRamBootRomCache[8] __attribute__((section(".ARM.__at_0x20000000"))); 
 /* polyspace-end DEFECT:HARD_CODED_BUFFER_SIZE [Justified:Not a defect] "Necessary reserved RAM allocation." */

enum ScuSciInst 
{
    eSCU_SCI_1 = SCU_CONTROL_SCI0_MODE, 
    eSCU_SCI_2 = SCU_CONTROL_SCI1_MODE, 
};

enum ScuSciMode
{
    eSCU_SCI_SPI_MODE = 0U,
    eSCU_UART_MODE = 1U,
};

enum ScuUartInst 
{
    eSCU_UART_1 = SCU_SEL_UART0, 
    eSCU_UART_2 = SCU_SEL_UART1, 
};

enum ScuSpiInst 
{
    eSCU_SPI_1 = SCU_SEL_SPI0, 
    eSCU_SPI_2 = SCU_SEL_SPI1,
};

enum ScuRxInput
{
    eSCU_RX_INPUT_D0 =  0x00U,
    eSCU_RX_INPUT_D1 =  0x01U,
    eSCU_RX_INPUT_D2 =  0x02U,  
    eSCU_RX_INPUT_D3 =  0x03U,
    eSCU_RX_INPUT_D4 =  0x04U,
    eSCU_RX_INPUT_D5 =  0x05U,  
    eSCU_RX_INPUT_D6 =  0x06U,
    eSCU_RX_INPUT_D7 =  0x07U,
    eSCU_RX_INPUT_D8 =  0x08U,
    eSCU_RX_INPUT_D9 =  0x09U,
    eSCU_RX_INPUT_D10 = 0x0AU,
    eSCU_RX_INPUT_D11 = 0x0BU,
    eSCU_RX_INPUT_D12 = 0x0CU,
    eSCU_RX_INPUT_D13 = 0x0DU,
    eSCU_RX_INPUT_D14 = 0x0EU,
    eSCU_RX_INPUT_D15 = 0x0FU,
};

/** Enumerate the Irq types */
enum SystemIrq
{
    /** The non-maskable interrupt request */
    eSYSTEMIRQ_NMI,
    /** The hard fault interrupt request */
    eSYSTEMIRQ_HARD_FAULT,
    /** The memory management interrupt request */
    eSYSTEMIRQ_MEMORY_MANAGEMENT,
    /** The bus interrupt request */
    eSYSTEMIRQ_BUS_FAULT,
    /** The usage interrupt request */
    eSYSTEMIRQ_USAGE_FAULT,
    /** The flash or system interrupt request */
    eSYSTEMIRQ_FLASH_OR_SYSTEM_FAULT
};

/** Enumerate miscellaneous SCU registers */
enum SystemMiscRegs
{
    eSCU_INVALID =  0xFF,
    eSCU_ECC_DSA =  0x08,
    eSCU_ECC_DSE =  0x09,
    eSCU_ECC_ISA =  0x0A,
    eSCU_ECC_ISE =  0x0B,
    eSCU_SEL_LIN =  0x12
};

// type definition for the system control register
typedef struct __attribute__((packed)) {
    uint32 reserved1[2];    // 0x00 - 0x07
    uint32 ctrl;            // 0x08 :(R/W)        <0>sten, <1>lken, <2>wdten, <3>rsten, <4>dbgdis, <5>sci0mode, <6>sci1mode, <32:7>RESERVED
    uint32 reserved2;       // 0x0C 
    uint32 instat;          // 0x10 :(R/W)        <0>fsec, <1>fded, <2>fspi, <3>frdv, <4>dssec, <5>dsded, <6>issec, <7>isded, <8>wdt, <9>caruv, <10>RESERVED, <11>ovtemp
    uint32 inten;           // 0x14 :(R/W)        <0>fsec, <1>fded, <2>fspi, <3>frdv, <4>dssec, <5>dsded, <6>issec, <7>isded, <8>wdt, <9>caruv, <10>RESERVED, <11>ovtemp
    uint32 diag;            // 0x18 :(R/ )        <0>por, <1>sys, <2>wdt, <3>lkup, <4>RESERVED, <5>varuv, <6>RESERVED, <7>ovtemp
    uint32 mcuclk;          // 0x1C :(R/W)        <3:0>freq[3:0], <4>arm_clkdis, <5>gtu_clkdis, <6>lin_clkdis, <7>RESERVED, <8>sci0_clkdis, <9>sci1_clkdis, <10>pwm_clkdis, <11>amct_clkdis, <12>dau_clkdis, <13>cau_clkdis
    uint32 eccDsa;          // 0x20 :(R/ )        <14:0>addr[14:0], <31:15>RESERVED
    uint32 eccDse;          // 0x24 :(R/ )        <6:0>ecc[6:0], <31:7>RESERVED
    uint32 eccIsa;          // 0x28 :(R/ )        <15:0>addr[15:0], <31:16>RESERVED
    uint32 eccIse;          // 0x2C :(R/ )        <6:0>ecc[6:0], <31:7>RESERVED
    uint32 wdt;             // 0x30 :(R/W)        <0>rstrt, <31:1>RESERVED
    uint32 wdtPeriod;       // 0x34 :(R/W)        <10:0>cnt, <31:11>RESERVED
    uint32 selUart0;        // 0x38 :(R/W)        <3:0>mux_gpi[3:0], <4>sel, <31:5>RESERVED
    uint32 selUart1;        // 0x3C :(R/W)        <3:0>mux_gpi[3:0], <4>sel, <31:5>RESERVED
    uint32 selSpi0;         // 0x40 :(R/W)        <3:0>mux_gpi[3:0], <4>sel, <8:5>sckin[3:0], <12:9>csin[3:0], <31:13>RESERVED
    uint32 selSpi1;         // 0x44 :(R/W)        <3:0>mux_gpi[3:0], <4>sel, <8:5>sckin[3:0], <12:9>csin[3:0], <31:13>RESERVED
    uint32 selLin;          // 0x48 :(R/W)        <3:0>mux_gpi[3:0], <4>sel, <31:5>RESERVED
    uint32 reserved3;       // 0x4C        
    uint32 reserved4;       // 0x50 
    uint32 selLctx;         // 0x54 :(R/W)        <0>LCTX output, <1> data value for SEL_LCTX.DIRECT mode
    uint32 debug;           // 0x58
} SystemControlRegs_t;

#define  stScuRegs (*( volatile SystemControlRegs_t *)eADDRESS_SYSCTRL_BASE)


/** Enumerate Flash Control register bits */
enum SystemFlashControlBits
{
    eECC_ENABLE =       0x20,
    eECC_DISABLE =      0xDF,
    ePREFETCH_ENABLE =  0x40,
    ePREFETCH_DISABLE = 0xBF,
};

/** Enumerate Flash Error register bit masks FlashIntStat and FlashIntEn*/
enum SystemFlashErrorBitMasks
{
    eLECCS = 0x02,
    eLECCD = 0x04,
    eMECCS = 0x08,
    eMECCD = 0x10
};


// type definition for the system control registers for Flash controller 
typedef struct __attribute__((packed)) {
    uint32 flashctrl;
    uint32 reserved[7];
    uint32 flashintstat;
    uint32 flashinten;
} SystemControlFlashRegs_t;

#define  stScuFlashRegs (*( volatile SystemControlFlashRegs_t *)eADDRESS_FLASH_BASE)

#include "SystemControlProduct.h"

/////////////////////////
// Reset macros
/////////////////////////

/**
 * @brief  Enable warm reset on lockup.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WarmResetOnLockupEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_LOCK_EN);
}

/**
 * @brief  Disable warm reset on lockup.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WarmResetOnLockupDisable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_LOCK_EN);
}

/**
 * @brief  Get the last reset reason.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline uint32 Scu_GetLastResetReason(void)
{
    uint32 u32RegValue = 0;
    GetRegister(spau8ReservedRamBootRomCache, u32RegValue); /* polyspace MISRA-C3:11.3 [Justified:Unset] "Necessary RAM address memory access" */
    return u32RegValue & (SCU_DIAG_POR_CACHE | SCU_DIAG_SYS_CACHE | SCU_DIAG_WDT_CACHE | SCU_DIAG_LKUP_CACHE);
}

/////////////////////////
// Clock macros
/////////////////////////

/**
 * @brief  Enable the PLL by clearing the PLL OFF signal.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_PllEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_PLL_OFF);
}

/**
 * @brief  Disable the PLL.
 * @remarks When the PLL is turned off, clock source will be changed to clk_wd and power consumption will be reduced.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_PllDisable(void)
{
    // Select CLK_WD as MCLK. This should be done before turning PLL off.
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_SELWD); 
    SetRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_PLL_OFF);
}

/**
 * @brief    Update the PLL output frequency.
 * @param[in] u32PllClockFreq The selected clock frequency.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_UpdatePllFreq(const uint32 u32PllClockFreq)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_CLEAR);
	
    switch(u32PllClockFreq) /* polyspace DEFECT:INAPPROPRIATE_TYPE_IN_SWITCH [Justified:Unset] "Full range of unsigned int explicitly handled" */
    {
      // Deliberate fallthrough statements for input validation and override to default.
      case SCU_MCUCLK_40MHZ:
      case SCU_MCUCLK_20MHZ:
      case SCU_MCUCLK_10MHZ:
      case SCU_MCUCLK_5MHZ:
      case SCU_MCUCLK_2P5MHZ:
      case SCU_MCUCLK_1P25MHZ:
      case SCU_MCUCLK_625KHZ:
      case SCU_MCUCLK_128KHZ:
          SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, u32PllClockFreq);
      break;
      default:
          SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_40MHZ);
			break;
    };
}

/**
 * @brief    Select CLK_WD as MCLK.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetClkWdAsMclk(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_SELWD);
}

/**
 * @brief    Disable PWM clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_PwmClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_PWM);
}

/**
 * @brief    Disable AMCT clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_AmctClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_AMCT);
}


/**
 * @brief    Disable DMA clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_DmaClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_DMA);
}

/**
 * @brief    Disable CAU clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_CauClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_CAU);
}

/**
 * @brief    Disable GDU clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_GduClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_GDU);
}

/**
 * @brief    Disable DAU clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_DauClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_DAU);
}

/**
 * @brief    Disable GPIO clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_GpioClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_GPIO);
}

/**
 * @brief    Disable GTU clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_GtuClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_GTU);
}

/**
 * @brief    Disable SCI0 clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_Sci0ClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_SCI0);
}

/**
 * @brief    Disable SCI1 clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_Sci1ClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_SCI1);
}

/**
 * @brief    Disable LIN clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_LinClockDisable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_LIN);
}

/**
 * @brief    Enable PWM clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_PwmClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_PWM);
}

/**
 * @brief    Enable AMCT clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_AmctClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_AMCT);
}

/**
 * @brief    Enable DMA clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_DmaClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_DMA);
}

/**
 * @brief    Enable CAU clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_CauClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_CAU);
}

/**
 * @brief    Enable GDU clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_GduClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_GDU);
}

/**
 * @brief    Enable DAU clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_DauClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_DAU);
}

/**
 * @brief    Enable GPIO clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_GpioClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_GPIO);
}

/**
 * @brief    Enable GTU clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_GtuClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_GTU);
}

/**
 * @brief    Enable SCI0 clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_Sci0ClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_SCI0);
}
 
/**
 * @brief    Enable SCI1 clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_Sci1ClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_SCI1);
}

/**
 * @brief    Enable LIN clock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_LinClockEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.mcuclk, SCU_MCUCLK_LIN);
}

/**
 * @brief    Get PLL loss of lock.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusPllLockLoss(void)
{
    uint32 u32RegValue = 0;
    GetRegister(&stScuRegs.instat, u32RegValue);
    return (boolean)((u32RegValue & SCU_INTSTAT_LOSTLOCK) > 0);
}

/**
 * @brief    Get PLL OK status.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusPllOk(void)
{
    uint32 u32RegValue = 0;
    GetRegister(&stScuRegs.diag, u32RegValue);
    return (boolean)((u32RegValue & SCU_DIAG_PLLOK_REG) > 0);
}

/**
 * @brief    Get PLL timeout detection status.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusPllTimeoutDetection(void)
{
	  uint32 u32RegValue = 0;
    GetRegister(spau8ReservedRamBootRomCache, u32RegValue); /* polyspace MISRA-C3:11.3 [Justified:Unset] "Necessary RAM address memory access" */	
    return (boolean)((u32RegValue & SCU_DIAG_PLLTO_CACHE) > 0);
}

/**
 * @brief   Get PLL source timeout status.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusPllSourceTimeout(void)
{
	  uint32 u32RegValue = 0;
    GetRegister(spau8ReservedRamBootRomCache, u32RegValue); /* polyspace MISRA-C3:11.3 [Justified:Unset] "Necessary RAM address memory access" */	
    return (boolean)((u32RegValue & SCU_DIAG_SRCTO_CACHE) > 0);
}


/**
 * @brief    Set PLL lost lock IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetLostLockIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_LOSTLOCK);
}


/**
 * @brief    Clear PLL lost lock IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearLostLockIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_LOSTLOCK);
}


/////////////////////////
// System IRQ macros
/////////////////////////

/**
 * @brief    Set a system IRQ enable.
 * @returns    None
 */
Std_ReturnType Scu_SetIrqEnable( enum SystemIrq const keIrq, boolean const kbEnable );


/////////////////////////
// Watchdog macros
/////////////////////////

/**
 * @brief    Watchdog enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WatchdogEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_WDT_EN);
}



/**
 * @brief    Watchdog disable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WatchdogDisable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_WDT_EN);
}


/**
 * @brief    Watchdog reset enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WatchdogResetEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_WDT_RESET_EN);
}


/**
 * @brief    Watchdog reset disable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WatchdogResetDisable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_WDT_RESET_EN);
}


/**
 * @brief    Watchdog IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static void inline Scu_WatchdogIrqEnable(void) 
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_WDT);
}


/**
 * @brief    Watchdog IRQ disable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WatchdogIrqDisable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_WDT);
}


/**
 * @brief    Clear Watchdog Timer Overflow.
 * @remarks Write 1 to clear.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearWatchdogTimerOverflow(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.instat, SCU_INTSTAT_WDT);
}


/**
 * @brief    Watchdog restart.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WatchdogRestart(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.wdt, SCU_WDT_RSTRT);
}


/**
 * @brief    Set the watchdog period.
 * @param[in]  u32WatchdogPeriod The watchdogPeriod in millisecond granularity.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetWatchdogPeriod(uint32 u32WatchdogPeriodMs)
{
    stScuRegs.wdtPeriod = u32WatchdogPeriodMs & SCU_WDT_PERIOD;
}


/**
 * @brief    Get Watchdog Timer Period.
 * @returns    uint32 watchdog period in milliseconds
 */
__attribute__( ( always_inline ) ) static inline uint32 Scu_GetWatchdogPeriod(void)
{
    return stScuRegs.wdtPeriod & SCU_WDT_PERIOD;
}


/**
 * @brief    Get Watchdog Timer Overflow.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetWatchdogTimerOverflow(void)
{
    return (boolean)((stScuRegs.instat & SCU_INTSTAT_WDT) > 0);
}

/**
 * @brief    Set watchdog period and action.
 * @param[in]  ku16Period The target watchdog period
 * @param[in]  kbResetNotIrq Flag set to True for Reset action else False for IRQ action
 * @returns    E_OK on success, else E_NOT_OK upon failure
 */
Std_ReturnType Scu_SetWatchdogPeriodAndAction( uint16 const ku16Period, boolean const kbResetNotIrq );

/////////////////////////
// Interrupt enable macros
/////////////////////////

/**
 * @brief    Set VD50 terminal undervoltage IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetVd50UndervoltageIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_VD50UV);
}


/**
 * @brief    Clear VD50 terminal undervoltage IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearVd50UndervoltageIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_VD50UV);
}


/**
 * @brief    Set VD33 terminal undervoltage IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetVd33UndervoltageIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_VD33UV);
}


/**
 * @brief    Clear VD33 terminal undervoltage IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearVd33UndervoltageIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_VD33UV);
}


/**
 * @brief    Set VDIO terminal undervoltage IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetVdioUndervoltageIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_VDIOUV);
}


/**
 * @brief    Clear VDIO terminal undervoltage IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearVdioUndervoltageIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_VDIOUV);
}


/**
 * @brief    Set over temperature IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetOverTemperatureIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_OVTEMP);
}


/**
 * @brief    Clear over temperature IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearOverTemperatureIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_OVTEMP);
}


/**
 * @brief    Set Data SRAM ECC Single Error IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetDataSramEccSingleErrorIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_DSSEC);
}


/**
 * @brief    Clear Data SRAM ECC Single Error IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearDataSramEccSingleErrorIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_DSSEC);
}


/**
 * @brief    Set Data SRAM ECC Double Error IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetDataSramEccDoubleErrorIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_DSDED);
}


/**
 * @brief    Clear Data SRAM ECC Double Error IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearDataSramEccDoubleErrorIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_DSDED);
}


/**
 * @brief    Set Instruction SRAM ECC Single Error IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetInstructionSramEccSingleErrorIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_ISSEC);
}


/**
 * @brief    Clear Instruction SRAM ECC Single Error IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearInstructionSramEccSingleErrorIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_ISSEC);
}


/**
 * @brief    Set Instruction SRAM ECC Double Error IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetInstructionSramEccDoubleErrorIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_ISDED);
}


/**
 * @brief    Clear Instruction SRAM ECC Double Error IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearInstructionSramEccDoubleErrorIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_ISDED);
}


/**
 * @brief    Set VD0 over voltage IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetVd50OverVoltageIrqEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_VD50OV);
}


/**
 * @brief    Clear VD50 over voltage IRQ enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_ClearVd50OverVoltageIrqEnable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.inten, SCU_INTENABLE_VD50OV);
}

/////////////////////////
// Interrupt status macros
/////////////////////////

/**
 * @brief    Get VD50 terminal undervoltage Note: Please refer the datasheet for the threshold described as VVD50_UV.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusVd50Undervoltage(void)
{
    return (boolean)(stScuRegs.instat & SCU_INTSTAT_VD50UV);
}

 
/**
 * @brief    Get VD33 terminal undervoltage Note: Please refer the datasheet for the threshold described as VVD33_UV.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusVD33Undervoltage(void)
{
    return (boolean)((stScuRegs.instat & SCU_INTSTAT_VD33UV) > 0);
}


/**
 * @brief    Get VDIO terminal undervoltage Note: Please refer the datasheet for the threshold described as VVDIO_UV.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusVDIOUndervoltage(void)
{
    return (boolean)((stScuRegs.instat & SCU_INTSTAT_VIOUV) > 0);
}

 
/**
 * @brief    Get TOT terminal undervoltage Note: Please refer the datasheet for the threshold described as ovtemp.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusOverTemperature(void)
{
    return (boolean)((stScuRegs.instat & SCU_INTSTAT_OVTEMP) > 0);
}


/**
 * @brief    Get Data SRAM ECC Single Error.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusDataSramEccSingleError(void)
{
    return (boolean)((stScuRegs.instat & SCU_INTSTAT_DSSEC) > 0);
}


/**
 * @brief    Get Data SRAM ECC Single Error.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusDataSramEccDoubleError(void)
{
    return (boolean)((stScuRegs.instat & SCU_INTSTAT_DSDED) > 0);
}

 
/**
 * @brief    Get Instruction SRAM ECC Single Error.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusInstructionSramEccSingleError(void)
{
    return (boolean)((stScuRegs.instat & SCU_INTSTAT_ISSECT) > 0);
}

/**
 * @brief    Get Instruction SRAM ECC Single Error.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusInstructionSramEccDoubleError(void)
{
    return (boolean)((stScuRegs.instat & SCU_INTSTAT_ISDED) > 0);
}


/**
 * @brief    Get VD50 overvoltage stataus. Please refer the datasheet for the threshold described as VVD50_OV.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline boolean Scu_GetStatusVD50Overvoltage(void)
{
    return (boolean)((stScuRegs.instat & SCU_INTSTAT_VD50OV) > 0);
}


/////////////////////////
// System monitor macros
/////////////////////////

/**
 * @brief    Enable VIO undervoltage monitor.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_VioMonitorStatusEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_VIO_MONITOR);
}


/**
 * @brief    Disable VIO undervoltage monitor.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_VioMonitorStatusDisable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_VIO_MONITOR);
}


/////////////////////////
// System comms macros
/////////////////////////

/**
 * @brief    Set serial communications SCI comms mode
 * @remarks GPIO need to be configured as digital input. Please refer GPIO section.
 * @param[in]  keSciSpiInst The SCI SPI instance
 * @param[in]  keSciMode The SCI mode
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetSerialCommsSciMode(enum ScuSciInst const keSciInst, enum ScuSciMode const keSciMode)
{
    if(keSciMode == eSCU_SCI_SPI_MODE)
    {
        ClearRegisterField((volatile uint32*)&stScuRegs.ctrl, keSciInst);
    }
    else
    {
        SetRegisterField((volatile uint32*)&stScuRegs.ctrl, keSciInst);
    }
}


/**
 * @brief    Set serial communications Uart Rx pin.
 * @remarks GPIO need to be configured as digital input. Please refer GPIO section.
 * @param[in]  keSciUartInst The SCI UART instance
 * @param[in]  keUartRxInput Uart Rx input pin
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetSerialCommsUartRxPin(enum ScuUartInst const keSciUartInst, enum ScuRxInput const keUartRxInput)
{
    ClearRegisterField((SCU_BASE_ADDRESS + keSciUartInst), SCU_SELUARTRX_MASK); 
    SetRegisterField((SCU_BASE_ADDRESS + keSciUartInst), keUartRxInput);
}


/**
 * @brief    Set serial communications SPI Master MISO pin.
 * @remarks GPIO need to be configured as digital input. Please refer GPIO section.
 * @param[in]  keSciSpiInst The SCI SPI instance
 * @param[in]  keRx MISO input pin
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetSerialCommsSpiMasterMisoPin(enum ScuSpiInst const keSciSpiInst, enum ScuRxInput const keRx)
{
    ClearRegisterField((SCU_BASE_ADDRESS + keSciSpiInst),SCU_SELSPIRX_MASK); 
    SetRegisterField((SCU_BASE_ADDRESS + keSciSpiInst), keRx);
}

/**
 * @brief    Set serial communications SPI slave input pins.
 * @remarks GPIO need to be configured as digital inputs. Please refer GPIO section.
 * @param[in]  keSciSpiInst The SCI SPI instance.
 * @param[in]  keCsIn CS input
 * @param[in]  keSckIn Sck input
 * @param[in]  keRx Rx input
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_SetSerialCommsSpiSlaveInputsPins(enum ScuSpiInst const keSciSpiInst, enum ScuRxInput const keCsIn, 
                                          enum ScuRxInput const keSckIn, enum ScuRxInput const keRx)
{
    ClearRegisterField((SCU_BASE_ADDRESS + keSciSpiInst), SCU_SELSPICSIN_MASK);
    ClearRegisterField((SCU_BASE_ADDRESS + keSciSpiInst), SCU_SELSPISCKIN_MASK);
    ClearRegisterField((SCU_BASE_ADDRESS + keSciSpiInst), SCU_SELSPIRX_MASK);
    SetRegisterField((SCU_BASE_ADDRESS + keSciSpiInst), ((uint32)keCsIn) << 8);
    SetRegisterField((SCU_BASE_ADDRESS + keSciSpiInst), ((uint32)keSckIn) << 4);
    SetRegisterField((SCU_BASE_ADDRESS + keSciSpiInst), ((uint32)keRx) << 0);
}

/**
 * @brief Set the SCU register SEL_LIN
* @param[in] ku8Value  sellin value 
 */
void Scu_SetLinRegister( uint8 const ku8Value );

/**
 * @brief Get the SCU register SEL_LIN
 * @returns 8-bit sellin value; 
 */
uint8 Scu_GetLinRegister( void );

/**
 * @brief Get one of the ECC SCU registers: ECC_DSA, ECC_DSE, ECC_ISA, ECC_ISE
 * @param[in] keMiscScuReg Scu register
 * @returns value in Scu register.
 */
uint16 Scu_GetEccRegister( enum SystemMiscRegs const keMiscScuReg );

/**
 * @brief Set the Flash Control Register bit PreFetchEn to true or false
 * @param[in] bPreFetchEn True/False
 */
void Scu_SetFlashControlPreFetchEnable( boolean const bPreFetchEn );

/**
 * @brief Get the Flash Control Register bit PreFetchEn
 * @returns True/False
 */
boolean Scu_GetFlashControlPreFetchEnable( void );

/**
 * @brief Set FlashStatus flag.
 * @param[in] keStatusBit status flag to set (to clear) in Flash status reg
 */
void Scu_SetFlashStatusFlag( enum SystemFlashErrorBitMasks const keStatusBit );

/**
 * @brief Get FlashStatus flag.
 * @param[in] keStatusBit status flag to read from Flash status reg
 * @returns True/False;
 */
boolean Scu_GetFlashStatusFlag( enum SystemFlashErrorBitMasks keStatusBit );
 
/**
 * @brief Set FlashInten bit to True or False.
 * @param[in] keIntenBit interrupt enable bit to set/reset in Flash interrupt enable reg
 * @param[in] kbState True/False 
 */
void Scu_SetFlashIntenFlag( enum SystemFlashErrorBitMasks const keIntenBit, boolean const kbState );

/**
 * @brief Get FlashInten bit.
 * @param[in] keIntenBit interrupt enable bit to read from Flash interrupt enable reg
 * @returns True/False;
 */
boolean Scu_GetFlashIntenFlag( enum SystemFlashErrorBitMasks const keIntenBit);

/**
 * @brief    Get diagnostic status.
 * @param[in]  keDiagnostic The target diagnostic status to retrieve
 * @returns    8-bit status
 */
uint32 Scu_GetDiagnostic( enum Diagnostic const keDiagnostic );

/**
 * @brief    Set system IRQ handler
 * @param[in]  keIrq The target system IRQ handler to set
 * @param[in]  pfnIrqHandler The system IRQ handler function pointer
 * @param[in]  uPrData The handler context data
 * @returns    E_OK on success, else E_NOT_OK upon failure
 */
Std_ReturnType Scu_SetSystemIrqHandler( enum SystemIrq const keIrq, void (pfnIrqHandler)(void* pContext), uintptr uPrData );

/**
 * @brief    Chgeck if the IRQ is enabled
 * @param[in]  keIrq The target system IRQ handler to set
 * @param[out]  pfnIrqHandler is the IRQ enabled
 * @returns    E_OK on success, else E_NOT_OK upon failure
 */
Std_ReturnType Scu_IsIrqEnabled( enum IrqNum const keIrq, boolean* pbIrqEnabled  );
	
/**
 * @brief    The System Control NMI handler
 * @returns    None
 */
void Scu_NmiIrqHandler(void);


/**
 * @brief    The System Control Hard Fault handler
 * @returns    None
 */
void Scu_HardFaultIrqHandler(void);


/**
 * @brief    The System Control Memory Management handler
 * @returns    None
 */
void Scu_MemManageIrqHandler(void);


/**
 * @brief    The System Control Bus Fault handler
 * @returns    None
 */
void Scu_BusFaultIrqHandler(void);


/**
 * @brief    The System Control Usage Fault handler
 * @returns    None
 */
void Scu_UsageFaultIrqHandler(void);


/**
 * @brief    The System Control Flash or System fault handler
 * @remarks  Because the system and flash interrupts are on the same IRQ
 * the flash interrupts to be checked here.
 * @returns    None
 */
void Scu_FlashOrSystemFaultIrqHandler(void);


#endif // HEADER_SYSTEM_CONTROL_COMMON_H

