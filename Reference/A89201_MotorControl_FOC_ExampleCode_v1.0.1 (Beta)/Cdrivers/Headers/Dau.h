#ifndef HEADER_DAU_H
#define HEADER_DAU_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Dau.h
 * @author       N. Moheeputh
 * @version      1.0
 *
 * @brief        Data Acquisition Unit driver: DAU
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

// DAU Definitions
// ---------------

// DauRegisters
#define DAU_BASE_ADDRESS     0x40002000U
#define DAU_CTRL                   0x00U
#define DAU_SWTRIG                 0x04U
#define DAU_CONFIG_SLOT0           0x08U
#define DAU_CONFIG_SLOT1           0x0CU
#define DAU_CONFIG_SLOT2           0x10U
#define DAU_CONFIG_SLOT3           0x14U
#define DAU_CONFIG_SLOT4           0x18U
#define DAU_CONFIG_SLOT5           0x1CU
#define DAU_CONFIG_SLOT6           0x20U
#define DAU_CONFIG_SLOT7           0x24U
#define DAU_CONFIG_SLOT8           0x28U
#define DAU_CONFIG_SLOT9           0x2CU
#define DAU_CONFIG_SLOT10          0x30U
#define DAU_CONFIG_SLOT11          0x34U
#define DAU_CONFIG_SLOT12          0x38U
#define DAU_CONFIG_SLOT13          0x3CU
#define DAU_CONFIG_SLOT14          0x40U
#define DAU_CONFIG_SLOT15          0x44U
#define DAU_ERROR_STATUS           0x48U
#define DAU_INT_ERR_ENABLE         0x4CU
#define DAU_OVER_RUN_STATUS        0x50U
#define DAU_OVER_RUN_ENABLE        0x54U
#define DAU_CONV_SLOT_STATUS       0x58U
#define DAU_CONV_SLOT_COMP_STATUS  0x5CU
#define DAU_CONV_SLOT_COMP_INTEN   0x60U
#define DAU_RESULT_SLOT0           0x64U
#define DAU_RESULT_SLOT1           0x68U
#define DAU_RESULT_SLOT2           0x6CU
#define DAU_RESULT_SLOT3           0x70U
#define DAU_RESULT_SLOT4           0x74U
#define DAU_RESULT_SLOT5           0x78U
#define DAU_RESULT_SLOT6           0x7CU
#define DAU_RESULT_SLOT7           0x80U
#define DAU_RESULT_SLOT8           0x84U
#define DAU_RESULT_SLOT9           0x88U
#define DAU_RESULT_SLOT10          0x8CU
#define DAU_RESULT_SLOT11          0x90U
#define DAU_RESULT_SLOT12          0x94U
#define DAU_RESULT_SLOT13          0x98U
#define DAU_RESULT_SLOT14          0x9CU
#define DAU_RESULT_SLOT15          0xA0U
#define DAU_DMA                    0xA4U
#define DAU_DMAEN                  0xA8U
#define DAU_APPEVAL                0xFFCU


// DauControlMode
#define DAU_CONTROL_SINGLE_SRC           0x0000U
#define DAU_CONTROL_COMMNON_TRIG         0x0001U
#define DAU_CONTROL_SEQUENT_TRIG         0x0002U
#define DAU_CONTROL_MASK                 0xFFF8U

// DauControlTrigEdgeSel
#define DAU_CONTROL_TRIGEDGESEL_RISING          ( 0x0000U )
#define DAU_CONTROL_TRIGEDGESEL_FALLING         ( 0x0008U )
#define DAU_CONTROL_TRIGEDGESEL_RISING_FALLING  ( 0x0010U )
#define DAU_CONTROL_TRIGEDGESEL_MASK            ( 0xFFE7U )

// DauControlTrigSel
#define DAU_CONTROL_TRIGSEL_PWM_CENTRE       ( 0x0000U )
#define DAU_CONTROL_TRIGSEL_SWTRIG           ( 0x0020U )
#define DAU_CONTROL_TRIGSEL_GTU0             ( 0x0040U )
#define DAU_CONTROL_TRIGSEL_GTU1             ( 0x0060U )
#define DAU_CONTROL_TRIGSEL_GTU2             ( 0x0080U )
#define DAU_CONTROL_TRIGSEL_GTU3             ( 0x00A0U )
#define DAU_CONTROL_TRIGSEL_GPIO0            ( 0x00C0U )
#define DAU_CONTROL_TRIGSEL_GPIO1            ( 0x00E0U )
#define DAU_CONTROL_TRIGSEL_GPIO2            ( 0x0100U )
#define DAU_CONTROL_TRIGSEL_GPIO3            ( 0x0120U )
#define DAU_CONTROL_TRIGSEL_GPIO4            ( 0x0140U )
#define DAU_CONTROL_TRIGSEL_GPIO5            ( 0x0160U )
#define DAU_CONTROL_TRIGSEL_GPIO6            ( 0x0180U )
#define DAU_CONTROL_TRIGSEL_GPIO7            ( 0x01A0U )
#define DAU_CONTROL_TRIGSEL_MASK             ( 0xFE1FU )

// DauControlCnvSpeed
#define DAU_CONTROL_CNVSPEED_1P0US          ( 0x0000U )
#define DAU_CONTROL_CNVSPEED_1P5US          ( 0x0200U )
#define DAU_CONTROL_CNVSPEED_2P0US          ( 0x0400U )
#define DAU_CONTROL_CNVSPEED_2P5US          ( 0x0600U )
#define DAU_CONTROL_CNVSPEED_MASK           ( 0xF9FFU )


// DauControlExtRef
#define DAU_CONTROL_EXTREF_INT_MASK         ( 0xF7FFU )
#define DAU_CONTROL_EXTREF_EXT              ( 0x0800U )


// DauControlExtGnd
#define DAU_CONTROL_EXTGND_GND_INT_MASK     ( 0xEFFFU )
#define DAU_CONTROL_EXTGND_GND_EXT          ( 0x1000U )


// DauControlDauEnable
#define DAU_CONTROL_DAUENABLE_DISABLE_MASK   ( 0xDFFFU )
#define DAU_CONTROL_DAUENABLE_ENABLE         ( 0x2000U )

// DauSwTrig
#define DAU_DAU_SWTRIG_DISABLED  0x00U
#define DAU_DAU_SWTRIG_ENABLED   0x01U

// DauConfigChannel
#define DAU_CONFIGCHANNEL_CHANNEL1   0x00U
#define DAU_CONFIGCHANNEL_CHANNEL2   0x01U
#define DAU_CONFIGCHANNEL_CHANNEL3   0x02U
#define DAU_CONFIGCHANNEL_CHANNEL4   0x03U
#define DAU_CONFIGCHANNEL_CHANNEL5   0x04U
#define DAU_CONFIGCHANNEL_CHANNEL6   0x05U
#define DAU_CONFIGCHANNEL_CHANNEL7   0x06U
#define DAU_CONFIGCHANNEL_CHANNEL8   0x07U
#define DAU_CONFIGCHANNEL_CHANNEL9   0x08U
#define DAU_CONFIGCHANNEL_CHANNEL10  0x09U
#define DAU_CONFIGCHANNEL_CHANNEL11  0x0AU
#define DAU_CONFIGCHANNEL_CHANNEL12  0x0BU
#define DAU_CONFIGCHANNEL_CHANNEL13  0x0CU
#define DAU_CONFIGCHANNEL_CHANNEL14  0x0DU
#define DAU_CONFIGCHANNEL_CHANNEL15  0x0EU
#define DAU_CONFIGCHANNEL_CHANNEL16  0x0FU
#define DAU_CONFIGCHANNEL_CHANNEL_MASK   0xFFF0U


// DauConfigSlotEnable
#define DAU_CONFIGSLOTENABLE_DISABLE_MASK ( 0xFFEFU )
#define DAU_CONFIGSLOTENABLE_ENABLE       ( 0x0010U )

// DauConfigSAHPeriod
#define DAU_CONFIGSAHPERIOD_1US    ( 0x0000U )
#define DAU_CONFIGSAHPERIOD_2US    ( 0x0020U )
#define DAU_CONFIGSAHPERIOD_3US    ( 0x0040U )
#define DAU_CONFIGSAHPERIOD_5US    ( 0x0060U )
#define DAU_CONFIGSAHPERIOD_10US   ( 0x0080U )
#define DAU_CONFIGSAHPERIOD_25US   ( 0x00A0U )
#define DAU_CONFIGSAHPERIOD_125US  ( 0x00C0U )
#define DAU_CONFIGSAHPERIOD_625US  ( 0x00E0U )
#define DAU_CONFIGSAHPERIOD_MASK   ( 0xFF1FU )

// DauErrorStatus
#define DAU_ERRORSTATUS_ANY_ERRORS          ( 0b1000000000000111U )
#define DAU_ERRORSTATUS_TRIG_SKIP_ERROR     ( 0b0000000000000001U )
#define DAU_ERRORSTATUS_ADC_ERROR           ( 0b0000000000000010U )
#define DAU_ERRORSTATUS_CONFIG_ERROR        ( 0b0000000000000100U )
#define DAU_ERRORSTATUS_OVERRUN             ( 0b1000000000000000U )

// DauIrqErrorEnable
#define DAU_IRQERRORENABLE_NONE                0
#define DAU_IRQERRORENABLE_TRIG_SKIP_ERROR     ( 0b00000001U )
#define DAU_IRQERRORENABLE_ADC_ERROR           ( 0b00000010U )
#define DAU_IRQERRORENABLE_CONFIG_ERROR        ( 0b00000100U )

// DauOverRunStatus
#define DAU_OVERRUNSTATUS_NONE    0
#define DAU_OVERRUNSTATUS_SLOT0   ( 0b0000000000000001U )
#define DAU_OVERRUNSTATUS_SLOT1   ( 0b0000000000000010U )
#define DAU_OVERRUNSTATUS_SLOT2   ( 0b0000000000000100U )
#define DAU_OVERRUNSTATUS_SLOT3   ( 0b0000000000001000U )
#define DAU_OVERRUNSTATUS_SLOT4   ( 0b0000000000010000U )
#define DAU_OVERRUNSTATUS_SLOT5   ( 0b0000000000100000U )
#define DAU_OVERRUNSTATUS_SLOT6   ( 0b0000000001000000U )
#define DAU_OVERRUNSTATUS_SLOT7   ( 0b0000000010000000U )
#define DAU_OVERRUNSTATUS_SLOT8   ( 0b0000000100000000U )
#define DAU_OVERRUNSTATUS_SLOT9   ( 0b0000001000000000U )
#define DAU_OVERRUNSTATUS_SLOT10  ( 0b0000010000000000U )
#define DAU_OVERRUNSTATUS_SLOT11  ( 0b0000100000000000U )
#define DAU_OVERRUNSTATUS_SLOT12  ( 0b0001000000000000U )
#define DAU_OVERRUNSTATUS_SLOT13  ( 0b0010000000000000U )
#define DAU_OVERRUNSTATUS_SLOT14  ( 0b0100000000000000U )
#define DAU_OVERRUNSTATUS_SLOT15  ( 0b1000000000000000U )
#define DAU_OVERRUNSTATUS_ANY     ( 0b1111111111111111U )

// DauOverRunEnable
#define DAU_OVERRUNENABLE_NONE   0
#define DAU_OVERRUNENABLE_SLOT0   ( 0b0000000000000001U )
#define DAU_OVERRUNENABLE_SLOT1   ( 0b0000000000000010U )
#define DAU_OVERRUNENABLE_SLOT2   ( 0b0000000000000100U )
#define DAU_OVERRUNENABLE_SLOT3   ( 0b0000000000001000U )
#define DAU_OVERRUNENABLE_SLOT4   ( 0b0000000000010000U )
#define DAU_OVERRUNENABLE_SLOT5   ( 0b0000000000100000U )
#define DAU_OVERRUNENABLE_SLOT6   ( 0b0000000001000000U )
#define DAU_OVERRUNENABLE_SLOT7   ( 0b0000000010000000U )
#define DAU_OVERRUNENABLE_SLOT8   ( 0b0000000100000000U )
#define DAU_OVERRUNENABLE_SLOT9   ( 0b0000001000000000U )
#define DAU_OVERRUNENABLE_SLOT10  ( 0b0000010000000000U )
#define DAU_OVERRUNENABLE_SLOT11  ( 0b0000100000000000U )
#define DAU_OVERRUNENABLE_SLOT12  ( 0b0001000000000000U )
#define DAU_OVERRUNENABLE_SLOT13  ( 0b0010000000000000U )
#define DAU_OVERRUNENABLE_SLOT14  ( 0b0100000000000000U )
#define DAU_OVERRUNENABLE_SLOT15  ( 0b1000000000000000U )

// DauConversionStatus
#define DAU_CONVERSIONSTATUS_SLOT0_READY   0x00U
#define DAU_CONVERSIONSTATUS_SLOT1_READY   0x01U
#define DAU_CONVERSIONSTATUS_SLOT2_READY   0x02U
#define DAU_CONVERSIONSTATUS_SLOT3_READY   0x03U
#define DAU_CONVERSIONSTATUS_SLOT4_READY   0x04U
#define DAU_CONVERSIONSTATUS_SLOT5_READY   0x05U
#define DAU_CONVERSIONSTATUS_SLOT6_READY   0x06U
#define DAU_CONVERSIONSTATUS_SLOT7_READY   0x07U
#define DAU_CONVERSIONSTATUS_SLOT8_READY   0x08U
#define DAU_CONVERSIONSTATUS_SLOT9_READY   0x09U
#define DAU_CONVERSIONSTATUS_SLOT10_READY  0x0AU
#define DAU_CONVERSIONSTATUS_SLOT11_READY  0x0BU
#define DAU_CONVERSIONSTATUS_SLOT12_READY  0x0CU
#define DAU_CONVERSIONSTATUS_SLOT13_READY  0x0DU
#define DAU_CONVERSIONSTATUS_SLOT14_READY  0x0EU
#define DAU_CONVERSIONSTATUS_SLOT15_READY  0x0FU
#define DAU_CONVERSIONSTATUS_SLOT0_BUSY    0x10U
#define DAU_CONVERSIONSTATUS_SLOT1_BUSY    0x11U
#define DAU_CONVERSIONSTATUS_SLOT2_BUSY    0x12U
#define DAU_CONVERSIONSTATUS_SLOT3_BUSY    0x13U
#define DAU_CONVERSIONSTATUS_SLOT4_BUSY    0x14U
#define DAU_CONVERSIONSTATUS_SLOT5_BUSY    0x15U
#define DAU_CONVERSIONSTATUS_SLOT6_BUSY    0x16U
#define DAU_CONVERSIONSTATUS_SLOT7_BUSY    0x17U
#define DAU_CONVERSIONSTATUS_SLOT8_BUSY    0x18U
#define DAU_CONVERSIONSTATUS_SLOT9_BUSY    0x19U
#define DAU_CONVERSIONSTATUS_SLOT10_BUSY   0x1AU
#define DAU_CONVERSIONSTATUS_SLOT11_BUSY   0x1BU
#define DAU_CONVERSIONSTATUS_SLOT12_BUSY   0x1CU
#define DAU_CONVERSIONSTATUS_SLOT13_BUSY   0x1DU
#define DAU_CONVERSIONSTATUS_SLOT14_BUSY   0x1EU
#define DAU_CONVERSIONSTATUS_SLOT15_BUSY   0x1FU
#define DAU_CONVERSIONSTATUS_ADC_READY     0x20U

// DauConvCompStatus
#define DAU_CONVCOMPSTATUS_CLEAR_ALL  0
#define DAU_CONVCOMPSTATUS_SLOT0   ( 0b0000000000000001U )
#define DAU_CONVCOMPSTATUS_SLOT1   ( 0b0000000000000010U )
#define DAU_CONVCOMPSTATUS_SLOT2   ( 0b0000000000000100U )
#define DAU_CONVCOMPSTATUS_SLOT3   ( 0b0000000000001000U )
#define DAU_CONVCOMPSTATUS_SLOT4   ( 0b0000000000010000U )
#define DAU_CONVCOMPSTATUS_SLOT5   ( 0b0000000000100000U )
#define DAU_CONVCOMPSTATUS_SLOT6   ( 0b0000000001000000U )
#define DAU_CONVCOMPSTATUS_SLOT7   ( 0b0000000010000000U )
#define DAU_CONVCOMPSTATUS_SLOT8   ( 0b0000000100000000U )
#define DAU_CONVCOMPSTATUS_SLOT9   ( 0b0000001000000000U )
#define DAU_CONVCOMPSTATUS_SLOT10  ( 0b0000010000000000U )
#define DAU_CONVCOMPSTATUS_SLOT11  ( 0b0000100000000000U )
#define DAU_CONVCOMPSTATUS_SLOT12  ( 0b0001000000000000U )
#define DAU_CONVCOMPSTATUS_SLOT13  ( 0b0010000000000000U )
#define DAU_CONVCOMPSTATUS_SLOT14  ( 0b0100000000000000U )
#define DAU_CONVCOMPSTATUS_SLOT15  ( 0b1000000000000000U )
#define DAU_CONVCOMPSTATUS_ANY     ( 0b1111111111111111U )

// DauConvCompIrqEnable
#define DAU_CONVCOMPIRQENABLE_NONE   0
#define DAU_CONVCOMPIRQENABLE_SLOT0   ( 0b0000000000000001U )
#define DAU_CONVCOMPIRQENABLE_SLOT1   ( 0b0000000000000010U )
#define DAU_CONVCOMPIRQENABLE_SLOT2   ( 0b0000000000000100U )
#define DAU_CONVCOMPIRQENABLE_SLOT3   ( 0b0000000000001000U )
#define DAU_CONVCOMPIRQENABLE_SLOT4   ( 0b0000000000010000U )
#define DAU_CONVCOMPIRQENABLE_SLOT5   ( 0b0000000000100000U )
#define DAU_CONVCOMPIRQENABLE_SLOT6   ( 0b0000000001000000U )
#define DAU_CONVCOMPIRQENABLE_SLOT7   ( 0b0000000010000000U )
#define DAU_CONVCOMPIRQENABLE_SLOT8   ( 0b0000000100000000U )
#define DAU_CONVCOMPIRQENABLE_SLOT9   ( 0b0000001000000000U )
#define DAU_CONVCOMPIRQENABLE_SLOT10  ( 0b0000010000000000U )
#define DAU_CONVCOMPIRQENABLE_SLOT11  ( 0b0000100000000000U )
#define DAU_CONVCOMPIRQENABLE_SLOT12  ( 0b0001000000000000U )
#define DAU_CONVCOMPIRQENABLE_SLOT13  ( 0b0010000000000000U )
#define DAU_CONVCOMPIRQENABLE_SLOT14  ( 0b0100000000000000U )
#define DAU_CONVCOMPIRQENABLE_SLOT15  ( 0b1000000000000000U )

// DauDmaEnable
#define DAU_DMAENABLE_NONE   0
#define DAU_DMAENABLE_SLOT0   ( 0x0001U )
#define DAU_DMAENABLE_SLOT1   ( 0x0001U ) << 1
#define DAU_DMAENABLE_SLOT2   ( 0x0001U ) << 2
#define DAU_DMAENABLE_SLOT3   ( 0x0001U ) << 3
#define DAU_DMAENABLE_SLOT4   ( 0x0001U ) << 4
#define DAU_DMAENABLE_SLOT5   ( 0x0001U ) << 5
#define DAU_DMAENABLE_SLOT6   ( 0x0001U ) << 6
#define DAU_DMAENABLE_SLOT7   ( 0x0001U ) << 7
#define DAU_DMAENABLE_SLOT8   ( 0x0001U ) << 8
#define DAU_DMAENABLE_SLOT9   ( 0x0001U ) << 9
#define DAU_DMAENABLE_SLOT10  ( 0x0001U ) << 10
#define DAU_DMAENABLE_SLOT11  ( 0x0001U ) << 11
#define DAU_DMAENABLE_SLOT12  ( 0x0001U ) << 12
#define DAU_DMAENABLE_SLOT13  ( 0x0001U ) << 13
#define DAU_DMAENABLE_SLOT14  ( 0x0001U ) << 14
#define DAU_DMAENABLE_SLOT15  ( 0x0001U ) << 15

// DauAppEval
#define DAU_APP_EVAL_NONE  0
#define DAU_APP_EVAL_NORMAL      0
#define DAU_APP_EVAL_EVAL_TRIG   1
#define DAU_APP_EVAL_EVAL0  0 << 1
#define DAU_APP_EVAL_EVAL1  1 << 1
#define DAU_APP_EVAL_EVAL2  2 << 1
#define DAU_APP_EVAL_EVAL3  3 << 1
#define DAU_APP_EVAL_EVAL4  4 << 1
#define DAU_APP_EVAL_EVAL5  5 << 1


// DauConfigSlot
#define DAU_CONFIGSLOT_SELECT_SLOT0  0U
#define DAU_CONFIGSLOT_SELECT_SLOT1  1U
#define DAU_CONFIGSLOT_SELECT_SLOT2  2U
#define DAU_CONFIGSLOT_SELECT_SLOT3  3U
#define DAU_CONFIGSLOT_SELECT_SLOT4  4U
#define DAU_CONFIGSLOT_SELECT_SLOT5  5U
#define DAU_CONFIGSLOT_SELECT_SLOT6  6U
#define DAU_CONFIGSLOT_SELECT_SLOT7  7U
#define DAU_CONFIGSLOT_SELECT_SLOT8  8U
#define DAU_CONFIGSLOT_SELECT_SLOT9  9U
#define DAU_CONFIGSLOT_SELECT_SLOT10  10U
#define DAU_CONFIGSLOT_SELECT_SLOT11  11U
#define DAU_CONFIGSLOT_SELECT_SLOT12  12U
#define DAU_CONFIGSLOT_SELECT_SLOT13  13U
#define DAU_CONFIGSLOT_SELECT_SLOT14  14U
#define DAU_CONFIGSLOT_SELECT_SLOT15  15U

// DauResultSlotSelect
#define DAU_RESULTSLOTSELECT_SLOT0  0U
#define DAU_RESULTSLOTSELECT_SLOT1  1U
#define DAU_RESULTSLOTSELECT_SLOT2  2U
#define DAU_RESULTSLOTSELECT_SLOT3  3U
#define DAU_RESULTSLOTSELECT_SLOT4  4U
#define DAU_RESULTSLOTSELECT_SLOT5  5U
#define DAU_RESULTSLOTSELECT_SLOT6  6U
#define DAU_RESULTSLOTSELECT_SLOT7  7U
#define DAU_RESULTSLOTSELECT_SLOT8  8U
#define DAU_RESULTSLOTSELECT_SLOT9  9U
#define DAU_RESULTSLOTSELECT_SLOT10  10U
#define DAU_RESULTSLOTSELECT_SLOT11  11U
#define DAU_RESULTSLOTSELECT_SLOT12  12U
#define DAU_RESULTSLOTSELECT_SLOT13  13U
#define DAU_RESULTSLOTSELECT_SLOT14  14U
#define DAU_RESULTSLOTSELECT_SLOT15  15U

// End of DAU Definitions
// ----------------------

enum  ControlMode 
{
    eSINGLE_SRC =          0x0000U,
    eCOMMNON_TRIG =        0x0001U,
    eSEQUENT_TRIG =        0x0002U,
    eRESET_MODE =          0xFFF8U
};

enum ControlTrigEdgeSel 
{
    eRISING =               ( 0x0000U ),
    eFALLING =              ( 0x0008U ),
    eRISING_FALLING =       ( 0x0010U ),
    eRESET_TRIG_EDGE_SEL =  ( 0xFFE7U )
};

enum ControlTrigSel 
{
    ePWM_CENTRE =       ( 0x0000U ),
    eSWTRIG =           ( 0x0020U ),
    eGTU0 =             ( 0x0040U ),
    eGTU1 =             ( 0x0060U ),
    eGTU2 =             ( 0x0080U ),
    eGTU3 =             ( 0x00A0U ),
    eGPIO0 =            ( 0x00C0U ),
    eGPIO1 =            ( 0x00E0U ),
    eGPIO2 =            ( 0x0100U ),
    eGPIO3 =            ( 0x0120U ),
    eGPIO4 =            ( 0x0140U ),
    eGPIO5 =            ( 0x0160U ),
    eGPIO6 =            ( 0x0180U ),
    eGPIO7 =            ( 0x01A0U ),
    eRESET_TRIG_SEL =   ( 0xFE1FU )
};

enum ControlCnvSpeed 
{
    e1P0US =            ( 0x0000U ),
    e1P5US =            ( 0x0200U ),
    e2P0US =            ( 0x0400U ),
    e2P5US =            ( 0x0600U ),
    eRESET_CNV_SPEED =  ( 0xF9FFU )
};

enum ControlExtRef 
{
    eINT =     ( 0xF7FFU ),
    eEXT =     ( 0x0800U )
};

enum ControlExtGnd 
{
    eGND_INT = ( 0xEFFFU ),
    eGND_EXT = ( 0x1000U )
};

enum ControlDauEnable 
{
    eDISABLE_DAU =  ( 0xDFFFU ),
    eENABLE_DAU =   ( 0x2000U )
};

enum  SwTrig 
{
    eDISABLED_SW_TRIG = 0x00U,
    eENABLED_SW_TRIG =  0x01U
};

enum  ConfigChannel 
{                               // Channel source names
    eCHANNEL1 =  0x00U,         // PD0
    eCHANNEL2 =  0x01U,         // PD1
    eCHANNEL3 =  0x02U,         // PD2
    eCHANNEL4 =  0x03U,         // PD3
    eCHANNEL5 =  0x04U,         // PD4
    eCHANNEL6 =  0x05U,         // PD5
    eCHANNEL7 =  0x06U,         // PD6
    eCHANNEL8 =  0x07U,         // PD7
    eCHANNEL9 =  0x08U,         // VD50/3
    eCHANNEL10 = 0x09U,         // VD33/2
    eCHANNEL11 = 0x0AU,         // VIO (5V application), VIO/2 (3.3V application)
    eCHANNEL12 = 0x0BU,         // VDD
    eCHANNEL13 = 0x0CU,         // VBE Die temperature
    eCHANNEL14 = 0x0DU,         // AOUT
    eCHANNEL15 = 0x0EU,         // VBG
    eCHANNEL16 = 0x0FU,         // VWDF
    eCHANNEL_RESET = 0xFFF0U
};

enum  ConfigSlotEnable 
{
    eSLOT_DISABLE =     ( 0xFFEFU ),
    eSLOT_ENABLE =      ( 0x0001U )<< 4
};

enum ConfigSAHPeriod 
{
    e1US_SAH_PERIOD   = ( 0x0000U ),
    e2US_SAH_PERIOD   = ( 0x0020U ),
    e3US_SAH_PERIOD   = ( 0x0040U ),
    e5US_SAH_PERIOD   = ( 0x0060U ),
    e10US_SAH_PERIOD  = ( 0x0080U ),
    e25US_SAH_PERIOD  = ( 0x00A0U ),
    e125US_SAH_PERIOD = ( 0x00C0U ),
    e625US_SAH_PERIOD = ( 0x00E0U ),
    eRESET_SAH_PERIOD = ( 0xFF1FU )
};

enum ErrorStatus 
{
    eANY_ERRORS =                ( 0b1000000000000111U ),
    eTRIG_SKIP_ERROR_STATUS =    ( 0b0000000000000001U ),
    eADC_ERROR_STATUS =          ( 0b0000000000000010U ),
    eCONFIG_ERROR_STATUS =       ( 0b0000000000000100U ),
    eOVERRUN_STATUS =            ( 0b1000000000000000U )
};

enum IrqErrorEnable 
{
    eNONE_IRQ =               0,
    eTRIG_SKIP_ERROR_IRQ =    ( 0b00000001U ),
    eADC_ERROR_IRQ =          ( 0b00000010U ),
    eCONFIG_ERROR_IRQ =       ( 0b00000100U )
};

enum  OverRunStatus
{
    eNONE_OVR_RUN_STATUS   =  0,
    eSLOT0_OVR_RUN_STATUS  =  ( 0b0000000000000001U ),
    eSLOT1_OVR_RUN_STATUS  =  ( 0b0000000000000010U ),
    eSLOT2_OVR_RUN_STATUS  =  ( 0b0000000000000100U ),
    eSLOT3_OVR_RUN_STATUS  =  ( 0b0000000000001000U ),
    eSLOT4_OVR_RUN_STATUS  =  ( 0b0000000000010000U ),
    eSLOT5_OVR_RUN_STATUS  =  ( 0b0000000000100000U ),
    eSLOT6_OVR_RUN_STATUS  =  ( 0b0000000001000000U ),
    eSLOT7_OVR_RUN_STATUS  =  ( 0b0000000010000000U ),
    eSLOT8_OVR_RUN_STATUS  =  ( 0b0000000100000000U ),
    eSLOT9_OVR_RUN_STATUS  =  ( 0b0000001000000000U ),
    eSLOT10_OVR_RUN_STATUS = ( 0b0000010000000000U ),
    eSLOT11_OVR_RUN_STATUS = ( 0b0000100000000000U ),
    eSLOT12_OVR_RUN_STATUS = ( 0b0001000000000000U ),
    eSLOT13_OVR_RUN_STATUS = ( 0b0010000000000000U ),
    eSLOT14_OVR_RUN_STATUS = ( 0b0100000000000000U ),
    eSLOT15_OVR_RUN_STATUS = ( 0b1000000000000000U ),
    eANY_OVR_RUN_STATUS    = ( 0b1111111111111111U )
};

enum OverRunEnable 
{
    eNONE_OVR_RUN_ENABLE   =    0,
    eSLOT0_OVR_RUN_ENABLE  =  ( 0b0000000000000001U ),
    eSLOT1_OVR_RUN_ENABLE  =  ( 0b0000000000000010U ),
    eSLOT2_OVR_RUN_ENABLE  =  ( 0b0000000000000100U ),
    eSLOT3_OVR_RUN_ENABLE  =  ( 0b0000000000001000U ),
    eSLOT4_OVR_RUN_ENABLE  =  ( 0b0000000000010000U ),
    eSLOT5_OVR_RUN_ENABLE  =  ( 0b0000000000100000U ),
    eSLOT6_OVR_RUN_ENABLE  =  ( 0b0000000001000000U ),
    eSLOT7_OVR_RUN_ENABLE  =  ( 0b0000000010000000U ),
    eSLOT8_OVR_RUN_ENABLE  =  ( 0b0000000100000000U ),
    eSLOT9_OVR_RUN_ENABLE  =  ( 0b0000001000000000U ),
    eSLOT10_OVR_RUN_ENABLE =  ( 0b0000010000000000U ),
    eSLOT11_OVR_RUN_ENABLE =  ( 0b0000100000000000U ),
    eSLOT12_OVR_RUN_ENABLE =  ( 0b0001000000000000U ),
    eSLOT13_OVR_RUN_ENABLE =  ( 0b0010000000000000U ),
    eSLOT14_OVR_RUN_ENABLE =  ( 0b0100000000000000U ),
    eSLOT15_OVR_RUN_ENABLE =  ( 0b1000000000000000U )
};

enum  ConversionStatus 
{
    eSLOT0_READY_CONV_STATUS =  0x00U,
    eSLOT1_READY_CONV_STATUS =  0x01U,
    eSLOT2_READY_CONV_STATUS =  0x02U,
    eSLOT3_READY_CONV_STATUS =  0x03U,
    eSLOT4_READY_CONV_STATUS =  0x04U,
    eSLOT5_READY_CONV_STATUS =  0x05U,
    eSLOT6_READY_CONV_STATUS =  0x06U,
    eSLOT7_READY_CONV_STATUS =  0x07U,
    eSLOT8_READY_CONV_STATUS =  0x08U,
    eSLOT9_READY_CONV_STATUS =  0x09U,
    eSLOT10_READY_CONV_STATUS = 0x0AU,
    eSLOT11_READY_CONV_STATUS = 0x0BU,
    eSLOT12_READY_CONV_STATUS = 0x0CU,
    eSLOT13_READY_CONV_STATUS = 0x0DU,
    eSLOT14_READY_CONV_STATUS = 0x0EU,
    eSLOT15_READY_CONV_STATUS = 0x0FU,
    eSLOT0_BUSY_CONV_STATUS =   0x10U,
    eSLOT1_BUSY_CONV_STATUS =   0x11U,
    eSLOT2_BUSY_CONV_STATUS =   0x12U,
    eSLOT3_BUSY_CONV_STATUS =   0x13U,
    eSLOT4_BUSY_CONV_STATUS =   0x14U,
    eSLOT5_BUSY_CONV_STATUS =   0x15U,
    eSLOT6_BUSY_CONV_STATUS =   0x16U,
    eSLOT7_BUSY_CONV_STATUS =   0x17U,
    eSLOT8_BUSY_CONV_STATUS =   0x18U,
    eSLOT9_BUSY_CONV_STATUS =   0x19U,
    eSLOT10_BUSY_CONV_STATUS =  0x1AU,
    eSLOT11_BUSY_CONV_STATUS =  0x1BU,
    eSLOT12_BUSY_CONV_STATUS =  0x1CU,
    eSLOT13_BUSY_CONV_STATUS =  0x1DU,
    eSLOT14_BUSY_CONV_STATUS =  0x1EU,
    eSLOT15_BUSY_CONV_STATUS =  0x1FU,
    eADC_READY_CONV_STATUS   =  0x20U
};

enum ConvCompStatus 
{
    eCLEAR_ALL = 0,
    eSLOT0_CONV_COMP_STATUS  =  ( 0b0000000000000001U ),
    eSLOT1_CONV_COMP_STATUS  =  ( 0b0000000000000010U ),
    eSLOT2_CONV_COMP_STATUS  =  ( 0b0000000000000100U ),
    eSLOT3_CONV_COMP_STATUS  =  ( 0b0000000000001000U ),
    eSLOT4_CONV_COMP_STATUS  =  ( 0b0000000000010000U ),
    eSLOT5_CONV_COMP_STATUS  =  ( 0b0000000000100000U ),
    eSLOT6_CONV_COMP_STATUS  =  ( 0b0000000001000000U ),
    eSLOT7_CONV_COMP_STATUS  =  ( 0b0000000010000000U ),
    eSLOT8_CONV_COMP_STATUS  =  ( 0b0000000100000000U ),
    eSLOT9_CONV_COMP_STATUS  =  ( 0b0000001000000000U ),
    eSLOT10_CONV_COMP_STATUS = ( 0b0000010000000000U ),
    eSLOT11_CONV_COMP_STATUS = ( 0b0000100000000000U ),
    eSLOT12_CONV_COMP_STATUS = ( 0b0001000000000000U ),
    eSLOT13_CONV_COMP_STATUS = ( 0b0010000000000000U ),
    eSLOT14_CONV_COMP_STATUS = ( 0b0100000000000000U ),
    eSLOT15_CONV_COMP_STATUS = ( 0b1000000000000000U ),
    eANY_CONV_COMP_STATUS    = ( 0b1111111111111111U )
};

enum  ConvCompIrqEnable 
{
    eNONE_CONV_COMP_IRQ_EN   = 0,
    eSLOT0_CONV_COMP_IRQ_EN  =  ( 0b0000000000000001U ),
    eSLOT1_CONV_COMP_IRQ_EN  =  ( 0b0000000000000010U ),
    eSLOT2_CONV_COMP_IRQ_EN  =  ( 0b0000000000000100U ),
    eSLOT3_CONV_COMP_IRQ_EN  =  ( 0b0000000000001000U ),
    eSLOT4_CONV_COMP_IRQ_EN  =  ( 0b0000000000010000U ),
    eSLOT5_CONV_COMP_IRQ_EN  =  ( 0b0000000000100000U ),
    eSLOT6_CONV_COMP_IRQ_EN  =  ( 0b0000000001000000U ),
    eSLOT7_CONV_COMP_IRQ_EN  =  ( 0b0000000010000000U ),
    eSLOT8_CONV_COMP_IRQ_EN  =  ( 0b0000000100000000U ),
    eSLOT9_CONV_COMP_IRQ_EN  =  ( 0b0000001000000000U ),
    eSLOT10_CONV_COMP_IRQ_EN = ( 0b0000010000000000U ),
    eSLOT11_CONV_COMP_IRQ_EN = ( 0b0000100000000000U ),
    eSLOT12_CONV_COMP_IRQ_EN = ( 0b0001000000000000U ),
    eSLOT13_CONV_COMP_IRQ_EN = ( 0b0010000000000000U ),
    eSLOT14_CONV_COMP_IRQ_EN = ( 0b0100000000000000U ),
    eSLOT15_CONV_COMP_IRQ_EN = ( 0b1000000000000000U )
};


enum DmaEnable 
{
    eNONE_DMA_EN   =    0,
    eSLOT0_DMA_EN  =  (uint32)( 0x0001U ) << 0,
    eSLOT1_DMA_EN  =  (uint32)( 0x0001U ) << 1,
    eSLOT2_DMA_EN  =  (uint32)( 0x0001U ) << 2,
    eSLOT3_DMA_EN  =  (uint32)( 0x0001U ) << 3,
    eSLOT4_DMA_EN  =  (uint32)( 0x0001U ) << 4,
    eSLOT5_DMA_EN  =  (uint32)( 0x0001U ) << 5,
    eSLOT6_DMA_EN  =  (uint32)( 0x0001U ) << 6,
    eSLOT7_DMA_EN  =  (uint32)( 0x0001U ) << 7,
    eSLOT8_DMA_EN  =  (uint32)( 0x0001U ) << 8,
    eSLOT9_DMA_EN  =  (uint32)( 0x0001U ) << 9,
    eSLOT10_DMA_EN =  (uint32)( 0x0001U ) << 10,
    eSLOT11_DMA_EN =  (uint32)( 0x0001U ) << 11,
    eSLOT12_DMA_EN =  (uint32)( 0x0001U ) << 12,
    eSLOT13_DMA_EN =  (uint32)( 0x0001U ) << 13,
    eSLOT14_DMA_EN =  (uint32)( 0x0001U ) << 14,
    eSLOT15_DMA_EN =  (uint32)( 0x0001U ) << 15
};


enum AppEval 
{
    eAPP_NONE = 0,
    eNORMAL =     0,
    eEVAL_TRIG =  1,
    eEVAL0 = 0 << 1,
    eEVAL1 = 1 << 1,
    eEVAL2 = 2 << 1,
    eEVAL3 = 3 << 1,
    eEVAL4 = 4 << 1,
    eEVAL5 = 5 << 1,
};

enum  ConfigSlot 
{
    eSELECT_SLOT0 = 0U,
    eSELECT_SLOT1 = 1U,
    eSELECT_SLOT2 = 2U,
    eSELECT_SLOT3 = 3U,
    eSELECT_SLOT4 = 4U,
    eSELECT_SLOT5 = 5U,
    eSELECT_SLOT6 = 6U,
    eSELECT_SLOT7 = 7U,
    eSELECT_SLOT8 = 8U,
    eSELECT_SLOT9 = 9U,
    eSELECT_SLOT10 = 10U,
    eSELECT_SLOT11 = 11U,
    eSELECT_SLOT12 = 12U,
    eSELECT_SLOT13 = 13U,
    eSELECT_SLOT14 = 14U,
    eSELECT_SLOT15 = 15U
};


enum ResultSlotSelect 
{
    eSLOT0_RESULT  = 0U,
    eSLOT1_RESULT  = 1U,
    eSLOT2_RESULT  = 2U,
    eSLOT3_RESULT  = 3U,
    eSLOT4_RESULT  = 4U,
    eSLOT5_RESULT  = 5U,
    eSLOT6_RESULT  = 6U,
    eSLOT7_RESULT  = 7U,
    eSLOT8_RESULT  = 8U,
    eSLOT9_RESULT  = 9U,
    eSLOT10_RESULT = 10U,
    eSLOT11_RESULT = 11U,
    eSLOT12_RESULT = 12U,
    eSLOT13_RESULT = 13U,
    eSLOT14_RESULT = 14U,
    eSLOT15_RESULT = 15U
};

typedef struct 
{
    uint32 control;                           //0x00U :(R/W)
    uint32 swtrig;                            //0x04U :(R/W)
    uint32 slot0Config;                       //0x08U :(R/W)
    uint32 slot1Config;                       //0x0CU :(R/W)
    uint32 slot2Config;                       //0x10U :(R/W)
    uint32 slot3Config;                       //0x14U :(R/W)
    uint32 slot4Config;                       //0x18U :(R/W)
    uint32 slot5Config;                       //0x1CU :(R/W)
    uint32 slot6Config;                       //0x20U :(R/W)
    uint32 slot7Config;                       //0x24U :(R/W)
    uint32 slot8Config;                       //0x28U :(R/W)
    uint32 slot9Config;                       //0x2CU :(R/W)
    uint32 slot10Config;                      //0x30U :(R/W)
    uint32 slot11Config;                      //0x34U :(R/W)
    uint32 slot12Config;                      //0x38U :(R/W)
    uint32 slot13Config;                      //0x3CU :(R/W)
    uint32 slot14Config;                      //0x40U :(R/W)
    uint32 slot15Config;                      //0x44U :(R/W)
    uint32 errorStatus;                       //0x48U :(R/1C)
    uint32 errorStatusInten;                  //0x4CU :(R/W)
    uint32 ovrStatus;                         //0x50U :(R/1C)
    uint32 ovrStatusInten;                    //0x54U :(R/W)
    uint32 conversionStatus;                  //0x58U :(R)
    uint32 convCompStatus;                    //0x5CU :(R/1C)
    uint32 convCompStatusInten;               //0x60U :(R/W)
    uint32 result0;                           //0x64U :(R)
    uint32 result1;                           //0x68U :(R)
    uint32 result2;                           //0x6CU :(R)
    uint32 result3;                           //0x70U :(R)
    uint32 result4;                           //0x74U :(R)
    uint32 result5;                           //0x78U :(R)
    uint32 result6;                           //0x7CU :(R)
    uint32 result7;                           //0x80U :(R)
    uint32 result8;                           //0x84U :(R)
    uint32 result9;                           //0x88U :(R)
    uint32 result10;                          //0x8CU :(R)
    uint32 result11;                          //0x90U :(R)
    uint32 result12;                          //0x94U :(R)
    uint32 result13;                          //0x98U :(R)
    uint32 result14;                          //0x9CU :(R)
    uint32 result15;                          //0xA0U :(R)
    uint32 DmaReqNum;                         //0xA4U :(R/W)
    uint32 DmaReqEn;                          //0xA8U :(R/W)
} DauRegs_t;

#define  stDauRegs (*( volatile DauRegs_t *)eADDRESS_DAU_BASE)

/**
 * @brief     Get one of 16 slot results.
 * @remarks     This is an optimized replacement for GetSlotResult. It's use should be minimized as it is an inline function.
 * @param[in]   keAddrOffset Slot = 0 to 15
 * @returns     Result
 */
__attribute__( ( always_inline ) ) static inline uint32 Dau_GetSlotResultFast( enum ResultSlotSelect const keAddrOffset )
{
    volatile uint32* const pu32SlotResult =  &(stDauRegs.result0) + (uint32)keAddrOffset;
    return ( *pu32SlotResult );
}
/**
 * @brief     Set convert complete status bit(s) to clear. Only bits set in input will be cleared.
 * @remarks     This is an optimized replacement for SetConvCompStatus. It's use should be minimized as it is an inline function.
 * @param[in]   keConvCompStatus Status bit to set - to clear.
 * @returns     None
 */
__attribute__( ( always_inline ) ) static inline void Dau_SetConvCompStatusFast( enum ConvCompStatus const keConvCompStatus )
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

/**
 * @brief     Clear control register
 * @param[in]   None
 * @returns     None
 */
void Dau_ClearControlReg( void );

/**
 * @brief     Set control mode
 * @param[in]   keControlMode control register mode
 * @returns     None
 */
void Dau_SetControlMode( enum ControlMode const keControlMode );

/**
 * @brief     Set control trigger edge select
 * @param[in]   keControlTrigEdgeSel control register trigger edge select
 * @returns     None
 */
void Dau_SetControlTrigEdgeSel( enum ControlTrigEdgeSel const keControlTrigEdgeSel );

/**
 * @brief     Set control trigger select
 * @param[in]   keControlTrigSel control register trigger select
 * @returns     None
 */
void Dau_SetControlTrigSel( enum ControlTrigSel const keControlTrigSel );

/**
 * @brief     Set control conversion speed
 * @param[in]   keControlCnvSpeed control register conversion speed
 * @returns     None
 */
void Dau_SetControlCnvSpeed( enum ControlCnvSpeed const keControlCnvSpeed );

/**
 * @brief     Set control external reference select
 * @param[in]   keControlExtRef control register external reference select
 * @returns     None
 */
void Dau_SetControlExtRef( enum ControlExtRef const keControlExtRef );

/**
 * @brief     Set control external reference ground select
 * @param[in]   keControlExtGnd control register external reference ground select
 * @returns     None
 */
void Dau_SetControlExtGnd( enum ControlExtGnd const keControlExtGnd );

/**
 * @brief     Set control DAU enable
 * @param[in]   keControlDauEnable control register DAU enable
 * @returns     None
 */
void Dau_SetControlDauEnable( enum ControlDauEnable const keControlDauEnable );

/**
 * @brief     Get control register
 * @param[in]   None
 * @returns     Control register setting
 */
uint32 Dau_GetControlReg( void );

/**
 * @brief     Set or reset software trigger enable
 * @param[in]   keSwTrig Setting: 0 or 1
 * @returns     None
 */
void Dau_SetSwTrig( enum SwTrig const keSwTrig );

/**
 * @brief     Get software trigger enable
 * @param[in]   None
 * @returns     Setting: 0 or 1
 */
enum SwTrig Dau_GetSwTrig( void );

/**
 * @brief     Clear one of 16 config registers
 * @param[in]   keAddrOffset Slot = 0 to 15
 * @returns     None
 */
void Dau_ClearSlotConfig( enum ConfigSlot const keAddrOffset );

/**
 * @brief     Set one of 16 config registers channel selection
 * @param[in]   keAddrOffset Slot = 0 to 15
 * @param[in]   keConfigChannel Configuration data: channel
 * @returns     None
 */
void Dau_SetSlotConfigChannel( enum ConfigSlot const keAddrOffset, enum ConfigChannel const keConfigChannel );

/**
 * @brief     Set one of 16 config registers slot enable
 * @param[in]   keAddrOffset Slot = 0 to 15
 * @param[in]   keConfigSlotEnable Configuration data: enable
 * @returns     None
 */
void Dau_SetSlotConfigSlotEnable( enum ConfigSlot const keAddrOffset, enum ConfigSlotEnable const keConfigSlotEnable );

/**
 * @brief     Set one of 16 config registers SAH period
 * @param[in]   keAddrOffset Slot = 0 to 15
 * @param[in]   keConfigSAHPeriod Configuration data: SAH
 * @returns     None
 */
void Dau_SetSlotConfigSAHPeriod( enum ConfigSlot const keAddrOffset, enum ConfigSAHPeriod const keConfigSAHPeriod );

/**
 * @brief     Get one of 16 config registers containing DAU channel selection and enable bit for slots 0-15
 * @param[in]   keAddrOffset Slot = 0 to 15
 * @returns     Configuration data : Channel[3:0], enable [4], SAH[7:5]
 */
uint16 Dau_GetSlotConfig( enum ConfigSlot const keAddrOffset );

/**
 * @brief     Set Error Status register with errors flags: 1 to clear.
 * @param[in]   keStatus Status bits
 * @returns     None
 */
void Dau_SetErrorStatus( enum ErrorStatus const keErrorStatus );

/**
 * @brief     Get Error Status register error flag bit status
 * @param[in]   None
 * @returns     true/false
 */
boolean Dau_GetErrorStatus( enum ErrorStatus const keErrorStatus );

/**
 * @brief     Set interrupt enable for status errors
 * @param[in]   keIrqErrEnable Interrupt enable bit
 * @returns     None
 */
void Dau_SetErrStatusIrqEnable( enum IrqErrorEnable const keIrqErrEnable );

/**
 * @brief     Get interrupt enable for status errors
 * @param[in]   None
 * @returns     Interrupt enables
 */
enum IrqErrorEnable Dau_GetErrStatusIrqEnable( void );

/**
 * @brief     Set overrun status bits for one of 16 slots: 1 to clear.
 * @param[in]   keOverRun Overrun bits to clear.
 * @returns     None
 */
void Dau_SetOverRunStatus( enum OverRunStatus const keOverRunStatus );

/**
 * @brief     Get overrun status bits for 16 slots.
 * @param[in]   None
 * @returns     true/false for status bit.
 */
boolean Dau_GetOverRunStatus( enum OverRunStatus const keOverRunStatus );

/**
 * @brief     Set overrun enable for one of 16 slots.
 * @param[in]   keOverRunEnable Setting: 0 or 1
 * @returns     None
 */
void Dau_SetOverRunEnable( enum OverRunEnable const keOverRunEnable );

/**
 * @brief     Get overrun enable register for 16 slots
 * @param[in]   None
 * @returns     Setting
 */
enum OverRunEnable Dau_GetOverRunEnable( void );

/**
 * @brief     Get ADC conversion status.
 * @param[in]   None
 * @returns     ADC conversion status
 */
enum ConversionStatus Dau_GetConversionStatus( void );

/**
 * @brief     Set conversion complete status for each slot conversion: 1 to clear.
 * @param[in]   keConvCompStatus Interrupt status bits.
 * @returns     None.
 */
void Dau_SetConvCompStatus(enum ConvCompStatus const keConvCompStatus );

/**
 * @brief     Get conversion complete status for each slot conversion.
 * @param[in]   None.
 * @returns     true/false.
 */
boolean Dau_GetConvCompStatus( enum ConvCompStatus const keConvCompStatus );

/**
 * @brief     Set conversion comlpete interrupt enable for each slot conversion.
 * @param[in]   keConvCompIrqEnable Interrupt enables.
 * @returns     None.
 */
void Dau_SetConvCompIrqEnable( enum ConvCompIrqEnable const keConvCompIrqEnable );

/**
 * @brief     Get conversion comlpete interrupt enables.
 * @param[in]   None.
 * @returns     Interrupt enables.
 */
enum ConvCompIrqEnable Dau_GetConvCompIrqEnable( void );

/**
 * @brief     Get one slot result
 * @param[in]   keAddrOffset Slot = 0 to 15
 * @returns     Slot result
 */
uint32 Dau_GetSlotResult( enum ResultSlotSelect const keAddrOffset );

/**
 * @brief     Set DMA counter register
 * @param[in]   ku16DmaAcquisitions DMA counter value
 * @returns     None
 */
void Dau_SetDma( uint16 const ku16DmaAcquisitions );

/**
 * @brief     Get DMA counter register
 * @param[in]   None
 * @returns     DMA counter value
 */
uint32 Dau_GetDma( void );

/**
 * @brief     Set DMA enable register for 16 slots
 * @param[in]   keDmaEnable DMA enables
 * @returns     None
 */
void Dau_SetDmaEnable( enum DmaEnable const keDmaEnable );

/**
 * @brief     Get DMA enable register for 16 slots
 * @param[in]   None
 * @returns     DMA enables
 */
enum DmaEnable Dau_GetDmaEnable( void );

/**
 * @brief Set DAU IRQ handler
 * @param[in] rclsIrqHandler An IRQ handler
 * @param[in] uPrData An argument to pass to the IRQ handler
 */
void Dau_SetIrqHandler( void (DauIrqHandler)( void ) );

/**
 * @brief Handler for DAU IRQ.
 */
void Dau_IrqHandler( void );
// AM private-section-end


#endif //HEADER_DAU_H