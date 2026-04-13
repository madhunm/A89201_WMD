#ifndef _PLATFORM_TYPES_
#define _PLATFORM_TYPES_
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Platform_Types.h
 * @author       G. Sloan
 * @version      1.0
 *
 * @brief        Platform specific types
 *
 * @copyright (c) 2025 Allegro MicroSystems, Inc. All Rights Reserved.
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
 
#include "ARMCM4_FP.h"

// All platform definitions
#define CPU_TYPE_8      8
#define CPU_TYPE_16    16
#define CPU_TYPE_32    32
#define CPU_TYPE_64    64
#define MSB_FIRST       0
#define LSB_FIRST       1
#define HIGH_BYTE_FIRST 0
#define LOW_BYTE_FIRST  1

// Symbols
#define CPU_TYPE        CPU_TYPE_32
#define CPU_BIT_ORDER   LSB_FIRST
#define CPU_BYTE_ORDER  LOW_BYTE_FIRST

// Types
typedef unsigned char       boolean;
typedef signed char         sint8;
typedef unsigned char       uint8;
typedef signed short        sint16;
typedef unsigned short      uint16;
typedef signed long         sint32;
typedef signed long long    sint64;
typedef unsigned long       uint32;
typedef unsigned long long  uint64;
typedef signed char         sint8_least;
typedef unsigned char       uint8_least;
typedef signed short        sint16_least;
typedef unsigned short      uint16_least;
typedef signed long         sint32_least;
typedef unsigned long       uint32_least;
typedef float               float32;
typedef double              float64;

typedef uint32              uintptr; // unsigned integer type that can hold a pointer address (i.e. sizeof(void*) on this platform).


#ifndef False
#define False		(boolean)0
#endif
#ifndef True
#define True		(boolean)!(False)
#endif

/**
 * @brief The platfrom address enumeration
 */
enum Address
{
    /** The start of the ROM */
    eADDRESS_ROMMIN                     = 0x00000000U,
    /** The end of the ROM */
    eADDRESS_ROMMAX                     = 0x00007FFFU,
    /** The start of the FLASH */
    eADDRESS_FLASHMIN                   = 0x08000000U,
    /** The end of the FLASH */
    eADDRESS_FLASHMAX                   = 0x0803FFFFU,
    /** The start of the instruction RAM */
    eADDRESS_ISRAMMIN                   = 0x00010000U,
    /** The end of the instruction RAM */
    eADDRESS_ISRAMMAX                   = 0x00011FFFU,
    /** The start of the data RAM */
    eADDRESS_RAMMIN                     = 0x20000000U,
    /** The end of the data RAM */
    eADDRESS_RAMMAX                     = 0x20007FFFU,
    /** The start of the debug RAM - FPGA only */
    eADDRESS_DBRAMMIN                   = 0x20008000U,
    /** The end of the debug RAM - FPGA only */
    eADDRESS_DBRAMMAX                   = 0x2000BFFFU,
    /** The start of the RAM bitband region of the ARM M4 Cortex processor */
    eADDRESS_RAMBITBANDMIN              = 0x22000000U,
    /** The end of the RAM bitband region of the ARM M4 Cortex processor */
    eADDRESS_RAMBITBANDMAX              = 0x220FFFFFU,     // ((32 * 1024) * 32) - 1

    /** The start of the peripheral memory space of the ARM M4 Cortex processor */
    eADDRESS_PERIPHERALMIN              = 0x40000000U,
    /** The end of the peripheral memory space of the ARM M4 Cortex processor */
    eADDRESS_PERIPHERALMAX              = 0x41FFFFFFU,
    /** The start of the FLASH bitband region ofeADDRESS_the ARM M4 Cortex processor */
    eADDRESS_PERIPHERALBITBANDMIN       = 0x42000000U,
    /** The end of the FLASH bitband eADDRESS_egion of the ARM M4 Cortex processor */
    eADDRESS_PERIPHERALBITBANDMAX       = 0x43FFFFFFU,
		
    /** The CAU base address */
    eADDRESS_CAU_BASE                   = 0x40000000U,
		
		/** The I2C peripheral base address */
    eADDRESS_I2C_BASE                   = 0x40001000U,
		
    /** The DAU base address */
    eADDRESS_DAU_BASE                   = 0x40002000U,
		
    /** The GDU peripheral base address */
    eADDRESS_GDU_BASE                   = eADDRESS_I2C_BASE + 0x18U,
		
    /** The GPIO peripheral base address */
    eADDRESS_GPIO_BASE                  = 0x40008000U,
		/** The GPIO peripheral blank registers base address */
		eADDRESS_GPIO_BLANK_BASE            = eADDRESS_GPIO_BASE + 0x40U,
		
    /** The Lin base address */
    eADDRESS_LIN_BASE                   = 0x4000C000U,
		
    /** The System Control peripheral base address */
    eADDRESS_SYSCTRL_BASE               = 0x4000D000U,
		
    /** The flash peripheral base address */
    eADDRESS_FLASH_BASE                 = 0x4000F000U,
    /** The flash address register */
    eADDRESS_FLASH_ADDR                 = eADDRESS_FLASH_BASE + 0x08U,
   
    /** The PWMGen base address */
    eADDRESS_PWMGEN_BASE                = 0x40010000U,
	
    /** The AMCT base address */
    eADDRESS_AMCT_BASE                  = 0x40020000U,

    /** Timer 1 base address */
    eADDRESS_TIMER1_BASE                = 0x40009100U,
    /** Timer 2 base address */
    eADDRESS_TIMER2_BASE                = 0x40009200U,
    /** Timer 3 base address */
    eADDRESS_TIMER3_BASE                = 0x40009300U,
    /** Timer 4 base address */
    eADDRESS_TIMER4_BASE                = 0x40009400U,
    /** Timer 5 base address */
    eADDRESS_TIMER5_BASE                = 0x40009500U,
    /** Timer 6 base address */
    eADDRESS_TIMER6_BASE                = 0x40009600U,
    /** Timer 7 base address */
    eADDRESS_TIMER7_BASE                = 0x40009700U,
    /** Timer 8 base address */
    eADDRESS_TIMER8_BASE                = 0x40009800U,
		
		/** The System timer base address */
    eADDRESS_SYSTEM_TIMER_BASE          = 0xE000E010U 
};

/**
 * @brief The IRQ enumeration
 */
enum IrqNum
{
    /** The non-maskable interrupt IRQ */
    eIRQNUM_NMI                        = -14,
    /** The hard fault IRQ */
    eIRQNUM_HARD_FAULT                 = -13,
    /** The memory management IRQ */
    eIRQNUM_MEMORY_MANAGEMENT          = -12,
    /** The bus fault IRQ */
    eIRQNUM_BUS_FAULT                  = -11,
    /** The usage fault IRQ */
    eIRQNUM_USAGE_FAULT                = -10,
    /** The system timer IRQ */
    eIRQNUM_SYSTEM_TIMER               = -1,
    /** The current acquisition unit IRQ */
    eIRQNUM_CAU                        = 0,
    /** The data acquisition unit IRQ */
    eIRQNUM_DAU                        = 1,
    /** The timer 1 IRQ */
    eIRQNUM_TIMER1                     = 2,
    /** The timer 2 IRQ */
    eIRQNUM_TIMER2                     = 3,
    /** The timer 3 IRQ */
    eIRQNUM_TIMER3                     = 4,
    /** The timer 4 IRQ */
    eIRQNUM_TIMER4                     = 5,
    /** The timer 5 IRQ */
    eIRQNUM_TIMER5                     = 6,
    /** The timer 6 IRQ */
    eIRQNUM_TIMER6                     = 7,
    /** The timer 7 IRQ */
    eIRQNUM_TIMER7                     = 8,
    /** The timer 8 IRQ */
    eIRQNUM_TIMER8                     = 9,
    /** The SCI 0 IRQ, UART 0 / SPI 0 */
    eIRQNUM_SCI0                       = 10,
    /** The SCI 1 IRQ, UART 1 / SPI 1 */
    eIRQNUM_SCI1                       = 11,
    /** The LIN IRQ */
    eIRQNUM_LIN                        = 12,
    /** The GDU IRQ */
    eIRQNUM_GDU                        = 13,
    /** The I2C IRQ */
    eIRQNUM_I2C                        = 14,
    /** The GPIO IRQ */
    eIRQNUM_GPIO                       = 16,
    /** The AMCT IRQ */
    eIRQNUM_AMCT                       = 17,
    /** The DMA IRQ */
    eIRQNUM_DMA                        = 18,

    // Flash interrupts set to only be generated upon an error
    // System IRQ on same line
    eIRQNUM_FLASH_OR_SYSTEM_FAULT      = 19
};

/**
 * @brief The flash page size
 */
static const uint16 gku16FlashPageSize = 4096;

/**
 * @brief The maximum number of supported GPIO pins. 
 */
static const uint32 gku32MaxNumberOfGpioPinsSupported = 16U;

/**
 * @brief The maximum number of supported GPIO pins. 
 */
static const uint32 gku32MaxNumGpioPins = 16U;

/**
 * @brief The maximum number of supported SCI ports. 
 */
static const uint8 gku8MaxNumSciPorts = 2U;

typedef IRQn_Type IrqType_t;
#define ARM_IRQ_NUM( value ) ((IrqType_t)( value ))


#endif // _PLATFORM_TYPES_