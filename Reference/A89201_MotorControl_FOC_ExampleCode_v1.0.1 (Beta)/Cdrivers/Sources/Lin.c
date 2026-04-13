/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/*
 * @file         Lin.c
 * @author       M. Shankey
 * @version      1.0
 *
 * @brief        LIN interface: LIN
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

#include "ARMCM4_FP.h"

#include "Std_Types.h"
#include "Lin.h"

static IrqType_t const skIrqLin = ARM_IRQ_NUM(eIRQNUM_LIN);

/**
 * @internal
 *
 * @details
 */
void Lin_SetEnable( const boolean kbEnable )
{
    if ( kbEnable == True )
    {
        stLinRegs.Config |= (uint32)( eLIN_ENABLE_BIT );
    }
    else
    {
        stLinRegs.Config &= (uint32)( eLIN_NOT_ENABLE_BIT );
    }
}

/**
 * @internal
 *
 * @details
 */
boolean Lin_GetEnable( void )
{
    boolean bEnable = False;
    if ( ( ( stLinRegs.Config ) & (uint32)( eLIN_ENABLE_BIT ) ) != 0 )
    {
        bEnable = True;
    }
    return bEnable;
}

/**
 * @internal
 *
 * @details
 */
void Lin_SetMasterInt( const boolean kbEnable )
{
    if ( kbEnable == True )
    {
         stLinRegs.Config |= (uint32)( eLIN_INTERRUPT_ENABLE_BIT );
    }
    else
    {
         stLinRegs.Config &= (uint32)( eLIN_NOT_INTERRUPT_ENABLE_BIT );
    }
}

/**
 * @internal
 *
 * @details
 */
boolean Lin_GetMasterInt( void )
{
    boolean bEnable = False;
    if ( ( ( stLinRegs.Config ) & (uint32)( eLIN_INTERRUPT_ENABLE_BIT ) ) != 0 )
    {
        bEnable = True;
    }
    return bEnable;
}

/**
 * @internal
 *
 * @details
 */
uint8 Lin_GetMessageIdentifier( void )
{
    uint32 u32MsgIdentifier = stLinRegs.Identifier;
    u32MsgIdentifier &= (uint32)( eLIN_MSG_MASK );
    const uint8 u8msgIdentifer = (uint8)( u32MsgIdentifier );
    return u8msgIdentifer;
}

/**
 * @internal
 *
 * @details
 */
void Lin_SetMessageFrame( const uint8* kpu8Frame, uint8 ku8Length )
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
 * @internal
 *
 * @details
 */
void Lin_GetMessageFrame( uint8* pu8Frame )
{
    uint8 u8Length = Lin_GetNumResponseBytes();
    u8Length++;
    uint8* const pu8dest = pu8Frame;
    volatile uint32* const pu32src = (volatile uint32* const)(&stLinRegs.Data1);
    uint8 u8i = 0U;
    while ( u8i < u8Length )
    {
        pu8dest[u8i] = (uint8)pu32src[u8i];
        u8i++;
    }
}

/**
 * @internal
 *
 * @details
 */
void Lin_SetLinVersion( const uint8 ku8Version )
{
    if ( ku8Version == (uint8)(eLIN_VERSION_1P3) ) 
    {
        stLinRegs.MsgParams |=  (uint32)( eLIN_PARAMS_REG_LIN_VERSION );
    }
    else
    {
        uint32 u32LinVersionBit = (uint32)( eLIN_PARAMS_REG_LIN_VERSION );
        u32LinVersionBit = ~u32LinVersionBit;
        stLinRegs.MsgParams &= u32LinVersionBit;
    }
}

/**
 * @internal
 *
 * @details
 */
uint8 Lin_GetLinVersion( void )
{
    uint8 u8RetVal;

    uint32 u32LinVer = stLinRegs.MsgParams;
    u32LinVer &= (uint32)( eLIN_PARAMS_REG_LIN_VERSION );
    if ( u32LinVer != 0U )
    {
        u8RetVal = (uint8)( eLIN_VERSION_1P3 );
    }
    else
    {
        u8RetVal = (uint8)( eLIN_VERSION_2P0 );
    }
    return u8RetVal;
}

/**
 * @internal
 *
 * @details
 */
void Lin_SetInterByteSpace( const boolean kbSpace )
{
    if ( kbSpace == True )
    {
        stLinRegs.MsgParams |= (uint32)( eLIN_PARAMS_REG_INTERBYTE_SPACE );
    }
    else
    {
        uint32 u32InterByteSpaceBit = (uint32)( eLIN_PARAMS_REG_INTERBYTE_SPACE );
        u32InterByteSpaceBit = ~u32InterByteSpaceBit;
        stLinRegs.MsgParams &= u32InterByteSpaceBit;
    }
}

/**
 * @internal
 *
 * @details
 */
boolean Lin_GetInterByteSpace( void )
{
    boolean bRetVal = False;
    uint32 u32InterByteSpace = stLinRegs.MsgParams;
    u32InterByteSpace &= (uint32)( eLIN_PARAMS_REG_INTERBYTE_SPACE );
    if ( u32InterByteSpace != 0U )
    {
        bRetVal = True;
    }
    return bRetVal;
}

/**
 * @internal
 *
 * @details
 */
void Lin_SetNumResponseBytes( const uint8 ku8NumBytes )
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
 * @internal
 *
 * @details
 */
uint8 Lin_GetNumResponseBytes( void )
{
    uint32 u32NumBytes32 = stLinRegs.MsgParams;
    const uint32 u32NumBytesMask = (uint32)( eLIN_PARAMS_REG_NUMBYTES );
    u32NumBytes32 &= u32NumBytesMask;
    const uint8 ku8NumBytes = (uint8)( u32NumBytes32 );
    return ( ku8NumBytes );
}


/**
 * @internal
 *
 * @details
 */
void Lin_SetBusIdleTimeOut( const uint8 ku8TimeOut )
{
    uint32 u32TimeOut = (uint32)( ku8TimeOut );
    const uint32 u32TimeOutLim = (uint32)( (uint8)(eLIN_BUS_TIMEOUT_10S) + 1U );

    if ( u32TimeOut < u32TimeOutLim )
    {
        uint32 busIdleTimeOutMask = (uint32)( eLIN_BUSIDLE_TIMEOUT_MASK );
        busIdleTimeOutMask = ~busIdleTimeOutMask;
        stLinRegs.Config &= busIdleTimeOutMask;
        u32TimeOut = (u32TimeOut << ((uint8)(eLIN_BUSIDLE_TIMEOUT_POSITION)));
        stLinRegs.Config |= u32TimeOut;
    }
}

/**
 * @internal
 *
 * @details
 */
uint8 Lin_GetBusIdleTimeOut( void )
{
    uint32 u32TimeOut = stLinRegs.Config;
    u32TimeOut &= (uint32)( eLIN_BUSIDLE_TIMEOUT_MASK );
    u32TimeOut = u32TimeOut >> ((uint8)( eLIN_BUSIDLE_TIMEOUT_POSITION ));
    const uint8 u8TimeOut8 = (uint8)( u32TimeOut );
    return( u8TimeOut8 );
}

/**
 * @internal
 *
 * @details
 */
void Lin_SetWakeUpRequest( const boolean kbWakeup )
{
    if ( kbWakeup == True )
    {
        stLinRegs.Control |= (uint32)( eLIN_WAKEUP_REQUEST_BIT );
    }
    else
    {
        stLinRegs.Control &= (uint32)( eLIN_NOT_WAKEUP_REQUEST_BIT );
    }
}

/**
 * @internal
 *
 * @details
 */
boolean Lin_GetWakeUpRequest( void )
{
    boolean bWakeup = False;
    if ( ( ( stLinRegs.Control ) & (uint32)( eLIN_WAKEUP_REQUEST_BIT ) ) != 0 )
    {
        bWakeup = True;
    }
    return bWakeup;
}

/**
 * @internal
 *
 * @details
 */
void Lin_SetRxTxControl( const uint8 ku8Cntrl )
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
 * @internal
 *
 * @details
 */
uint8 Lin_GetRxTxControl( void )
{
    uint32 u32RxTxCntrl = stLinRegs.Control;
    const uint32 rxTxCntrlMask32 = (uint32)( eLIN_CNTRL_REG_RXTX_CNTRL );
    u32RxTxCntrl &= rxTxCntrlMask32;
    const uint8 rxTxCntrl = (uint8)( u32RxTxCntrl );
    return rxTxCntrl;
}


/**
 * @internal
 *
 * @details
 */
void Lin_SetStatusRegFlag( enum StatusReg const eFlag, const boolean kbState )
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
 * @internal
 *
 * @details
 */
boolean Lin_GetStatusRegFlag( enum StatusReg const eFlag )
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
 * @internal
 *
 * @details
 */
uint8 Lin_GetErrorRegister( void )
{
    return (uint8)( stLinRegs.Errors ); 
}


/**
 * @internal
 *
 * @details
 */
void Lin_SetErrorRegFlag( enum ErrorsReg const eFlag, const boolean kbState )
{
    if ( eFlag != eLIN_INVALID_ERROR_FLAG )
    {
        uint32 const u32MaskBit =  (uint32)( (uint8)(1U << ((uint8)( eFlag ))) );
        if ( kbState == True )
        {
            stLinRegs.Errors |= u32MaskBit;
        }
        else
        {
            stLinRegs.Errors &= ~u32MaskBit;
        }
    }
}

/**
 * @internal
 *
 * @details
 */
boolean Lin_GetErrorRegFlag( enum ErrorsReg const eFlag )
{
    boolean bRetVal = False;
    if ( eFlag != eLIN_INVALID_ERROR_FLAG ) 
    {
        uint32 const u32MaskBit =  (uint32)( (uint8)(1U << ((uint8)( eFlag ))) );
        if ( ( ( stLinRegs.Errors ) & u32MaskBit ) != 0 )
        {
            bRetVal = True;
        }
    }
    return bRetVal;
}

/**
 * @internal
 *
 * @details
 */
void Lin_SetIrqEnableStatusRegister( enum IrqEnableStatus const eFlag, const boolean kbState )
{
    if( eFlag != eLIN_IRQENABLE_INVALID_FLAG )
    {
        if ( eFlag == eLIN_IRQENABLE_ALL_FLAGS )
        {
            if ( kbState == True )
            {
                stLinRegs.StatusInten = (uint32)( eLIN_IRQENABLE_ALL_FLAGS );
            }
            else
            {
                stLinRegs.StatusInten = 0;
            }
        }
        else
        {
            uint32 const u32MaskBit =  (uint32)( (uint8)(1U << ((uint8)( eFlag ))) );
            if ( kbState == True )
            {
                stLinRegs.StatusInten |= u32MaskBit;
            }
            else
            {
                stLinRegs.StatusInten &= ~u32MaskBit;
            }
        }
    }
}

/**
 * @internal
 *
 * @details
 */
boolean Lin_GetIrqEnableStatusRegister( enum IrqEnableStatus const eFlag )
{
    boolean bRetVal = False;
    if ( (eFlag != eLIN_IRQENABLE_INVALID_FLAG) && (eFlag != eLIN_IRQENABLE_ALL_FLAGS ) )
    {
        uint32 const u32MaskBit =  (uint32)( (uint8)(1U << ((uint8)( eFlag ))) );
        if ( ( stLinRegs.StatusInten & u32MaskBit ) != 0 )
        {
            bRetVal = True;
        }
    }
    return bRetVal;
}

/**
 * @internal
 *
 * @details
 */
void Lin_SetIrqEnableErrorsRegister( enum IrqEnableErrors const eFlag, const boolean kbState )
{
    if ( eFlag != eLIN_IRQENABLE_ERROR_INVALID_FLAG )
    {
        if ( eFlag == eLIN_IRQENABLE_ERROR_ALL_FLAGS )
        {
            if ( kbState == True )
            {
                stLinRegs.ErrorInten = (uint32)( eLIN_IRQENABLE_ERROR_ALL_FLAGS );
            }
            else
            {
                stLinRegs.ErrorInten = 0;
            }
        }
        else
        {
            uint32 const u32MaskBit =  (uint32)( (uint8)(1U << ((uint8)( eFlag ))) );            
            if ( kbState == True )
            {
                stLinRegs.ErrorInten |= u32MaskBit;
            }
            else
            {
                stLinRegs.ErrorInten &= ~u32MaskBit;
            }
        }
    }
}

/**
 * @internal
 *
 * @details
 */
boolean Lin_GetIrqEnableErrorsRegister( enum IrqEnableErrors const eFlag )
{
    boolean bRetVal = False;
    if ( eFlag != eLIN_IRQENABLE_ERROR_INVALID_FLAG )
    {
        uint32 const u32MaskBit =  (uint32)( (uint8)(1U << ((uint8)( eFlag ))) );
        if ( ( ( stLinRegs.ErrorInten ) & u32MaskBit ) != 0 )
        {
            bRetVal = True;
        }
    }
    return bRetVal;
}


/**
 * @internal
 *
 * @details
 */
uint16 Lin_GetLinBitRate( void )
{
    uint16 const u16LinBitRate = (uint16)(stLinRegs.TBit);
    return u16LinBitRate;
}

/**
 * @internal
 *
 * @details
 */
uint8 Lin_GetTxChecksum( void )
{
    const uint32 u32ChecksumReg = stLinRegs.CSum;
    const uint8 u8ChecksumReg = (uint8)( u32ChecksumReg );
    return u8ChecksumReg;
}



// Declare a function pointer for use by LIN Irq
static void (*fpLinIrqHandler)( void ) = NULLPTR;

/**
 * @internal Set the LIN Irq Handler to required function
 *
 * @details
 */
void Lin_SetIrqHandler( void (LinIrqHandler)( void ) )
{
    fpLinIrqHandler = *LinIrqHandler;
}

/**
 * @internal Execute the required LIN interrupt function 
 *
 * @details
 */
void Lin_IrqHandler( void )
{
    NVIC_DisableIRQ( skIrqLin );
    NVIC_ClearPendingIRQ( skIrqLin );

    if( fpLinIrqHandler != NULLPTR )
    {
        fpLinIrqHandler();
    }
    NVIC_EnableIRQ( skIrqLin );
}
