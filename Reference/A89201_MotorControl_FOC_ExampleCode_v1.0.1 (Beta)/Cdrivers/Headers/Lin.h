#ifndef HEADER_LIN_H
#define HEADER_LIN_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Lin.h
 * @author       M. Shankey
 * @version      1.0
 *
 * @brief        LIN driver: LIN
 *
 * @copyright (c) 2024 - 2025 Allegro MicroSystems, Inc. All Rights Reserved.
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

/**
 * @brief LIN constants connected to LIN hardware block.
 */
enum LinConstants
{
    eLIN_MAX_FRAME_SIZE =           0x08U,
    eLIN_MSG_MASK =                 0x3FU,
    eLIN_BUSIDLE_TIMEOUT_MASK =     0x1CU,
    eLIN_BUSIDLE_TIMEOUT_POSITION = 2U,

    // LIN parameters register bit(s) constants.
    eLIN_PARAMS_REG_LIN_VERSION =       0x10U,
    eLIN_PARAMS_REG_INTERBYTE_SPACE =   0x08U,
    eLIN_PARAMS_REG_NUMBYTES =          0x07U,

    // LIN bit constants
    eLIN_ENABLE_BIT =                   0x01U,
    eLIN_NOT_ENABLE_BIT =               0xFEU,
    eLIN_INTERRUPT_ENABLE_BIT =         0x02U,
    eLIN_NOT_INTERRUPT_ENABLE_BIT =     0xFDU,

    // Bus idle time constants
    eLIN_BUS_TIMEOUT_4S =  0U,
    eLIN_BUS_TIMEOUT_5S =  1U,
    eLIN_BUS_TIMEOUT_6S =  2U,
    eLIN_BUS_TIMEOUT_7S =  3U,
    eLIN_BUS_TIMEOUT_8S =  4U,
    eLIN_BUS_TIMEOUT_9S =  5U,
    eLIN_BUS_TIMEOUT_10S = 6U,
    
    eLIN_VERSION_1P3  = 1U,
    eLIN_VERSION_2P0  = 2U,

    // For LIN_CONTROL_REG
    eLIN_CNTRL_REG_RXTX_CNTRL =         0x03U,
    eLIN_TX_RESPONSE  =                    1U,
    eLIN_RX_RESPONSE  =                    2U,
    eLIN_MAX_CNTRL =                       4U,
    eLIN_WAKEUP_REQUEST_BIT =           0x04U,
    eLIN_NOT_WAKEUP_REQUEST_BIT =       0xFBU,
};

enum LinMessageIDs
{
    eLIN_DIAG_MASTER_REQ    = 60U,
    eLIN_DIAG_SLAVE_RESP    = 61U
};

/**
 * @brief Enumerates the status register bits
 */
enum StatusReg
{
    eLIN_STATUS_RESPONSE_ONGOING =    7,
    eLIN_STATUS_INTERRUPT =           6,
    eLIN_STATUS_SLEEPMODE =           5,
    eLIN_STATUS_WAKEUP_DETECT =       4,
    eLIN_STATUS_ERROR_FLAG =          3,
    eLIN_STATUS_RESPONSE_SENT =       2,
    eLIN_STATUS_RESPONSE_RECEIVED =   1,
    eLIN_STATUS_HEADER_RECEIVED =     0,
    eLIN_STATUS_CLEAR_FLAGS =         0x5F,
    eLIN_INVALID_STATUS_FLAG =        0xFF
};

/**
 * @brief Enumerates the LIN errors register
 */
enum ErrorsReg
{
    eLIN_ERROR_RESPONSE =        7,
    eLIN_ERROR_LOSTRESPONSE =    6,
    eLIN_ERROR_PARITY =          5,
    eLIN_ERROR_NO_RESPONSE =     4,
    eLIN_ERROR_FRAMING =         3,
    eLIN_ERROR_READBACK =        2,
    eLIN_ERROR_RESPONSE_SHORT =  1,
    eLIN_ERROR_CHECKSUM =        0,
    eLIN_INVALID_ERROR_FLAG =    0xFF
};

/**
 * @brief Enumerates the LIN interrupts that can be enabled for change in status
 */
enum IrqEnableStatus
{
    eLIN_IRQENABLE_ALL_FLAGS =         0x37,
    eLIN_IRQENABLE_GOTOSLEEP =         5,
    eLIN_IRQENABLE_WAKEUP_DETECT =     4,
    eLIN_IRQENABLE_RESPONSE_SENT =     2,
    eLIN_IRQENABLE_RESPONSE_RECEIVED = 1,
    eLIN_IRQENABLE_HEADER_RECEIVED =   0,
    eLIN_IRQENABLE_INVALID_FLAG =      0xFF
};

/**
 * @brief Enumerates the LIN interrupts that can be enabled for errors
 */
enum IrqEnableErrors
{
    eLIN_IRQENABLE_ERROR_ALL_FLAGS =     0x7F,
    eLIN_IRQENABLE_ERROR_LOSTRESPONSE =  6,
    eLIN_IRQENABLE_ERROR_PARITY =        5,
    eLIN_IRQENABLE_ERROR_NO_RESPONSE =   4,
    eLIN_IRQENABLE_ERROR_FRAMING =       3,
    eLIN_IRQENABLE_ERROR_READBACK =      2,
    eLIN_IRQENABLE_ERROR_RESPONSE_SHORT = 1,
    eLIN_IRQENABLE_ERROR_CHECKSUM =      0,
    eLIN_IRQENABLE_ERROR_INVALID_FLAG =  0xFF
};

// LIN register set
typedef struct
{
    uint32 Identifier;  // offset:  0x00U 
    uint32 Data1;       // offset:  0x04U 
    uint32 Data2;       // offset:  0x08U 
    uint32 Data3;       // offset:  0x0CU 
    uint32 Data4;       // offset:  0x10U   
    uint32 Data5;       // offset:  0x14U   
    uint32 Data6;       // offset:  0x18U        
    uint32 Data7;       // offset:  0x1CU 
    uint32 Data8;       // offset:  0x20U  
    uint32 MsgParams;   // offset:  0x24U 
    uint32 Control;     // offset:  0x28U 
    uint32 Status;      // offset:  0x2CU 
    uint32 Errors;      // offset:  0x30U 
    uint32 TBit;        // offset:  0x34U 
    uint32 Reserved;    // offset:  0x38U 
    uint32 CSum;        // offset:  0x3CU 
    uint32 Config;      // offset:  0x40U 
    uint32 StatusInten; // offset:  0x44U 
    uint32 ErrorInten;  // offset:  0x48U 
} LinRegs_t;

#define  stLinRegs (*( volatile LinRegs_t *)eADDRESS_LIN_BASE)

/**
 * @brief      Set LIN status register bit - the fast inline version.
 * @param[in]  eFlag StatusReg register bit number
 * @param[in]  kbState
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Lin_SetStatusRegFlagFast( enum StatusReg const eFlag, const boolean kbState )
{
    if ( eFlag != eLIN_INVALID_STATUS_FLAG )
    {
        if ( eFlag == eLIN_STATUS_CLEAR_FLAGS )
        {
            stLinRegs.Status = (uint32)( eLIN_STATUS_CLEAR_FLAGS );
        }
        else if ( ( eFlag != eLIN_STATUS_RESPONSE_ONGOING ) 
               && ( eFlag != eLIN_STATUS_SLEEPMODE ) 
               && ( eFlag != eLIN_STATUS_CLEAR_FLAGS ) 
               && ( eFlag != eLIN_INVALID_STATUS_FLAG ) )
        {
            uint32 const u32MaskBit =  (uint32)( (uint8)(1U << ((uint8)( eFlag ))) );
            if ( kbState == True )
            {
                stLinRegs.Status |= u32MaskBit;
            }
            else
            {
                stLinRegs.Status &= ~u32MaskBit;
            }
        }
        else {}
    }
}

/**
 * @brief      Get LIN status register bit - the fast inline version.
 * @param[in]  eFlag StatusReg register bit number
 * @returns    Status register bit value
 */
__attribute__( ( always_inline ) ) static inline boolean Lin_GetStatusRegFlagFast( enum StatusReg const eFlag )
{
    boolean bRetVal = False;
    if ( ( eFlag != eLIN_INVALID_STATUS_FLAG ) && ( eFlag != eLIN_STATUS_CLEAR_FLAGS ) )
    {
        uint32 const u32MaskBit =  (uint32)( (uint8)(1U << ((uint8)( eFlag ))) );
        if ( ( ( stLinRegs.Status ) & u32MaskBit ) != 0 )
        {
            bRetVal = True;
        }
    }
    return bRetVal;
}

/**
 * @brief      Gets LIN message identifier directly from the LIN register set - the fast inline version.
 * @param[in]  None
 * @returns    Value in LIN message identifier register (ro_identifer)
 */
__attribute__( ( always_inline ) ) static inline uint8 Lin_GetMessageIdentifierFast(void)
{
    uint32 u32MsgIdentifier = stLinRegs.Identifier;
    u32MsgIdentifier &= (uint32)( eLIN_MSG_MASK );
    const uint8 u8msgIdentifer = (uint8)( u32MsgIdentifier );
    return u8msgIdentifer;
}
   
/**
 * @brief      Set TxRx control bits in LIN register set - the fast inline version.
 * @param[in]  ku8Cntrl control setting
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Lin_SetRxTxControlFast( const uint8 ku8Cntrl )
{
    if ( ku8Cntrl < (uint8)( eLIN_MAX_CNTRL ) )
    {
        const uint32 u32RxTxCntrl = (uint32)( ku8Cntrl );
        uint32 u32RxTxCntrlMask = (uint32)( eLIN_CNTRL_REG_RXTX_CNTRL );
        u32RxTxCntrlMask = ~u32RxTxCntrlMask;
        stLinRegs.Control &= u32RxTxCntrlMask;
        stLinRegs.Control |= u32RxTxCntrl;
    }
}

/**
 * @brief      Sets number of response bytes in message in the LIN register set - the fast inline version.
 * @param[in]  ku8NumBytes Number of bytes
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Lin_SetNumResponseBytesFast( const uint8 ku8NumBytes )
{
    if ( ku8NumBytes <= (uint8)(eLIN_MAX_FRAME_SIZE) )
    {
        uint32 u32NumBytes = (uint32)( ku8NumBytes );
        u32NumBytes = ( u32NumBytes - 1U );
        uint32 u32NumBytesMask = (uint32)( eLIN_PARAMS_REG_NUMBYTES );
        u32NumBytesMask = ~u32NumBytesMask;
        stLinRegs.MsgParams &= u32NumBytesMask;
        stLinRegs.MsgParams |= u32NumBytes;
    }
}

/**
 * @brief      Sets up message (up to 8 bytes) for transmit in the LIN register set - the fast inline version.
 * @param[in]  kpu8Frame Message
 * @param[in]  ku8Length Length
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Lin_SetMessageFrameFast( const uint8* kpu8Frame, uint8 ku8Length )
{
    if ( ku8Length <= (uint8)(eLIN_MAX_FRAME_SIZE) )
    {
        volatile uint32* const pu32Dest = (volatile uint32* const)(&stLinRegs.Data1);
        uint8 const* const pu8Src =  kpu8Frame;
        uint8 u8i = 0U;
        while ( u8i < ku8Length )
        {
            pu32Dest[u8i] = pu8Src[u8i];
            u8i++;
        }
    }
}

/**
 * @brief      Enable LIN block
 * @param[in]  kbEnable - true to enable
 * @returns    None
 */
void Lin_SetEnable( const boolean kbEnable );

/**
 * @brief      Get LIN block enable
 * @param[in]  None
 * @returns    State
 */
boolean Lin_GetEnable( void );

/**
 * @brief      Set LIN Master interrupt enable
 * @param[in]  kbEnable - true to enable
 * @returns    None
 */
void Lin_SetMasterInt( const boolean kbEnable );

/**
 * @brief      Get LIN Master interrupt enable
 * @param[in]  None
 * @returns    State
 */
boolean Lin_GetMasterInt( void );

/**
 * @brief      Gets LIN message identifier directly from the LIN register set.
 * @param[in]  None
 * @returns    Value in LIN message identifier register (ro_identifer)
 */
uint8 Lin_GetMessageIdentifier( void );

/**
 * @brief      Sets up message (up to 8 bytes) for transmit in the LIN register set.
 * @param[in]  kpu8Frame Message
 * @param[in]  ku8Length Length
 * @returns    None
 */
void Lin_SetMessageFrame( const uint8* kpu8Frame, uint8 ku8Length );

/**
 * @brief      Gets message (of length defined by message parameter: nbytes) received in the LIN register set.
 * @param[out] kpu8Frame Message
 * @returns    None
 */
void Lin_GetMessageFrame( uint8* pu8Frame );

/**
 * @brief      Set LIN version parameter in the LIN register set. Default (=0) is LIN 2.0
 * @param[in]  ku8Version LIN version: 1.3 or 2.0
 * @returns    None
 */
void Lin_SetLinVersion( const uint8 ku8Version );

/**
 * @brief      Gets LIN version from LIN register set.
 * @param[in]  None
 * @returns    LIN version: 1.3 or 2.0
 */
uint8 Lin_GetLinVersion( void );

/**
 * @brief      Sets LIN inter-byte space in the LIN register set.
 * @param[in]  kbSpace If true for inter-byte space of 1 Tbit when transmitting. If set to false then no space.
 * @returns    None
 */
void Lin_SetInterByteSpace( const boolean kbSpace );

/**
 * @brief      Gets LIN inter-byte space from the LIN register set.
 * @param[in]  None
 * @returns    Inter-byte space from the LIN register set.
 */
boolean Lin_GetInterByteSpace( void );

/**
 * @brief      Sets number of response bytes in message in the LIN register set.
 * @param[in]  ku8NumBytes Number of bytes
 * @returns    None
 */
void Lin_SetNumResponseBytes( const uint8 ku8NumBytes );

/**
 * @brief      Gets number of response bytes in message from the LIN register set.
 * @param[in]  None
 * @returns    Number of response bytes in the message.
 */
uint8 Lin_GetNumResponseBytes( void );

/**
 * @brief      Set timeout in the LIN register set.
 * @param[in]  ku8TimeOut 3-bit constant for configuration register in range 28tbit (0x00) to 112tbit (0x06) .
 * @returns    None
 */
void Lin_SetBusIdleTimeOut( const uint8 ku8TimeOut );

/**
 * @brief      Get timeout in the LIN register set.
 * @param[in]  None
 * @returns    3-bit constant for configuration register in range 28 tbit (0x00) to 112 tbit (0x06) .
 */
uint8 Lin_GetBusIdleTimeOut( void );

/**
 * @brief      Sets wake-up request bit in the LIN register set.
 * @param[in]  kbWakeup Wakeup: true or false
 * @returns    None
 */
void Lin_SetWakeUpRequest( const boolean kbWakeup );

/**
 * @brief      Get wake-up request bit from the LIN register set.
 * @param[in]  None
 * @returns    Wake-up request bit from the LIN register set.
 */
boolean Lin_GetWakeUpRequest( void );

/**
 * @brief      Set TxRx control bits in LIN register set.
 * @param[in]  ku8Cntrl control setting
 * @returns    None
 */
void Lin_SetRxTxControl( const uint8 ku8Cntrl );

/**
 * @brief      Get TxRx control setting
 * @param[in]  None
 * @returns    TxRx control setting
 */
uint8 Lin_GetRxTxControl( void );

/**
 * @brief      Set LIN status register bit
 * @param[in]  eFlag StatusReg register bit number
 * @param[in]  kbState
 * @returns    None
 */
void Lin_SetStatusRegFlag( enum StatusReg const eFlag, const boolean kbState );

/**
 * @brief      Get LIN status register bit
 * @param[in]  eFlag StatusReg register bit number
 * @returns    Status register bit value
 */
boolean Lin_GetStatusRegFlag( enum StatusReg const eFlag );

/**
 * @brief      Read LIN errors register: all 8-bits
 * @param[in]  None
 * @returns    Error register
 */
uint8 Lin_GetErrorRegister( void );

/**
 * @brief      Set LIN errors register bit
 * @param[in]  eFlag Error register bit number
 * @param[in]  kbState State
 * @returns    None
 */
void Lin_SetErrorRegFlag( enum ErrorsReg const eFlag, const boolean kbState );

/**
 * @brief      Get LIN errors register bit
 * @param[in]  eFlag Error register bit number
 * @returns    Error register bit value
 */
boolean Lin_GetErrorRegFlag( enum ErrorsReg const eFlag );

/**
 * @brief      Set LIN IRQ enable status register bit
 * @param[in]  eFlag Interrupt enable status register bit
 * @param[in]  kbState State
 */
void Lin_SetIrqEnableStatusRegister( enum IrqEnableStatus const eFlag, const boolean kbState );

/**
 * @brief      Get LIN IRQ enable status register bit
 * @param[in]  eFlag Interrupt enable status register bit
 * @returns    Status register bit value
 */
boolean Lin_GetIrqEnableStatusRegister( enum IrqEnableStatus const eFlag );

/**
 * @brief      Set LIN IRQ enable errors register bit
 * @param[in]  eFlag Interrupt enable errors register bit
 * @param[in]  kbState State
 * @returns    None
 */
void Lin_SetIrqEnableErrorsRegister( enum IrqEnableErrors const eFlag, const boolean kbState );

/**
 * @brief      Get LIN IRQ Enable errors register bit
 * @param[in]  eFlag Interrupt enable errors register bit
 * @returns    Errors register bit value
 */
boolean Lin_GetIrqEnableErrorsRegister( enum IrqEnableErrors const eFlag );

/**
 * @brief      Get LIN bitrate detected by hardware
 * @param[in]  None
 * @returns    Bit rate
 */
uint16 Lin_GetLinBitRate( void );

/**
 * @brief      Get the checksum of the transmitted frame
 * @param[in]  None
 * @returns    Checksum
 */
uint8 Lin_GetTxChecksum( void );

/**
 * @brief Set Lin IRQ handler
 * @param LinIrqHandler An IRQ handler
 */
void Lin_SetIrqHandler( void (LinIrqHandler)( void ) );

/**
 * @brief Handler for LIN IRQ.
 */
void Lin_IrqHandler( void );

#endif //HEADER_LIN_H
