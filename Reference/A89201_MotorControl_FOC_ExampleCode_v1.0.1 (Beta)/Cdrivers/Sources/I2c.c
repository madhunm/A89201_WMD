/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @internal
 *
 * @file         I2c.c
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
 *
 * @details      Implementation reflects I2C Design Intent v0.9
 */
 /* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
#include "I2c.h"

static IrqType_t const skIrqI2c = ARM_IRQ_NUM(eIRQNUM_I2C);

static void (*m_apclsIrqHandler[s_ku8NumIrqHandlers])(void* uprContext) = 
    { NULLPTR, NULLPTR, NULLPTR, NULLPTR, NULLPTR, NULLPTR };

static uint8 au8Context[s_ku8NumIrqHandlers] = 
    { 0, 0, 0, 0, 0, 0 };
    
/**
 * @internal
 * @brief Enumerate the index for the status switch statement
 */
enum StatusIndex
{
    eIRQ_STATUS_TX     = 0U,
    eIRQ_STATUS_RX     = 1U,
    eIRQ_STATUS_ACK    = 2U,
    eIRQ_STATUS_CRC    = 3U,
    eIRQ_STATUS_OVF    = 4U,
    eIRQ_STATUS_FF     = 5U,
    eIRQ_STATUS_BUSY   = 6U
};


/**
 * @internal
 * @brief Enumerate the index for the IRQ switch statement
 */
enum IrqIndex
{
    eIRQ_INDEX_TX      = 0U,
    eIRQ_INDEX_RX      = 1U,
    eIRQ_INDEX_ACK     = 2U,
    eIRQ_INDEX_CRC     = 3U,
    eIRQ_INDEX_OVF     = 4U,
    eIRQ_INDEX_FF      = 5U,
    eIRQ_INDEX_INVALID = 0xFFU
};


/**
 * @internal
 * @details Set the I2C module in an initial state.
 */
void I2c_Initialize(void)
{
    NVIC_DisableIRQ( skIrqI2c );
    NVIC_ClearPendingIRQ( skIrqI2c );

    // Disable the I2C.
    I2c_SetMode( eI2C_MODE_OFF );
    // Set Clock
    I2c_SetClock( eI2C_CLOCK_5_MHZ );
    // Enable User operational mode as default
    I2c_SetMode( eI2C_MODE_USER_OPERATION_ONLY );

    // Clear all status flags.
    I2c_ClearStatusFlags();

}


/**
 * @internal
 * @details Clear down all interrupts and handlers.
 */
void I2c_ClearAllIrqs(void)
{
    // Disable the Irq
    NVIC_DisableIRQ( skIrqI2c );
    NVIC_ClearPendingIRQ( skIrqI2c );

    // Switch off the I2C module
    I2c_SetMode( eI2C_MODE_OFF );

    // Disable TX Irq.
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_TX, False );
    // Disable RX Irq.
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_RX, False );
    // Disable ACK Irq.
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_ACK, False );
    // Disable CRC Irq.
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_CRC, False );
    // Disable OVF Irq.
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_OVF, False );
    // Disable FF Irq.
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_FF, False );

    // Clear all the Irq handlers.
    uint8 u8IrqIndex = 0;
    for(u8IrqIndex=0; u8IrqIndex < s_ku8NumIrqHandlers; u8IrqIndex++)
    {
         m_apclsIrqHandler[u8IrqIndex] = NULLPTR;
    }
}

/**
 * @internal
 * @details Write the address register (ADDR) to start an I2C read.
 * This is a blocking read that blocks until receive the data from the slave.
 */
uint16 I2c_Read( uint8 const ku8Address )
{
    // Set to read mode. Combine the 7 bit-address with the Read/Write bit.
    // Set the Read/Write bit to 1 after the left swift.
    uint8 const u8Addr = (uint8)( (ku8Address << 1U) | 0x01U);

    // Perform the I2C Read by writing the ADDR register
    stI2cRegs.Addr = u8Addr;

    // The Rx data to read.
    uint16 u16Rx;
  
    // critical section.
    while ( True )
    {
        // This is a critical path. when the condition is true, the read from the RX register should not be interrupted.
        // An Irq at this place may perform a new I2C Read and if the old one is too much delayed
        // it may read the latest value which is for the second read and not for the first.
        if ( I2c_GetRxFlag() == True )
        {
            // Read the RX register
            u16Rx = (uint16)stI2cRegs.Rx;
            break;
        }
    }
    // critical section.

    return u16Rx;
}

/**
 * @internal
 * @details The received data is in bits 15:0 of the RX register.
 * Returns the received data.
 */
uint16 I2c_GetRxValue(void)
{
    return (uint16)stI2cRegs.Rx;
}

/**
 * @internal
 * @details Set the I2C mode (MODE). It is in bits 1:0 of the control register.
 */
void I2c_SetMode( enum I2cMode const keI2cMode )
{
    uint8 u8Mode;

    switch ( keI2cMode )
    {
    case eI2C_MODE_OFF:
        u8Mode = (uint8)0U;
        break;
    case eI2C_MODE_USER_OPERATION_ONLY:
        u8Mode = (uint8)1U;
        break;
    case eI2C_MODE_OFF1:
        u8Mode = (uint8)2U;
        break;
    case eI2C_MODE_AUTOMATIC_POLLING_AND_USER_OPERATION:
        u8Mode = (uint8)3U;
        break;
		default:
        u8Mode = (uint8)1U; // default user operation mode
        break;
    }

    ReadModifyWriteField((volatile uint32*)&stI2cRegs.Ctrl, I2C_CONTROL_MODE_MASK, I2C_CONTROL_MODE_BITPOS, u8Mode);
    if(keI2cMode == eI2C_MODE_AUTOMATIC_POLLING_AND_USER_OPERATION)
    {
        volatile uint8 u8Count = 0;
        uint16 const ku16ModeSwitchInertialDelay5us = 200U;
        while( u8Count < ku16ModeSwitchInertialDelay5us )
        {
            __asm volatile("nop");
            u8Count++;
        }
        stI2cRegs.IntStat = I2C_INTSTAT_RX_MASK;
    }
}

/**
 * @internal
 * @details The I2C mode (MODE) is in bits 1:0 of the control register so mask this out.
 * This can be safely cast to an enumeration, protection is only required when writing.
 * Returns the I2C mode.
 */
enum I2cMode I2c_GetMode(void)
{
    uint32 u32RegData;
    ReadField((volatile uint32*)&stI2cRegs.Ctrl, I2C_CONTROL_MODE_MASK, I2C_CONTROL_MODE_BITPOS, u32RegData);
    return (enum I2cMode)u32RegData;
}

/**
 * @internal
 * @details Set the I2C clock frequency (FREQ). It is in bits 3:2 of the control register.
 */
void I2c_SetClock( enum I2cClock const keI2cClock )
{
    uint8 u8Clock;

    switch ( keI2cClock )
    {
    case eI2C_CLOCK_5_MHZ:
        u8Clock = (uint8)1U;
        break;
    case eI2C_CLOCK_2_MHZ:
        u8Clock = (uint8)2U;
        break;
    case eI2C_CLOCK_1_MHZ:
        u8Clock = (uint8)3U;
        break;
		default:
        u8Clock = (uint8)1U; // default 5MHz
        break;
    }

    ReadModifyWriteField((volatile uint32*)&stI2cRegs.Ctrl, I2C_CONTROL_FREQ_MASK, I2C_CONTROL_FREQ_BITPOS, u8Clock);
}

/**
 * @internal
 * @details The I2C clock frequency (FREQ) is in bits 3:2 of the control register so mask this out.
 * This can be safely cast to an enumeration, protection is only required when writing.
 * Returns the I2C clock frequency.
 */
enum I2cClock I2c_GetClock(void)
{
    uint32 u32RegData;
    ReadField((volatile uint32*)&stI2cRegs.Ctrl, I2C_CONTROL_FREQ_MASK, I2C_CONTROL_FREQ_BITPOS, u32RegData);
    return (enum I2cClock)u32RegData;
}

/**
 * @internal
 * @details The transmit complete flag (TX) is in bit 0 of the status register clear it out by writing 1.
 */
void I2c_ClearTxFlag(void)
{
    stI2cRegs.IntStat = (uint8)I2C_INTSTAT_TX_MASK;
}

/**
 * @internal
 * @details The receive complete flag (RX) is in bit 1 of the status register clear it out by writing 1.
 */
void I2c_ClearRxFlag(void)
{
    stI2cRegs.IntStat = (uint8)I2C_INTSTAT_RX_MASK;
}

/**
 * @internal
 * @details The acknowledge error flag (ACK) is in bit 2 of the status register clear it out by writing 1.
 */
void I2c_ClearAckFlag(void)
{
    stI2cRegs.IntStat = (uint8)I2C_INTSTAT_ACK_MASK;
}

/**
 * @internal
 * @details The crc error flag (CRC) is in bit 3 of the status register clear it out by writing 1.
 */
void I2c_ClearCrcFlag(void)
{
    stI2cRegs.IntStat = (uint8)I2C_INTSTAT_CRC_MASK;
}

/**
 * @internal
 * @details The ovf flag (OVF) is in bit 4 of the status register clear it out by writing 1.
 */
void I2c_ClearOvfFlag(void)
{
    stI2cRegs.IntStat = (uint8)I2C_INTSTAT_OVF_MASK;
}

/**
 * @internal
 * @details The GDU fault flag (FF) is in bit 5 of the status register clear it out by writing 1.
 */
void I2c_ClearGduFfFlag(void)
{
    stI2cRegs.IntStat = (uint8)I2C_INTSTAT_FF_MASK;
}

/**
 * @internal
 * @details Clear out all the status flags by writing 0x3F.
 */
void I2c_ClearStatusFlags(void)
{
    stI2cRegs.IntStat = (uint8)I2C_INTSTAT_ALL_MASK;
}


/**
 * @internal
 * @details The transmit complete flag (TX) is in bit 0 of the status register.
 * Use bitband technic to read the value.
 * Returns true if the TX is enabled else False.
 */
boolean I2c_GetTxFlag(void)
{
    uint32 u32RegData;
    ReadField((volatile uint32*)&stI2cRegs.IntStat, I2C_INTSTAT_TX_MASK, I2C_INTSTAT_TX_BITPOS, u32RegData);
    return (boolean)u32RegData;
}


/**
 * @internal
 * @details The receive complete flag (RX) is in bit 1 of the status register.
 * Use bitband technic to read the value.
 * Returns true if the RX is enabled else False.
 */
boolean I2c_GetRxFlag(void)
{
    uint32 u32RegData;
    ReadField((volatile uint32*)&stI2cRegs.IntStat, I2C_INTSTAT_RX_MASK, I2C_INTSTAT_RX_BITPOS, u32RegData);
    return (boolean)u32RegData;
}


/**
 * @internal
 * @details The acknowledge error flag (ACK) is in bit 2 of the status register.
 * Use bitband technic to read the value.
 * Returns true if the ACK is enabled else False.
 */
boolean I2c_GetAckFlag(void)
{
    uint32 u32RegData;
    ReadField((volatile uint32*)&stI2cRegs.IntStat, I2C_INTSTAT_ACK_MASK, I2C_INTSTAT_ACK_BITPOS, u32RegData);
    return (boolean)u32RegData;
}


/**
 * @internal
 * @details The crc error flag (CRC) is in bit 3 of the status register.
 * Use bitband technic to read the value.
 * Returns true if the CRC is enabled else False.
 */
boolean I2c_GetCrcFlag(void)
{
    uint32 u32RegData;
    ReadField((volatile uint32*)&stI2cRegs.IntStat, I2C_INTSTAT_CRC_MASK, I2C_INTSTAT_CRC_BITPOS, u32RegData);
    return (boolean)u32RegData;
}


/**
 * @internal
 * @details The ovf flag (OVF) is in bit 4 of the status register.
 * Use bitband technic to read the value.
 * Returns true if the OVF is enabled else False.
 */
boolean I2c_GetOvfFlag(void)
{
    uint32 u32RegData;
    ReadField((volatile uint32*)&stI2cRegs.IntStat, I2C_INTSTAT_OVF_MASK, I2C_INTSTAT_OVF_BITPOS, u32RegData);
    return (boolean)u32RegData;
}


/**
 * @internal
 * @details The GDU fault flag (FF) is in bit 5 of the status register.
 * Use bitband technic to read the value.
 * Returns true if the FF is enabled else False.
 */
boolean I2c_GetFfFlag(void)
{
    uint32 u32RegData;
    ReadField((volatile uint32*)&stI2cRegs.IntStat, I2C_INTSTAT_FF_MASK, I2C_INTSTAT_FF_BITPOS, u32RegData);
    return (boolean)u32RegData;
}


/**
 * @internal
 * @details The busy flag (BUSY) is in bit 6 of the status register.
 * Use bitband technic to read the value.
 * Returns true if the FF is enabled else False.
 */
boolean I2c_GetBusyFlag(void)
{
    uint32 u32RegData;
    ReadField((volatile uint32*)&stI2cRegs.IntStat, I2C_INTSTAT_BUSY_MASK, I2C_INTSTAT_BUSY_BITPOS, u32RegData);
    return (boolean)u32RegData;
}


/**
 * @internal
 * @details This IRQ service routine calls an interrupt specific handler
 * if one has been registered.
 */
static void I2c_ServiceIrq(void)
{
    boolean bResult;
    Std_ReturnType u8Status;

    // check the Status register if the interrupt is enabled.
    if ( I2c_GetTxFlag() == True )
    {
        // check the Irq enable register if the interrupt is Set
        u8Status = I2c_IsIrqEnabled(eI2C_IRQ_SOURCE_TX, &bResult);
        if( (u8Status == E_OK) && (bResult==True) )
        {
            // check if there is an assigned interrupt handler.
            if ( m_apclsIrqHandler[(uint8)( eIRQ_INDEX_TX )] != NULLPTR )
            {
                // call user's interrupt handler.
                au8Context[((uint8)eIRQ_INDEX_TX)] = (uint8)eI2C_IRQ_SOURCE_TX;
                m_apclsIrqHandler[((uint8)eIRQ_INDEX_TX)]( &au8Context[((uint8)eIRQ_INDEX_TX)]  );
            }
            // clear the Tx status bit.
            I2c_ClearTxFlag();
        }
    }

    if ( I2c_GetRxFlag() == True )
    {
        u8Status = I2c_IsIrqEnabled(eI2C_IRQ_SOURCE_RX, &bResult);
        if( (u8Status == E_OK) && (bResult==True) )
        {
            if ( m_apclsIrqHandler[((uint8)eIRQ_INDEX_RX)] != NULLPTR )
            {
                au8Context[((uint8)eIRQ_INDEX_RX)] = (uint8)eI2C_IRQ_SOURCE_RX;
                m_apclsIrqHandler[((uint8)eIRQ_INDEX_RX)]( &au8Context[((uint8)eIRQ_INDEX_RX)] );
            }
            I2c_ClearRxFlag();
        }
    }

    if ( I2c_GetAckFlag() == True )
    {
         u8Status = I2c_IsIrqEnabled(eI2C_IRQ_SOURCE_ACK, &bResult);
        if( (u8Status == E_OK) && (bResult==True) )
        {
            if ( m_apclsIrqHandler[((uint8)eIRQ_INDEX_ACK)] != NULLPTR )
            {
                au8Context[((uint8)eIRQ_INDEX_ACK)] = (uint8)eI2C_IRQ_SOURCE_ACK;
                m_apclsIrqHandler[((uint8)eIRQ_INDEX_ACK)]( &au8Context[((uint8)eIRQ_INDEX_ACK)] );
            }
            I2c_ClearAckFlag();
        }
    }

    if ( I2c_GetCrcFlag() == True )
    {
        u8Status = I2c_IsIrqEnabled(eI2C_IRQ_SOURCE_CRC, &bResult);
        if( (u8Status == E_OK) && (bResult==True) )
        {
            if ( m_apclsIrqHandler[((uint8)eIRQ_INDEX_CRC)] != NULLPTR )
            {
                au8Context[((uint8)eIRQ_INDEX_CRC)] = (uint8)eI2C_IRQ_SOURCE_CRC;
                m_apclsIrqHandler[((uint8)eIRQ_INDEX_CRC)]( &au8Context[((uint8)eIRQ_INDEX_CRC)]  );
            }
            I2c_ClearCrcFlag();
        }
    }

    if ( I2c_GetOvfFlag() == True )
    {
        u8Status = I2c_IsIrqEnabled(eI2C_IRQ_SOURCE_OVF, &bResult);
        if( (u8Status == E_OK) && (bResult==True) )
        {
            if ( m_apclsIrqHandler[((uint8)eIRQ_INDEX_OVF)] != NULLPTR )
            {
                au8Context[((uint8)eIRQ_INDEX_OVF)] = (uint8)eI2C_IRQ_SOURCE_OVF;
                m_apclsIrqHandler[((uint8)eIRQ_INDEX_OVF)]( &au8Context[((uint8)eIRQ_INDEX_OVF)] );
            }
            I2c_ClearOvfFlag();
        }
    }


    if ( I2c_GetFfFlag() == True )
    {
        u8Status = I2c_IsIrqEnabled(eI2C_IRQ_SOURCE_FF, &bResult);
        if( (u8Status == E_OK) && (bResult==True) )
        {
            if ( m_apclsIrqHandler[((uint8)eIRQ_INDEX_FF )] != NULLPTR )
            {
                au8Context[((uint8)eIRQ_INDEX_FF)] = (uint8)eI2C_IRQ_SOURCE_FF;
                m_apclsIrqHandler[((uint8)eIRQ_INDEX_FF )]( &au8Context[((uint8)eIRQ_INDEX_FF)] );
            }
            I2c_ClearGduFfFlag();
        }
    }
}

/**
 * @internal
 * @details Add the address of the IRQ specific handler to the IRQ handler table.
 * This implementation is protected against changes to the Irq enumeration.
 */
Std_ReturnType I2c_SetIrqHandler( enum I2cIrqSource const keI2cIrqSource, void (pfnI2cIrqHandler)(void* pContext) )
{
    Std_ReturnType u8Status = E_OK;
    switch ( keI2cIrqSource )
    {
    case eI2C_IRQ_SOURCE_TX:
        m_apclsIrqHandler[(uint8)( eIRQ_INDEX_TX )] = *pfnI2cIrqHandler;
        au8Context[((uint8)eIRQ_INDEX_TX)] = 0;
        break;

    case eI2C_IRQ_SOURCE_RX:
        m_apclsIrqHandler[(uint8)( eIRQ_INDEX_RX )] =  *pfnI2cIrqHandler;
        au8Context[((uint8)eIRQ_INDEX_RX)] = 0;
        break;

    case eI2C_IRQ_SOURCE_ACK:
        m_apclsIrqHandler[(uint8)( eIRQ_INDEX_ACK )] = *pfnI2cIrqHandler;
        au8Context[((uint8)eIRQ_INDEX_ACK)] = 0;
        break;

    case eI2C_IRQ_SOURCE_CRC:
        m_apclsIrqHandler[(uint8)( eIRQ_INDEX_CRC )] = *pfnI2cIrqHandler;
        au8Context[((uint8)eIRQ_INDEX_CRC)] = 0;
        break;

    case eI2C_IRQ_SOURCE_OVF:
        m_apclsIrqHandler[(uint8)( eIRQ_INDEX_OVF )] = *pfnI2cIrqHandler;
        au8Context[((uint8)eIRQ_INDEX_OVF)] = 0;
        break;

    case eI2C_IRQ_SOURCE_FF:
        m_apclsIrqHandler[(uint8)( eIRQ_INDEX_FF )] = *pfnI2cIrqHandler;
        au8Context[((uint8)eIRQ_INDEX_FF)] = 0;
        break;

    default:
        u8Status = E_NOT_OK;
        break;
    }
    return u8Status;
}

/**
 * @internal
 * @details Enables/Disables an IRQ.
 */
void I2c_SetIrqEnable( enum I2cIrqSource const keI2cIrqSource, boolean const kbEnable )
{
    NVIC_DisableIRQ( skIrqI2c );
    NVIC_ClearPendingIRQ( skIrqI2c );
    if(kbEnable == True)
    {
        SetRegisterField((volatile uint32*)&stI2cRegs.IntEn, (uint32)keI2cIrqSource);
    }
    else
    {
        ClearRegisterField((volatile uint32*)&stI2cRegs.IntEn, (uint32)keI2cIrqSource);
    }
    NVIC_EnableIRQ( skIrqI2c );
}

/**
 * @internal
 * @details Gets whether an IRQ is enabled.
 * This implementation performs uninitialised error checking as it is not needed
 * for time critical code.
 */
Std_ReturnType I2c_IsIrqEnabled( enum I2cIrqSource const keI2cIrqSource, boolean* const pbResult )
{
    Std_ReturnType u8Status = E_OK;
  
    if(pbResult != NULLPTR)
    {
        if( (stI2cRegs.IntEn & (uint32)keI2cIrqSource) > 0)
        {
            *pbResult = True;
        }
        else
        {
            *pbResult = False;
        }
    }
    else
    {
        u8Status = E_NOT_OK;
    }
    return u8Status;
}

/**
 * @internal
 * @details This I2C_INTERRUPT handler is registered in the vector table.
 */
__attribute__( ( interrupt( "IRQ" ) ) ) void I2c_IrqHandler(void)
{
    NVIC_DisableIRQ( skIrqI2c );
    NVIC_ClearPendingIRQ( skIrqI2c );

    I2c_ServiceIrq();

    NVIC_EnableIRQ( skIrqI2c );
}