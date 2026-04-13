/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @internal
 *
 * @file         SystemControl.c
 *
 * @copyright (c) 2024 Allegro MicroSystems, Inc. All Rights Reserved.
 * The information contained in this file is confidential and proprietary.
 *
 * The information contained in this file is governed by Software License
 * acknowledged during the download from Allegro’s Software Download
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
 */
 /* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
#include "SystemControl.h"

// NMI
static void (*spclsNmiIrqHandler)(void* pContext) = NULLPTR;
static uintptr suPrNmiData;
static IrqType_t const skIrqNmi = ARM_IRQ_NUM(eIRQNUM_NMI);

// Hard Fault
static void (*spclsHardFaultIrqHandler)(void* pContext) = NULLPTR;
static uintptr suPrHardFaultData;
static IrqType_t const skIrqHardFault = ARM_IRQ_NUM(eIRQNUM_HARD_FAULT);

// Memory Management Fault
static void (*spclsMemoryManagementFaultIrqHandler)(void* pContext) = NULLPTR;
static uintptr suPrMemoryManagementFaultData;
static IrqType_t const skIrqMemoryManagementFault = ARM_IRQ_NUM(eIRQNUM_MEMORY_MANAGEMENT);

// Bus Fault
static void (*spclsBusFaultIrqHandler)(void* pContext) = NULLPTR;
static uintptr suPrBusFaultData;
static IrqType_t const skIrqBusFault = ARM_IRQ_NUM(eIRQNUM_BUS_FAULT);

// Usage Fault
static void (*spclsUsageFaultIrqHandler)(void* pContext) = NULLPTR;
static uintptr suPrUsageFaultData;
static IrqType_t const skIrqUsageFault = ARM_IRQ_NUM(eIRQNUM_USAGE_FAULT);

// Flash or System Fault
static void (*spclsFlashOrSystemFaultIrqHandler)(void* pContext) = NULLPTR;
static uintptr suPrFlashOrSystemFaultData;
static IrqType_t const skIrqFlashOrSystemFault = ARM_IRQ_NUM(eIRQNUM_FLASH_OR_SYSTEM_FAULT);

static uint8 const sku8MaxNumberSystemIrqs = 0x6U;
// Array of IRQ values
static const IRQn_Type skaIrqFlashOrSystemFault[sku8MaxNumberSystemIrqs] = {
    skIrqNmi,
    skIrqHardFault,
    skIrqMemoryManagementFault,
    skIrqBusFault,
    skIrqUsageFault,
    skIrqFlashOrSystemFault
};


/**
 * @internal
 * @details Set the watchdog period and the action to take upon expiry
 */
Std_ReturnType Scu_SetWatchdogPeriodAndAction( uint16 const ku16Period, boolean const kbResetNotIrq )
{
    Std_ReturnType u8Status = E_OK;
    // Write the 11 bit period value
    Scu_SetWatchdogPeriod(ku16Period);

    // Set the mode of operation to the reset enable bit
    if( kbResetNotIrq == True)
    {
        Scu_WatchdogResetEnable();
    }
    else
    {
        Scu_WatchdogResetDisable();
    }
    // Clear the watchog timer overflow status bit
    Scu_ClearWatchdogTimerOverflow();
    
    // Enable or disable the IRQ
    if( kbResetNotIrq == False)
    {
        Scu_WatchdogIrqEnable();
    }
    else
    {
        Scu_WatchdogIrqDisable();
    }
  
    // If the IRQ is enabled the system IRQ will need to be enabled too
    if ( kbResetNotIrq == False )
    {
        u8Status = Scu_SetIrqEnable( eSYSTEMIRQ_FLASH_OR_SYSTEM_FAULT, True );
    }
	return u8Status;
}

void Scu_SetLinRegister( uint8 const ku8Value )
{
    stScuRegs.selLin = (uint32)ku8Value;
}

uint8 Scu_GetLinRegister( void )
{
    return (uint8)stScuRegs.selLin;
}

uint16 Scu_GetEccRegister( enum SystemMiscRegs const keMiscScuReg )
{
    if( ( keMiscScuReg == eSCU_ECC_DSA ) || 
        ( keMiscScuReg == eSCU_ECC_DSE ) ||
        ( keMiscScuReg == eSCU_ECC_ISA ) || 
        ( keMiscScuReg == eSCU_ECC_ISE ) )
    {
        volatile uint32* const pu32ScuReg = (volatile uint32*)&stScuRegs + (uint32)keMiscScuReg;        // Test note: this pointer should only be in the range = 0x4000D020 to 0x4000D02C
        return (uint16)*pu32ScuReg;
    }
    else
    {
        return (uint16)eSCU_INVALID;
    }
}

void Scu_SetFlashControlPreFetchEnable( boolean const bPreFetchEn )
{
    if( bPreFetchEn == True )
    {
        stScuFlashRegs.flashctrl |= (uint32)ePREFETCH_ENABLE;  
    }
    else
    {
        stScuFlashRegs.flashctrl &= (uint32)ePREFETCH_DISABLE;   
    }
}

boolean Scu_GetFlashControlPreFetchEnable( void )
{
    if( ( (stScuFlashRegs.flashctrl) & (uint32)ePREFETCH_ENABLE) != 0 )
    {
        return True;
    }
    else
    {
        return False;
    }
}

void Scu_SetFlashStatusFlag( enum SystemFlashErrorBitMasks const keStatusBit )
{
    stScuFlashRegs.flashintstat |= (uint32)keStatusBit;
}

boolean Scu_GetFlashStatusFlag( enum SystemFlashErrorBitMasks keStatusBit )
{
    if( ( (stScuFlashRegs.flashintstat) & (uint32)keStatusBit) != 0 )
    {
        return True;
    }
    else
    {
        return False;
    }
}

void Scu_SetFlashIntenFlag( enum SystemFlashErrorBitMasks const keIntenBit, boolean const kbState )
{
    if( kbState == True )
    {
        stScuFlashRegs.flashinten |= (uint32)(keIntenBit);
    }
    else
    {
        uint8 const u8InvIntenBit = ~(uint8)(keIntenBit);
        stScuFlashRegs.flashinten &= (uint32)u8InvIntenBit;
    }
}

boolean Scu_GetFlashIntenFlag( enum SystemFlashErrorBitMasks const keIntenBit)
{
    if( ( (stScuFlashRegs.flashinten) & (uint32)keIntenBit ) != 0 ) 
    {
        return True;
    }
    else
    {
        return False;
    }
}

/**
 * @internal
 * @details Diagnostic register is used unless we are reading one of the 
 * Boot ROM cached status bits in reserved RAM.
 * These are the RAM cached status bits used for last reset reason:
 * spau8ReservedRamBootRomCache[0] 7:5 is PLL status
 * spau8ReservedRamBootRomCache[0] 4:0 is reset status
 * Bitmaks for either register or cache are encoded into the Diagnostic enum for efficiency.
 */
uint32 Scu_GetDiagnostic( enum Diagnostic const keDiagnostic )
{
    uint32 u32Value;

    switch ( keDiagnostic )
    {
    case eDIAGNOSTIC_POR:
    case eDIAGNOSTIC_SYS:
    case eDIAGNOSTIC_WDT:
    case eDIAGNOSTIC_LOCKUP:
    case eDIAGNOSTIC_PLLSRCTIMEOUT:
    case eDIAGNOSTIC_PLLTIMEOUT:
    {
        uint32 u32RegValue = 0;
        GetRegister(spau8ReservedRamBootRomCache, u32RegValue); /* polyspace MISRA-C3:11.3 [Justified:Unset] "Necessary RAM address memory access" */
        u32Value = u32RegValue & (uint32)keDiagnostic;
    }
    break;

    default:
        u32Value = stScuRegs.diag & (SCU_DIAG_PCFG_REG | SCU_DIAG_PLLOK_REG);
        break;
    }

    return u32Value;
}


Std_ReturnType Scu_SetIrqEnable( enum SystemIrq const keIrq, boolean const kbEnable )
{
    Std_ReturnType u8Status = E_OK;
    // A number of IRQ type
    IRQn_Type irqNumber;
    // Flag if IRQ is in valid range
    boolean bValidNumber = False;

    switch ( keIrq )
    {
    case eSYSTEMIRQ_NMI:
    case eSYSTEMIRQ_HARD_FAULT:
    case eSYSTEMIRQ_MEMORY_MANAGEMENT:
    case eSYSTEMIRQ_BUS_FAULT:
    case eSYSTEMIRQ_USAGE_FAULT:
    case eSYSTEMIRQ_FLASH_OR_SYSTEM_FAULT:
        bValidNumber = True;
        irqNumber = skaIrqFlashOrSystemFault[((uint8)keIrq)];
        break;

    default:
        u8Status = E_NOT_OK;
        break;
    }

    if ( bValidNumber == True )
    {
        if ( kbEnable == True )
        {
            NVIC_ClearPendingIRQ( irqNumber );
            NVIC_EnableIRQ( irqNumber );
        }
        else
        {
            NVIC_DisableIRQ( irqNumber );
        }
    }
    return u8Status;
}


Std_ReturnType Scu_IsIrqEnabled( enum IrqNum const keIrq, boolean* pbIrqEnabled  )
{
    Std_ReturnType u8Status = E_OK;
    // Target IRQ selected
    IRQn_Type irqNumber;
  
    if(pbIrqEnabled != NULLPTR)
    {
        switch ( keIrq )
        {
        case eIRQNUM_NMI:
        case eIRQNUM_HARD_FAULT:
        case eIRQNUM_MEMORY_MANAGEMENT:
        case eIRQNUM_BUS_FAULT:
        case eIRQNUM_USAGE_FAULT:
        case eIRQNUM_FLASH_OR_SYSTEM_FAULT:
            irqNumber = skaIrqFlashOrSystemFault[((uint8)((uint32)keIrq & (uint32)(sku8MaxNumberSystemIrqs-1)))];
            break;

        default:
            *pbIrqEnabled = False;
            u8Status = E_NOT_OK;
            break;
        }
        
        if ( u8Status == E_OK )
        {
            *pbIrqEnabled = (boolean)NVIC_GetEnableIRQ ( irqNumber );
        }
    }
    else
    {
      u8Status = E_NOT_OK;
    }
    return u8Status;
}


Std_ReturnType Scu_SetSystemIrqHandler( enum SystemIrq const keIrq, void (pfnIrqHandler)(void* pContext), uintptr uPrData )
{
    Std_ReturnType u8Status = E_OK;
    switch ( keIrq )
    {
    case eSYSTEMIRQ_NMI:
        spclsNmiIrqHandler = *pfnIrqHandler;
        suPrNmiData = uPrData;
        break;

    case eSYSTEMIRQ_HARD_FAULT:
        spclsHardFaultIrqHandler = *pfnIrqHandler;
        suPrHardFaultData = uPrData;
        break;

    case eSYSTEMIRQ_MEMORY_MANAGEMENT:
        spclsMemoryManagementFaultIrqHandler = *pfnIrqHandler;
        suPrMemoryManagementFaultData = uPrData;
        break;

    case eSYSTEMIRQ_BUS_FAULT:
        spclsBusFaultIrqHandler = *pfnIrqHandler;
        suPrBusFaultData = uPrData;
        break;

    case eSYSTEMIRQ_USAGE_FAULT:
        spclsUsageFaultIrqHandler = *pfnIrqHandler;
        suPrUsageFaultData = uPrData;
        break;

    case eSYSTEMIRQ_FLASH_OR_SYSTEM_FAULT:
        spclsFlashOrSystemFaultIrqHandler = *pfnIrqHandler;
        suPrFlashOrSystemFaultData = uPrData;
        break;

    default:
        u8Status = E_NOT_OK;
        break;
    }
    return u8Status;
}


__attribute__( ( interrupt( "IRQ" ) ) ) void Scu_NmiIrqHandler(void)
{
    NVIC_DisableIRQ( skIrqNmi );
    NVIC_ClearPendingIRQ( skIrqNmi );

    if ( spclsNmiIrqHandler != NULLPTR )
    {
        spclsNmiIrqHandler( (void*)&suPrNmiData ); /* polyspace MISRA-C3:11.6 [Justified:Unset] "" */
    }

    NVIC_EnableIRQ( skIrqNmi );
}

__attribute__( ( interrupt( "IRQ" ) ) ) void Scu_HardFaultIrqHandler(void)
{
    NVIC_DisableIRQ( skIrqHardFault );
    NVIC_ClearPendingIRQ( skIrqHardFault );

    if ( spclsHardFaultIrqHandler != NULLPTR )
    {
        spclsHardFaultIrqHandler( (void*)&suPrHardFaultData );
    }

    NVIC_EnableIRQ( skIrqHardFault );
}

__attribute__( ( interrupt( "IRQ" ) ) ) void Scu_MemManageIrqHandler(void)
{
    NVIC_DisableIRQ( skIrqMemoryManagementFault );
    NVIC_ClearPendingIRQ( skIrqMemoryManagementFault );

    if ( spclsMemoryManagementFaultIrqHandler != NULLPTR )
    {
        spclsMemoryManagementFaultIrqHandler( (void*)&suPrMemoryManagementFaultData );
    }

    NVIC_EnableIRQ( skIrqMemoryManagementFault );
}

__attribute__( ( interrupt( "IRQ" ) ) )  void Scu_BusFaultIrqHandler(void)
{
    NVIC_DisableIRQ( skIrqBusFault );
    NVIC_ClearPendingIRQ( skIrqBusFault );


    if ( spclsBusFaultIrqHandler != NULLPTR )
    {
        spclsBusFaultIrqHandler( (void*)&suPrBusFaultData );
    }

    NVIC_EnableIRQ( skIrqBusFault );
}

__attribute__( ( interrupt( "IRQ" ) ) ) void Scu_UsageFaultIrqHandler()
{
    NVIC_DisableIRQ( skIrqUsageFault );
    NVIC_ClearPendingIRQ( skIrqUsageFault );

    if ( spclsUsageFaultIrqHandler != NULLPTR )
    {
        spclsUsageFaultIrqHandler( (void*)&suPrUsageFaultData );
    }

    NVIC_EnableIRQ( skIrqUsageFault );
}

/**
 * @internal
 * @details   Because the system and flash interrupts are using a shared IRQ
 * the flash interrupts to be checked here. Watchdog uses this IRQ line
 * when its action mode is set to IRQ (and not reset) mode.
 */
__attribute__( ( interrupt( "IRQ" ) ) ) void Scu_FlashOrSystemFaultIrqHandler()
{
    NVIC_DisableIRQ( skIrqFlashOrSystemFault );
    NVIC_ClearPendingIRQ( skIrqFlashOrSystemFault );

    if ( spclsFlashOrSystemFaultIrqHandler != NULLPTR )
    {
        spclsFlashOrSystemFaultIrqHandler( (void*)&suPrFlashOrSystemFaultData );
    }

    NVIC_EnableIRQ( skIrqFlashOrSystemFault );
}
