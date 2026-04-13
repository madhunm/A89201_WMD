/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Gpio.c
 * @author       Guy Sloan
 * @version      1.1
 *
 * @brief        Gpio implementation.
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
#include "A89201RegisterDefinitions.h"
#include "SystemControl.h"
#include "Gpio.h"


static uint8 const sku8ValidPd0To15Mask = 0xFU;
static uint32 const sku32WordClear = 0xFFFFFFFFU;
static uint16 const ku16ShortCircuitDetectionTime = 200U;
static uint32 const ku32EvaluationRegisterBaseAddress = GPIO_BASE_ADDRESS + GPIO_USRC0;
static IrqType_t const skIrqGpio = ARM_IRQ_NUM(eIRQNUM_GPIO);
/**
 * @internal
 * @brief A mapping of Gpio pins to blanking register numbers.
 */
static const uint8 ku8BlankingRegisterTable[gku32MaxNumGpioPins] = { 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };

/**
 * @internal
 * @brief A mapping of Gpio pins to bit masks.
 */
static const uint16  keBlankBitmaskTable[gku32MaxNumGpioPins] = 
{
    BLANK3_0_MASK_SET, BLANK7_4_MASK_SET, BLANK11_8_MASK_SET, BLANK12_15_MASK_SET,
    BLANK3_0_MASK_SET, BLANK7_4_MASK_SET, BLANK11_8_MASK_SET, BLANK12_15_MASK_SET,
    BLANK3_0_MASK_SET, BLANK7_4_MASK_SET, BLANK11_8_MASK_SET, BLANK12_15_MASK_SET,
    BLANK3_0_MASK_SET, BLANK7_4_MASK_SET, BLANK11_8_MASK_SET, BLANK12_15_MASK_SET,
};

/**
 * @internal
 * @brief A mapping of Gpio pins to bit positions.
 */
static const uint16  keBlankBitposTable[gku32MaxNumGpioPins] = 
{
    BLANK3_0_BITPOS, BLANK7_4_BITPOS, BLANK11_8_BITPOS, BLANK12_15_BITPOS,
    BLANK3_0_BITPOS, BLANK7_4_BITPOS, BLANK11_8_BITPOS, BLANK12_15_BITPOS,
    BLANK3_0_BITPOS, BLANK7_4_BITPOS, BLANK11_8_BITPOS, BLANK12_15_BITPOS,
    BLANK3_0_BITPOS, BLANK7_4_BITPOS, BLANK11_8_BITPOS, BLANK12_15_BITPOS,
};


typedef struct __attribute__((packed, aligned(4))) _Gpio 
{         
    /* Function pointer for use by GPIO Irq */
    void (*m_fnIrqHandler)(void* pstContext);
    
    /* Irq handler context */
    GpioIrqContext_t stGpioIrqContext;
    
    /** The pin number used by the GPIO object */
    uint8 m_u8PinNum;
} Gpio_t;

static Gpio_t stGpio[gku32MaxNumGpioPins] = { 
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
    {NULLPTR, {}, s_ku8InvalidPinNum},
};

static const uint32 u32ArrBitmaskFromPinNumLut[gku32MaxNumGpioPins] =
{ 
  0x1U,
  0x2U,
  0x4U,
  0x8U,
  0x10U,
  0x20U,
  0x40U,
  0x80U,
  0x100U,
  0x200U,
  0x400U,
  0x800U,
  0x1000U,
  0x2000U,
  0x4000U,
  0x8000U
};



uint8 Gpio_GetPinNum(enum GpioInstanceId const keGpioInstanceId)
{
    return stGpio[keGpioInstanceId].m_u8PinNum;
}


Std_ReturnType Gpio_SetMode(enum GpioInstanceId const keGpioInstanceId, enum GpioMode const keMode)
{
    Std_ReturnType u8Status = E_OK;
    if ( stGpio[keGpioInstanceId].m_u8PinNum  != s_ku8InvalidPinNum )
    {
        uint8 u8Mode;

        switch ( keMode )
        {
        case eGPIO_MODE_DISABLED:
        case eGPIO_MODE_DIGITAL_INPUT:
        case eGPIO_MODE_DIGITAL_INPUT_PULLUP:
        case eGPIO_MODE_DIGITAL_INPUT_PULLDOWN:
        case eGPIO_MODE_ANALOG_INPUT_SOURCE:
        case eGPIO_MODE_OUTPUT_PUSH_PULL:
        case eGPIO_MODE_OUTPUT_PULLDOWN:
        case eGPIO_MODE_OUTPUT_PULLUP:
            u8Mode = (uint8)keMode;
            break;

        default:
            u8Mode = ku8Invalid;
            u8Status = E_NOT_OK;
            break;
        }

        if ( u8Mode != ku8Invalid )
        {
            uint32 u32Addr;
            GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
            ReadModifyWriteField(u32Addr, CONTROL_MODE_MASK_SET, CONTROL_MODE_BITPOS, u8Mode);
        }
    }
    else
    {
        u8Status = E_NOT_OK;
    }
    
    return u8Status;
}


Std_ReturnType Gpio_GetMode(enum GpioInstanceId const keGpioInstanceId, enum GpioMode* peGpioMode)
{
    Std_ReturnType u8Status = E_OK;
  
    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {

        if(peGpioMode != NULLPTR)
        {
            uint32 u32Addr;
            GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
      
            uint32 u32RegData;
            GetRegister(u32Addr, u32RegData);
            u32RegData = (u32RegData & CONTROL_MODE_MASK_SET) >> CONTROL_MODE_BITPOS;
            *peGpioMode = (enum GpioMode)u32RegData;
        }
        else
        {
            u8Status = E_NOT_OK;
        }
    }
    else
    {
        if(peGpioMode != NULLPTR)
        {
            *peGpioMode = eGPIO_MODE_DISABLED;
        }
        u8Status = E_NOT_OK;
    }
    return u8Status;

}


Std_ReturnType Gpio_SetRxPinPort( enum GpioInstanceId const keGpioInstanceId, uint8 const ku8PortNum, uint8 const ku8RxPinNum )
{
    Std_ReturnType u8Status = E_OK;
  
    if ( (stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum) && (ku8RxPinNum <= gku32MaxNumGpioPins))
    {
        // Configure RX pin
        if ( ku8PortNum == 0U )
        {
            Scu_SetSerialCommsSciMode(eSCU_SCI_1, eSCU_UART_MODE);
            Scu_SetSerialCommsUartRxPin(eSCU_UART_1, (enum ScuRxInput const)ku8RxPinNum);
        }
        else if ( ku8PortNum == 1U )
        {
            Scu_SetSerialCommsSciMode(eSCU_SCI_2, eSCU_UART_MODE);
            Scu_SetSerialCommsUartRxPin(eSCU_UART_2, (enum ScuRxInput const)ku8RxPinNum);
        }
        else
        {
            u8Status = E_NOT_OK;
        }
    }
    else
    {
        u8Status = E_NOT_OK;
    }
    return u8Status;
}


Std_ReturnType Gpio_SetSource(enum GpioInstanceId const keGpioInstanceId, enum GpioSource const keSource )
{
    Std_ReturnType u8Status = E_OK;

    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum)
    {
        // Enable or disable the evaluation mux.
        // This is the FOSEL controlled mux on the output so if its enabled the other settings do not matter.  
        uint32 const ku32SourceAddress = ku32EvaluationRegisterBaseAddress + (stGpio[keGpioInstanceId].m_u8PinNum * sizeof(uint32));
        boolean const kbIsEvalModeActive = (const boolean)((keSource >= eGPIO_ESRC0 ) && ( keSource <= eGPIO_ESRC7 ));
        ReadModifyWriteField(ku32SourceAddress, EVAL_MODE_MASK_SET, EVAL_MODE_BITPOS, kbIsEvalModeActive);

        if ( kbIsEvalModeActive == True )
        {
            // The evaluation source is encoded in the bottom 3 bits of the enumeration.
            uint32 const ku32Source = ((uint32)keSource) & ((uint32)s_ku8EvalSourceMask);
            ReadModifyWriteField(ku32SourceAddress, EVAL_SOURCE_MASK_SET, EVAL_SOURCE_BITPOS, ku32Source);
        }
        else
        {
            // Multiplexer 1 is the Application Output Selection (AOSEL) bit[13]
            // Set multiplexer 1 to use DOUT as the source
            if ( keSource == eGPIO_SOURCE_DOUT )
            {
                uint32 u32Addr;
                GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
                ReadModifyWriteField(u32Addr, CONTROL_MUX1_MASK_SET, CONTROL_MUX1_BITPOS, 0);
            }
            else
            {
                if ( keSource <= eGPIO_ASRC15 )
                {
                    uint32 u32Addr;
                    GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
                    // Set multiplexer 1 to use multiplexer 0 as the source
                    ReadModifyWriteField(u32Addr, CONTROL_MUX1_MASK_SET, CONTROL_MUX1_BITPOS, 1U);
                    // Set multiplexer 0 to select the current source
                    // Multiplexer 0 is the Application Source (ASRC) Selection bits[7:4]
                    ReadModifyWriteField(u32Addr, CONTROL_MUX0_MASK_SET, CONTROL_MUX0_BITPOS, keSource); 
                }
                else
                {
                    u8Status = E_NOT_OK;
                }
            }
        }
    }
    else
    {
        u8Status = E_NOT_OK;
    }
    return u8Status;
}


Std_ReturnType Gpio_GetSource(enum GpioInstanceId const keGpioInstanceId, enum GpioSource* const peGpioSource )
{
    Std_ReturnType u8Status = E_OK;


    if ( (stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum) && (peGpioSource != NULLPTR))
    {
        // Read the application evaluation mux.
        // This is the last mux on the output so if its enabled the other settings do not matter.
        const uint32 ku32SourceAddress = ku32EvaluationRegisterBaseAddress + 
              (uint32)( stGpio[keGpioInstanceId].m_u8PinNum * sizeof(uint32) );

        uint32 u32RegData;
        ReadField(ku32SourceAddress, EVAL_MODE_MASK_SET, EVAL_MODE_BITPOS, u32RegData);
        const boolean kbIsEvalModeActive = (const boolean)u32RegData & EVAL_MODE_MASK_SET;
        
        if ( kbIsEvalModeActive == True )
        {
            ReadField(ku32SourceAddress, EVAL_SOURCE_MASK_SET, EVAL_SOURCE_BITPOS, u32RegData);
            uint32 const ku32TempSource = u32RegData | 0x80U;
            *peGpioSource = (enum GpioSource)ku32TempSource;
        }
        else
        {
            uint32 u32Addr;
            GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
            ReadField(u32Addr, CONTROL_MUX1_MASK_SET, CONTROL_MUX1_BITPOS, u32RegData);
            const uint8 ku8Mux1 = (uint8)u32RegData;
            
            if ( ku8Mux1 == 0U )
            {
                *peGpioSource = eGPIO_SOURCE_DOUT;
            }
            else
            {
                // Multiplexer 0 is the current source
                // Read the register to fund which source is selected
                ReadField(u32Addr, CONTROL_MUX0_MASK_SET, CONTROL_MUX0_BITPOS, u32RegData);
                *peGpioSource = (enum GpioSource)((uint8)u32RegData);

            }
        }
    }
    else
    {
        if(peGpioSource != NULLPTR)
        {
            *peGpioSource = eGPIO_ASRC_NONE;
        }
        u8Status = E_NOT_OK;
    }

    return u8Status;
}


Std_ReturnType Gpio_SetDriveBoost(enum GpioInstanceId const keGpioInstanceId, const boolean kbBoostEnabled )
{
    Std_ReturnType u8Status = E_OK;

    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum)
    {
        uint32 u32Addr;
        GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
        ReadModifyWriteField(u32Addr, CONTROL_DRIVE_MASK_SET, CONTROL_DRIVE_BITPOS, kbBoostEnabled );  
    }
    else
    {
        u8Status = E_NOT_OK;
    }
    return u8Status;
}


Std_ReturnType Gpio_IsDriveBoostEnabled(enum GpioInstanceId const keGpioInstanceId, boolean* const kpbEnabled)
{
    Std_ReturnType u8Status = E_OK;

    if ( (stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum) && (kpbEnabled != NULLPTR) )
    {
        uint32 u32Addr;
        uint32 u32RegData;
        GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
        ReadField(u32Addr, CONTROL_DRIVE_MASK_SET, CONTROL_DRIVE_BITPOS, u32RegData );
        *kpbEnabled = (boolean)u32RegData;
    }
    else
    {
        if(kpbEnabled != NULLPTR)
        {
            *kpbEnabled = False;
        }
        u8Status = E_NOT_OK;
    }
    return u8Status;
}


boolean Gpio_IsSet(enum GpioInstanceId const keGpioInstanceId)
{
    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {
        uint8 const ku8MaskedPinNum = (stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask;
        if ( (stGpioRegs.Pdout & (u32ArrBitmaskFromPinNumLut[ku8MaskedPinNum])) > 0)
        {
            return True;
        }
        else
        {
            return False;
        }
    }
    else
    {
        return False;
    }
}


void Gpio_Set(enum GpioInstanceId const keGpioInstanceId)
{
    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {
        uint8 const ku8MaskedPinNum = (stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask;
        SetRegisterField((volatile uint32*)&stGpioRegs.Pdout, (u32ArrBitmaskFromPinNumLut[ku8MaskedPinNum]));
    }
}



void Gpio_Clear(enum GpioInstanceId const keGpioInstanceId)
{
    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {
        uint8 const ku8MaskedPinNum = (stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask;
        ClearRegisterField((volatile uint32*)&stGpioRegs.Pdout, (u32ArrBitmaskFromPinNumLut[ku8MaskedPinNum]));
    }
}


boolean Gpio_Read(enum GpioInstanceId const keGpioInstanceId) 
{  
    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {
        uint8 const ku8MaskedPinNum = (stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask;
        if ( (stGpioRegs.Pdin & (u32ArrBitmaskFromPinNumLut[ku8MaskedPinNum])) > 0)
        {
            return True;
        }
        else
        {
            return False;
        }  
    }
    else
    {
        return False;
    }
}


Std_ReturnType Gpio_SetIrqHandler(enum GpioInstanceId const keGpioInstanceId, void (pfGpioIrqHandler)(void* pstContext) )
{  
    Std_ReturnType u8Status = E_OK;
  
    if (stGpio[keGpioInstanceId].m_u8PinNum == s_ku8InvalidPinNum)
    {
        u8Status = E_NOT_OK;
    }
    else
    {
        stGpio[keGpioInstanceId].m_fnIrqHandler = *pfGpioIrqHandler;
        NVIC_EnableIRQ( skIrqGpio );
    }
    return u8Status;
}


boolean Gpio_HasIrqHandler(enum GpioInstanceId const keGpioInstanceId)
{
    return (boolean)(stGpio[keGpioInstanceId].m_fnIrqHandler != NULLPTR);
}


Std_ReturnType Gpio_SetIrqTrigger(enum GpioInstanceId const keGpioInstanceId, enum GpioIrqTrigger const keIrqTrigger )
{
    Std_ReturnType u8Status = E_OK;

    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {
        // Disable GPIO interrupts
        NVIC_DisableIRQ( skIrqGpio );

        uint8 u8Trigger;

        // Set the trigger type
        switch ( keIrqTrigger )
        {
        case eGPIO_IRQ_TRIGGER_EDGE_RISING:
            u8Trigger = 0U;
            break;

        case eGPIO_IRQ_TRIGGER_EDGE_FALLING:
            u8Trigger = 1U;
            break;

        case eGPIO_IRQ_TRIGGER_EDGE_BOTH:
            u8Trigger = 2U;
            break;

        case eGPIO_IRQ_TRIGGER_LEVEL_HIGH:
            u8Trigger = 3U;
            break;

        case eGPIO_IRQ_TRIGGER_LEVEL_LOW:
            u8Trigger = 4U;
            break;

        default:
            u8Trigger = ku8Invalid;
            u8Status = E_NOT_OK;
            break;
        }

        if ( u8Trigger != ku8Invalid )
        {          
            uint32 u32Addr;
            GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
            ReadModifyWriteField(u32Addr, CONTROL_IRQCFG_MASK_SET, CONTROL_IRQCFG_BITPOS, (uint32)u8Trigger ); 
        }

        // Enable GPIO interrupts
        NVIC_EnableIRQ( skIrqGpio );
    }
    else
    {
      u8Status = E_NOT_OK;
    }
    return u8Status;
}


enum GpioIrqTrigger Gpio_GetIrqTrigger(enum GpioInstanceId const keGpioInstanceId)
{
    enum GpioIrqTrigger eIrqTrigger;

    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {
        uint32 u32RegData;
        uint32 u32Addr;
        GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
        ReadField(u32Addr, CONTROL_IRQCFG_MASK_SET, CONTROL_IRQCFG_BITPOS, u32RegData);
        const uint8 ku8IrqTrigger = (uint8)u32RegData;
        
        // There is no NONE source option so add 1 to map to actual enumerations
        eIrqTrigger = (enum GpioIrqTrigger)((uint8)(ku8IrqTrigger + 1U));
    }
    else
    {
        
        eIrqTrigger = eGPIO_IRQ_TRIGGER_NONE;
    }
    return eIrqTrigger;
}


Std_ReturnType Gpio_SetTriggerIrqEnable(enum GpioInstanceId const keGpioInstanceId, const boolean kbEnable )
{
    Std_ReturnType u8Status = E_OK;

    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {
        uint32 u32Addr;
        GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
        if(kbEnable == True)
        {
            SetRegisterField(u32Addr, CONTROL_IRQEN_MASK_SET);
        }
        else
        {
            ClearRegisterField(u32Addr, CONTROL_IRQEN_MASK_SET);
        }
    }
    else
    {
        u8Status = E_NOT_OK;
    }
    return u8Status;
}


boolean Gpio_IsTriggerIrqEnabled(enum GpioInstanceId const keGpioInstanceId)
{
    uint32 u32Addr;
    uint32 u32Data;
    GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
    GetRegister(u32Addr, u32Data);
    
    if ( (u32Data & CONTROL_IRQEN_MASK_SET) > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}


boolean Gpio_IsTriggerIrqSet(enum GpioInstanceId const keGpioInstanceId)
{ 
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {
        if ( (stGpioRegs.EventStatus & (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask])) > 0)
        {
            return True;
        }
    }
    return False;
}


void Gpio_ClearTriggerIrq(enum GpioInstanceId const keGpioInstanceId)
{
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {
        SetRegisterField((volatile uint32*)&stGpioRegs.EventStatus, (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask]));
    }
}


void Gpio_SetShortToGroundIrqEnable(enum GpioInstanceId const keGpioInstanceId, const boolean kbEnable )
{  
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {
        if(kbEnable == True)
        {
            SetRegisterField((volatile uint32*)&stGpioRegs.StgInten, (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask]));
        }
        else
        {
            ClearRegisterField((volatile uint32*)&stGpioRegs.StgInten, (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask]));
        }
    }
}


boolean Gpio_IsShortToGroundIrqEnabled(enum GpioInstanceId const keGpioInstanceId)
{
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {
        if ( (stGpioRegs.StgInten & (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask])) > 0)
        {
            return True;
        }
    }
    return False;
}


boolean Gpio_IsShortToGroundIrqSet(enum GpioInstanceId const keGpioInstanceId)
{
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {
        if(( stGpioRegs.Stg & (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask])) > 0)
        {
            return True;
        }
    }
    return False;
}


void Gpio_ClearShortToGroundIrq(enum GpioInstanceId const keGpioInstanceId)
{
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {
        SetRegisterField((volatile uint32*)&stGpioRegs.Stg, (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask]));
    }
}


void Gpio_SetShortToSupplyIrqEnable(enum GpioInstanceId const keGpioInstanceId, const boolean kbEnable )
{
    
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {  
        if(kbEnable == True)
        {
            SetRegisterField((volatile uint32*)&stGpioRegs.StsInten, (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask]));
        }
        else
        {
            ClearRegisterField((volatile uint32*)&stGpioRegs.StsInten, (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask]));
        }
    }
}

boolean Gpio_IsShortToSupplyIrqEnabled(enum GpioInstanceId const keGpioInstanceId)
{
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {  
        if( (stGpioRegs.StsInten & (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask])) == True )
        {
            return True;
        }
    }
    return False;
}


boolean Gpio_IsShortToSupplyIrqSet(enum GpioInstanceId const keGpioInstanceId)
{
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {  
        if(( stGpioRegs.Sts & (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask])) > 0)
        {
            return True;
        }
    }
    return False;
}


void Gpio_ClearShortToSupplyIrq(enum GpioInstanceId const keGpioInstanceId)
{
    if ( keGpioInstanceId < eGPIO_INSTANCE_ID8 )
    {
        SetRegisterField((volatile uint32*)&stGpioRegs.Sts, (u32ArrBitmaskFromPinNumLut[(stGpio[keGpioInstanceId].m_u8PinNum) & sku8ValidPd0To15Mask]));
    }
}



Std_ReturnType Gpio_SetShortCircuitDetectionTime(enum GpioInstanceId const keGpioInstanceId, const uint16 ku16TimeInNanoseconds )
{
    Std_ReturnType u8Status = E_OK;
  
    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {
        if ( ( ku16TimeInNanoseconds % ku16ShortCircuitDetectionTime ) == 0 )
        {
            uint32 const ku32BlankTableEntry = 4U * (uint32)((ku8BlankingRegisterTable[(stGpio[keGpioInstanceId].m_u8PinNum)]));
            uintptr const kuPrBlankBaseAddress = ((uintptr)eADDRESS_GPIO_BLANK_BASE) + ku32BlankTableEntry;

            uint32 const ku32TimeIn200nsIntervals = (uint32)((uint16)(ku16TimeInNanoseconds / ku16ShortCircuitDetectionTime));  

            ReadModifyWriteField(kuPrBlankBaseAddress, (keBlankBitmaskTable[stGpio[keGpioInstanceId].m_u8PinNum]), 
                   (keBlankBitposTable[stGpio[keGpioInstanceId].m_u8PinNum]), ku32TimeIn200nsIntervals);
        }
        else
        {  
            u8Status = E_NOT_OK;
        }
    }
    else
    {
        u8Status = E_NOT_OK;
    }
    
    return u8Status;
}


uint16 Gpio_GetShortCircuitProtectionTime(enum GpioInstanceId const keGpioInstanceId)
{
    uint16 u16Time = ku16Invalid;

    if ( stGpio[keGpioInstanceId].m_u8PinNum != s_ku8InvalidPinNum )
    {
        const uintptr kuPrBlankBaseAddress = (uintptr)( eADDRESS_GPIO_BLANK_BASE ) + (uintptr)(uint8)( 4U * ku8BlankingRegisterTable[(stGpio[keGpioInstanceId].m_u8PinNum)] );

        uint32 u32RegData;
        uint16 const ku16Mask = keBlankBitmaskTable[(stGpio[keGpioInstanceId].m_u8PinNum)];
        uint16 const ku16Bitpos = keBlankBitposTable[(stGpio[keGpioInstanceId].m_u8PinNum)];
        ReadField(kuPrBlankBaseAddress, (uint32)ku16Mask, (uint32)ku16Bitpos, u32RegData);
        u16Time = (uint16)u32RegData;        
        u16Time *= ku16ShortCircuitDetectionTime;
    }

    return u16Time;
}

/**
 * @brief Initialises the GPIO instance to enabled or disabled state 
 * @details A GPIO instance must have a valid pin number and is assigned its instance Id number
 * in a 1:1 mapping. 0xFF is an inv
 *
 * The number of valid GPIO pins is product dependent. The first pin is numbered 0.
 * If a pin number is used outside of the supported range.
 * The pin number value 0xFF is reserved by Allegro to indicate an unassigned pin
 * number.
 *
 * If a GPIO instance does not have a valid pin number then none of the methods of the
 * instance will have any effect.
 *
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] kbEnable Enable or Disable
 */
static Std_ReturnType Gpio_SetPinEnable(enum GpioInstanceId const keGpioInstanceId, boolean const kbEnable  )
{
    Std_ReturnType u8Status = E_OK;
    NVIC_DisableIRQ( skIrqGpio );

    if ( kbEnable == False )
    {
        // Ensure interrupts are disabled
        u8Status = Gpio_SetTriggerIrqEnable(keGpioInstanceId, False );
        if (u8Status != E_OK)
        {
           // Clearing trigger in the valid pin assignment case, do nothing for this unallocated case.
        }
        Gpio_SetShortToGroundIrqEnable(keGpioInstanceId, False );
        Gpio_SetShortToSupplyIrqEnable(keGpioInstanceId, False );

        // Turn off short circuit detection
        u8Status = Gpio_SetShortCircuitDetectionTime(keGpioInstanceId, 0U );
        if (u8Status != E_OK)
        {
            // invalid pin number do nothing
        }
        
        // Clear handler
        stGpio[keGpioInstanceId].m_fnIrqHandler = NULLPTR;

        // Set pin mode to disabled
        u8Status = Gpio_SetMode(keGpioInstanceId, eGPIO_MODE_DISABLED );
        if (u8Status != E_OK)
        {
            // invalid pin number do nothing
        }

        // Set pin number out of range
        stGpio[keGpioInstanceId].m_u8PinNum = s_ku8InvalidPinNum;
    }
    else // kbEnable is True
    {
        stGpio[keGpioInstanceId].m_u8PinNum = (uint8)keGpioInstanceId;
        stGpio[keGpioInstanceId].m_fnIrqHandler = NULLPTR;
      
        // Clear the control register
        uint32 u32Addr;
        GenControlRegAddrFromPinNum((uint32)keGpioInstanceId, u32Addr);
        ClearRegisterField( u32Addr, sku32WordClear );
        
        // Clear IRQ enable bits
        u8Status = Gpio_SetTriggerIrqEnable(keGpioInstanceId, False );
        if (u8Status != E_OK)
        {
            // ku8PinNum validated to always be in range above. Do nothing.
        }

        Gpio_SetShortToGroundIrqEnable(keGpioInstanceId, False);
        Gpio_SetShortToSupplyIrqEnable(keGpioInstanceId, False);
      
        // Clear IRQ status bits
        Gpio_ClearTriggerIrq(keGpioInstanceId);
        Gpio_ClearShortToGroundIrq(keGpioInstanceId);
        Gpio_ClearShortToSupplyIrq(keGpioInstanceId);      
    }
		
    NVIC_EnableIRQ( skIrqGpio );
    return u8Status;
}


Std_ReturnType Gpio_SetPinActive(enum GpioInstanceId const keGpioInstanceId)
{
    return Gpio_SetPinEnable(keGpioInstanceId, True);
}


Std_ReturnType  Gpio_SetPinInactive(enum GpioInstanceId const keGpioInstanceId)
{
    return Gpio_SetPinEnable(keGpioInstanceId, False);
}


Std_ReturnType  Gpio_SetAllPinsInactive(void)
{
    Std_ReturnType u8Status = E_OK;
    uint8 uint8Pin;
  
    for ( uint8Pin = 0; uint8Pin < gku32MaxNumGpioPins; uint8Pin++ )
    {
        u8Status |= Gpio_SetPinEnable((enum GpioInstanceId)uint8Pin, False);
    }
    return u8Status;
}



/**
 * @internal
 *
 * @brief Process the current status register
 * @param[in] ruPrStatusRegister A reference to the current status register to process
 * @param[in] keIrqSource The current IRQ source
 * @details There are 16 interrupt sources to check for each status register.
 * Cycle through the array and call the interrupt handler if one is registered for any active interrupt
 */

static void Gpio_ProcessIrqStatusRegister( uintptr uPrStatusRegister, enum GpioIrqSource const keIrqSource )
{
    volatile uintptr uPrMask = 0x01U;
    static enum GpioIrqSource skeIrqSource;
    skeIrqSource = keIrqSource;
    uint8 u8IrqHandlerIndex;
  
    for ( u8IrqHandlerIndex=0; u8IrqHandlerIndex < gku32MaxNumGpioPins; u8IrqHandlerIndex++ )
    {
        if ( ( uPrStatusRegister & uPrMask ) != 0U )
        {
            if ( stGpio[((enum GpioInstanceId)u8IrqHandlerIndex)].m_fnIrqHandler != NULLPTR )
            {
                stGpio[((enum GpioInstanceId)u8IrqHandlerIndex)].stGpioIrqContext.keGpioInstanceId = (enum GpioInstanceId)u8IrqHandlerIndex;
                stGpio[((enum GpioInstanceId)u8IrqHandlerIndex)].stGpioIrqContext.eIrqSource = skeIrqSource;
                stGpio[((enum GpioInstanceId)u8IrqHandlerIndex)].m_fnIrqHandler((void*)&stGpio[((enum GpioInstanceId)u8IrqHandlerIndex)].stGpioIrqContext);
            }
            uPrStatusRegister |= uPrMask;
        }
        uPrMask <<= 1U;
    }
}


__attribute__( ( interrupt( "IRQ" ) ) ) void Gpio_IrqHandler(void)
{
    NVIC_DisableIRQ( skIrqGpio );
    NVIC_ClearPendingIRQ( skIrqGpio );

    if ( stGpioRegs.EventStatus != 0U )
    {
        Gpio_ProcessIrqStatusRegister( stGpioRegs.EventStatus, eGPIO_IRQ_SOURCE_TRIGGER );
    }

    if ( stGpioRegs.Stg != 0U )
    {
        Gpio_ProcessIrqStatusRegister( stGpioRegs.Stg, eGPIO_IRQ_SOURCE_SHORT_TO_GROUND );
    }

    if ( stGpioRegs.Sts != 0U )
    {
        Gpio_ProcessIrqStatusRegister( stGpioRegs.Sts, eGPIO_IRQ_SOURCE_SHORT_TO_SUPPLY );
    }

    NVIC_EnableIRQ( skIrqGpio );
}

