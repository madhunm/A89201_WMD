/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/*
 * @file         Dau.c
 * @author       N. Moheeputh
 * @version      1.0
 *
 * @brief        Data Acquisition Unit interface: DAU 
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
#include "ARMCM4_FP.h"

#include "Std_Types.h"
#include "Dau.h"

void Dau_ClearControlReg( void )
{
    stDauRegs.control = 0;
}

void Dau_SetControlMode( enum ControlMode const keControlMode )
{
    stDauRegs.control &= (uint32) eRESET_MODE;
    stDauRegs.control |= (uint32) keControlMode;
}

void Dau_SetControlTrigEdgeSel( enum ControlTrigEdgeSel const keControlTrigEdgeSel )
{
    stDauRegs.control &= (uint32) eRESET_TRIG_EDGE_SEL;
    stDauRegs.control |= (uint32) keControlTrigEdgeSel;
}

void Dau_SetControlTrigSel( enum ControlTrigSel const keControlTrigSel )
{
    stDauRegs.control &= (uint32) eRESET_TRIG_SEL;
    stDauRegs.control |= (uint32) keControlTrigSel;
}

void Dau_SetControlCnvSpeed(enum ControlCnvSpeed const keControlCnvSpeed )
{
    stDauRegs.control &= (uint32) eRESET_CNV_SPEED;
    stDauRegs.control |= (uint32) keControlCnvSpeed;
}

void  Dau_SetControlExtRef( enum ControlExtRef const keControlExtRef )
{
    if ( keControlExtRef == eINT )
    {
        stDauRegs.control &= (uint32) eINT;

    }
    else
    {
        stDauRegs.control |= (uint32) eEXT;
    }
}

void Dau_SetControlExtGnd( enum ControlExtGnd const keControlExtGnd )
{
    if ( keControlExtGnd == eGND_INT )
    {
        stDauRegs.control &= (uint32) eGND_INT;
    }                                    
    else                                 
    {                                    
        stDauRegs.control |= (uint32) eGND_EXT;
    }
}

void Dau_SetControlDauEnable( enum ControlDauEnable const keControlDauEnable )
{
    if ( keControlDauEnable == eDISABLE_DAU )
    {
        stDauRegs.control &= (uint32) eDISABLE_DAU;
    }                           
    else                        
    {                           
        stDauRegs.control |= (uint32) eENABLE_DAU;
    }
}

uint32 Dau_GetControlReg( void )
{
    return stDauRegs.control;
}

void Dau_SetSwTrig( enum SwTrig const keSwTrig )
{
    stDauRegs.swtrig = (uint32) keSwTrig;
}

enum SwTrig Dau_GetSwTrig( void )
{
    return ( (enum SwTrig)( stDauRegs.swtrig ) );
}

void Dau_ClearSlotConfig( enum ConfigSlot const keAddrOffset )
{
    
    volatile uint32* const pu32SlotReg = { &stDauRegs.slot0Config + (uint32)( keAddrOffset ) };
    *pu32SlotReg = 0;
}

void Dau_SetSlotConfigChannel( enum ConfigSlot const keAddrOffset, enum ConfigChannel const keConfigChannel )
{
    volatile uint32* const pu32SlotReg = { &stDauRegs.slot0Config + (uint32)( keAddrOffset ) };

    *pu32SlotReg &= (uint32)( eCHANNEL_RESET );
    *pu32SlotReg |= (uint32)( keConfigChannel );
}

void Dau_SetSlotConfigSlotEnable( enum ConfigSlot const keAddrOffset, enum ConfigSlotEnable const keConfigSlotEnable)
{       
    volatile uint32* const pu32SlotRegEnable = { &stDauRegs.slot0Config + (uint32)( keAddrOffset ) };

    if ( keConfigSlotEnable == eSLOT_DISABLE )
    {
        *pu32SlotRegEnable &= (uint32) eSLOT_DISABLE;
    }                                 
    else                              
    {                                 
        *pu32SlotRegEnable |= (uint32) keConfigSlotEnable;
    }
}

void Dau_SetSlotConfigSAHPeriod( enum ConfigSlot const keAddrOffset, enum ConfigSAHPeriod const keConfigSAHPeriod )
{
    volatile uint32* const pu32SlotConfigRegSah = { &stDauRegs.slot0Config + (uint32)( keAddrOffset ) };

    *pu32SlotConfigRegSah &= (uint32) ( eRESET_SAH_PERIOD );
    *pu32SlotConfigRegSah |= (uint32) ( keConfigSAHPeriod );
}

uint16 Dau_GetSlotConfig( enum ConfigSlot const keAddrOffset )
{     
    volatile uint32* const pu32SlotReg = { &stDauRegs.slot0Config + (uint32)( keAddrOffset ) };
    return (uint16)( *pu32SlotReg );
}

void Dau_SetErrorStatus( enum ErrorStatus const keErrorStatus )
{
    // Eliminate Overrun bit as it's read only
    uint32 const u32ErrorStatusMasked = { ((uint32) keErrorStatus ) & (~((uint32) eOVERRUN_STATUS ) ) };
    
    //Read the register and OR in bit/bits to reset: they are R/1C
    uint32 u32ErrorStatus = { ( stDauRegs.errorStatus ) };
    u32ErrorStatus |= ( u32ErrorStatusMasked );
    stDauRegs.errorStatus = ( u32ErrorStatus );
}

boolean Dau_GetErrorStatus( enum ErrorStatus const keErrorStatus )
{
    boolean bRetVal ={ False };
    uint32 const u32ErrorStatus = { ( stDauRegs.errorStatus ) };
    if ( ( u32ErrorStatus & (uint32) keErrorStatus) != 0 )
    {
        bRetVal = True;
    }
    return bRetVal;
}

void Dau_SetErrStatusIrqEnable( enum IrqErrorEnable const keIrqErrEnable )
{
    if ( keIrqErrEnable == eNONE_IRQ )
    {
        stDauRegs.errorStatusInten = 0;
    }
    else
    {
        stDauRegs.errorStatusInten |= (uint32) keIrqErrEnable;
    }
}

enum IrqErrorEnable Dau_GetErrStatusIrqEnable( void )
{
    return (enum IrqErrorEnable)( stDauRegs.errorStatusInten );
}

void Dau_SetOverRunStatus( enum OverRunStatus const keOverRunStatus )
{
    if ( keOverRunStatus == eNONE_OVR_RUN_STATUS )
    {
        stDauRegs.ovrStatus = 0;
    }
    else
    {
        stDauRegs.ovrStatus |= (uint32) keOverRunStatus;
    }
}

boolean Dau_GetOverRunStatus( enum OverRunStatus const keOverRunStatus )
{
    boolean bRetVal = { False };
    uint32 const u32OverRunStatus = { ( stDauRegs.ovrStatus ) };
    if ( ( u32OverRunStatus & (uint32) keOverRunStatus) != 0 )
    {
        bRetVal = True;
    }
    return bRetVal;
}

void Dau_SetOverRunEnable(enum OverRunEnable const keOverRunEnable )
{
    if ( keOverRunEnable == eNONE_OVR_RUN_ENABLE )
    {
        stDauRegs.ovrStatusInten = 0;
    }
    else
    {
        stDauRegs.ovrStatusInten |= (uint32) keOverRunEnable;
    }
}

enum OverRunEnable Dau_GetOverRunEnable( void )
{
    return (enum OverRunEnable)( stDauRegs.ovrStatusInten );
}

enum ConversionStatus Dau_GetConversionStatus( void )
{
    
    return ( enum ConversionStatus)( stDauRegs.conversionStatus );
}

void Dau_SetConvCompStatus( enum ConvCompStatus const keConvCompStatus )
{
    if ( keConvCompStatus == eCLEAR_ALL )
    {
        stDauRegs.convCompStatus = (~((uint32) eCLEAR_ALL ));
    }
    else
    {
        stDauRegs.convCompStatus |= (uint32) keConvCompStatus;
    }
}

boolean Dau_GetConvCompStatus( enum ConvCompStatus const keConvCompStatus )
{
    boolean bRetVal = { False };
    uint32 const u32ConvCompStatus = ( stDauRegs.convCompStatus );
    if ( ( u32ConvCompStatus & (uint32) keConvCompStatus) != 0)
    {
        bRetVal = True;
    }
    return bRetVal;
}

void Dau_SetConvCompIrqEnable( enum ConvCompIrqEnable const keConvCompIrqEnable )
{
    if ( keConvCompIrqEnable == eNONE_CONV_COMP_IRQ_EN )
    {
        stDauRegs.convCompStatusInten = 0;
    }
    else
    {
        stDauRegs.convCompStatusInten |= (uint32) keConvCompIrqEnable;
    }
}

enum ConvCompIrqEnable Dau_GetConvCompIrqEnable( void )
{
    return (enum ConvCompIrqEnable)( stDauRegs.convCompStatusInten );
}

uint32 Dau_GetSlotResult( enum ResultSlotSelect const keAddrOffset )
{
    volatile uint32* const pu32SlotResult =  &(stDauRegs.result0) + (uint32)keAddrOffset;
    return ( *pu32SlotResult );
}

void Dau_SetDma( uint16 const ku16DmaAcquisitions )
{
    stDauRegs.DmaReqNum = ( ku16DmaAcquisitions );
}

uint32 Dau_GetDma( void )
{
    return ( stDauRegs.DmaReqNum );
}

void Dau_SetDmaEnable( enum DmaEnable const keDmaEnable )
{
    if ( keDmaEnable == eNONE_DMA_EN )
    {
       stDauRegs.DmaReqEn = 0;
    }
    else
    {
        stDauRegs.DmaReqEn |= (uint32) keDmaEnable;
    }
}

enum DmaEnable Dau_GetDmaEnable( void )
{
    return (enum DmaEnable)( stDauRegs.DmaReqEn );
}

// Declare a function pointer for use by DAU Irq
static void (*fpDauIrqHandler)( void ) = NULLPTR;

void Dau_SetIrqHandler( void (DauIrqHandler)( void ))
{
    fpDauIrqHandler = *DauIrqHandler;
}

void Dau_IrqHandler( void )
{
    NVIC_DisableIRQ((IRQn_Type) eIRQNUM_DAU );
    NVIC_ClearPendingIRQ((IRQn_Type) eIRQNUM_DAU );

    if ( fpDauIrqHandler != NULLPTR )
    {
        fpDauIrqHandler();
    }
    NVIC_EnableIRQ((IRQn_Type)eIRQNUM_DAU );

}
