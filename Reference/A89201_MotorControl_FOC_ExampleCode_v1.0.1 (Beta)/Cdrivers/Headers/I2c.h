#ifndef HEADER_I2C_H
#define HEADER_I2C_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         I2c.h
 * @author       G. Sloan
 * @version      1.0
 *
 * @brief        The implementation of the I2C interface.
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
#include "Std_Types.h"
#include "A89201RegisterDefinitions.h"


#define I2C_ADDRESS_RNW_MASK           0x1U
#define I2C_ADDRESS_RNW_BITPOS         0U
#define I2C_ADDRESS_ADDR_MASK          0xFEU
#define I2C_ADDRESS_ADDR_BITPOS        0x1U 
#define I2C_TX_DATA_MASK               0xFFFFU
#define I2C_TX_DATA_BITPOS             0U
#define I2C_RX_DATA_MASK               0xFFFFU
#define I2C_RX_DATA_BITPOS             0U
#define I2C_CONTROL_MODE_MASK          0x3U
#define I2C_CONTROL_MODE_BITPOS        0U
#define I2C_CONTROL_FREQ_MASK          0xCU
#define I2C_CONTROL_FREQ_BITPOS        2U
#define I2C_INTEN_TX_MASK              0x1U
#define I2C_INTEN_TX_BITPOS            0U
#define I2C_INTEN_RX_MASK              0x2U
#define I2C_INTEN_RX_BITPOS            1U
#define I2C_INTEN_ACK_MASK             0x4U
#define I2C_INTEN_ACK_BITPOS           2U
#define I2C_INTEN_CRC_MASK             0x8U
#define I2C_INTEN_CRC_BITPOS           3U
#define I2C_INTEN_OVF_MASK             0x10U
#define I2C_INTEN_OVF_BITPOS           4U
#define I2C_INTEN_FF_MASK              0x20U 
#define I2C_INTEN_FF_BITPOS            5U
#define I2C_INTSTAT_TX_MASK            0x1U
#define I2C_INTSTAT_TX_BITPOS          0U
#define I2C_INTSTAT_RX_MASK            0x2U
#define I2C_INTSTAT_RX_BITPOS          1U
#define I2C_INTSTAT_ACK_MASK           0x4U
#define I2C_INTSTAT_ACK_BITPOS         2U
#define I2C_INTSTAT_CRC_MASK           0x8U
#define I2C_INTSTAT_CRC_BITPOS         3U
#define I2C_INTSTAT_OVF_MASK           0x10U
#define I2C_INTSTAT_OVF_BITPOS         4U
#define I2C_INTSTAT_FF_MASK            0x20U 
#define I2C_INTSTAT_FF_BITPOS          5U
#define I2C_INTSTAT_BUSY_MASK          0x40U 
#define I2C_INTSTAT_BUSY_BITPOS        6U
#define I2C_INTSTAT_ALL_MASK           0x3FU

// I2C Registers represented in a struct
typedef struct __attribute__((packed)) 
{
    uint32 Addr;        // offset:  0x00U 
    uint32 Tx;          // offset:  0x04U 
    uint32 Rx;          // offset:  0x08U 
    uint32 Ctrl;        // offset:  0x0CU 
    uint32 IntEn;       // offset:  0x10U   
    uint32 IntStat;     // offset:  0x14U
} I2cRegs_t;

#define  stI2cRegs (*( volatile I2cRegs_t *)eADDRESS_I2C_BASE)

/**
 * @brief Enumerates the IRQ enables
 */
enum I2cIrqEnable
{
    eI2C_IRQ_ENABLE_TXON           = ( 0b0000000000000001U ),
    eI2C_IRQ_ENABLE_TXOFF          = ( 0b1111111111111110U ),
    eI2C_IRQ_ENABLE_RXON           = ( 0b0000000000000010U ),
    eI2C_IRQ_ENABLE_RXOFF          = ( 0b1111111111111101U ),
    eI2C_IRQ_ENABLE_ACKERRON       = ( 0b0000000000000100U ),
    eI2C_IRQ_ENABLE_ACKERROFF      = ( 0b1111111111111011U ),
    eI2C_IRQ_ENABLE_CRCERRON       = ( 0b0000000000001000U ),
    eI2C_IRQ_ENABLE_CRCERROFF      = ( 0b1111111111110111U ),
    eI2C_IRQ_ENABLE_OVFERRON       = ( 0b0000000000010000U ),
    eI2C_IRQ_ENABLE_OVFERROFF      = ( 0b1111111111101111U ),
    eI2C_IRQ_ENABLE_FFERRON        = ( 0b0000000000100000U ),
    eI2C_IRQ_ENABLE_FFERROFF       = ( 0b1111111111011111U ),
};

/**
 * @brief Enumerates the IRQ status
 */
enum I2cStatus
{
    eI2C_STATUS_TX             = ( 0b0000000000000001U ),
    eI2C_STATUS_RX             = ( 0b0000000000000010U ),
    eI2C_STATUS_ACK            = ( 0b0000000000000100U ),
    eI2C_STATUS_CRC            = ( 0b0000000000001000U ),
    eI2C_STATUS_OVF            = ( 0b0000000000010000U ),
    eI2C_STATUS_FF             = ( 0b0000000000100000U ),
    eI2C_STATUS_BUSY           = ( 0b0000000001000000U ),
    eI2C_STATUS_POLLING        = ( 0b0000000010000000U ),
};

/**
 * @brief Enumerates the IRQ types
 */
enum I2cIrqSource
{
    /** Data transmition completed */
    eI2C_IRQ_SOURCE_TX = I2C_INTEN_TX_MASK,
    /** Data received */
    eI2C_IRQ_SOURCE_RX = I2C_INTEN_RX_MASK,
    /** Acknowledge Error*/
    eI2C_IRQ_SOURCE_ACK = I2C_INTEN_ACK_MASK,
    /** CRC error*/
    eI2C_IRQ_SOURCE_CRC = I2C_INTEN_CRC_MASK,
    /** Overflow flag */
    eI2C_IRQ_SOURCE_OVF = I2C_INTEN_OVF_MASK,
    /** GDU Fault flag  */
    eI2C_IRQ_SOURCE_FF = I2C_INTEN_FF_MASK,
};

/**
 * @brief Enumerate the different I2C Modes
 */
enum I2cMode
{
    /** Off */
    eI2C_MODE_OFF = 0x0,
    /** User operation only */
    eI2C_MODE_USER_OPERATION_ONLY = 0x1,
    /** Off */
    eI2C_MODE_OFF1 = 0x2,
    /** Automatic polling and user operation */
    eI2C_MODE_AUTOMATIC_POLLING_AND_USER_OPERATION = 0x3
};

/**
 * @brief Enumerate the different I2C clock frequencies
 */
enum I2cClock
{
    /** 5 MHz */
    eI2C_CLOCK_5_MHZ,
    /** 2 MHz */
    eI2C_CLOCK_2_MHZ,
    /** 1 MHz */
    eI2C_CLOCK_1_MHZ
};

/** The number of IRQ handlers */
static uint8 const s_ku8NumIrqHandlers = 6U;

/** The number of status bits */
static uint8 const s_ku8NumStatusBits = 7U;

/** The number of control bits */
static uint8 const s_ku8NumIrqEnableBits = 6U;

/**
 * @brief Set the I2C module in an initial state.
 */
void I2c_Initialize(void);

/**
 * @brief Write over I2C.
 * @param[in] ku8Address The register address to write.
 * @param[in] ku16TxData The TX data to write in the register.
 */
__attribute__( ( always_inline ) ) static inline void I2c_Write( uint8 const ku8Address, uint16 const ku16TxData )
{
    // First write the TX register.
    stI2cRegs.Tx = ku16TxData;
  
    // Set to write mode. Combine the 7 bit-address with the Read/Write bit.
    // Set implicitly the Read/Write bit to 0 after the left swift.
    uint8 const ku8Addr = (uint8)( ku8Address << 1U );

    // Perform the I2C Write by writing the ADDR register.
    stI2cRegs.Addr = ku8Addr;
}

/**
 * @brief Read over I2C.
 * @param[in] ku8Address The register address to read.
 * @returns The read data.
 */
uint16 I2c_Read( uint8 const ku8Address );

/**
 * @brief Non blocking Read over I2C.
 * @param[in] ku8Address The register address to read.
 */
__attribute__( ( always_inline ) ) static inline void I2c_NonBlockingRead( uint8 const ku8Address )
{
    // Set to read mode. Combine the 7 bit-address with the Read/Write bit.
    // Set the Read/Write bit to 1 after the left swift.
    uint8 const ku8Addr = (uint8)( ( ku8Address << 1U ) | 0x01U );

    // Perform the I2C Read by writing the ADDR register
    stI2cRegs.Addr = ku8Addr;
}

/**
 * @brief Read the Rx register.
 * @returns The Rx da/ta.
 */
uint16 I2c_GetRxValue(void);

/**
 * @brief Set the I2C mode.
 * @param[in] keI2cMode An I2C mode.
 */
void I2c_SetMode( enum I2cMode const keI2cMode );

/**
 * @brief Get the I2C mode.
 * @returns An I2C mode
 */
enum I2cMode I2c_GetMode(void);

/**
 * @brief Set the I2C clock frequency.
 * @param[in] keI2cClock The I2C clock frequency.
 */
void I2c_SetClock( enum I2cClock const keI2cClock );

/**
 * @brief Get the I2C clock frequency.
 * @returns An I2C clock frequency.
 */
enum I2cClock I2c_GetClock(void);

/**
 * @brief Clear to 0 the TX status flag.
 */
void I2c_ClearTxFlag(void);

/**
 * @brief Clear to 0 the RX status flag.
 */
void I2c_ClearRxFlag(void);

/**
 * @brief Clear to 0 the ACK status flag.
 */
void I2c_ClearAckFlag(void);

/**
 * @brief Clear to 0 the CRC status flag.
 */
void I2c_ClearCrcFlag(void);

/**
 * @brief Clear to 0 the OVF status flag.
 */
void I2c_ClearOvfFlag(void);

/**
 * @brief Clear to 0 the GDU_FF status flag.
 */
void I2c_ClearGduFfFlag(void);

/**
 * @brief Clear to 0 all the status flags.
 */
void I2c_ClearStatusFlags(void);

/**
 * @brief Return the TX status flag.
 * @returns True if it is set else False.
 */
boolean I2c_GetTxFlag(void);

/**
 * @brief Return the RX status flag.
 * @returns True if it is set else False.
 */
boolean I2c_GetRxFlag(void);

/**
 * @brief Return the ACK status flag.
 * @returns True if it is set else False.
 */
boolean I2c_GetAckFlag(void);

/**
 * @brief Return the CRC status flag.
 * @returns True if it is set else False.
 */
boolean I2c_GetCrcFlag(void);

/**
 * @brief Return the OVF status flag.
 * @returns True if it is set else False.
 */
boolean I2c_GetOvfFlag(void);

/**
 * @brief Return the GDU_FF status flag.
 * @returns True if it is set else False.
 */
boolean I2c_GetFfFlag(void);

/**
 * @brief Return the BUSY status flag.
 * @returns True if it is set else False.
 */
boolean I2c_GetBusyFlag(void);

/**
 * @brief       Get state of I2C Tx Irq Enable
 * @remarks     The use of this should be minimized as it is an inline function.
 * @returns     State of I2C Tx Irq Enable
 */
__attribute__( ( always_inline ) ) static inline boolean I2c_GetTxIrqEnableFast( void )
{
    if((stI2cRegs.IntEn & (uint32)eI2C_IRQ_ENABLE_TXON) > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}

/**
 * @brief      Set I2C Tx Irq Enable
 * @remarks    The use of this should be minimized as it is an inline function.
 * @param[in]  state of Interrupt enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_SetTxIrqEnableFast( boolean const kbIrqEnable )
{
    if ( kbIrqEnable == False )
    {
        stI2cRegs.IntEn &= (uint32)eI2C_IRQ_ENABLE_TXOFF;
    }
    else
    {
        stI2cRegs.IntEn |= (uint32)eI2C_IRQ_ENABLE_TXON;
    }
}

/**
 * @brief      Reset I2C Tx status Flag
 * @remarks    The use of this should be minimized as it is an inline function.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_ResetTxFlagFast( void )
{
    stI2cRegs.IntStat |= (uint32)eI2C_STATUS_TX;
}


/**
 * @brief       Get state of I2C Rx Irq Enable
 * @remarks     The use of this should be minimized as it is an inline function.
 * @returns     State of I2C Rx Irq Enable
 */
__attribute__( ( always_inline ) ) static inline boolean I2c_GetRxIrqEnableFast( void )
{
    if((stI2cRegs.IntEn & (uint32)eI2C_IRQ_ENABLE_RXON) > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}

/**
 * @brief      Set I2C Rx Irq Enable
 * @remarks    The use of this should be minimized as it is an inline function.
 * @param[in]  state of Interrupt enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_SetRxIrqEnableFast( boolean const kbIrqEnable )
{
    if ( kbIrqEnable == False )
    {
        stI2cRegs.IntEn &= (uint32)eI2C_IRQ_ENABLE_RXOFF;
    }
    else
    {
        stI2cRegs.IntEn |= (uint32)eI2C_IRQ_ENABLE_RXON;
    }
}

/**
 * @brief      Reset I2C Rx status Flag
 * @remarks    The use of this should be minimized as it is an inline function.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_ResetRxFlagFast( void )
{
    stI2cRegs.IntStat |= (uint32)eI2C_STATUS_RX;
}

/**
 * @brief      Set Acknowledge error IRQ enable.
 * @remarks    The use of this should be minimized as it is an inline function.
 * @param[in]  state of Acknowledge error Interrupt enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_SetAckErrIrqEnableFast( boolean const kbIrqEnable )
{

    if ( kbIrqEnable == False )
    {
        stI2cRegs.IntEn &= (uint32)eI2C_IRQ_ENABLE_ACKERROFF;
    }
    else
    {
        stI2cRegs.IntEn |= (uint32)eI2C_IRQ_ENABLE_ACKERRON;
    }
}

/**
 * @brief       Get state of Acknowledge error IRQ enable.
 * @remarks     The use of this should be minimized as it is an inline function.
 * @returns     State of I2C Ackknowledge error IRQ enable.
 */
__attribute__( ( always_inline ) ) static inline boolean I2c_GetAckErrIrqEnableFast( void )
{
    if((stI2cRegs.IntEn & (uint32)eI2C_IRQ_ENABLE_ACKERRON) > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}

/**
 * @brief      Reset I2C Acknowledge error Flag
 * @remarks    The use of this should be minimized as it is an inline function.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_ResetAckErrFlagFast( void )
{
    stI2cRegs.IntStat |= (uint32)eI2C_STATUS_ACK;
}

/**
 * @brief      Set CRC error IRQ enable
 * @remarks    The use of this should be minimized as it is an inline function.
 * @param[in]  state of CRC error Interrupt enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_SetCrcErrIrqEnableFast( boolean const kbIrqEnable )
{
    if ( kbIrqEnable == False )
    {
        stI2cRegs.IntEn &= (uint32)eI2C_IRQ_ENABLE_CRCERROFF;
    }
    else
    {
        stI2cRegs.IntEn |= (uint32)eI2C_IRQ_ENABLE_CRCERRON;
    }
}

/**
 * @brief       Get state of CRC error IRQ enable
 * @remarks     The use of this should be minimized as it is an inline function.
 * @returns     State of I2C CRC error IRQ enable
 */
__attribute__( ( always_inline ) ) static inline boolean I2c_GetCrcErrIrqEnableFast( void )
{
    if((stI2cRegs.IntEn & (uint32)eI2C_IRQ_ENABLE_CRCERRON) > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}

/**
 * @brief      Reset I2C CRC error Flag
 * @remarks    The use of this should be minimized as it is an inline function.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_ResetCrckErrFlagFast( void )
{
    stI2cRegs.IntStat |= (uint32)eI2C_STATUS_CRC;
}

/**
 * @brief      Set pending buffer overflow error Irq Enable.
 * @remarks    The use of this should be minimized as it is an inline function.
 * @param[in]  State of pending buffer overflow error interrupt enable.
 * @returns    None.
 */
__attribute__( ( always_inline ) ) static inline void I2c_SetOvfErrIrqEnableFast( boolean const kbIrqEnable )
{
    if ( kbIrqEnable == False )
    {
        stI2cRegs.IntEn &= (uint32)eI2C_IRQ_ENABLE_OVFERROFF;
    }
    else
    {
        stI2cRegs.IntEn |= (uint32)eI2C_IRQ_ENABLE_OVFERRON;
    }
}

/**
 * @brief       Get state of pending buffer overflow error IRQ enable.
 * @remarks     The use of this should be minimized as it is an inline function.
 * @returns     State of I2C pending buffer overflow IRQ enable.
 */
__attribute__( ( always_inline ) ) static inline boolean I2c_GetOvfErrIrqEnableFast( void )
{
    if((stI2cRegs.IntEn & (uint32)eI2C_IRQ_ENABLE_OVFERRON) > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}

/**
 * @brief      Reset I2C pending buffer overflow error Flag
 * @remarks    The use of this should be minimized as it is an inline function.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_ResetOvfErrFlagFast( void )
{
    stI2cRegs.IntStat |= (uint32)eI2C_STATUS_OVF;
}

/**
 * @brief      Set GDU fault flag error Irq Enable.
 * @remarks    The use of this should be minimized as it is an inline function.
 * @param[in]  State of pending buffer overflow error interrupt enable.
 * @returns    None.
 */
__attribute__( ( always_inline ) ) static inline void I2c_SetFfErrIrqEnableFast( boolean const kbIrqEnable )
{
    if ( kbIrqEnable == False )
    {
        stI2cRegs.IntEn &= (uint32)eI2C_IRQ_ENABLE_FFERROFF;
    }
    else
    {
        stI2cRegs.IntEn |= (uint32)eI2C_IRQ_ENABLE_OVFERRON;
    }
}

/**
 * @brief       Get state of GDU fault flag error IRQ enable.
 * @remarks     The use of this should be minimized as it is an inline function.
 * @returns     State of I2C GDU fault flag IRQ enable.
 */
__attribute__( ( always_inline ) ) static inline boolean I2c_GetFfErrIrqEnableFast( void )
{
    if((stI2cRegs.IntEn & (uint32)eI2C_IRQ_ENABLE_FFERRON) > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}

/**
 * @brief      Reset I2C GDU fault error flag
 * @remarks    The use of this should be minimized as it is an inline function.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void I2c_ResetFfErrFlagFast( void )
{
    stI2cRegs.IntStat |= (uint32)eI2C_STATUS_FF;
}

/**
 * @internal
 * @details The busy flag (BUSY) is in bit 6 of the status register.
 * Use bitband technic to read the value.
 * Returns True if the BUSY is enabled else False.
 */
// This is an inline method, it exist in the header file.
__attribute__( ( always_inline ) ) static inline boolean I2c_GetBusyFlagFast(void)
{
    if((stI2cRegs.IntStat & (uint32)eI2C_STATUS_BUSY) > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}

/**
 * @brief      Reset I2C busy flag
 * @remarks    The use of this should be minimized as it is an inline function.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static void I2c_ResetBusyFlagFast( void )
{
    stI2cRegs.IntStat |= (uint32)eI2C_STATUS_BUSY;
}

/**
 * @brief       Get state of I2C Polling flag
 * @remarks     The use of this should be minimized as it is an inline function.
 * @returns     State of I2C Polling flag
 */
__attribute__( ( always_inline ) ) static boolean I2c_GetPollingFlagFast( void )
{
    
    if((stI2cRegs.IntStat & (uint32)eI2C_STATUS_POLLING) > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}

/**
 * @brief      Reset I2C polling flag
 * @remarks    The use of this should be minimized as it is an inline function.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static void I2c_ResetPollingFlagFast( void )
{
    stI2cRegs.IntStat |= (uint32)eI2C_STATUS_POLLING;
}

/**
 * @brief Set the IRQ to be enabled or disabled
 * @details eDOR interrupt is not maskable enabling or disabling it will not do anything
 * @param[in] I2cIrqSource The IRQ to set
 * @param[in] kbEnable True to be enabled else False.
 */
void I2c_SetIrqEnable( enum I2cIrqSource const keI2cIrqSource, boolean const kbEnable );

/**
 * @brief Set a handler for the IRQ.
 * @details Does nothing if I2C is not initialized.
 * @param[in] I2cIrqSource The enumerated IRQ.
 * @param[in] pfnI2cIrqHandler A function pointer to the IRQ handler
 */
Std_ReturnType I2c_SetIrqHandler( enum I2cIrqSource const keI2cIrqSource, void (pfnI2cIrqHandler)(void* pContext) );

/**
 * @brief Gets if the IRQ is enabled or disable
 * @param[in] I2cIrqSource The IRQ to query
 * @param[out] pbResult,  to query True if enabled else False.
 * @returns E_OK on success else E_NOT_OK upon failure with pbResult set to a INVALID if a valid pointer.
 */
Std_ReturnType I2c_IsIrqEnabled( enum I2cIrqSource const keI2cIrqSource, boolean* const pbResult );

 /**
 * @brief Disable all interrupts and handlers
 */
void I2c_ClearAllIrqs(void);
	
/**
 * @brief The IRQ handler for I2c.
 */
void I2c_IrqHandler(void);

#endif //HEADER_I2C_H
