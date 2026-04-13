#ifndef HEADER_GPIO_H
#define HEADER_GPIO_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 *
 * @file         Gpio.h
 * @author       Guy Sloan
 * @version      1.0
 *
 * @brief        Gpio interface.
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

// GPIO Definitions
// ----------------

// GpioRegisters
#define GPIO_BASE_ADDRESS                                         0x40008000U
#define GPIO_CONTROL0                                             0x00U
#define GPIO_CONTROL1                                             0x04U
#define GPIO_CONTROL2                                             0x08U
#define GPIO_CONTROL3                                             0x0CU
#define GPIO_CONTROL4                                             0x10U
#define GPIO_CONTROL5                                             0x14U
#define GPIO_CONTROL6                                             0x18U
#define GPIO_CONTROL7                                             0x1CU
#define GPIO_CONTROL8                                             0x20U
#define GPIO_CONTROL9                                             0x24U
#define GPIO_CONTROL10                                            0x28U
#define GPIO_CONTROL11                                            0x2CU
#define GPIO_CONTROL12                                            0x30U
#define GPIO_CONTROL13                                            0x34U
#define GPIO_CONTROL14                                            0x38U
#define GPIO_CONTROL15                                            0x3CU
#define GPIO_BLANK0                                               0x40U
#define GPIO_BLANK1                                               0x44U
#define GPIO_PDIN                                                 0x50U
#define GPIO_PDOUT                                                0x54U
#define GPIO_STG                                                  0x58U
#define GPIO_STS                                                  0x5CU
#define GPIO_STG_INTEN                                            0x60U
#define GPIO_STS_INTEN                                            0x64U
#define GPIO_EVENT_STATUS                                         0x68U
#define GPIO_USRC0                                                0xFC0U
#define GPIO_USRC1                                                0xFC4U
#define GPIO_USRC2                                                0xFC8U
#define GPIO_USRC3                                                0xFCCU
#define GPIO_USRC4                                                0xFD0U
#define GPIO_USRC5                                                0xFD4U
#define GPIO_USRC6                                                0xFD8U
#define GPIO_USRC7                                                0xFDCU
#define GPIO_USRC8                                                0xFE0U
#define GPIO_USRC9                                                0xFE4U
#define GPIO_USRC10                                               0xFE8U
#define GPIO_USRC11                                               0xFECU
#define GPIO_USRC12                                               0xFF0U
#define GPIO_USRC13                                               0xFF4U
#define GPIO_USRC14                                               0xFF8U
#define GPIO_USRC15                                               0xFFCU

// GpioControl
#define GPIO_CONTROL_PINMODE_MASK_SET                             0x000FU
#define GPIO_CONTROL_PINMODE_MASK_CLEAR                           0xFFF0U
#define GPIO_CONTROL_PINMODE_HIGHZ                                0x0000U
#define GPIO_CONTROL_PINMODE_DIGITAL_INPUT                        0x0001U
#define GPIO_CONTROL_PINMODE_DIGITAL_INPUT_PULLUP                 0x0002U
#define GPIO_CONTROL_PINMODE_DIGITAL_INPUT_PULLDOWN               0x0003U
#define GPIO_CONTROL_PINMODE_ANALOG                               0x0004U
#define GPIO_CONTROL_PINMODE_DIGITAL_OUTPUT_PUSHPULL              0x0005U
#define GPIO_CONTROL_PINMODE_DIGITAL_OUTPUT_PULLDOWN_OPENDRAIN    0x0006U
#define GPIO_CONTROL_PINMODE_DIGITAL_OUTPUT_PULLUP_OPENDRAIN      0x0007U

#define GPIO_CONTROL_ASRC_MASK_SET                                0x00F0U
#define GPIO_CONTROL_ASRC_MASK_CLEAR                              0xFF0FU
#define GPIO_CONTROL_ASRC_SEL_SCIO_TX                             0x0000U
#define GPIO_CONTROL_ASRC_SEL_SCI1_TX                             0x0010U
#define GPIO_CONTROL_ASRC_SEL_SPIO_SCKOUT                         0x0020U
#define GPIO_CONTROL_ASRC_SEL_SPI1_SCKOUT                         0x0030U
#define GPIO_CONTROL_ASRC_SEL_SPIO_CSOUT                          0x0040U
#define GPIO_CONTROL_ASRC_SEL_SPI1_CSOUT                          0x0050U
#define GPIO_CONTROL_ASRC_SEL_LIN_TX                              0x0080U
#define GPIO_CONTROL_ASRC_SEL_BEMF_COMP_TACHO                     0x0090U
#define GPIO_CONTROL_ASRC_SEL_T1_TRIGGER                          0x00A0U
#define GPIO_CONTROL_ASRC_SEL_T2_TRIGGER                          0x00B0U
#define GPIO_CONTROL_ASRC_SEL_T3_TRIGGER                          0x00C0U
#define GPIO_CONTROL_ASRC_SEL_T4_TRIGGER                          0x00D0U
#define GPIO_CONTROL_ASRC_SEL_PWN_CENTRE                          0x00E0U
#define GPIO_CONTROL_ASRC_SEL_ANY_INTERRUPT                       0x00F0U

#define GPIO_CONTROL_OUTPUT_DRIVE_STRENGTH_MASK_SET               0x0100U
#define GPIO_CONTROL_OUTPUT_DRIVE_STRENGTH_MASK_CLEAR             0xFEFFU
#define GPIO_CONTROL_OUTPUT_DRIVE_STRENGTH_NORMAL                 0x0000U
#define GPIO_CONTROL_OUTPUT_DRIVE_STRENGTH_BOOST                  0x0100U

#define GPIO_CONTROL_INTCFG_MASK_SET                              0x0E00U
#define GPIO_CONTROL_INTCFG_MASK_CLEAR                            0xF1FFU
#define GPIO_CONTROL_INTCFG_RISING_EDGE                           0x0000U
#define GPIO_CONTROL_INTCFG_RISING_FALLING_EDGE                   0x0400U
#define GPIO_CONTROL_INTCFG_LEVEL_HIGH                            0x0600U
#define GPIO_CONTROL_INTCFG_LEVEL_LOW                             0x0800U

#define GPIO_CONTROL_INTEN_MASK_SET                               0x1000U
#define GPIO_CONTROL_INTEN_MASK_CLEAR                             0xEFFFU

#define GPIO_CONTROL_AOSEL_MASK_SET                               0x2000U
#define GPIO_CONTROL_AOSEL_MASK_CLEAR                             0xDFFFU

// GpioBlank - used in bits 15:12 or 11:8 or 7:4 or 3:0 of the two GPIO Blank registers
#define GPIO_BLANK_TIMING_0NS                                     0x0U
#define GPIO_BLANK_TIMING_200NS                                   0x1U
#define GPIO_BLANK_TIMING_400NS                                   0x2U
#define GPIO_BLANK_TIMING_600NS                                   0x3U
#define GPIO_BLANK_TIMING_800NS                                   0x4U
#define GPIO_BLANK_TIMING_1P0US                                   0x5U
#define GPIO_BLANK_TIMING_1P2US                                   0x6U
#define GPIO_BLANK_TIMING_1P4US                                   0x7U
#define GPIO_BLANK_TIMING_1P6US                                   0x8U
#define GPIO_BLANK_TIMING_1P8US                                   0x9U
#define GPIO_BLANK_TIMING_2P0US                                   0xAU
#define GPIO_BLANK_TIMING_2P2US                                   0xBU
#define GPIO_BLANK_TIMING_2P4US                                   0xCU
#define GPIO_BLANK_TIMING_2P6US                                   0xDU
#define GPIO_BLANK_TIMING_2P8US                                   0xEU
#define GPIO_BLANK_TIMING_3P0US                                   0xFU

// GpioStg
#define GPIO_STG_STATUS_PIN0                                      ( 0b0000000000000001U )
#define GPIO_STG_STATUS_PIN1                                      ( 0b0000000000000010U )
#define GPIO_STG_STATUS_PIN2                                      ( 0b0000000000000100U )
#define GPIO_STG_STATUS_PIN3                                      ( 0b0000000000001000U )
#define GPIO_STG_STATUS_PIN4                                      ( 0b0000000000010000U )
#define GPIO_STG_STATUS_PIN5                                      ( 0b0000000000100000U )
#define GPIO_STG_STATUS_PIN6                                      ( 0b0000000001000000U )
#define GPIO_STG_STATUS_PIN7                                      ( 0b0000000010000000U )

// GpioSts
#define GPIO_STS_STATUS_PIN0                                      ( 0b0000000000000001U )
#define GPIO_STS_STATUS_PIN1                                      ( 0b0000000000000010U )
#define GPIO_STS_STATUS_PIN2                                      ( 0b0000000000000100U )
#define GPIO_STS_STATUS_PIN3                                      ( 0b0000000000001000U )
#define GPIO_STS_STATUS_PIN4                                      ( 0b0000000000010000U )
#define GPIO_STS_STATUS_PIN5                                      ( 0b0000000000100000U )
#define GPIO_STS_STATUS_PIN6                                      ( 0b0000000001000000U )
#define GPIO_STS_STATUS_PIN7                                      ( 0b0000000010000000U )

// GpioStgIntEn
#define GPIO_STG_INTEN_MASK_SET                                   ( 0b0000000011111111U )
#define GPIO_STG_INTEN_MASK_CLEAR                                 ( 0b1111111100000000U )
#define GPIO_STG_INTEN_PIN0                                       ( 0b0000000000000001U )
#define GPIO_STG_INTEN_PIN1                                       ( 0b0000000000000010U )
#define GPIO_STG_INTEN_PIN2                                       ( 0b0000000000000100U )
#define GPIO_STG_INTEN_PIN3                                       ( 0b0000000000001000U )
#define GPIO_STG_INTEN_PIN4                                       ( 0b0000000000010000U )
#define GPIO_STG_INTEN_PIN5                                       ( 0b0000000000100000U )
#define GPIO_STG_INTEN_PIN6                                       ( 0b0000000001000000U )
#define GPIO_STG_INTEN_PIN7                                       ( 0b0000000010000000U )

// GpioStsIntEn
#define GPIO_STS_INTEN_MASK_SET                                   ( 0b0000000011111111U )
#define GPIO_STS_INTEN_MASK_CLEAR                                 ( 0b1111111100000000U )
#define GPIO_STS_INTEN_PIN0                                       ( 0b0000000000000001U )
#define GPIO_STS_INTEN_PIN1                                       ( 0b0000000000000010U )
#define GPIO_STS_INTEN_PIN2                                       ( 0b0000000000000100U )
#define GPIO_STS_INTEN_PIN3                                       ( 0b0000000000001000U )
#define GPIO_STS_INTEN_PIN4                                       ( 0b0000000000010000U )
#define GPIO_STS_INTEN_PIN5                                       ( 0b0000000000100000U )
#define GPIO_STS_INTEN_PIN6                                       ( 0b0000000001000000U )
#define GPIO_STS_INTEN_PIN7                                       ( 0b0000000010000000U )

// GpioEventStatus
#define GPIO_EVENTSTATUS_PIN0                                     ( 0b0000000000000001U )
#define GPIO_EVENTSTATUS_PIN1                                     ( 0b0000000000000010U )
#define GPIO_EVENTSTATUS_PIN2                                     ( 0b0000000000000100U )
#define GPIO_EVENTSTATUS_PIN3                                     ( 0b0000000000001000U )
#define GPIO_EVENTSTATUS_PIN4                                     ( 0b0000000000010000U )
#define GPIO_EVENTSTATUS_PIN5                                     ( 0b0000000000100000U )
#define GPIO_EVENTSTATUS_PIN6                                     ( 0b0000000001000000U )
#define GPIO_EVENTSTATUS_PIN7                                     ( 0b0000000010000000U )

// GpioUsrc
#define GPIO_USRC_FOSEL                                           0x01U
#define GPIO_USRC_ESRCSEL0                                        0x00U
#define GPIO_USRC_ESRCSEL1                                        0x02U
#define GPIO_USRC_ESRCSEL2                                        0x04U
#define GPIO_USRC_ESRCSEL3                                        0x06U
#define GPIO_USRC_ESRCSEL4                                        0x08U
#define GPIO_USRC_ESRCSEL5                                        0x0AU
#define GPIO_USRC_ESRCSEL6                                        0x0CU
#define GPIO_USRC_ESRCSEL7                                        0x0EU


#define CONTROL_MODE_MASK_SET                                     0xFU
#define CONTROL_MODE_BITPOS                                       0U
#define CONTROL_MUX0_MASK_SET                                     0xF0U
#define CONTROL_MUX0_BITPOS                                       0x4U
#define CONTROL_DRIVE_MASK_SET                                    0x100U
#define CONTROL_DRIVE_BITPOS                                      8U
#define CONTROL_IRQCFG_MASK_SET                                   0xE00U
#define CONTROL_IRQCFG_BITPOS                                     9U
#define CONTROL_IRQEN_MASK_SET                                    0x1000U
#define CONTROL_IRQEN_BITPOS                                      12U
#define CONTROL_MUX1_MASK_SET                                     0x2000U
#define CONTROL_MUX1_BITPOS                                       13U
#define BLANK3_0_MASK_SET                                         0xFU
#define BLANK3_0_BITPOS                                           0U
#define BLANK7_4_MASK_SET                                         0xF0U
#define BLANK7_4_BITPOS                                           4U
#define BLANK11_8_MASK_SET                                        0xF00U
#define BLANK11_8_BITPOS                                          8U
#define BLANK12_15_MASK_SET                                       0xF000U
#define BLANK12_15_BITPOS                                         12U
#define EVAL_MODE_MASK_SET                                        0x1U
#define EVAL_MODE_BITPOS                                          0U
#define EVAL_SOURCE_MASK_SET                                      0xEU
#define EVAL_SOURCE_BITPOS                                        1U

#define GenControlRegAddrFromPinNum(GpioInstanceId, u32Addr){\
    (u32Addr = ((uint32)(((uint8*)GPIO_BASE_ADDRESS) + (GpioInstanceId * sizeof(uint32)))));\
}

/**
 * @internal
 * @brief Define register and base addresses
 */
// type definition for the GPIO control register
typedef struct {
    uint32 Control0;             //  0x00  :(R/W)
    uint32 Control1;             //  0x04  :(R/W)
    uint32 Control2;             //  0x08  :(R/W)
    uint32 Control3;             //  0x0C  :(R/W)
    uint32 Control4;             //  0x10  :(R/W)
    uint32 Control5;             //  0x14  :(R/W)
    uint32 Control6;             //  0x18  :(R/W)
    uint32 Control7;             //  0x1C  :(R/W)
    uint32 Control8;             //  0x20  :(R/W)
    uint32 Control9;             //  0x24  :(R/W)
    uint32 Control10;            //  0x28  :(R/W)
    uint32 Control11;            //  0x2C  :(R/W)
    uint32 Control12;            //  0x30  :(R/W)
    uint32 Control13;            //  0x34  :(R/W)
    uint32 Control14;            //  0x38  :(R/W)
    uint32 Control15;            //  0x3C  :(R/W)
    uint32 Blank0;               //  0x40  :(R/W)
    uint32 Blank1;               //  0x44  :(R/W)
    uint32 Reserved0[0x2];       //  0x48 - 0x4C (Not used)
    uint32 Pdin;                 //  0x50  :(R)
    uint32 Pdout;                //  0x54  :(R/W)
    uint32 Stg;                  //  0x58  :(R/1C)
    uint32 Sts;                  //  0x5C  :(R/1C)
    uint32 StgInten;             //  0x60  :(R/W)
    uint32 StsInten;             //  0x64  :(R/W)
    uint32 EventStatus;          //  0x68  :(R/1C)
    uint32 Reserved1[0x3D5];     //  0x6C - 0xFBC (Not used)
    uint32 Usrc0;                //  0xFC0 :(R/W)
    uint32 Usrc1;                //  0xFC4 :(R/W)
    uint32 Usrc2;                //  0xFC8 :(R/W)
    uint32 Usrc3;                //  0xFCC :(R/W)
    uint32 Usrc4;                //  0xFD0 :(R/W)
    uint32 Usrc5;                //  0xFD4 :(R/W)
    uint32 Usrc6;                //  0xFD8 :(R/W)
    uint32 Usrc7;                //  0xFDC :(R/W)
    uint32 Usrc8;                //  0xFE0 :(R/W)
    uint32 Usrc9;                //  0xFE4 :(R/W)
    uint32 Usrc10;               //  0xFE8 :(R/W)
    uint32 Usrc11;               //  0xFEC :(R/W)
    uint32 Usrc12;               //  0xFF0 :(R/W)
    uint32 Usrc13;               //  0xFF4 :(R/W)
    uint32 Usrc14;               //  0xFF8 :(R/W)
    uint32 Usrc15;               //  0xFFC :(R/W)
} GpioRegs_t;

#define  stGpioRegs (*( volatile GpioRegs_t *)eADDRESS_GPIO_BASE)

// End of GPIO Definitions
// -----------------------

/** @brief Unallocated pins are set to 0xFF */
static const uint8 s_ku8InvalidPinNum = 0xFFU;
static const uint8 s_ku8EvalSourceMask = 0x07U;

/**
 * @brief Enumerates the different GPIO modes of operation
 */
enum GpioInstanceId
{
    eGPIO_INSTANCE_ID0,
    eGPIO_INSTANCE_ID1,
    eGPIO_INSTANCE_ID2,
    eGPIO_INSTANCE_ID3,
    eGPIO_INSTANCE_ID4,
    eGPIO_INSTANCE_ID5,
    eGPIO_INSTANCE_ID6,
    eGPIO_INSTANCE_ID7,
    eGPIO_INSTANCE_ID8,
    eGPIO_INSTANCE_ID9,
    eGPIO_INSTANCE_ID10,
    eGPIO_INSTANCE_ID11,
    eGPIO_INSTANCE_ID12,
    eGPIO_INSTANCE_ID13,
    eGPIO_INSTANCE_ID14,
    eGPIO_INSTANCE_ID15
};


/**
 * @brief Enumerates the different GPIO modes of operation
 */
enum GpioMode
{
    /** The pin is disconnected (high impedance) */
    eGPIO_MODE_DISABLED               = 0U,
    /** Input pin */
    eGPIO_MODE_DIGITAL_INPUT          = 1U,
    /** Input pin with internal pull-up */
    eGPIO_MODE_DIGITAL_INPUT_PULLUP   = 2U,
    /** Input pin with internal pull-down */
    eGPIO_MODE_DIGITAL_INPUT_PULLDOWN = 3U,
    /** Input pin connected to an analog source */
    eGPIO_MODE_ANALOG_INPUT_SOURCE    = 4U,
    /** Output pin */
    eGPIO_MODE_OUTPUT_PUSH_PULL       = 5U,
    /** Output pin  with open drain pull-down */
    eGPIO_MODE_OUTPUT_PULLDOWN        = 6U,
    /** Output pin  with open drain pull-up */
    eGPIO_MODE_OUTPUT_PULLUP          = 7U,
};


/**
 * @brief Enumerates the different GPIO sources
 */
enum GpioSource
{   /** Not set*/
    eGPIO_ASRC_NONE = ku8Invalid,
    /** Use a signal external to the GPIO as a source*/
    eGPIO_ASRC0 = 0U,
    eGPIO_ASRC1 = 1U,
    eGPIO_ASRC2 = 2U,
    eGPIO_ASRC3 = 3U,
    eGPIO_ASRC4 = 4U,
    eGPIO_ASRC5 = 5U,
    eGPIO_ASRC6 = 6U,
    eGPIO_ASRC7 = 7U,
    eGPIO_ASRC8 = 8U,
    eGPIO_ASRC9 = 9U,
    eGPIO_ASRC10 = 10U,
    eGPIO_ASRC11 = 11U,
    eGPIO_ASRC12 = 12U,
    eGPIO_ASRC13 = 13U,
    eGPIO_ASRC14 = 14U,
    eGPIO_ASRC15 = 15U,

    /** Use the GPIO module as a source*/
    eGPIO_SOURCE_DOUT = 16U,

    /** Use an application source */
    eGPIO_ESRC0 = 0x80U,
    eGPIO_ESRC1 = 0x81U,
    eGPIO_ESRC2 = 0x82U,
    eGPIO_ESRC3 = 0x83U,
    eGPIO_ESRC4 = 0x84U,
    eGPIO_ESRC5 = 0x85U,
    eGPIO_ESRC6 = 0x86U,
    eGPIO_ESRC7 = 0x87U,
};


/**
 * @brief Enumerates the different GPIO USRCs
 */
enum GpioUsrc
{
    eGPIO_USRC_NONE = ku8Invalid,
    eGPIO_USRC0 = 0U,
    eGPIO_USRC1 = 1U,
    eGPIO_USRC2 = 2U,
    eGPIO_USRC3 = 3U,
    eGPIO_USRC4 = 4U,
    eGPIO_USRC5 = 5U,
    eGPIO_USRC6 = 6U,
    eGPIO_USRC7 = 7U,
    eGPIO_USRC8 = 8U,
    eGPIO_USRC9 = 9U,
    eGPIO_USRC10 = 10U,
    eGPIO_USRC11 = 11U,
    eGPIO_USRC12 = 12U,
    eGPIO_USRC13 = 13U,
    eGPIO_USRC14 = 14U,
    eGPIO_USRC15 = 15U
};


/**
 * @brief Enumerates the different values for GPIO USRCs
 */
enum GpioUsrcValues
{
    eGPIO_USRC_FOSEL =    0x01U,
    eGPIO_USRC_ESRCSEL0 = 0x00U,
    eGPIO_USRC_ESRCSEL1 = 0x02U,
    eGPIO_USRC_ESRCSEL2 = 0x04U,
    eGPIO_USRC_ESRCSEL3 = 0x06U,
    eGPIO_USRC_ESRCSEL4 = 0x08U,
    eGPIO_USRC_ESRCSEL5 = 0x0AU,
    eGPIO_USRC_ESRCSEL6 = 0x0CU,
    eGPIO_USRC_ESRCSEL7 = 0x0EU
};

/**
 * @brief Enumerates the different GPIO IRQ triggers
 */
enum GpioIrqTrigger
{   /** Not set*/
    eGPIO_IRQ_TRIGGER_NONE,
    /** Trigger the interrupt on the rising edge */
    eGPIO_IRQ_TRIGGER_EDGE_RISING,
    /** Trigger the interrupt on the falling edge */
    eGPIO_IRQ_TRIGGER_EDGE_FALLING,
    /** Trigger the interrupt on the rising and falling edges */
    eGPIO_IRQ_TRIGGER_EDGE_BOTH,
    /** Trigger on a high level */
    eGPIO_IRQ_TRIGGER_LEVEL_HIGH,
    /** Trigger on a low level */
    eGPIO_IRQ_TRIGGER_LEVEL_LOW
};

/**
 * @brief Enumerates the different IRQ sources
 */
enum GpioIrqSource
{
    /** IRQ source is a trigger */
    eGPIO_IRQ_SOURCE_TRIGGER,
    /** IRQ source is a short to ground */
    eGPIO_IRQ_SOURCE_SHORT_TO_GROUND,
    /** IRQ source is a short to supply */
    eGPIO_IRQ_SOURCE_SHORT_TO_SUPPLY
};

/**
 * @brief IRQ handler context definition
 */
typedef struct {
  enum GpioIrqSource eIrqSource;
  enum GpioInstanceId keGpioInstanceId;
} GpioIrqContext_t;


/**
 * @brief Returns the pin number of the current instance.
 * @remark An invalid pin number can be returned which is 0xFFFF.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns U8 A pin number.
 */
uint8 Gpio_GetPinNum(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Sets a GPIO pin active (enables NVIC IRQ and initialises GPIO object).
 * @details A GPIO instance is setup with an active pin mapping.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 */
Std_ReturnType  Gpio_SetPinActive(enum GpioInstanceId const keGpioInstanceId);


/**
 * @brief Sets a GPIO pin inactive (enables NVIC IRQ and initialises GPIO object).
 * @details A GPIO instance is setup with an active pin mapping.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 */
Std_ReturnType  Gpio_SetPinInactive(enum GpioInstanceId const keGpioInstanceId);


/**
 * @brief Sets all GPIO pins inactive (enables NVIC IRQ and initialises GPIO object).
 * @details A GPIO instance is setup with an active pin mapping.
 */
Std_ReturnType  Gpio_SetAllPinsInactive(void);


/**
 * @brief Sets the mode a GPIO pin will use
 * @details Mode values outside the expected range result in status error
 * ErrorType::eINVALID_ARGUMENT being set and no change will occur.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] keMode A GPIO mode
 * @returns E_OK on success else E_NOT_OK upon failure.
 */
Std_ReturnType Gpio_SetMode(enum GpioInstanceId const keGpioInstanceId, enum GpioMode const keMode);

/**
 * @brief Gets the mode of a GPIO pin
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[out] peGpioMode The GPIO mode.
 * @returns E_OK if successful else E_NOT_OK upon failure.
 */
Std_ReturnType Gpio_GetMode(enum GpioInstanceId const keGpioInstanceId, enum GpioMode* peGpioMode);

/**
 * @brief Configure the RX pin.
 * @details Configure which GpioPin to use for receiving on a particular UART port.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] ku8PortNum The UART port to use (0 or 1).
 * @param[in] ku8TxPinNum The pin number to use for receiving (Depends on number of GpioPins).
 */
Std_ReturnType Gpio_SetRxPinPort( enum GpioInstanceId const keGpioInstanceId, uint8 const ku8PortNum, uint8 const ku8RxPinNum );

/**
 * @brief Sets the source for the target GPIO
 * @par Details:  
 *            This sets the source for a target GPIO using the GpioSoource enumerated list of possible sources.
 *            This function internally handles the multiplexer enables and select lines that put the GPIO block under either
 *            software control or driven by a peripheral when configured as an output. 
 *            The Application Output Selection (AOSEL),  Application Source (ASRC) Selection, 
 *            Final Output Selection (FOSEL) and Evaluation Source Selection (ESRCSEL) controlled multiplexers defined in the 
 *            user manual connect the selected source to drive the target output GPIO pin.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] keSource The desired source.
 */
Std_ReturnType Gpio_SetSource(enum GpioInstanceId const keGpioInstanceId, enum GpioSource const keSource );

/**
 * @brief Gets the current source selection for a given GPIO 
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[out] peGpioSource The GPIO source.
 * @returns Source The selected source.
 */
Std_ReturnType Gpio_GetSource(enum GpioInstanceId const keGpioInstanceId, enum GpioSource* const peGpioSource );

/**
 * @brief Sets the multiplexer configuration for routing the output.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] bBoostEnabled True if boost enabled.
 */
Std_ReturnType Gpio_SetDriveBoost(enum GpioInstanceId const keGpioInstanceId, const boolean kbBoostEnabled );

/**
 * @brief Gets whether the drive boost is enabled.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[out] kpbEnabled True if boost enabled or false if disabled.
 * @returns E_OK on success and E_NOT_OK upon failure.
 */
Std_ReturnType Gpio_IsDriveBoostEnabled(enum GpioInstanceId const keGpioInstanceId, boolean* const kpbEnabled);

/**
 * @brief Gets the state of a GPIO pin.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns True if set and object is correctly initialized else false.
 */
boolean Gpio_IsSet(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Sets a GPIO pin
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @details Does nothing if object not initialized.
 */
void Gpio_Set(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Clears a GPIO pin.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @details Does nothing if object not initialized.
 */
void Gpio_Clear(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Read a GPIO pin.
 * @remarks Needs pin to be configured as an input
 * and attached to a signal source if not pulled up or down.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns State of input pin. True the pin is set, False its clear.
 */
boolean Gpio_Read(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Set the IRQ trigger
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] keIrqTrigger The enumerated trigger value
 * @details Does nothing if object not initialized
 * @returns  E_OK on success else E_NOT_OK upon failure.
 */
Std_ReturnType Gpio_SetIrqTrigger(enum GpioInstanceId const keGpioInstanceId, enum GpioIrqTrigger const keIrqTrigger );

/**
 * @brief Get the IRQ trigger
 * @returns IrqTrigger The enumerated trigger value
 */
enum GpioIrqTrigger Gpio_GetIrqTrigger(enum GpioInstanceId const keGpioInstanceId);


/**
 * @brief Set the IRQ enable.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] kbEnable True enables the IRQ.
 * @return E_OK on success else E_NOT_OK upon failure.
 */
Std_ReturnType Gpio_SetTriggerIrqEnable(enum GpioInstanceId const keGpioInstanceId, const boolean kbEnable );

/**
 * @brief Get the IRQ enable.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns True if enabled else false.
 */
boolean Gpio_IsTriggerIrqEnabled(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Gets the state of a GPIO Trigger IRQ.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns true if set and object was correctly initialized else false.
 */
boolean Gpio_IsTriggerIrqSet(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Clears a GPIO trigger IRQ.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @details Does nothing if object not initialized.
 */
void Gpio_ClearTriggerIrq(enum GpioInstanceId const keGpioInstanceId);


/**
 * @brief Set the short to ground IRQ enable.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] kbEnable True enables the IRQ.
 */
void Gpio_SetShortToGroundIrqEnable(enum GpioInstanceId const keGpioInstanceId, const boolean kbEnable );

/**
 * @brief Get the short to ground IRQ enable.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns True if enabled else false.
 */
boolean Gpio_IsShortToGroundIrqEnabled(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Gets the short to ground IRQ state.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns true if set and object was correctly initialized else false.
 */
boolean Gpio_IsShortToGroundIrqSet(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Clears a short to ground GPIO Trigger IRQ.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @details Does nothing if object not initialized.
 */
void Gpio_ClearShortToGroundIrq(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Set the short to supply IRQ enable.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] kbEnable True enables the IRQ.
 */
void Gpio_SetShortToSupplyIrqEnable(enum GpioInstanceId const keGpioInstanceId, const boolean kbEnable );

/**
 * @brief Get the short to supply IRQ enable.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns True if enabled else false.
 */
boolean Gpio_IsShortToSupplyIrqEnabled(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Gets the state of the short to supply IRQ.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns true if set and object was correctly initialized else false.
 */
boolean Gpio_IsShortToSupplyIrqSet(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Clears a short to supply Trigger IRQ.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @details Does nothing if object not initialized.
 */
void Gpio_ClearShortToSupplyIrq(enum GpioInstanceId const keGpioInstanceId);


/**
 * @brief Set a handler for the IRQ
 * @details Does nothing if object not initialized
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] pfGpioIrqHandler A function pointer to the IRQ handler.
 * @returns E_OK if set and object was correctly initialized else E_NOT_OK.
 */
Std_ReturnType Gpio_SetIrqHandler(enum GpioInstanceId const keGpioInstanceId, void (pfGpioIrqHandler)(void* pstGpioIrqContext) );

/**
 * @brief Gets whether an IRQ handler has been assigned
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns True when a handler has been assigned or False otherwise.
 */
boolean  Gpio_HasIrqHandler(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief Sets the short circuit detection time for the current pin.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @param[in] ku16TimeInNanoseconds A time in the inclusive range of 0 -> 3000 nanoseconds
 * in 200nS steps. Zero means no short circuit detection.
 * @returns E_OK if successful else E_NOT_OK upon failure.
 */
Std_ReturnType Gpio_SetShortCircuitDetectionTime(enum GpioInstanceId const keGpioInstanceId, const uint16 ku16TimeInNanoseconds );

/**
 * @brief Gets the short circuit detection time for the current pin.
 * @param[in] keGpioInstanceId A GPIO instance ID.
 * @returns A time in nanoseconds.
 */
uint16 Gpio_GetShortCircuitProtectionTime(enum GpioInstanceId const keGpioInstanceId);

/**
 * @brief    The GPIO IRQ handler
 * @returns    None
 */
void Gpio_IrqHandler(void);
#endif /* HEADER_GPIO_H */