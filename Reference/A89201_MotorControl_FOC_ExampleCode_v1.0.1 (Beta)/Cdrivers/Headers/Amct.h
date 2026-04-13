#ifndef HEADER_AMCT_H
#define HEADER_AMCT_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Amct.h
 * @author       M. Shankey
 * @version      1.0
 *
 * @brief        Advanced Motor Control Timer driver: AMCT
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

enum AmctConfigModeSelect                   // Config register setting
{
    eAMCT_CONFIG_BEMF =  ( 0x001FFFFEU ),
    eAMCT_CONFIG_HALL =  ( 0x00000001U )
};

enum AmctConfigMotorDirection               // Config register setting
{
    eAMCT_CONFIG_RVS = ( 0x001FFFFDU ),
    eAMCT_CONFIG_FWD = ( 0x00000002U )
};

enum AmctConfigDegaussFilterBypass          // Config register setting
{
    eAMCT_CONFIG_NO_BYPASS =   ( 0x001FFFFBU ),
    eAMCT_CONFIG_BYPASS =      ( 0x00000004U )
};

enum AmctConfigPhaseDisableSelect           // Config register setting
{
    eAMCT_CONFIG_CMD =  ( 0x001FFFF7U ),
    eCPU_CONFIG_CMD  =  ( 0x00000008U )
};

enum AmctConfigHallASelect                  // Config register setting
{
    eAMCT_CONFIG_HALLASELECT_MASK =    ( 0x001FFF8FU ), 
    eAMCT_CONFIG_HALLASELECT_GPIO_0 =  ( 0x00000000U ),
    eAMCT_CONFIG_HALLASELECT_GPIO_1 =  ( 0x00000010U ),
    eAMCT_CONFIG_HALLASELECT_GPIO_2 =  ( 0x00000020U ),
    eAMCT_CONFIG_HALLASELECT_GPIO_3 =  ( 0x00000030U ),
    eAMCT_CONFIG_HALLASELECT_GPIO_4 =  ( 0x00000040U ),
    eAMCT_CONFIG_HALLASELECT_GPIO_5 =  ( 0x00000050U ),
    eAMCT_CONFIG_HALLASELECT_GPIO_6 =  ( 0x00000060U ),
    eAMCT_CONFIG_HALLASELECT_GPIO_7 =  ( 0x00000070U )
};


enum AmctConfigHallBSelect                  // Config register setting
{
    eAMCT_CONFIG_HALLBSELECT_MASK =    ( 0x001FFC7FU ),
    eAMCT_CONFIG_HALLBSELECT_GPIO_0 =  ( 0x00000000U ),
    eAMCT_CONFIG_HALLBSELECT_GPIO_1 =  ( 0x00000080U ),
    eAMCT_CONFIG_HALLBSELECT_GPIO_2 =  ( 0x00000100U ),
    eAMCT_CONFIG_HALLBSELECT_GPIO_3 =  ( 0x00000180U ),
    eAMCT_CONFIG_HALLBSELECT_GPIO_4 =  ( 0x00000200U ),
    eAMCT_CONFIG_HALLBSELECT_GPIO_5 =  ( 0x00000280U ),
    eAMCT_CONFIG_HALLBSELECT_GPIO_6 =  ( 0x00000300U ),
    eAMCT_CONFIG_HALLBSELECT_GPIO_7 =  ( 0x00000380U )
};


enum AmctConfigHallCSelect                  // Config register setting
{
    eAMCT_CONFIG_HALLCSELECT_MASK =    ( 0x001FE3FFU ),
    eAMCT_CONFIG_HALLCSELECT_GPIO_0 =  ( 0x00000000U ),
    eAMCT_CONFIG_HALLCSELECT_GPIO_1 =  ( 0x00000400U ),
    eAMCT_CONFIG_HALLCSELECT_GPIO_2 =  ( 0x00000800U ),
    eAMCT_CONFIG_HALLCSELECT_GPIO_3 =  ( 0x00000C00U ),
    eAMCT_CONFIG_HALLCSELECT_GPIO_4 =  ( 0x00001000U ),
    eAMCT_CONFIG_HALLCSELECT_GPIO_5 =  ( 0x00001400U ),
    eAMCT_CONFIG_HALLCSELECT_GPIO_6 =  ( 0x00001800U ),
    eAMCT_CONFIG_HALLCSELECT_GPIO_7 =  ( 0x00001C00U )
};

enum AmctConfigIPDTriggerSelect             // Config register setting
{
    eAMCT_CONFIG_IPDTRIGGERSELECT_MASK = ( 0xFFFCFFFFU ),
    eAMCT_CONFIG_IPDTRIGGERSELECT_GHA =  ( 0x00000000U ),
    eAMCT_CONFIG_IPDTRIGGERSELECT_GHB =  ( 0x00010000U ),
    eAMCT_CONFIG_IPDTRIGGERSELECT_GHC =  ( 0x00020000U )
};

enum AmctConfigIPDComparatorSelect          // Config register setting    
{
    eAMCT_CONFIG_IPDCOMPARATORSELECT_MASK =  ( 0xFFF3FFFFU ),
    eAMCT_CONFIG_IPDCOMPARATORSELECT_OUTA =  ( 0x00000000U ),
    eAMCT_CONFIG_IPDCOMPARATORSELECT_OUTB =  ( 0x00040000U ),
    eAMCT_CONFIG_IPDCOMPARATORSELECT_OUTC =  ( 0x00080000U )
};


enum AmctConfigIPDUpdateModeSelect          // Config register setting
{
    eAMCT_CONFIG_RESULT_NO_IRQ =   ( 0x000FFFFF ),
    eAMCT_CONFIG_RESULT_ALWAYS =   ( 0x00100000 )
};

enum AmctCommand                            // Command register setting
{
    eAMCT_COMMAND_CLEAR_ALL  =            ( 0x0000 ),
    eAMCT_COMMAND_MASTER_DISABLE =        ( 0xFFFE ),
    eAMCT_COMMAND_MASTER_ENABLE  =        ( 0x0001 ),
    eAMCT_COMMAND_IPD_DISABLE    =        ( 0xFFFC ),
    eAMCT_COMMAND_IPD_ENABLE     =        ( 0x0003 ),
    eAMCT_COMMAND_CPD_DISABLE    =        ( 0xFFFA ),
    eAMCT_COMMAND_CPD_ENABLE     =        ( 0x0005 ),
    eAMCT_COMMAND_HALL_DISABLE   =        ( 0xFFF6 ),
    eAMCT_COMMAND_HALL_ENABLE    =        ( 0x0009 ),
    eAMCT_COMMAND_BEMF_DISABLE   =        ( 0xFFEE ),
    eAMCT_COMMAND_BEMF_ENABLE    =        ( 0x0011 )
};

enum  AmctPhaseDisable                      // PhaseDisable register setting
{
    eAMCT_PHASEDISABLE_NONE =    ( 0x0000 ),
    eAMCT_PHASEDISABLE_A    =    ( 0x0001 ),
    eAMCT_PHASEDISABLE_B    =    ( 0x0002 ),
    eAMCT_PHASEDISABLE_C    =    ( 0x0004 ),
    eAMCT_PHASEDISABLE_AB   =    ( 0x0003 ),
    eAMCT_PHASEDISABLE_AC   =    ( 0x0005 ),
    eAMCT_PHASEDISABLE_BC   =    ( 0x0006 ),
    eAMCT_PHASEDISABLE_ALL  =    ( 0x0007 )
};


enum AmctPhaseDisableOutput                 // PhaseDisableOutput register setting
{
    eAMCT_PHASEDISABLE_OUTPUT_NONE  =   ( 0x0000 ),
    eAMCT_PHASEDISABLE_OUTPUT_A     =   ( 0x0001 ),
    eAMCT_PHASEDISABLE_OUTPUT_B     =   ( 0x0002 ),
    eAMCT_PHASEDISABLE_OUTPUT_AB    =   ( 0x0003 ),
    eAMCT_PHASEDISABLE_OUTPUT_C     =   ( 0x0004 ),
    eAMCT_PHASEDISABLE_OUTPUT_AC    =   ( 0x0005 ),
    eAMCT_PHASEDISABLE_OUTPUT_BC    =   ( 0x0006 ),
    eAMCT_PHASEDISABLE_OUTPUT_ABC   =   ( 0x0007 )
};



enum AmctFilteredBemfCompOutput             // FilteredBemfCompOutput register setting  
{
    eAMCT_FILTEREDBEMFCOMP_OUTPUT_NONE  =   ( 0x0000 ),
    eAMCT_FILTEREDBEMFCOMP_OUTPUT_A     =   ( 0x0001 ),
    eAMCT_FILTEREDBEMFCOMP_OUTPUT_B     =   ( 0x0002 ),
    eAMCT_FILTEREDBEMFCOMP_OUTPUT_AB    =   ( 0x0003 ),
    eAMCT_FILTEREDBEMFCOMP_OUTPUT_C     =   ( 0x0004 ),
    eAMCT_FILTEREDBEMFCOMP_OUTPUT_AC    =   ( 0x0005 ),
    eAMCT_FILTEREDBEMFCOMP_OUTPUT_BC    =   ( 0x0006 ),
    eAMCT_FILTEREDBEMFCOMP_OUTPUT_ABC   =   ( 0x0007 )
};


enum AmctBemfCompTacho                      // BemfCompTacho register setting
{
    eAMCT_TACHO_SIGNAL = ( 0x0001 )
};


enum AmctApplyAngleCorrection               // ApplyAngleCorrection register setting
{
    eAMCT_SET_LOW  =  ( 0x0000 ),
    eAMCT_SET_HIGH =  ( 0x0001 )
};

enum AmctIrqEnable                          // IrqEnable register setting
{
    eAMCT_IRQENABLE_NONE            =  ( 0x0000 ),
    eAMCT_IRQENABLE_PHASE_A_DISABLE =  ( 0x0001 ),
    eAMCT_IRQENABLE_PHASE_B_DISABLE =  ( 0x0002 ),
    eAMCT_IRQENABLE_PHASE_C_DISABLE =  ( 0x0004 ),
    eAMCT_IRQENABLE_HALL_T2_OVR     =  ( 0x0008 ),
    eAMCT_IRQENABLE_HALL_COMU_FAULT =  ( 0x0010 ),
    eAMCT_IRQENABLE_BEMF_COMP_STATE =  ( 0x0020 ),
    eAMCT_IRQENABLE_BEMF_COMU_FAULT =  ( 0x0040 ), 
    eAMCT_IRQENABLE_IPD_STAT2       =  ( 0x0080 ),
    eAMCT_IRQENABLE_IPD_STAT1       =  ( 0x0100 )
};

enum AmctIrqStatus                          // IrqStatus register setting
{
    eAMCT_IRQSTATUS_ANY_INTERRUPT    =   0,
    eAMCT_IRQSTATUS_ALL_BITS         =  ( 0x01FF ),
    eAMCT_IRQSTATUS_PHASE_A_DISABLE  =  ( 0x0001 ),
    eAMCT_IRQSTATUS_PHASE_B_DISABLE  =  ( 0x0002 ),
    eAMCT_IRQSTATUS_PHASE_C_DISABLE  =  ( 0x0004 ),
    eAMCT_IRQSTATUS_HALL_OVERFLOW    =  ( 0x0008 ),
    eAMCT_IRQSTATUS_HALL_STATE_FAULT =  ( 0x0010 ),
    eAMCT_IRQSTATUS_BEMF_COMP_STATE  =  ( 0x0020 ),
    eAMCT_IRQSTATUS_BEMF_COMP_FAULT  =  ( 0x0040 ),
    eAMCT_IRQSTATUS_IPD_STAT2        =  ( 0x0080 ),
    eAMCT_IRQSTATUS_IPD_STAT1        =  ( 0x0100 )
};

// AMCT register set
typedef struct
{
    uint32 Config;                          // offset:  0x00U 
    uint32 Command;                         // offset:  0x04U 
    uint32 PhaseDisable;                    // offset:  0x08U 
    uint32 HallFilterTime;                  // offset:  0x0CU 
    sint32 HallPhaseShift;                  // offset:  0x10U   
    uint32 AngleStep;                       // offset:  0x14U   
    uint32 HallFreqLimit;                   // offset:  0x18U        
    uint32 ApplyAngleCorrection;            // offset:  0x1CU 
    uint32 AngleCorrectionFactor;           // offset:  0x20U  
    uint32 InitialAngle;                    // offset:  0x24U 
    uint32 BemfDegaussFilterBlankTime;      // offset:  0x28U 
    uint32 BemFilterTime;                   // offset:  0x2CU 
    uint32 IrqEnable;                       // offset:  0x30U 
    uint32 PhaseDisableOutput;              // offset:  0x34U 
    uint32 FilteredBemfCompOutput;          // offset:  0x38U 
    uint32 BemfCompTacho;                   // offset:  0x3CU 
    uint32 IrqStatus;                       // offset:  0x40U 
    uint32 ComuSection;                     // offset:  0x44U 
    uint32 HallFreq;                        // offset:  0x48U 
    uint32 Angle;                           // offset:  0x4CU
    uint32 BemfAngleError;                  // offset:  0x50U
    uint32 IpdResult1;                      // offset:  0x54U
    uint32 IpdResult2;                      // offset:  0x58U
    uint32 CpdResult;                       // offset:  0x5CU
} AmctRegs_t;

#define  stAmctRegs (*( volatile AmctRegs_t *)eADDRESS_AMCT_BASE)

// Amct Function Definitions

/**
 * @brief     Reset config register: all bits
 * @param[in]   None
 * @returns     None
 */
void Amct_ResetConfig( void );

/**
 * @brief     Set Config register: mode select
 * @param[in]   keConfigModeSel Config register mode select
 * @returns     None
 */
void Amct_SetConfigModeSelect( enum AmctConfigModeSelect const keConfigModeSel );

/**
 * @brief     Set Config register: motor direction
 * @param[in]   keConfigMotorDir Config register motor direction setting
 * @returns     None
 */
void Amct_SetConfigMotorDir( enum AmctConfigMotorDirection const keConfigControlMotorDir );

/**
 * @brief     Set Config register: Degauss filter bypass
 * @param[in]   keConfigDegaussFilterBypass Config register Degauss filter bypass
 * @returns     None
 */
void Amct_SetConfigDegaussFilterBypass( enum AmctConfigDegaussFilterBypass const keConfigDegaussFilterBypass );

/**
 * @brief     Set Config register: phase disable select
 * @param[in]   keConfigPhaseDisableSelect Config register phase disable select
 * @returns     None
 */
void Amct_SetConfigPhaseDisableSelect( enum AmctConfigPhaseDisableSelect const keConfigPhaseDisableSelect );

/**
 * @brief     Set Config register: Hall A select
 * @param[in]   keConfigHallASelect Config register Hall A select
 * @returns     None
 */
void Amct_SetConfigHallASelect( enum AmctConfigHallASelect const keConfigHallASelect );

/**
 * @brief     Set Config register: Hall B select
 * @param[in]   keConfigHallBSelect Config register Hall B select
 * @returns     None
 */
void Amct_SetConfigHallBSelect( enum AmctConfigHallBSelect const keConfigHallBSelect );

/**
 * @brief     Set Config register: Hall C select
 * @param[in]   keConfigHallCSelect Config register Hall C select
 * @returns     None
 */
void Amct_SetConfigHallCSelect( enum AmctConfigHallCSelect const keConfigHallCSelect );

/**
 * @brief     Set Config register: IPD trigger select
 * @param[in]   keConfigIPDTriggerSelect Config register IPD trigger select
 * @returns     None
 */
void Amct_SetConfigIPDTriggerSelect( enum AmctConfigIPDTriggerSelect const keConfigIPDTriggerSelect );

/**
 * @brief     Set Config register: IPD comparator select
 * @param[in]   keConfigIPDComparatorSelect Config register IPD comparator select
 * @returns     None
 */
void Amct_SetConfigIPDComparatorSelect( enum AmctConfigIPDComparatorSelect const keConfigIPDComparatorSelect );

/**
 * @brief     Set Config register: IPD update mode select
 * @param[in]   keConfigIPDUpdateModeSelect Config register IPD update mode select
 * @returns     None
 */
void Amct_SetConfigIPDUpdateModeSelect( enum AmctConfigIPDUpdateModeSelect const keConfigIPDUpdateModeSelect );

/**
 * @brief     Get Config register
 * @param[in]   None
 * @returns     Config register setting: all 10 values combined in one 32-bit value
 */
uint32 Amct_GetConfig( void );

/**
 * @brief     Set Command register
 * @param[in]   keCommand Command register setting
 * @returns     None
 */
void Amct_SetCommand( enum AmctCommand const keCommand );

/**
 * @brief     Get Command register setting
 * @param[in]   None
 * @returns     Command register setting
 */
uint16 Amct_GetCommand( void );

/**
 * @brief     Set phase disable of C, B or A
 * @param[in]   kePhaseDisable One phase
 * @returns     None
 */
void Amct_SetPhaseDisable( enum  AmctPhaseDisable const kePhaseDisable );

/**
 * @brief     Get phase disable setting C, B and A
 * @param[in]   None
 * @returns     3-bit phase disable setting for C, B and A
 */
uint16 Amct_GetPhaseDisable( void );

/**
 * @brief     Set Hall filter time
 * @param[in]   ku16HallFilterTime 16-bit filter time setting
 * @returns     None
 */
void Amct_SetHallFilterTime( uint16 const ku16HallFilterTime );

/**
 * @brief     Get Hall filter time
 * @param[in]   None
 * @returns     16-bit filter time setting
 */
uint16 Amct_GetHallFilterTime( void );

/**
 * @brief     Set Hall phase time
 * @param[in]   ks32HallPhaseShift 32-bit signed phase time setting
 * @returns     None
 */
void Amct_SetHallPhaseShift( const sint32 ks32HallPhaseShift );

/**
 * @brief     Get Hall phase time
 * @param[in]   None
 * @returns     32-bit signed phase time setting
 */
sint32 Amct_GetHallPhaseShift( void );

/**
 * @brief     Set angle step
 * @param[in]   ku32AngleStep 32-bit angle step
 * @returns     None
 */
void Amct_SetAngleStep( uint32 const ku32AngleStep );

/**
 * @brief     Get angle step
 * @param[in]   None
 * @returns     32-bit angle step
 */
uint32 Amct_GetAngleStep( void );

/**
 * @brief     Set Hall Frequency Limit.
 * @param[in]   ku32HallFreqLimit 32-bit freq limit
 * @returns     None
 */
void Amct_SetHallFreqLimit( uint32 const ku32HallFreqLimit );

/**
 * @brief     Get Hall Frequency Limit.
 * @param[in]   None
 * @returns     32-bit frequency limit
 */
uint32 Amct_GetHallFreqLimit( void );

/**
 * @brief     Set to apply the angle correction
 * @param[in]   keAngleCorrectionFactor State.
 * @returns     None
 */
void Amct_SetApplyAngleCorrection( enum AmctApplyAngleCorrection const keApplyAngleCorrection );

/**
 * @brief     Get state of apply angle correction
 * @param[in]   None
 * @returns     0 or 1 (8-Bit)
 */
uint16 Amct_GetApplyAngleCorrection( void );

/**
 * @brief     Set angle correction
 * @param[in]   ku32AngleCorrection 32-bit angle correction value
 * @returns     None
 */
void Amct_SetAngleCorrectionFactor( uint32 const ku32AngleCorrectionFactor );

/**
 * @brief     Get angle correction
 * @param[in]   None
 * @returns     32-bit angle correction value
 */
uint32 Amct_GetAngleCorrectionFactor( void );

/**
 * @brief     Set initial angle
 * @param[in]   ku32InitialAngle 32-bit initial angle value
 * @returns     None
 */
void Amct_SetInitialAngle( uint32 const ku32InitialAngle );

/**
 * @brief     Get initial angle
 * @param[in]   None
 * @returns     32-bit initital angle value
 */
uint32 Amct_GetInitialAngle( void );

/**
 * @brief     Set BEMF Degauss filter blank time
 * @param[in]   ku16BemfDegaussFilterBlankTime 10-bit blank time value.
 * @returns     None
 */
void Amct_SetBemfDegaussFilterBlankTime( uint16 const ku16BemfDegaussFilterBlankTime );

/**
 * @brief     Get BEMF Degauss filter blank time
 * @param[in]   None
 * @returns     10-bit blank time value.
 */
uint16 Amct_GetBemfDegaussFilterBlankTime( void );

/**
 * @brief     Set BEMF filter time
 * @param[in]   ku32BemfFilterTime 20-bit BEMF filter time.
 * @returns     None
 */
void Amct_SetBemfFilterTime( uint32 const ku32BemfFilterTime );

/**
 * @brief     Get BEMF filter time
 * @param[in]   None
 * @returns     20-bit BEMF filter time.
 */
uint32 Amct_GetBemfFilterTime( void );

/**
 * @brief     Get phase disable output setting C, B and A
 * @param[in]   None
 * @returns     3-bit TP phase disable setting
 */
uint16 Amct_GetPhaseDisableOutput( void );

/**
 * @brief     Get Filtered BEMF comparator output for C, B and A
 * @param[in]   None
 * @returns     3-bit BEMF comparator filter out for C, B and A
 */
uint16 Amct_GetFilteredBemfComparatorOutput( void );

/**
 * @brief     Get BEMF comparator tacho signal
 * @param[in]   None
 * @returns     Tacho signal (0 or 1)
 */
uint16 Amct_GetBemfComparatorTacho( void );

/**
 * @brief     Get commutation section for C, B and A
 * @param[in]   None
 * @returns     3-bit commutation sections for C, B and A
 */
uint8 Amct_GetCommutationSection( void );

/**
 * @brief     Get Hall frequency
 * @param[in]   None
 * @returns     32-bit Hall frequency
 */
uint32 Amct_GetHallFrequency( void );

/**
 * @brief     Get angle
 * @param[in]   None
 * @returns     32-bit  angle
 */
uint32 Amct_GetAngle( void );

/**
 * @brief     Get BEMF angle error
 * @param[in]   None
 * @returns     32-bit BEMF angle error
 */
uint32 Amct_GetBemfAngleError( void );

/**
 * @brief     Get IPD result 1
 * @param[in]   None
 * @returns     16-bit IPD result 1
 */
uint16 Amct_GetIpdResult1( void );

/**
 * @brief     Get IPD result 2
 * @param[in]   None
 * @returns     16-bit IPD result 2
 */
uint16 Amct_GetIpdResult2( void );

/**
 * @brief     Get current polarity detector (CPD)
 * @param[in]   None
 * @returns     6-bit CPD result
 */
uint16 Amct_GetCpdResult( void );

/**
 * @brief     Set interrupt enabled.
 * @param[in]   keIrqEnable Interrupt enable.
 * @returns     None.
 */
void Amct_SetIrqEnable( enum AmctIrqEnable const keIrqEnable );

/**
 * @brief     Get interrupt enables.
 * @param[in]   None.
 * @returns     Interrupt enables.
 */
uint16 Amct_GetIrqEnable( void );

/**
 * @brief     Set interrupt status: 1 to clear.
 * @param[in]   keIrqStatus Interrupt status bit
 * @returns     None.
 */
void Amct_SetIrqStatus( enum AmctIrqStatus const keIrqStatusBit );

/**
 * @brief     Get state of interrupt status register or a specific interrupt status bit
 * @param[in]   keIrqStatus Interrupt status bit
 * @returns     true/false
 */
boolean Amct_GetIrqStatus( enum AmctIrqStatus const keIrqStatusBit );

/**
 * @brief Set AMCT IRQ handler
 * @param AmctIrqHandler An IRQ handler
 */
void Amct_SetIrqHandler( void (AmctIrqHandler)( void ) );

/**
 * @brief Handler for AMCT IRQ.
 */
void Amct_IrqHandler( void );


#endif //HEADER_AMCT_H
