#ifndef HEADER_PWMGEN_H
#define HEADER_PWMGEN_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         PwmGen.h
 *
 * @brief        Driver for Pulse Width Modulation (PWM) generator.
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
#include "Std_Types.h"

enum PwmSelect // To select PWM A, B or C
{
    ePWM_SELECT_A  = 0U,
    ePWM_SELECT_B  = 1U,
    ePWM_SELECT_C  = 2U
};

enum PwmDutyCycleMasks // Masks for 2 Duty Cycles held in Duty Cycle register for A, B or C
{
    ePWM_DUTY_CYCLE_MASK1 =       0x00003FFFU,
    ePWM_DUTY_CYCLE_MASK2 =       0x3FFF0000U,
    ePWM_DUTY_CYCLE_MASK2_SHIFT = 16,
	  ePWM_DUTY_CYCLE_FULLMASK =    0x3FFFFFFFU
};

enum PwmOutputSelection // Output Selection register options
{
    ePWM_OUTPUT_SELECTION_NONE =                    ( 0 ),
    ePWM_OUTPUT_SELECTION_MASK =                    ( 0xFF3FU ),
    ePWM_OUTPUT_SELECTION_LA =                      ( 0x0001U ),
    ePWM_OUTPUT_SELECTION_HA =                      ( 0x0002U ),
    ePWM_OUTPUT_SELECTION_HLA =                     ( 0x0003U ),
    ePWM_OUTPUT_SELECTION_LB =                      ( 0x0004U ),
    ePWM_OUTPUT_SELECTION_HB =                      ( 0x0008U ),
    ePWM_OUTPUT_SELECTION_HLB =                     ( 0x000CU ),
    ePWM_OUTPUT_SELECTION_LC =                      ( 0x0010U ),
    ePWM_OUTPUT_SELECTION_HC =                      ( 0x0020U ),
    ePWM_OUTPUT_SELECTION_HLC =                     ( 0x0030U ),
    ePWM_OUTPUT_SELECTION_HLA_HLB =                 ( 0x000FU ),
    ePWM_OUTPUT_SELECTION_HLA_HLC =                 ( 0x0033U ),
    ePWM_OUTPUT_SELECTION_HLB_HLC =                 ( 0x003CU ),
    ePWM_OUTPUT_SELECTION_HA_HB_HC =                ( 0x002AU ),
    ePWM_OUTPUT_SELECTION_LA_LB_LC =                ( 0x0015U ),
    ePWM_OUTPUT_SELECTION_ENABLE_ALL_OUTSEL_XX =    ( 0x003FU ),
    ePWM_OUTPUT_SELECTION_PHASE_DIS_ENABLE =        ( 0x0040U ),
    ePWM_OUTPUT_SELECTION_PHASE_DIS_ENABLE_INV =    ( 0xFFBFU ), 
};

enum PwmControlRegisterAlignment  // Control Register 1 PWMMOD option.
{
    ePWM_CONTROL_REGISTER_ALIGNMENT_CENTRE_ALIGNED =    ( 0x0001U ),
    ePWM_CONTROL_REGISTER_ALIGNMENT_EDGE_ALIGNED_MASK = ( 0xFFFEU )
};

enum PwmControlRegisterPhase      // Control Register 1 Phasex_Inv option.  
{
    ePWM_CONTROL_REGISTER_PHASE_NONE =       ( 0 ),
    ePWM_CONTROL_REGISTER_PHASE_MASK =       ( 0xFFE1U ),
    ePWM_CONTROL_REGISTER_PHASE_A_INV =      ( 0x0004U ),
    ePWM_CONTROL_REGISTER_PHASE_B_INV =      ( 0x0008U ),
    ePWM_CONTROL_REGISTER_PHASE_C_INV =      ( 0x0010U ),
    ePWM_CONTROL_REGISTER_PHASE_AB_INV =     ( 0x000CU ),
    ePWM_CONTROL_REGISTER_PHASE_AC_INV =     ( 0x0014U ),
    ePWM_CONTROL_REGISTER_PHASE_BC_INV =     ( 0x0018U ),
	  ePWM_CONTROL_REGISTER_PHASE_ABC_INV =    ( 0x001CU ),
    ePWM_CONTROL_REGISTER_PHASE_A_NOTINV =   ( 0xFFFBU ),
    ePWM_CONTROL_REGISTER_PHASE_B_NOTINV =   ( 0xFFF7U ),
    ePWM_CONTROL_REGISTER_PHASE_C_NOTINV =   ( 0xFFEFU ),
    ePWM_CONTROL_REGISTER_PHASE_AB_NOTINV =  ( 0xFFF3U ),
    ePWM_CONTROL_REGISTER_PHASE_AC_NOTINV =  ( 0xFFEBU ),
    ePWM_CONTROL_REGISTER_PHASE_BC_NOTINV =  ( 0xFFE7U )
};

enum PwmControlRegisterAdcNormalTrig   // Control Register 2 ADCxNT option.
{
    ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_NONE =              ( 0 ),
    ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_MASK =              ( 0xFFC7U ),
    ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_A_ENABLE =          ( 0x000FU ),
    ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_B_ENABLE =          ( 0x0017U ),
    ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_C_ENABLE =          ( 0x0027U ),
    ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_AB_ENABLE =         ( 0x001FU ),
    ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_AC_ENABLE =         ( 0x002FU ),
    ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_BC_ENABLE =         ( 0x0037U ),
    ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_ABC_ENABLE =        ( 0x003FU )
};

enum PwmControlRegisterAdcCalibTrigger  // Control Register 2 ADCxCT option.
{
    ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_NONE =        ( 0 ),
    ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_MASK =        ( 0xFE3FU ),
    ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_A_ENABLE =    ( 0x0040U ),
    ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_B_ENABLE =    ( 0x0080U ),
    ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_C_ENABLE =    ( 0x0100U ),
    ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_AB_ENABLE =   ( 0x00C0U ),
    ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_AC_ENABLE =   ( 0x0140U ),
    ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_BC_ENABLE =   ( 0x0180U ),
    ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_ABC_ENABLE=   ( 0x01C0U )
};

// PwmCentre
#define PWM_CENTRE_SIGNAL ( 0x0001U )

// PWM Generator register set
typedef struct 
{
    uint32 DutyCyclePhaseA;             // offset:  0x00U 
    uint32 DutyCyclePhaseB;             // offset:  0x04U 
    uint32 DutyCyclePhaseC;             // offset:  0x08U 
    uint32 Period;                      // offset:  0x0CU 
    uint32 Reserved[3];                 // offset:  0x10U   
    uint32 AdcNormalTrigRefA;           // offset:  0x1CU 
    uint32 AdcNormalTrigRefB;           // offset:  0x20U  
    uint32 AdcNormalTrigRefC;           // offset:  0x24U 
    uint32 AdcCalibTrigRefA;            // offset:  0x28U 
    uint32 AdcCalibTrigRefB;            // offset:  0x2CU 
    uint32 AdcCalibTrigRefC;            // offset:  0x30U 
    uint32 OutputSelection;             // offset:  0x34U 
    uint32 OutputLogicLevelSelection;   // offset:  0x38U 
    uint32 ControlRegister1;            // offset:  0x3CU 
    uint32 ControlRegister2;            // offset:  0x40U 
    uint32 DeadTime;                    // offset:  0x44U 
    uint32 Carrier;                     // offset:  0x48U 
    uint32 CentreAndStart;              // offset:  0x4CU
} PwmGenRegs_t;

#define  stPwmGenRegs (*( volatile PwmGenRegs_t *)eADDRESS_PWMGEN_BASE)

// PwmGen Inline Functions

/**
 * @brief     Set phase duty register A.
 * @remarks     This is an optimized replacement for the PwmGen_generic SetPhaseDuty. It's use should be minimized as it is an inline function.
 * @param[in]   u32Thresholds First and second threshold (each 14-bit) contained in one 32-bit input.
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline void PwmGen_SetPhaseDutyAFast( uint32 const ku32Thresholds )
{
    stPwmGenRegs.DutyCyclePhaseA = ku32Thresholds;
}


/**
 * @brief     Set phase duty register B.
 * @remarks     This is an optimized replacement for the generic PwmGen_SetPhaseDuty. It's use should be minimized as it is an inline function.
 * @param[in]   u32Thresholds First and second threshold (each 14-bit) contained in one 32-bit input.
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline void PwmGen_SetPhaseDutyBFast( uint32 const ku32Thresholds )
{
    stPwmGenRegs.DutyCyclePhaseB = ku32Thresholds;
}


/**
 * @brief     Set phase duty register C.
 * @remarks     This is an optimized replacement for the generic PwmGen_SetPhaseDuty. It's use should be minimized as it is an inline function.
 * @param[in]   u32Thresholds First and second threshold (each 14-bit) contained in one 32-bit input.
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline void PwmGen_SetPhaseDutyCFast( uint32 const ku32Thresholds )
{
    stPwmGenRegs.DutyCyclePhaseC = ku32Thresholds;
}


/**
 * @brief     Sets period
 * @remarks     This is an optimized replacement for PwmGen_SetPeriod. It's use should be minimized as it is an inline function.
 * @param[in]   ku16Period Period value (14-bit value)
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline void PwmGen_SetPeriodFast( uint16 const ku16Period )
{
    stPwmGenRegs.Period = ku16Period;
}


/**
 * @brief     Set dead time
 * @remarks     This is an optimized replacement for PwmGen_SetDeadTime. It's use should be minimized as it is an inline function.
 * @param[in]   ku16DeadTime Dead time value
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline void PwmGen_SetDeadTimeFast( uint16 const ku16DeadTime )
{
    stPwmGenRegs.DeadTime = (uint32)( ku16DeadTime );
}


/**
 * @brief     Set ADC trigger reference register A
 * @remarks     This is an optimized replacement for PwmGen_SetAdcTriggerRef. It's use should be minimized as it is an inline function.
 * @param[in]   ku16AdcTriggerRef ADC trigger reference
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline void PwmGen_SetAdcTriggerRefAFast( uint16 const ku16AdcTriggerRef )
{
    stPwmGenRegs.AdcNormalTrigRefA = (uint32)( ku16AdcTriggerRef );
}


/**
 * @brief     Set ADC trigger reference register B
 * @remarks     This is an optimized replacement for PwmGen_SetAdcTriggerRef. It's use should be minimized as it is an inline function.
 * @param[in]   ku16AdcTriggerRef ADC trigger reference
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline void PwmGen_SetAdcTriggerRefBFast( uint16 const ku16AdcTriggerRef )
{
    stPwmGenRegs.AdcNormalTrigRefB = (uint32)( ku16AdcTriggerRef );
}

/**
 * @brief     Set ADC trigger reference register C
 * @remarks     This is an optimized replacement for PwmGen_SetAdcTriggerRef. It's use should be minimized as it is an inline function. 
 * @param[in]   ku16AdcTriggerRef ADC trigger reference
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline void PwmGen_SetAdcTriggerRefCFast( uint16 const ku16AdcTriggerRef )
{
    stPwmGenRegs.AdcNormalTrigRefC = (uint32)( ku16AdcTriggerRef );
}

// PwmGen inline functions


/**
 * @brief     Set one of the phase duty registers
 * @param[in]   kePhaseRegSelect Select phase A,B or C
 * @param[in]   ku16FirstThreshold First-threshold (14-bit value)
 * @param[in]   ku16SecondThreshold Second-threshold (14-bit value)
 * @returns     None
 */
void PwmGen_SetPhaseDuty( enum PwmSelect const kePhaseRegSelect, uint16 const ku16FirstThreshold, uint16 const ku16SecondThreshold );    


/**
 * @brief     Get one of the phase duty registers
 * @param[in]   ke8PhaseRegSelect Select phase A, B or C
 * @param[out]  pku16FirstThreshold First-threshold (14-bit value)
 * @param[out]  pku16SecondThreshold Second-threshold (14-bit value)
 * @returns     None
 */
void PwmGen_GetPhaseDuty( enum PwmSelect const kePhaseRegSelect, uint16* pku16FirstThreshold, uint16* pku16SecondThreshold );

/**
 * @brief     Sets period
 * @param[in]   ku16Period Period value (14-bit value)
 * @returns     None
 */
void PwmGen_SetPeriod( uint16 const ku16Period );

/**
 * @brief     Gets period
 * @param[in]   None
 * @returns     Period value (14-bit value)
 */
uint16 PwmGen_GetPeriod( void );

/**
 * @brief     Set one of ADC trigger reference registers
 * @param[in]   keAdcSelect Select ADC A, B or C
 * @param[in]   ku16AdcTriggerRef ADC trigger reference value
 * @returns     None
 */
void PwmGen_SetAdcTriggerRef( enum PwmSelect const keAdcSelect, uint16 const ku16AdcTriggerRef );

/**
 * @brief     Get one of ADC trigger reference registers
 * @param[in]   keAdcSelect Select ADC A, B or C
 * @returns     Adc trigger reference value of ADC selected (14-bit value)
 */
uint16 PwmGen_GetAdcTriggerRef( enum PwmSelect const keAdcSelect );

/**
 * @brief     Set one of ADC calibration trigger reference registers
 * @param[in]   keAdcSelect Select ADC A, B or C
 * @param[in]   ku16AdcCalibTriggerRef ADC calibration trigger reference (14-bit value)
 * @returns     None
 */
void PwmGen_SetAdcCalibTriggerRef( enum PwmSelect const keAdcSelect, uint16 const ku16AdcCalibTriggerRef );

/**
 * @brief     Get one of ADC calibration trigger reference registers
 * @param[in]   keAdcSelect Select ADC A, B or C
 * @returns     Adc calibration trigger reference of ADC selected (14-bit value)
 */
uint16 PwmGen_GetAdcCalibTriggerRef( enum PwmSelect const keAdcSelect );

/**
 * @brief     Set output selection controlling the select the bits for the multiplexer that
 *            defines if the output signals, Hx and Lx should be generated from independent PWM generator
 *            or by the output level selection register, across each of the 3 phases (A,B,C)
 *            Sets LA, HA, LB, HB, LC, HC only, does not set PhaseDis_En and preserves it's state between calls.
 *            To set and clear PhaseDis_En bit use SetOutputSelectionPhaseDisEnable().
 * @param[in]   keOutputSelection Sets output selection
 * @returns     None
 */
void PwmGen_SetOutputSelection( enum PwmOutputSelection const keOutputSelection );

/**
 * @brief     Set output selection PhaseDisEnable bit
 * @param[in]   kbEnable true or false
 * @returns     None
 */
void PwmGen_SetOutputSelectionPhaseDisEnable( boolean const kbEnable );

/**
 * @brief     Get output selection to bridge
 * @param[in]   None
 * @returns     Setting for outputs: LA, HA, LB, HB, LC, HC
 */
uint16 PwmGen_GetOutputSelection( void );

/**
 * @brief     Set output logic level
 * @param[in]   ku16OutputLogicLevel Sets output logic level: LA, HA, LB, HB, LC, HC
 * @returns     None
 */
void PwmGen_SetOutputLogicLevel( uint16 const ku16OutputLogicLevel );

/**
 * @brief     Get output logic level
 * @param[in]   None
 * @returns     Setting for output logic level: LA, HA, LB, HB, LC, HC
 */
uint16 PwmGen_GetOutputLogicLevel( void );

/**
 * @brief     Set Control Register 1 - phase
 * @param[in]   kePhase Phase settings only
 * @returns     None
 */
void PwmGen_SetControlRegister1Phase( enum PwmControlRegisterPhase const kePhase );

/**
 * @brief     Set Control Register 1 - alignment
 * @param[in]   keAlignment Alignment settings only
 * @returns     None
 */
void PwmGen_SetControlRegister1Alignment( enum PwmControlRegisterAlignment const keAlignment );

/**
 * @brief     Get Control Register 1
 * @param[in]   None
 * @returns     Control Register 1.
 */
uint32 PwmGen_GetControlRegister1( void );

/**
 * @brief     Set Control Register 2 - ADC Normal Trigger
 * @param[in]   keAdcNormalTrig ADC normal trigger.
 * @returns     None
 */
void PwmGen_SetControlRegister2AdcNormalTrig(  enum PwmControlRegisterAdcNormalTrig const keAdcNormalTrig );

/**
 * @brief     Set Control Register 2 - ADC Calibration Trigger
 * @param[in]   ku16AdcCalibTrig ADC calibration trigger.
 * @returns     None
 */
void PwmGen_SetControlRegister2AdcCalibTrig(  enum PwmControlRegisterAdcCalibTrigger const keAdcCalibTrig );

/**
 * @brief     Get Control Register 2
 * @param[in]   None
 * @returns     Control Register 2.
 */
uint16 PwmGen_GetControlRegister2( void );

/**
 * @brief     Set dead time
 * @param[in]   ku16DeadTime Dead time value (10-bit value)
 * @returns     None
 */
void PwmGen_SetDeadTime( uint16 const ku16DeadTime );

/**
 * @brief     Get dead time
 * @param[in]   None
 * @returns     Dead time value (10-bit value)
 */
uint16 PwmGen_GetDeadTime( void );

/**
 * @brief     Get carrier
 * @param[in]   None
 * @returns     Carrier value (14-bit value)
 */
uint16 PwmGen_GetCarrier( void );

/**
 * @brief     Get PWM centre
 * @param[in]   None
 * @returns     PWM Centre state
 */
boolean PwmGen_GetPwmCentre( void );

#endif //HEADER_PWMGEN_H