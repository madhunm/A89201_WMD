#ifndef HEADER_CAU_H
#define HEADER_CAU_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Cau.h
 * @author       G. Sloan
 * @version      1.0
 *
 * @brief        Current Acquisition Unit interface: CAU
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
 */
 /* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
#include "Std_Types.h"


enum CauAdcSelect
{
    eCAU_A = 0U,
    eCAU_B = 1U,
    eCAU_C = 2U
};

enum CauAdcConfigCalibrationEnable
{
    eCAU_ADCCFGCAL_ENABLE =  0x01U,
    eCAU_ADCCFGCAL_DISABLE = 0xFEU
};

enum CauAdcConfigConvSpeed
{
    eCAU_CONVSPEED_1P0US =   0x00U,
    eCAU_CONVSPEED_1P5US =   0x02U,
    eCAU_CONVSPEED_2P0US =   0x04U,
    eCAU_CONVSPEED_2P5US =   0x06U,
};


enum CauAdcConfigAdcEnable
{
    eCAU_ADCCFGADC_ENABLE =               0x08U,
    eCAU_ADCCFGADC_DISABLE =              0xF7U
};

enum CauAdcConfigOverwritePermission
{
    eCAU_ADCCFG_PERMITTED =      0x10U,
    eCAU_ADCCFG_NOTPERMITTED =  0xEFU
};

enum CauTrigSelect
{
    eCAU_TRIGSEL_A = 0U,
    eCAU_TRIGSEL_B = 1U,
    eCAU_TRIGSEL_C = 2U
};

enum CauTrigSahPeriod
{
    eCAU_TRIGSAHPERIOD_0P0US = 0x00U,
    eCAU_TRIGSAHPERIOD_0P5US = 0x01U,
    eCAU_TRIGSAHPERIOD_1P0US = 0x02U,
    eCAU_TRIGSAHPERIOD_1P5US = 0x03U,
    eCAU_TRIGSAHPERIOD_2P0US = 0x04U,
    eCAU_TRIGSAHPERIOD_3P0US = 0x05U,
    eCAU_TRIGSAHPERIOD_4P0US = 0x06U,
    eCAU_TRIGSAHPERIOD_5P0US = 0x07U,
};

enum CauTrigSahTrig
{
    eCAU_TRIGSAHTRIG_PWM  =  ( 0x00U << 3 ),
    eCAU_TRIGSAHTRIG_SW   =  ( 0x01U << 3 ),
    eCAU_TRIGSAHTRIG_GTU0 =  ( 0x02U << 3 ),
    eCAU_TRIGSAHTRIG_GTU1 =  ( 0x03U << 3 ),
    eCAU_TRIGSAHTRIG_GTU2 =  ( 0x04U << 3 ),
    eCAU_TRIGSAHTRIG_GTU3 =  ( 0x05U << 3 ),
    eCAU_TRIGSAHTRIG_GPIO0 = ( 0x06U << 3 ),
    eCAU_TRIGSAHTRIG_GPIO1 = ( 0x07U << 3 ),
    eCAU_TRIGSAHTRIG_GPIO2 = ( 0x08U << 3 ),
    eCAU_TRIGSAHTRIG_GPIO3 = ( 0x09U << 3 ),
    eCAU_TRIGSAHTRIG_GPIO4 = ( 0x0AU << 3 ),
    eCAU_TRIGSAHTRIG_GPIO5 = ( 0x0BU << 3 ),
    eCAU_TRIGSAHTRIG_GPIO6 = ( 0x0CU << 3 ),
    eCAU_TRIGSAHTRIG_GPIO7 = ( 0x0DU << 3 ),
};

enum CauTrigSahTrigEdge
{
    eCAU_TRIGSAHTRIGEDGE_RISING =             ( 0x00U << 7 ),
    eCAU_TRIGSAHTRIGEDGE_FALLING =            ( 0x01U << 7 ),
    eCAU_TRIGSAHTRIGEDGE_RISING_OR_FALLING =  ( 0x02U << 7 ),
};

enum CauSwTrig
{
    eCAU_SWTRIG_DISABLED =      0U,
    eCAU_SWTRIG_A_ENABLED =     1U,
    eCAU_SWTRIG_B_ENABLED =     2U,
    eCAU_SWTRIG_BA_ENABLED =    3U,
    eCAU_SWTRIG_C_ENABLED =     4U,
    eCAU_SWTRIG_CA_ENABLED =    5U,
    eCAU_SWTRIG_CB_ENABLED =    6U,
    eCAU_SWTRIG_ALL_ENABLED =   7U
};

enum CauCalSelect
{
    eCAU_CALSELECT_A = 0U,
    eCAU_CALSELECT_B = 1U,
    eCAU_CALSELECT_C = 2U
};

enum CauCalTrigSourceSel
{
    eCAU_CALTRIGSOURCESELECT_PWM  =  0x00U,
    eCAU_CALTRIGSOURCESELECT_SW   =  0x01U,
    eCAU_CALTRIGSOURCESELECT_GTU0 =  0x02U,
    eCAU_CALTRIGSOURCESELECT_GTU1 =  0x03U,
    eCAU_CALTRIGSOURCESELECT_GTU2 =  0x04U,
    eCAU_CALTRIGSOURCESELECT_GTU3 =  0x05U,
    eCAU_CALTRIGSOURCESELECT_GPIO0 = 0x06U,
    eCAU_CALTRIGSOURCESELECT_GPIO1 = 0x07U,
    eCAU_CALTRIGSOURCESELECT_GPIO2 = 0x08U,
    eCAU_CALTRIGSOURCESELECT_GPIO3 = 0x09U,
    eCAU_CALTRIGSOURCESELECT_GPIO4 = 0x0AU,
    eCAU_CALTRIGSOURCESELECT_GPIO5 = 0x0BU,
    eCAU_CALTRIGSOURCESELECT_GPIO6 = 0x0CU,
    eCAU_CALTRIGSOURCESELECT_GPIO7 = 0x0DU,
};

enum CauCalEdgeSel
{
    eCAU_CALEDGESELECT_RISING =             ( 0x00U << 4 ),
    eCAU_CALEDGESELECT_FALLING =            ( 0x01U << 4 ),
    eCAU_CALEDGESELECT_RISING_OR_FALLING =  ( 0x02U << 4 ),
};


enum CauSwTrigCal
{
    eCAU_SWTRIGCAL_DISABLED =      0U,
    eCAU_SWTRIGCAL_A_ENABLED =     1U,
    eCAU_SWTRIGCAL_B_ENABLED =     2U,
    eCAU_SWTRIGCAL_BA_ENABLED =    3U,
    eCAU_SWTRIGCAL_C_ENABLED =     4U,
    eCAU_SWTRIGCAL_CA_ENABLED =    5U,
    eCAU_SWTRIGCAL_CB_ENABLED =    6U,
    eCAU_SWTRIGCAL_ALL_ENABLED =   7U
};

enum CauIirSelect
{
    eCAU_IIRSELECT_A = 0U,
    eCAU_IIRSELECT_B = 1U,
    eCAU_IIRSELECT_C = 2U
};

enum CauIir                      // Infinite Impulse Response filter settings.
{
    eCAU_IIR_COEFFICIENT_DIV1 = 0x0U,
    eCAU_IIR_COEFFICIENT_DIV2 = 0x1U,
    eCAU_IIR_COEFFICIENT_DIV4 = 0x2U,
    eCAU_IIR_COEFFICIENT_DIV8 = 0x3U,
    eCAU_IIR_ENABLE =           0x4U,
    eCAU_IIR_DISABLE =          0x5U
};

enum CauStatusErr
{
    eCAU_CONVSTATUSERR_OVR_A =            0b0000000000000001U ,
    eCAU_CONVSTATUSERR_OVR_B =            0b0000000000000010U ,
    eCAU_CONVSTATUSERR_OVR_C =            0b0000000000000100U ,
    eCAU_CONVSTATUSERR_OOR_A =            0b0000000000001000U ,
    eCAU_CONVSTATUSERR_OOR_B =            0b0000000000010000U ,
    eCAU_CONVSTATUSERR_OOR_C =            0b0000000000100000U ,
    eCAU_CONVSTATUSERR_CONFIG_ERROR_A =   0b0000000001000000U ,
    eCAU_CONVSTATUSERR_CONFIG_ERROR_B =   0b0000000010000000U ,
    eCAU_CONVSTATUSERR_CONFIG_ERROR_C =   0b0000000100000000U ,
    eCAU_CONVSTATUSERR_TRIG_SKIP_A =      0b0000001000000000U ,
    eCAU_CONVSTATUSERR_TRIG_SKIP_B =      0b0000010000000000U ,
    eCAU_CONVSTATUSERR_TRIG_SKIP_C =      0b0000100000000000U ,
    eCAU_CONVSTATUSERR_TRIG_PWRUP =       0b0001000000000000U ,
    eCAU_CONVSTATUSERR_TRIG_ERR_A =       0b0010000000000000U ,
    eCAU_CONVSTATUSERR_TRIG_ERR_B =       0b0100000000000000U ,
    eCAU_CONVSTATUSERR_TRIG_ERR_C =       0b1000000000000000U ,
    eCAU_CONVSTATUSERR_ANY =              0b1111111111111111U   // Will clear any set Irqs
};


enum CauIrqErrorEnable
{
    eCAU_IRQERRORENABLE_NONE =            0,
    eCAU_IRQERRORENABLE_OVR_A =            0b0000000000000001U ,
    eCAU_IRQERRORENABLE_OVR_B =            0b0000000000000010U ,
    eCAU_IRQERRORENABLE_OVR_C =            0b0000000000000100U ,
    eCAU_IRQERRORENABLE_OOR_A =            0b0000000000001000U ,
    eCAU_IRQERRORENABLE_OOR_B =            0b0000000000010000U ,
    eCAU_IRQERRORENABLE_OOR_C =            0b0000000000100000U ,
    eCAU_IRQERRORENABLE_CONFIG_ERROR_A =   0b0000000001000000U ,
    eCAU_IRQERRORENABLE_CONFIG_ERROR_B =   0b0000000010000000U ,
    eCAU_IRQERRORENABLE_CONFIG_ERROR_C =   0b0000000100000000U ,
    eCAU_IRQERRORENABLE_TRIG_SKIP_A =      0b0000001000000000U ,
    eCAU_IRQERRORENABLE_TRIG_SKIP_B =      0b0000010000000000U ,
    eCAU_IRQERRORENABLE_TRIG_SKIP_C =      0b0000100000000000U ,
    eCAU_IRQERRORENABLE_TRIG_PWRUP =       0b0001000000000000U ,
    eCAU_IRQERRORENABLE_TRIG_ERR_A =       0b0010000000000000U ,
    eCAU_IRQERRORENABLE_TRIG_ERR_B =       0b0100000000000000U ,
    eCAU_IRQERRORENABLE_TRIG_ERR_C =       0b1000000000000000U 
};

enum CauPwrUpStatus
{
    eCAU_PWRUPSTATUS_ALL_DISABLED =  0U,
    eCAU_PWRUPSTATUS_A_PWRED =   1U,
    eCAU_PWRUPSTATUS_B_PWRED =   2U,
    eCAU_PWRUPSTATUS_BA_PWRED =  3U,
    eCAU_PWRUPSTATUS_C_PWRED =   4U,
    eCAU_PWRUPSTATUS_CA_PWRED =  5U,
    eCAU_PWRUPSTATUS_CB_PWRED =  6U,
    eCAU_PWRUPSTATUS_ALL_PWRED = 7U
};

enum CauConvStatus
{
    eCAU_CONVSTATUS_CONVERSION_COMPLETE_IRQ_A =   0b0000000000000001U ,
    eCAU_CONVSTATUS_CONVERSION_COMPLETE_IRQ_B =   0b0000000000000010U ,
    eCAU_CONVSTATUS_CONVERSION_COMPLETE_IRQ_C =   0b0000000000000100U ,
    eCAU_CONVSTATUS_CALIBRATION_COMPLETE_IRQ_A =  0b0000000000001000U ,
    eCAU_CONVSTATUS_CALIBRATION_COMPLETE_IRQ_B =  0b0000000000010000U ,
    eCAU_CONVSTATUS_CALIBRATION_COMPLETE_IRQ_C =  0b0000000000100000U ,
    eCAU_CONVSTATUS_ANY =                         0b0000000000111111U   // Will clear any set Irqs
};

enum CauConvIrqEnable
{
    eCAU_CONVIRQENABLE_NONE = 0,
    eCAU_CONVIRQENABLE_CONVERSION_COMPLETE_IRQ_A =   0b0000000000000001U ,
    eCAU_CONVIRQENABLE_CONVERSION_COMPLETE_IRQ_B =   0b0000000000000010U ,
    eCAU_CONVIRQENABLE_CONVERSION_COMPLETE_IRQ_C =   0b0000000000000100U ,
    eCAU_CONVIRQENABLE_CALIBRATION_COMPLETE_IRQ_A =  0b0000000000001000U ,
    eCAU_CONVIRQENABLE_CALIBRATION_COMPLETE_IRQ_B =  0b0000000000010000U ,
    eCAU_CONVIRQENABLE_CALIBRATION_COMPLETE_IRQ_C =  0b0000000000100000U 
};

enum CauOffsetSelect
{
    eCAU_OFFSETSELECT_A = 0U,
    eCAU_OFFSETSELECT_B = 1U,
    eCAU_OFFSETSELECT_C = 2U
};



enum CauResultSelect
{
    eCAU_RESULTSELECT_A = 0U,
    eCAU_RESULTSELECT_B = 1U,
    eCAU_RESULTSELECT_C = 2U
};


enum CauDmaConfig
{
    eCAU_DMACONFIG_ACQUISITIONS_BEFORE_DMA_0 = 0U,
    eCAU_DMACONFIG_ACQUISITIONS_BEFORE_DMA_1 = 1U,
    eCAU_DMACONFIG_ACQUISITIONS_BEFORE_DMA_2 = 2U,
    eCAU_DMACONFIG_ACQUISITIONS_BEFORE_DMA_3 = 3U,

    eCAU_DMACONFIG_ENABLE_RESET = 0xE3U,
    eCAU_DMACONFIG_ENABLE_A = ( 0x0001U ) << 2,
    eCAU_DMACONFIG_ENABLE_B = ( 0x0001U ) << 3,
    eCAU_DMACONFIG_ENABLE_C = ( 0x0001U ) << 4
};

/**
 * @internal
 * @brief Define register and base addresses
 */
typedef struct __attribute__((packed)) {
    uint32 CauControl;            // 0x00  :(R/W)
    uint32 CauAdcA;               // 0x04  :(R/W)
    uint32 CauAdcB;               // 0x08  :(R/W)
    uint32 CauAdcC;               // 0x0C  :(R/W)
    uint32 CauTrigA;              // 0x10  :(R/W)
    uint32 CauTrigB;              // 0x14  :(R/W)
    uint32 CauTrigC;              // 0x18  :(R/W)
    uint32 CauSwtrig;             // 0x1C  :(R/W)
    uint32 CauCalibrationA;       // 0x20  :(R/W)
    uint32 CauCalibrationB;       // 0x24  :(R/W)
    uint32 CauCalibrationC;       // 0x28  :(R/W)
    uint32 CauSwtrigCal;          // 0x2C  :(R/W)
    uint32 CauIirA;               // 0x30  :(R/W)
    uint32 CauIirB;               // 0x34  :(R/W)
    uint32 CauIirC;               // 0x38  :(R/W)
    uint32 CauErrStatus;          // 0x3C  :(R/1C)
    uint32 CauErrInten;           // 0x40  :(R/W)
    uint32 CauPwrupStatus;        // 0x44  :(R)
    uint32 CauConvStatus;         // 0x48  :(R/1C)
    uint32 CauConvInten;          // 0x4C  :(R/W)
    uint32 CauOffsetA;            // 0x50  :(R)
    uint32 CauOffsetB;            // 0x54  :(R)
    uint32 CauOffsetC;            // 0x58  :(R)
    uint32 CauResultA;            // 0x5C  :(R)
    uint32 CauResultB;            // 0x60  :(R)
    uint32 CauResultC;            // 0x64  :(R)
    uint32 CauDmaConfig;          // 0x68  :(R/W)
} CauRegs_t;

#define  stCauRegs (*( volatile CauRegs_t *)eADDRESS_CAU_BASE)


/**
 * @brief       Get ADC result A
 * @remarks     This is an optimized replacement for GetResult. It's use should be minimized as it is an inline function.
 * @returns     Result
 */
__attribute__( ( always_inline ) ) static inline  sint16 Cau_GetResultAFast( void )
{
    return (sint16)stCauRegs.CauResultA;
}


/**
 * @brief       Get ADC result B
 * @remarks     This is an optimized replacement for GetResult. It's use should be minimized as it is an inline function.
 * @returns     Result
 */
__attribute__( ( always_inline ) ) static inline  sint16 Cau_GetResultBFast( void )
{
    return (sint16)stCauRegs.CauResultB;
}


/**
 * @brief       Get ADC result C
 * @remarks     This is an optimized replacement for GetResult. It's use should be minimized as it is an inline function.
 * @returns     Result
 */
__attribute__( ( always_inline ) ) static inline  sint16 Cau_GetResultCFast( void )
{
    return (sint16)stCauRegs.CauResultC;
}


/**
 * @brief     Set converion status bit(s) to clear. Only bits set in input will be cleared.
 * @remarks     This is an optimized replacement for SetConvStatus. It's use should be minimized as it is an inline function.
 * @param[in]   keStatus Status bit to set - to clear.
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline  void Cau_SetConvStatusFast( enum CauConvStatus const keStatus )
{
    if ( keStatus == eCAU_CONVSTATUS_ANY )
    {
        stCauRegs.CauConvStatus = (uint32)eCAU_CONVSTATUS_ANY;
    }
    else
    {
        stCauRegs.CauConvStatus |= (uint32)keStatus;
    }
}


/**
 * @brief    Set converiosn status interrupt enable bit
 * @remarks    This is an optimized replacement for SetConvStatusIrqEnable. It's use should be minimized as it is an inline function.
 * @param[in]  keIrqEnable Interrupt enable.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline  void Cau_SetConvIrqEnableFast( enum CauConvIrqEnable const keIrqEnable )
{
    if ( keIrqEnable == eCAU_CONVIRQENABLE_NONE )
    {
        stCauRegs.CauConvInten = 0;
    }
    else
    {
        stCauRegs.CauConvInten |= (uint32)keIrqEnable;
    }
}

/**
 * @brief    Enable/Disble CAU
 * @param[in]  kbState boolean enable or disable
 * @returns    None
 */
void Cau_SetEnable( boolean const kbState );

/**
 * @brief    Get enable bit of CAU
 * @param[in]  None
 * @returns    state of enable bit
 */
boolean Cau_GetEnable( void );

/**
 * @brief    Clear selected ADC configuration
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @returns    None
 */
void Cau_ClearAdcConfig( enum CauAdcSelect const keAdcSelect );

/**
 * @brief    Set selected ADC with calibration enable
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  keAdcConfigCalibrationEnable ADC calibration enable
 * @returns    None
 */
void Cau_SetAdcConfigCalibEnable( enum CauAdcSelect const keAdcSelect, enum CauAdcConfigCalibrationEnable const keAdcConfigCalibrationEnable  );

/**
 * @brief    Set selected ADC with conversion speed
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  keAdcConfigConvSpeed ADC conversion speed
 * @returns    None
 */
void Cau_SetAdcConfigConvSpeed( enum CauAdcSelect const keAdcSelect, enum CauAdcConfigConvSpeed const keAdcConfigConvSpeed  );

/**
 * @brief    Set selected ADC enable
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  AdcConfigAdcEnable ADC enable
 * @returns    None
 */
void Cau_SetAdcConfigAdcEnable( enum CauAdcSelect const keAdcSelect, enum CauAdcConfigAdcEnable const keAdcConfigAdcEnable  );

/**
 * @brief    Set selected ADC with overwrite permission
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  keAdcConfigOverwritePermission ADC overwrite permission
 * @returns    None
 */
void Cau_SetAdcConfigOverwritePermission( enum CauAdcSelect const keAdcSelect, enum CauAdcConfigOverwritePermission const keAdcConfigOverwritePermission  );

/**
 * @brief    Get configuration parameters of ADC.
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @returns    ADC config parameters
 */
uint16 Cau_GetAdcConfig( enum CauAdcSelect const keAdcSelect );

/**
 * @brief    Clear selected Trig register
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @returns    None
 */
void Cau_ClearTrig( enum CauTrigSelect const keAdcSelect );

/**
 * @brief    Set Trig sample and hold period.
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  keTrigSAHPeriod Trig SAH period
 * @returns    None
 */
void Cau_SetTrigSahPeriod( enum CauTrigSelect const keAdcSelect, enum CauTrigSahPeriod const keTrigSahPeriod );

/**
 * @brief    Set Trig sample and hold trigger selection.
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  keTrigSAHTrig Trig SAH trigger select
 * @returns    None
 */
void Cau_SetTrigSahTrig( enum CauTrigSelect const keAdcSelect, enum CauTrigSahTrig const keTrigSahTrig );

/**
 * @brief    Set Trig sample and hold edge selection.
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  keTrigSAHTrigEdge Trig SAH trigger edge select
 * @returns    None
 */
void Cau_SetTrigSahTrigEdge( enum CauTrigSelect const keAdcSelect, enum CauTrigSahTrigEdge const keTrigSahTrigEdge );

/**
 * @brief    Get TRIG configuration parameters: sample and hold, trigger selection and edge selection.
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @returns    ADC trigger config
 */
uint16 Cau_GetTrig( enum CauTrigSelect const keAdcSelect );

/**
 * @brief    Set SW trigger enable
 * @param[in]  keSwTrig 3-bit Sw trig enables for ADC A, B and C
 * @returns    None
 */
void Cau_SetSwTrig( enum CauSwTrig const keSwTrig );

/**
 * @brief    Get SW trigger enable
 * @param[in]  None
 * @returns    SWTrig value
 */
enum CauSwTrig Cau_GetSwTrig( void );

/**
 * @brief    Clear selected Cal register
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @returns    None
 */
void Cau_ClearCal( enum CauCalSelect const keAdcSelect );

/**
 * @brief    Set calibration trigger source selection of one ADC channel
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  keCalTrigSourceSel ADC calibration trigger source selection
 * @returns    None
 */
void Cau_SetCalTrigSourceSel( enum CauCalSelect const keAdcSelect, enum CauCalTrigSourceSel const keCalTrigSourceSel );

/**
 * @brief    Set calibration trigger edge selection of one ADC channel
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  keCalEdgeSel ADC calibration trigger edge selection
 * @returns    None
 */
void Cau_SetCalEdgeSel( enum CauCalSelect const keAdcSelect, enum CauCalEdgeSel const keCalEdgeSel );

/**
 * @brief    Get calibration config of one ADC channel
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @returns    ADC cal config
 */
uint16 Cau_GetCal( enum CauCalSelect const keAdcSelect );

/**
 * @brief    Set SW trigger calibration enable
 * @param[in]  keSwTrigCal 3-bit Sw trig calibration enables for ADC A, B and C
 * @returns    None
 */
void Cau_SetSwTrigCal( enum CauSwTrigCal const keSwTrigCal );

/**
 * @brief    Set SW trigger calibration enable
 * @param[in]  None
 * @returns    SW trigger calibration value
 */
enum CauSwTrigCal Cau_GetSwTrigCal( void );

/**
 * @brief    Set infinite impulse response filter config
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @param[in]  keIir IIR value: filter enable and coefficient
 * @returns    None
 */
void Cau_SetIir( enum CauIirSelect const keAdcSelect, enum CauIir const keIir );

/**
 * @brief    Get infinite impulse response filter config
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @returns    IIR value: filter enable and coefficient
 */
enum CauIir Cau_GetIir( enum CauIirSelect const keAdcSelect );

/**
 * @brief    Set error status bit to clear. Only bit set in input will be cleared.
 * @param[in]  keStatus Status bit to set.
 * @returns    None
 */
void Cau_SetStatusError( enum CauStatusErr const keStatus );

/**
 * @brief    Get error status bit(s)
 * @param[in]  keStatus Status bit(s) to read.
 * @returns    true/false
 */
boolean Cau_GetStatusError( enum CauStatusErr const keStatus );

/**
 * @brief    Set interrupt enable for error status bits
 * @param[in]  keIntErr Interrupt enable bit
 * @returns    None
 */
void Cau_SetIrqErrorEnable( enum CauIrqErrorEnable const keIntErr );

/**
 * @brief    Get interrupt enable for error status bits
 * @param[in]  None
 * @returns    Interrupt enable bits
 */
enum CauIrqErrorEnable Cau_GetIrqErrorEnable( void );

/**
 * @brief    Get power up status of the ADCs
 * @param[in]  None
 * @returns    3-bit power-up status of ADC A, B and C
 */
enum CauPwrUpStatus Cau_GetPwrUpStatus( void );

/**
 * @brief    Get conversion status bit(s) to reset (R/1C)
 * @param[in]  keStatus Interrupt status bit to clear.
 * @returns    None
 */
void Cau_SetConvStatus( enum CauConvStatus const keStatus );

/**
 * @brief    Get conversion status bit(s)
 * @param[in]  None
 * @returns    Interrupt status
 */
boolean Cau_GetConvStatus( enum CauConvStatus const keStatus );

/**
 * @brief    Set a conversion interrupt enable
 * @param[in]  keIrqEnable Interrupt enable.
 * @returns    None
 */
void Cau_SetConvIrqEnable( enum CauConvIrqEnable const keIrqEnable );

/**
 * @brief    Get conversion interrupt enables
 * @param[in]  None
 * @returns    Interrupt enables.
 */
enum CauConvIrqEnable Cau_GetConvIrqEnable( void );

/**
 * @brief    Get offset result in one of ADCs
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @returns    16-bit offset
 */
uint16 Cau_GetOffset ( enum CauOffsetSelect const keAdcSelect );

/**
 * @brief    Get ADC result
 * @param[in]  keAdcSelect ADC channel A, B or C
 * @returns    16-bit result
 */
sint16 Cau_GetResult ( enum CauResultSelect const keAdcSelect );

/**
 * @brief    Set DMA configuration, preserves register state between calls.
 * To clear the state call with eENABLE_RESET enumerated type.
 * @param[in]  keDmaConfig Number of acquisitions + enable per ADC
 * @returns    None
 */
void Cau_SetDmaConfig ( enum CauDmaConfig const keDmaConfig );

/**
 * @brief    Get DMA configuration
 * @param[in]  None
 * @returns    DMA config
 */
enum CauDmaConfig Cau_GetDmaConfig( void );

/**
 * @brief    Set CAU IRQ handler
 * @param[in]  rclsIrqHandler  - an IRQ handler
 * @param[in]  uPrData - an argument to pass to the IRQ handler
 * @returns    None
 */
void Cau_SetIrqHandler( void (pfnIrqHandler)( void), const uintptr uPrData );

/**
 * @brief    The CAU IRQ handler
 * @returns    None
 */
void Cau_IrqHandler(void);

#endif //HEADER_CAU_H