/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/*
 * @file         Amct.c
 * @author       M. Shankey
 * @version      1.0
 *
 * @brief        Advanced Motor Control Timer interface: AMCT
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
#include "Amct.h"


static IrqType_t const skIrqAmct = ARM_IRQ_NUM(eIRQNUM_AMCT);

void Amct_ResetConfig( void )
{
    stAmctRegs.Config = 0;
}


void Amct_SetConfigModeSelect( enum AmctConfigModeSelect const keConfigModeSel )
{
    if ( keConfigModeSel == eAMCT_CONFIG_BEMF )
    {
        stAmctRegs.Config &= (uint32)(eAMCT_CONFIG_BEMF);
    }
    else
    {
        stAmctRegs.Config |=  (uint32)(eAMCT_CONFIG_HALL);
    }
}


void Amct_SetConfigMotorDir( enum AmctConfigMotorDirection const keConfigControlMotorDir )
{
    if ( keConfigControlMotorDir == eAMCT_CONFIG_RVS )
    {
        stAmctRegs.Config &= (uint32)(eAMCT_CONFIG_RVS);
    }
    else
    {
        stAmctRegs.Config |= (uint32)(eAMCT_CONFIG_FWD);
    }
}


void Amct_SetConfigDegaussFilterBypass( enum AmctConfigDegaussFilterBypass const keConfigDegaussFilterBypass )
{
    if ( keConfigDegaussFilterBypass == eAMCT_CONFIG_NO_BYPASS )
    {
        stAmctRegs.Config &=  (uint32)(eAMCT_CONFIG_NO_BYPASS);
    }
    else
    {
        stAmctRegs.Config |= (uint32)(eAMCT_CONFIG_BYPASS);
    }
}


void Amct_SetConfigPhaseDisableSelect( enum AmctConfigPhaseDisableSelect const keConfigPhaseDisableSelect )
{
    if ( keConfigPhaseDisableSelect == eAMCT_CONFIG_CMD )
    {
        stAmctRegs.Config &= (uint32)(eAMCT_CONFIG_CMD);
    }
    else
    {
        stAmctRegs.Config |= (uint32)(eCPU_CONFIG_CMD);
    }
}


void Amct_SetConfigHallASelect( enum AmctConfigHallASelect const keConfigHallASelect )
{
    stAmctRegs.Config &= (uint32)(eAMCT_CONFIG_HALLASELECT_MASK);
    stAmctRegs.Config |= (uint32)(keConfigHallASelect);
}


void Amct_SetConfigHallBSelect( enum AmctConfigHallBSelect const keConfigHallBSelect )
{
    stAmctRegs.Config &= (uint32)(eAMCT_CONFIG_HALLBSELECT_MASK);
    stAmctRegs.Config |= (uint32)(keConfigHallBSelect);
}


void Amct_SetConfigHallCSelect( enum AmctConfigHallCSelect const keConfigHallCSelect )
{
    stAmctRegs.Config &= (uint32)( eAMCT_CONFIG_HALLCSELECT_MASK );
    stAmctRegs.Config |= (uint32)( keConfigHallCSelect );
}


void Amct_SetConfigIPDTriggerSelect( enum AmctConfigIPDTriggerSelect const keConfigIPDTriggerSelect )
{
    stAmctRegs.Config &= (uint32)( eAMCT_CONFIG_IPDTRIGGERSELECT_MASK );
    stAmctRegs.Config |= (uint32)( keConfigIPDTriggerSelect );
}


void Amct_SetConfigIPDComparatorSelect( enum AmctConfigIPDComparatorSelect const keConfigIPDComparatorSelect )
{
    stAmctRegs.Config &= (uint32)( eAMCT_CONFIG_IPDCOMPARATORSELECT_MASK );
    stAmctRegs.Config |= (uint32)( keConfigIPDComparatorSelect );
}


void Amct_SetConfigIPDUpdateModeSelect( enum AmctConfigIPDUpdateModeSelect const keConfigIPDUpdateModeSelect )
{
    if ( keConfigIPDUpdateModeSelect == eAMCT_CONFIG_RESULT_NO_IRQ )
    {
        stAmctRegs.Config &= (uint32)( keConfigIPDUpdateModeSelect );
    }
    else
    {
        stAmctRegs.Config |= (uint32)( keConfigIPDUpdateModeSelect );
    }
}


uint32 Amct_GetConfig( void )
{
    return stAmctRegs.Config;
}


void Amct_SetCommand( enum AmctCommand const keCommand )
{
    uint32 u32CommandValue = stAmctRegs.Command;

    switch ( keCommand )
    {
        case eAMCT_COMMAND_CLEAR_ALL:
        case eAMCT_COMMAND_MASTER_DISABLE:
        case eAMCT_COMMAND_IPD_DISABLE:
        case eAMCT_COMMAND_CPD_DISABLE:
        case eAMCT_COMMAND_HALL_DISABLE:
        case eAMCT_COMMAND_BEMF_DISABLE:
            u32CommandValue &= (uint32)(keCommand);
        break;

        case eAMCT_COMMAND_MASTER_ENABLE:
        case eAMCT_COMMAND_IPD_ENABLE:
        case eAMCT_COMMAND_CPD_ENABLE:
        case eAMCT_COMMAND_HALL_ENABLE:
        case eAMCT_COMMAND_BEMF_ENABLE:
            u32CommandValue |= (uint32)(keCommand);
        break;

    default:    // No update
        break;
    }

    stAmctRegs.Command = u32CommandValue;
}


uint16 Amct_GetCommand( void )
{
    return (uint16)( stAmctRegs.Command );
}


void Amct_SetPhaseDisable( enum  AmctPhaseDisable const kePhaseDisable )
{
    if ( kePhaseDisable == eAMCT_PHASEDISABLE_NONE )
    {
        stAmctRegs.PhaseDisable = 0;
    }
    else
    {
        stAmctRegs.PhaseDisable |= (uint32)( kePhaseDisable );
    }
}


uint16 Amct_GetPhaseDisable( void )
{
    return (uint16)( stAmctRegs.PhaseDisable );
}



void Amct_SetHallFilterTime( uint16 const ku16HallFilterTime )
{
    stAmctRegs.HallFilterTime = (uint32)( ku16HallFilterTime );
}


uint16 Amct_GetHallFilterTime( void )
{
    return (uint16)( stAmctRegs.HallFilterTime );
}


void Amct_SetHallPhaseShift( sint32 const ks32HallPhaseShift )
{
    stAmctRegs.HallPhaseShift = ks32HallPhaseShift;
}


sint32 Amct_GetHallPhaseShift( void )
{
    return stAmctRegs.HallPhaseShift;
}


void Amct_SetAngleStep( uint32 const ku32AngleStep )
{
    stAmctRegs.AngleStep = ku32AngleStep;
}


uint32 Amct_GetAngleStep( void )
{
    return( stAmctRegs.AngleStep );
}


void Amct_SetHallFreqLimit( uint32 const ku32HallFreqLimit )
{
    stAmctRegs.HallFreqLimit = ku32HallFreqLimit;
}


uint32 Amct_GetHallFreqLimit( void )
{
    return( stAmctRegs.HallFreqLimit );
}


void Amct_SetApplyAngleCorrection( enum AmctApplyAngleCorrection const keApplyAngleCorrection )
{
    if ( keApplyAngleCorrection == eAMCT_SET_HIGH )
    {
        stAmctRegs.ApplyAngleCorrection |= (uint32)(keApplyAngleCorrection);
    }
    else
    {
        stAmctRegs.ApplyAngleCorrection &= (uint32)(keApplyAngleCorrection);
    }
}


uint16 Amct_GetApplyAngleCorrection( void )
{
    return (uint16)(stAmctRegs.ApplyAngleCorrection );
}


void Amct_SetAngleCorrectionFactor( uint32 const ku32AngleCorrectionFactor )
{
    stAmctRegs.AngleCorrectionFactor = ku32AngleCorrectionFactor;
}


uint32 Amct_GetAngleCorrectionFactor( void )
{
    return stAmctRegs.AngleCorrectionFactor;
}


void Amct_SetInitialAngle( uint32 const ku32InitialAngle )
{
    stAmctRegs.InitialAngle = ku32InitialAngle;
}


uint32 Amct_GetInitialAngle( void )
{
    return stAmctRegs.InitialAngle;
}


void Amct_SetBemfDegaussFilterBlankTime( uint16 const ku16BemfDegaussFilterBlankTime )
{
    stAmctRegs.BemfDegaussFilterBlankTime = ku16BemfDegaussFilterBlankTime;
}


uint16 Amct_GetBemfDegaussFilterBlankTime( void )
{
    return (uint16)( stAmctRegs.BemfDegaussFilterBlankTime );
}


void Amct_SetBemfFilterTime( uint32 const ku32BemfFilterTime )
{
    stAmctRegs.BemFilterTime = ku32BemfFilterTime;
}


uint32 Amct_GetBemfFilterTime( void )
{
    return stAmctRegs.BemFilterTime;
}


uint16 Amct_GetPhaseDisableOutput( void )
{
    return (uint16)( stAmctRegs.PhaseDisableOutput );
}


uint16 Amct_GetFilteredBemfComparatorOutput( void )
{
    return (uint16)( stAmctRegs.FilteredBemfCompOutput );
}


uint16 Amct_GetBemfComparatorTacho( void )
{
    return (uint16)( stAmctRegs.BemfCompTacho );
}


uint8 Amct_GetCommutationSection( void )
{
    return (uint8)( stAmctRegs.ComuSection );
}


uint32 Amct_GetHallFrequency( void )
{
    return stAmctRegs.HallFreq;
}


uint32 Amct_GetAngle( void )
{
    return stAmctRegs.Angle;
}


uint32 Amct_GetBemfAngleError( void )
{
    return stAmctRegs.BemfAngleError;
}


uint16 Amct_GetIpdResult1( void )
{
    return (uint16)(stAmctRegs.IpdResult1);
}


uint16 Amct_GetIpdResult2( void )
{
    return (uint16)(stAmctRegs.IpdResult2);
}


uint16 Amct_GetCpdResult( void )
{
    return (uint16)(stAmctRegs.CpdResult);
}


void Amct_SetIrqEnable( enum AmctIrqEnable const keIrqEnable )
{
    if ( keIrqEnable == eAMCT_IRQENABLE_NONE )
    {
        stAmctRegs.IrqEnable = 0;
    }
    else
    {
        stAmctRegs.IrqEnable |= (uint32)(keIrqEnable); /* polyspace MISRA-C3:D4.1 [Not a defect:Unset] signed long integer width (32 bit enumeration) cast to same width register OR'ed */
    }
}


uint16 Amct_GetIrqEnable( void )
{
    return (uint16)(stAmctRegs.IrqEnable);
}


void Amct_SetIrqStatus( enum AmctIrqStatus const keIrqStatusBit )
{
    if ( keIrqStatusBit != eAMCT_IRQSTATUS_ANY_INTERRUPT )
    {
        stAmctRegs.IrqStatus |= (uint32)(keIrqStatusBit);
    }
}


boolean Amct_GetIrqStatus( enum AmctIrqStatus const keIrqStatusBit )
{
    boolean bIrqStat = False;
    uint32 u32IrqStatus = stAmctRegs.IrqStatus;
    switch ( keIrqStatusBit )
    {
    case eAMCT_IRQSTATUS_ANY_INTERRUPT:
        u32IrqStatus &= (uint32)(eAMCT_IRQSTATUS_ALL_BITS);
        break;

    case eAMCT_IRQSTATUS_PHASE_A_DISABLE:
    case eAMCT_IRQSTATUS_PHASE_B_DISABLE:
    case eAMCT_IRQSTATUS_PHASE_C_DISABLE:
    case eAMCT_IRQSTATUS_HALL_OVERFLOW:
    case eAMCT_IRQSTATUS_HALL_STATE_FAULT:
    case eAMCT_IRQSTATUS_BEMF_COMP_STATE:
    case eAMCT_IRQSTATUS_BEMF_COMP_FAULT:
    case eAMCT_IRQSTATUS_IPD_STAT2:
    case eAMCT_IRQSTATUS_IPD_STAT1:
        u32IrqStatus &= (uint32)(keIrqStatusBit);
        break;

    default:
        u32IrqStatus = 0;
        break;
    }

    if ( u32IrqStatus != 0 )
    {
        bIrqStat = True;
    }
    return bIrqStat;
}


// Declare a function pointer for use by AMCT Irq
static void (*fpAmctIrqHandler)( void ) = NULLPTR;

// Set the AMCT Irq Handler to required function
void Amct_SetIrqHandler( void (AmctIrqHandler)( void ) )
{
    fpAmctIrqHandler = *AmctIrqHandler;
}

// Execute the required interrupt function 
void Amct_IrqHandler( void )
{
    NVIC_DisableIRQ( skIrqAmct );
    NVIC_ClearPendingIRQ( skIrqAmct );

    if( fpAmctIrqHandler != NULLPTR )
    {
        fpAmctIrqHandler();
    }
    
    NVIC_EnableIRQ( skIrqAmct );
}
