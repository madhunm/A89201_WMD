/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @internal
 *
 * @file         PwmGen.c
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
#include "PwmGen.h"
 

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetPhaseDuty( enum PwmSelect const kePhaseRegSelect, uint16 const ku16FirstThreshold, uint16 const ku16SecondThreshold )    
{
    volatile uint32* const pu32PhaseDutyReg = &stPwmGenRegs.DutyCyclePhaseA + (uint32)( kePhaseRegSelect );
    *pu32PhaseDutyReg = (uint32)((ku16FirstThreshold + ((uint32)((uint32)ku16SecondThreshold << (uint8)ePWM_DUTY_CYCLE_MASK2_SHIFT))) & (uint32)ePWM_DUTY_CYCLE_FULLMASK);
}

/**
 * @internal
 *
 * @details
 */
void PwmGen_GetPhaseDuty( enum PwmSelect const kePhaseRegSelect, uint16* pku16FirstThreshold, uint16* pku16SecondThreshold )
{
    volatile uint32* const pu32PhaseDutyReg = &stPwmGenRegs.DutyCyclePhaseA + (uint32)( kePhaseRegSelect );
    uint32 const u32Thresholds = *pu32PhaseDutyReg;
    *pku16FirstThreshold =  (uint16)( u32Thresholds & (uint32)( ePWM_DUTY_CYCLE_MASK1 ) );
    *pku16SecondThreshold = (uint16)( (u32Thresholds & (uint32)( ePWM_DUTY_CYCLE_MASK2 )) >> (uint8)( ePWM_DUTY_CYCLE_MASK2_SHIFT) );
}

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetPeriod( uint16 const ku16Period )
{
    stPwmGenRegs.Period = (uint32)( ku16Period );
}

/**
 * @internal
 *
 * @details
 */
uint16 PwmGen_GetPeriod( void )
{
    return (uint16)( stPwmGenRegs.Period );
}


/**
 * @internal
 *
 * @details
 */
void PwmGen_SetAdcTriggerRef( enum PwmSelect const keAdcSelect, uint16 const ku16AdcTriggerRef )
{

    volatile uint32* const pu32AdcTriggerRef = &stPwmGenRegs.AdcNormalTrigRefA + (uint32)( keAdcSelect );
    *pu32AdcTriggerRef = ku16AdcTriggerRef;
}

/**
 * @internal
 *
 * @details
 */
uint16 PwmGen_GetAdcTriggerRef( enum PwmSelect const keAdcSelect )
{
    volatile uint32* const pu32AdcTriggerRef = &stPwmGenRegs.AdcNormalTrigRefA + (uint32)( keAdcSelect );
    return (uint16)*pu32AdcTriggerRef;
}

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetAdcCalibTriggerRef( enum PwmSelect const keAdcSelect, uint16 const ku16AdcCalibTriggerRef )
{
    volatile uint32* const pu32AdcCalibTriggerRef = &stPwmGenRegs.AdcCalibTrigRefA + (uint32)( keAdcSelect );
    *pu32AdcCalibTriggerRef = ku16AdcCalibTriggerRef;
}

/**
 * @internal
 *
 * @details
 */
uint16 PwmGen_GetAdcCalibTriggerRef( enum PwmSelect const keAdcSelect )
{
    volatile uint32* const pu32AdcCalibTriggerRef = &stPwmGenRegs.AdcCalibTrigRefA + (uint32)( keAdcSelect );
    return (uint16)*pu32AdcCalibTriggerRef;

}

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetOutputSelection( enum PwmOutputSelection const keOutputSelection )
{
    // Preserve state of PhaseDis_En bit before applying output selection setting
    uint32 const u32PhaseDisEn = stPwmGenRegs.OutputSelection & (uint32)( ePWM_OUTPUT_SELECTION_PHASE_DIS_ENABLE );
    stPwmGenRegs.OutputSelection = (uint32)( (uint32)( keOutputSelection ) | u32PhaseDisEn );
}

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetOutputSelectionPhaseDisEnable( boolean const kbEnable )
{
    if ( kbEnable == True )
    {
        stPwmGenRegs.OutputSelection |= (uint32)( ePWM_OUTPUT_SELECTION_PHASE_DIS_ENABLE );
    }
    else
    {
        stPwmGenRegs.OutputSelection &= (uint32)( ePWM_OUTPUT_SELECTION_PHASE_DIS_ENABLE_INV );
    }
}

/**
 * @internal
 *
 * @details
 */
uint16 PwmGen_GetOutputSelection( void )
{
    return (uint16)( stPwmGenRegs.OutputSelection );
}

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetOutputLogicLevel( uint16 const ku16OutputLogicLevel )
{
    stPwmGenRegs.OutputLogicLevelSelection = (uint32)( ku16OutputLogicLevel );
}

/**
 * @internal
 *
 * @details
 */
uint16 PwmGen_GetOutputLogicLevel( void )
{
    return (uint16)( stPwmGenRegs.OutputLogicLevelSelection );
}

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetControlRegister1Phase( enum PwmControlRegisterPhase const kePhase )
{
    switch ( kePhase  )
    {
        case ePWM_CONTROL_REGISTER_PHASE_MASK:
        case ePWM_CONTROL_REGISTER_PHASE_NONE:
            stPwmGenRegs.ControlRegister1 = 0;
        break;

        case ePWM_CONTROL_REGISTER_PHASE_A_INV:
        case ePWM_CONTROL_REGISTER_PHASE_B_INV:
        case ePWM_CONTROL_REGISTER_PHASE_C_INV:
        case ePWM_CONTROL_REGISTER_PHASE_AB_INV:
        case ePWM_CONTROL_REGISTER_PHASE_AC_INV:
        case ePWM_CONTROL_REGISTER_PHASE_BC_INV:
				case ePWM_CONTROL_REGISTER_PHASE_ABC_INV:
            stPwmGenRegs.ControlRegister1 |= (uint32)( kePhase );
        break;

        case ePWM_CONTROL_REGISTER_PHASE_A_NOTINV:
        case ePWM_CONTROL_REGISTER_PHASE_B_NOTINV:
        case ePWM_CONTROL_REGISTER_PHASE_C_NOTINV:
        case ePWM_CONTROL_REGISTER_PHASE_AB_NOTINV:
        case ePWM_CONTROL_REGISTER_PHASE_AC_NOTINV:
        case ePWM_CONTROL_REGISTER_PHASE_BC_NOTINV:
            stPwmGenRegs.ControlRegister1 &= (uint32)( kePhase );
        break;

    default:    // No update
        break;
    }
}

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetControlRegister1Alignment( enum PwmControlRegisterAlignment const keAlignment )
{
    switch ( keAlignment  )
    {
        case ePWM_CONTROL_REGISTER_ALIGNMENT_CENTRE_ALIGNED:
            stPwmGenRegs.ControlRegister1 |= (uint32)( keAlignment );
        break;

        case ePWM_CONTROL_REGISTER_ALIGNMENT_EDGE_ALIGNED_MASK:
            stPwmGenRegs.ControlRegister1 &= (uint32)( keAlignment );
        break;

        default:    // No update
        break;
    }
}


/**
 * @internal
 *
 * @details
 */
uint32 PwmGen_GetControlRegister1( void )
{
    return stPwmGenRegs.ControlRegister1;
}



/**
 * @internal
 *
 * @details
 */
void PwmGen_SetControlRegister2AdcNormalTrig(  enum PwmControlRegisterAdcNormalTrig const keAdcNormalTrig )
{
    // Reset the 3 bits
    stPwmGenRegs.ControlRegister2 &= (uint32)( ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_MASK );

    // OR in required bits
    if ( keAdcNormalTrig != ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_NONE )
    {
        stPwmGenRegs.ControlRegister2 |= (uint32)( keAdcNormalTrig );
    }
}

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetControlRegister2AdcCalibTrig(  enum PwmControlRegisterAdcCalibTrigger const keAdcCalibTrig )
{
    // Reset the 3 bits
    stPwmGenRegs.ControlRegister2 &= (uint32)( ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_MASK );

    // OR in required bits
    if ( keAdcCalibTrig != ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_NONE )
    {
        stPwmGenRegs.ControlRegister2 |= (uint32)( keAdcCalibTrig );
    }
}

/**
 * @internal
 *
 * @details
 */
uint16 PwmGen_GetControlRegister2( void )
{
    return (uint16)( stPwmGenRegs.ControlRegister2 );
}

/**
 * @internal
 *
 * @details
 */
void PwmGen_SetDeadTime( uint16 const ku16DeadTime )
{
    stPwmGenRegs.DeadTime = (uint32)( ku16DeadTime );
}

/**
 * @internal
 *
 * @details
 */
uint16 PwmGen_GetDeadTime( void )
{
    return (uint16)( stPwmGenRegs.DeadTime );
}

/**
 * @internal
 *
 * @details
 */
uint16 PwmGen_GetCarrier( void )
{
    return (uint16)( stPwmGenRegs.Carrier );
}

/**
 * @internal
 *
 * @details
 */
boolean PwmGen_GetPwmCentre( void )
{
    boolean bRetVal = False;
    uint32 const u32PwmCentre = stPwmGenRegs.CentreAndStart;
    if ( ( u32PwmCentre & PWM_CENTRE_SIGNAL ) != 0 )
    {
        bRetVal = True;
    }
    return bRetVal;
}

