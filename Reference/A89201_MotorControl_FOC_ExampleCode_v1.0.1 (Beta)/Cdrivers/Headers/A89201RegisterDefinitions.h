/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         A89201RegisterDefinitions.h
 * @author       M. Shankey
 * @version      1.0
 *
 * @brief        The register definitions for A89201
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

// The following macros are to be used with standard non-inverted register field masks

#define ClearRegisterField(addr, mask){\
    volatile uint32* pu32RegisterAddr = (volatile uint32*)(addr);\
    *pu32RegisterAddr &= ~mask;\
}

#define SetRegisterField(addr, data){\
    volatile uint32* pu32RegisterAddr = (volatile uint32*)(addr);\
    *pu32RegisterAddr |= data;\
}

#define GetRegister(addr, rvalue){\
    volatile uint32* pu32RegisterAddr = (volatile uint32*)(addr);\
    rvalue = *pu32RegisterAddr;\
}

#define ReadModifyWriteField(addr, mask, lsbitpos, data){\
    volatile uint32* pu32RegisterAddr = (volatile uint32*)(addr);\
    uint32 u32RegData = *pu32RegisterAddr;\
    u32RegData &= ~mask;\
    u32RegData |= (data << lsbitpos);\
    *pu32RegisterAddr = u32RegData;\
}

#define ReadField(addr, mask, lsbitpos, data){\
    volatile uint32* pu32RegisterAddr = (volatile uint32*)(addr);\
    data = *pu32RegisterAddr;\
    data = (data & mask) >> lsbitpos;\
}

// AMCT Definitions
// ----------------

// AmctRegisters
#define AMCT_BASE_ADDRESS                     0x40020000U
#define AMCT_CONFIG                           0x00U
#define AMCT_COMMAND                          0x04U
#define AMCT_PHASE_DISABLE                    0x08U
#define AMCT_HALL_FILTER_TIME                 0x0CU
#define AMCT_HALL_PHASE_SHIFT                 0x10U
#define AMCT_ANGLE_STEP                       0x14U
#define AMCT_HALL_FREQ_LIMIT                  0x18U
#define AMCT_APPLY_ANGLE_CORRECTION           0x1CU
#define AMCT_ANGLE_CORRECTION_FACTOR          0x20U
#define AMCT_INITIAL_ANGLE                    0x24U
#define AMCT_BEMF_DEGAUSS_FILTER_BLANK_TIME   0x28U
#define AMCT_BEMF_FILTER_TIME                 0x2CU
#define AMCT_IRQ_ENABLE                       0x30U
#define AMCT_PHASE_DISABLE_OUTPUT             0x34U
#define AMCT_FILTERED_BEMF_COMP_OUTPUT        0x38U
#define AMCT_BEMF_COMP_TACHO                  0x3CU
#define AMCT_IRQ_STATUS                       0x40U
#define AMCT_COMU_SECTION                     0x44U
#define AMCT_HALL_FREQ                        0x48U
#define AMCT_ANGLE                            0x4CU
#define AMCT_BEMF_ANGLE_ERROR                 0x50U
#define AMCT_IPD_RESULT1                      0x54U
#define AMCT_IPD_RESULT2                      0x58U
#define AMCT_CPD_RESULT                       0x5CU


// AmctConfigModeSelect
#define AMCT_CONFIG_BEMF   ( 0x001FFFFE )
#define AMCT_CONFIG_HALL   ( 0x00000001 )

// AmctConfigMotorDirection
#define AMCT_CONFIG_RVS   ( 0x001FFFFD )
#define AMCT_CONFIG_FWD   ( 0x00000002 )

// AmctConfigDegaussFilterBypass
#define AMCT_CONFIG_NO_BYPASS    ( 0x001FFFFB )
#define AMCT_CONFIG_BYPASS       ( 0x00000004 )

//  AmctConfigPhaseDisableSelect
#define AMCT_CONFIG_CMD   ( 0x001FFFF7 )
#define CPU_CONFIG_CMD    ( 0x00000008 )

//  AmctConfigHallASelect
#define AMCT_CONFIG_HALLASELECT_MASK     ( 0x001FFF8F ) 
#define AMCT_CONFIG_HALLASELECT_GPIO_0   ( 0x00000000 )
#define AMCT_CONFIG_HALLASELECT_GPIO_1   ( 0x00000010 )
#define AMCT_CONFIG_HALLASELECT_GPIO_2   ( 0x00000020 )
#define AMCT_CONFIG_HALLASELECT_GPIO_3   ( 0x00000030 )
#define AMCT_CONFIG_HALLASELECT_GPIO_4   ( 0x00000040 )
#define AMCT_CONFIG_HALLASELECT_GPIO_5   ( 0x00000050 )
#define AMCT_CONFIG_HALLASELECT_GPIO_6   ( 0x00000060 )
#define AMCT_CONFIG_HALLASELECT_GPIO_7   ( 0x00000070 )


// AmctConfigHallBSelect
#define AMCT_CONFIG_HALLBSELECT_MASK     ( 0x001FFC7F )
#define AMCT_CONFIG_HALLBSELECT_GPIO_0   ( 0x00000000 )
#define AMCT_CONFIG_HALLBSELECT_GPIO_1   ( 0x00000080 )
#define AMCT_CONFIG_HALLBSELECT_GPIO_2   ( 0x00000100 )
#define AMCT_CONFIG_HALLBSELECT_GPIO_3   ( 0x00000180 )
#define AMCT_CONFIG_HALLBSELECT_GPIO_4   ( 0x00000200 )
#define AMCT_CONFIG_HALLBSELECT_GPIO_5   ( 0x00000280 )
#define AMCT_CONFIG_HALLBSELECT_GPIO_6   ( 0x00000300 )
#define AMCT_CONFIG_HALLBSELECT_GPIO_7   ( 0x00000380 )


//  AmctConfigHallCSelect
#define AMCT_CONFIG_HALLCSELECT_MASK     ( 0x001FE3FF )
#define AMCT_CONFIG_HALLCSELECT_GPIO_0   ( 0x00000000 )
#define AMCT_CONFIG_HALLCSELECT_GPIO_1   ( 0x00000400 )
#define AMCT_CONFIG_HALLCSELECT_GPIO_2   ( 0x00000800 )
#define AMCT_CONFIG_HALLCSELECT_GPIO_3   ( 0x00000C00 )
#define AMCT_CONFIG_HALLCSELECT_GPIO_4   ( 0x00001000 )
#define AMCT_CONFIG_HALLCSELECT_GPIO_5   ( 0x00001400 )
#define AMCT_CONFIG_HALLCSELECT_GPIO_6   ( 0x00001800 )
#define AMCT_CONFIG_HALLCSELECT_GPIO_7   ( 0x00001C00 )

//  AmctConfigIPDTriggerSelect
#define AMCT_CONFIG_IPDTRIGGERSELECT_MASK  ( 0xFFFCFFFF )
#define AMCT_CONFIG_IPDTRIGGERSELECT_GHA   ( 0x00000000 )
#define AMCT_CONFIG_IPDTRIGGERSELECT_GHB   ( 0x00010000 )
#define AMCT_CONFIG_IPDTRIGGERSELECT_GHC   ( 0x00020000 )

//  AmctConfigIPDComparatorSelect
#define AMCT_CONFIG_IPDCOMPARATORSELECT_MASK   ( 0xFFF3FFFF)
#define AMCT_CONFIG_IPDCOMPARATORSELECT_OUTA   ( 0x00000000 )
#define AMCT_CONFIG_IPDCOMPARATORSELECT_OUTB   ( 0x00040000 )
#define AMCT_CONFIG_IPDCOMPARATORSELECT_OUTC   ( 0x00080000 )


//  AmctConfigIPDUpdateModeSelect
#define AMCT_CONFIG_RESULT_NO_IRQ   ( 0x000FFFFF )
#define AMCT_CONFIG_RESULT_ALWAYS   ( 0x00100000 )

//  AmctCommand
#define AMCT_COMMAND_CLEAR_ALL              ( 0x0000 )
#define AMCT_COMMAND_MASTER_DISABLE         ( 0xFFFE )
#define AMCT_COMMAND_MASTER_ENABLE          ( 0x0001 )
#define AMCT_COMMAND_IPD_DISABLE            ( 0xFFFC )
#define AMCT_COMMAND_IPD_ENABLE             ( 0x0003 )
#define AMCT_COMMAND_CPD_DISABLE            ( 0xFFFA )
#define AMCT_COMMAND_CPD_ENABLE             ( 0x0005 )
#define AMCT_COMMAND_HALL_DISABLE           ( 0xFFF6 )
#define AMCT_COMMAND_HALL_ENABLE            ( 0x0009 )
#define AMCT_COMMAND_BEMF_DISABLE           ( 0xFFEE )
#define AMCT_COMMAND_BEMF_ENABLE            ( 0x0011 )

//  AmctPhaseDisable
#define AMCT_PHASEDISABLE_NONE     ( 0x0000 )
#define AMCT_PHASEDISABLE_A        ( 0x0001 )
#define AMCT_PHASEDISABLE_B        ( 0x0002 )
#define AMCT_PHASEDISABLE_C        ( 0x0004 )
#define AMCT_PHASEDISABLE_AB       ( 0x0003 )
#define AMCT_PHASEDISABLE_AC       ( 0x0005 )
#define AMCT_PHASEDISABLE_BC       ( 0x0006 )
#define AMCT_PHASEDISABLE_ALL      ( 0x0007 )


// AmctPhaseDisableOutput
#define AMCT_PHASEDISABLE_OUTPUT_NONE     ( 0x0000 )
#define AMCT_PHASEDISABLE_OUTPUT_A        ( 0x0001 )
#define AMCT_PHASEDISABLE_OUTPUT_B        ( 0x0002 )
#define AMCT_PHASEDISABLE_OUTPUT_AB       ( 0x0003 )
#define AMCT_PHASEDISABLE_OUTPUT_C        ( 0x0004 )
#define AMCT_PHASEDISABLE_OUTPUT_AC       ( 0x0005 )
#define AMCT_PHASEDISABLE_OUTPUT_BC       ( 0x0006 )
#define AMCT_PHASEDISABLE_OUTPUT_ABC      ( 0x0007 )



// AmctFilteredBemfCompOutput
#define AMCT_FILTEREDBEMFCOMP_OUTPUT_NONE     ( 0x0000 )
#define AMCT_FILTEREDBEMFCOMP_OUTPUT_A        ( 0x0001 )
#define AMCT_FILTEREDBEMFCOMP_OUTPUT_B        ( 0x0002 )
#define AMCT_FILTEREDBEMFCOMP_OUTPUT_AB       ( 0x0003 )
#define AMCT_FILTEREDBEMFCOMP_OUTPUT_C        ( 0x0004 )
#define AMCT_FILTEREDBEMFCOMP_OUTPUT_AC       ( 0x0005 )
#define AMCT_FILTEREDBEMFCOMP_OUTPUT_BC       ( 0x0006 )
#define AMCT_FILTEREDBEMFCOMP_OUTPUT_ABC      ( 0x0007 )


// AmctBemfCompTacho
#define AMCT_TACHO_SIGNAL   ( 0x0001 )

//  AmctApplyAngleCorrection
#define AMCT_SET_LOW    ( 0x0000 )
#define AMCT_SET_HIGH   ( 0x0001 )

// AmctIrqEnable
#define AMCT_IRQENABLE_NONE              ( 0x0000 )
#define AMCT_IRQENABLE_PHASE_A_DISABLE   ( 0x0001 )
#define AMCT_IRQENABLE_PHASE_B_DISABLE   ( 0x0001 ) << 1
#define AMCT_IRQENABLE_PHASE_C_DISABLE   ( 0x0001 ) << 2
#define AMCT_IRQENABLE_HALL_T2_OVR       ( 0x0001 ) << 3
#define AMCT_IRQENABLE_HALL_COMU_FAULT   ( 0x0001 ) << 4
#define AMCT_IRQENABLE_BEMF_COMP_STATE   ( 0x0001 ) << 5
#define AMCT_IRQENABLE_BEMF_COMU_FAULT   ( 0x0001 ) << 6
#define AMCT_IRQENABLE_IPD_STAT2         ( 0x0001 ) << 7
#define AMCT_IRQENABLE_IPD_STAT1         ( 0x0001 ) << 8

//  AmctIrqStatus
#define AMCT_IRQSTATUS_ANY_INTERRUPT      0
#define AMCT_IRQSTATUS_ALL_BITS           ( 0x01FF )
#define AMCT_IRQSTATUS_PHASE_A_DISABLE    ( 0x0001 )
#define AMCT_IRQSTATUS_PHASE_B_DISABLE    ( 0x0001 ) << 1
#define AMCT_IRQSTATUS_PHASE_C_DISABLE    ( 0x0001 ) << 2
#define AMCT_IRQSTATUS_HALL_OVERFLOW      ( 0x0001 ) << 3
#define AMCT_IRQSTATUS_HALL_STATE_FAULT   ( 0x0001 ) << 4
#define AMCT_IRQSTATUS_BEMF_COMP_STATE    ( 0x0001 ) << 5
#define AMCT_IRQSTATUS_BEMF_COMP_FAULT    ( 0x0001 ) << 6
#define AMCT_IRQSTATUS_IPD_STAT2          ( 0x0001 ) << 7
#define AMCT_IRQSTATUS_IPD_STAT1          ( 0x0001 ) << 8



// End of AMCT Definitions
// -----------------------

// LIN Definitions
// ---------------

// LinRegisters
#define LIN_BASE_ADDRESS  0x4000C000U
#define LIN_MSGIDENTIFIER 0x00U
#define LIN_MSGCONTENT1   0x04U
#define LIN_MSGCONTENT2   0x08U
#define LIN_MSGCONTENT3   0x0CU
#define LIN_MSGCONTENT4   0x10U
#define LIN_MSGCONTENT5   0x14U
#define LIN_MSGCONTENT6   0x18U
#define LIN_MSGCONTENT7   0x1CU
#define LIN_MSGCONTENT8   0x20U
#define LIN_MSGPARAMS     0x24U
#define LIN_CONTROL_REG   0x28U
#define LIN_LSTATUS       0x2CU
#define LIN_ERRORS        0x30U
#define LIN_BITRATELO     0x34U
#define LIN_BITRATEHI     0x38U
#define LIN_CHECKSUM      0x3CU
#define LIN_CONFIG        0x40U
#define LIN_IESTATUS      0x44U
#define LIN_IEERRORS      0x48U

// LinMsgParams
#define LIN_MSG_PARAMS_NBYTES0   0x00
#define LIN_MSG_PARAMS_NBYTES1   0x01
#define LIN_MSG_PARAMS_NBYTES2   0x02
#define LIN_MSG_PARAMS_NBYTES3   0x03
#define LIN_MSG_PARAMS_NBYTES4   0x04
#define LIN_MSG_PARAMS_NBYTES5   0x05
#define LIN_MSG_PARAMS_NBYTES6   0x06
#define LIN_MSG_PARAMS_NBYTES7   0x07
#define LIN_MSG_PARAMS_IB_SPACE  0x08
#define LIN_MSG_PARAMS_CSUM_TYPE 0x10

// LinControl
#define LIN_CONTROL_RXTX_CTRL_DEFAULT                   0x00
#define LIN_CONTROL_RXTX_CTRL_TRANSMIT_FRAME_RESPONSE   0x01
#define LIN_CONTROL_RXTX_CTRL_RECIEVE_FRAME_RESPONSE    0x02
#define LIN_CONTROL_RXTX_CTRL_DISCARD_FRAME_RESPONSE    0x03
#define LIN_CONTROL_WU_REQ                              0x04
#define LIN_CONTROL_WU_REQ_MASK                         0xFB

// LinStatus
#define LIN_STATUS_HEADER_REC   ( 0b0000000000000001U )
#define LIN_STATUS_RESP_REC     ( 0b0000000000000010U )
#define LIN_STATUS_RESP_SENT    ( 0b0000000000000100U )
#define LIN_STATUS_ERROR_FLAG   ( 0b0000000000001000U )
#define LIN_STATUS_WU_DET       ( 0b0000000000010000U )
#define LIN_STATUS_SLEEP_MODE   ( 0b0000000000100000U )
#define LIN_STATUS_SERVICE_REQ  ( 0b0000000001000000U )
#define LIN_STATUS_RESP_ON      ( 0b0000000010000000U )

// LinErrors
#define LIN_ERRORS_CSUM         ( 0b0000000000000001U )
#define LIN_ERRORS_RESPSHORT    ( 0b0000000000000010U )
#define LIN_ERRORS_READBACK     ( 0b0000000000000100U )
#define LIN_ERRORS_FRAMING      ( 0b0000000000001000U )
#define LIN_ERRORS_NORESP       ( 0b0000000000010000U )
#define LIN_ERRORS_PARITY       ( 0b0000000000100000U )
#define LIN_ERRORS_LOSTRESP     ( 0b0000000001000000U )
#define LIN_ERRORS_RESP_ERR     ( 0b0000000010000000U )

// LinConfig
#define LIN_CONFIG_LIN_EN          ( 0b0000000000000001U )
#define LIN_CONFIG_INT_EN          ( 0b0000000000000010U )
#define LIN_CONFIG_BI_TO_SEL_MASK  ( 0b1111111111100011U )
#define LIN_CONFIG_BI_TO_SEL_4S    ( 0b0000000000000000U )
#define LIN_CONFIG_BI_TO_SEL_5S    ( 0b0000000000000100U )
#define LIN_CONFIG_BI_TO_SEL_6S    ( 0b0000000000001000U )
#define LIN_CONFIG_BI_TO_SEL_7S    ( 0b0000000000001100U )
#define LIN_CONFIG_BI_TO_SEL_8S    ( 0b0000000000010000U )
#define LIN_CONFIG_BI_TO_SEL_9S    ( 0b0000000000010100U )
#define LIN_CONFIG_BI_TO_SEL_10S   ( 0b0000000000011100U )

// LinStatusIntEn
#define LIN_STATUS_INTEN_HEADER_REC   ( 0b0000000000000001U )
#define LIN_STATUS_INTEN_RESP_REC     ( 0b0000000000000010U )
#define LIN_STATUS_INTEN_RESP_SENT    ( 0b0000000000000100U )
#define LIN_STATUS_INTEN_WU_DET       ( 0b0000000000010000U )
#define LIN_STATUS_INTEN_GOTO_SLEEP   ( 0b0000000000100000U )

// LinErrorsIntEn
#define LIN_ERRORS_INTEN_CSUM         ( 0b0000000000000001U )
#define LIN_ERRORS_INTEN_RESPSHORT    ( 0b0000000000000010U )
#define LIN_ERRORS_INTEN_READBACK     ( 0b0000000000000100U )
#define LIN_ERRORS_INTEN_FRAMING      ( 0b0000000000001000U )
#define LIN_ERRORS_INTEN_NORESP       ( 0b0000000000010000U )
#define LIN_ERRORS_INTEN_PARITY       ( 0b0000000000100000U )
#define LIN_ERRORS_INTEN_LOSTRESP     ( 0b0000000001000000U )


// End of LIN Definitions
// ----------------------

// Timers Definitions
// ------------------

// Timers Registers
#define TIMERS_BASE_ADDRESS          0x40009100U
#define TIMER1_CONTROL               0x0000
#define TIMER1_THRESHOLD             0x0004
#define TIMER1_STARTSTOP             0x0008
#define TIMER1_COUNTMAX              0x000C
#define TIMER1_RESULT                0x0010
#define TIMER1_STATUS                0x0014
#define TIMER2_CONTROL               0x0100
#define TIMER2_THRESHOLD             0x0104
#define TIMER2_STARTSTOP             0x0108
#define TIMER2_COUNTMAX              0x010C
#define TIMER2_RESULT                0x0110
#define TIMER2_STATUS                0x0114
#define TIMER3_CONTROL               0x0200
#define TIMER3_THRESHOLD             0x0204
#define TIMER3_STARTSTOP             0x0208
#define TIMER3_COUNTMAX              0x020C
#define TIMER3_RESULT                0x0210
#define TIMER3_STATUS                0x0214
#define TIMER4_CONTROL               0x0300
#define TIMER4_THRESHOLD             0x0304
#define TIMER4_STARTSTOP             0x0308
#define TIMER4_COUNTMAX              0x030C
#define TIMER4_RESULT                0x0310
#define TIMER4_STATUS                0x0314
#define TIMER5_CONTROL               0x0400
#define TIMER5_THRESHOLD             0x0404
#define TIMER5_STARTSTOP             0x0408
#define TIMER5_COUNTMAX              0x040C
#define TIMER5_RESULT                0x0410
#define TIMER5_STATUS                0x0414
#define TIMER6_CONTROL               0x0500
#define TIMER6_THRESHOLD             0x0504
#define TIMER6_STARTSTOP             0x0508
#define TIMER6_COUNTMAX              0x050C
#define TIMER6_RESULT                0x0510
#define TIMER6_STATUS                0x0514
#define TIMER7_CONTROL               0x0600
#define TIMER7_THRESHOLD             0x0604
#define TIMER7_STARTSTOP             0x0608
#define TIMER7_COUNTMAX              0x060C
#define TIMER7_RESULT                0x0610
#define TIMER7_STATUS                0x0614
#define TIMER8_CONTROL               0x0700
#define TIMER8_THRESHOLD             0x0704
#define TIMER8_STARTSTOP             0x0708
#define TIMER8_COUNTMAX              0x070C
#define TIMER8_RESULT                0x0710
#define TIMER8_STATUS                0x0714


#define TIMER_CONTROL_SRC_MASK              0xFFF0
#define TIMER_CONTROL_SRC_GPI0              0x00
#define TIMER_CONTROL_SRC_GPI1              0x01
#define TIMER_CONTROL_SRC_GPI2              0x02
#define TIMER_CONTROL_SRC_GPI3              0x03
#define TIMER_CONTROL_SRC_GPI4              0x04
#define TIMER_CONTROL_SRC_GPI5              0x05
#define TIMER_CONTROL_SRC_GPI6              0x06
#define TIMER_CONTROL_SRC_GPI7              0x07
#define TIMER_CONTROL_SRC_SCI0_RX0          0x08
#define TIMER_CONTROL_SRC_SCI1_RX1          0x09
#define TIMER_CONTROL_SRC_PWM_CENTRE        0x0A
#define TIMER_CONTROL_SRC_BEMF_COMP_TACHO   0x0B
#define TIMER_CONTROL_SRC_LCRX_SYNC         0x0C
#define TIMER_CONTROL_SRC_T5_TRIGGER        0x0D
#define TIMER_CONTROL_SRC_T6_TRIGGER        0x0E
#define TIMER_CONTROL_SRC_T7_TRIGGER        0x0F

#define TIMER_CONTROL_TCLK_MASK             0xFE0F
#define TIMER_CONTROL_TCLK_PRESCALE_1       0x0000
#define TIMER_CONTROL_TCLK_PRESCALE_2       0x0010
#define TIMER_CONTROL_TCLK_PRESCALE_4       0x0020
#define TIMER_CONTROL_TCLK_PRESCALE_8       0x0030
#define TIMER_CONTROL_TCLK_PRESCALE_16      0x0040
#define TIMER_CONTROL_TCLK_PRESCALE_32      0x0050
#define TIMER_CONTROL_TCLK_PRESCALE_64      0x0060
#define TIMER_CONTROL_TCLK_PRESCALE_128     0x0070
#define TIMER_CONTROL_TCLK_PRESCALE_256     0x0080
#define TIMER_CONTROL_TCLK_PRESCALE_512     0x0090
#define TIMER_CONTROL_TCLK_PRESCALE_1024    0x00A0
#define TIMER_CONTROL_TCLK_PRESCALE_2048    0x00B0
#define TIMER_CONTROL_TCLK_PRESCALE_4096    0x00C0
#define TIMER_CONTROL_TCLK_PRESCALE_8192    0x00D0
#define TIMER_CONTROL_TCLK_PRESCALE_16384   0x00E0
#define TIMER_CONTROL_TCLK_PRESCALE_32768   0x00F0
#define TIMER_CONTROL_TCLK_PRESCALE_65536   0x0100

#define TIMER_CONTROL_TMODE_MASK                                0xE1FF
#define TIMER_CONTROL_TMODE_HI_TIME                             0x0000  
#define TIMER_CONTROL_TMODE_LO_TIME                             0x0200
#define TIMER_CONTROL_TMODE_PERIOD_RISING_TO_RISING             0x0400
#define TIMER_CONTROL_TMODE_PERIOD_FALLING_TO_FALLING           0x0600
#define TIMER_CONTROL_TMODE_EVENT_CNTR_RISING_EDGES             0x0800
#define TIMER_CONTROL_TMODE_EVENT_CNTR_FALLING_EDGES            0x0A00
#define TIMER_CONTROL_TMODE_EVENT_CNTR_RISING_OR_FALLING_EDGES  0x0C00
#define TIMER_CONTROL_TMODE_FDCTG                               0x0E00
#define TIMER_CONTROL_TMODE_START_STOP                          0x1000
#define TIMER_CONTROL_TMODE_VDCTG                               0x1200

#define TIMER_CONTROL_IEN_OVF   ( 0b0010000000000000U )
#define TIMER_CONTROL_IEN_DVL   ( 0b0100000000000000U )
#define TIMER_CONTROL_TIMER_EN  ( 0b1000000000000000U )

#define TIMER_STARTSTOP_FREEZE  ( 0b0000000000000000U )
#define TIMER_STARTSTOP_RUN     ( 0b0000000000000001U )

#define TIMER_STATUS_OVF        ( 0b0000000000000001U )
#define TIMER_STATUS_DVL        ( 0b0000000000000010U )
#define TIMER_STATUS_DOR        ( 0b0000000000000100U )


// End of Timers Definitions
// -------------------------



