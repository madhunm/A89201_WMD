#ifndef HEADER_TIMER_H
#define HEADER_TIMER_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Gtu.h
 * @author       M. Shankey
 * @version      1.0
 *
 * @brief        General Purpose Timer driver
 *
 * @copyright (c) 2025 Allegro MicroSystems, Inc. All Rights Reserved.
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

#define NUM_TIMERS 8U

enum Timer
{
    eTIMER1 = 0U,
    eTIMER2 = 1U,
    eTIMER3 = 2U,
    eTIMER4 = 3U,    
    eTIMER5 = 4U,
    eTIMER6 = 5U,
    eTIMER7 = 6U,
    eTIMER8 = 7U   
};

enum TimerInterrupts
{
    eTIMER_INTERRUPT1 = 2U, 
    eTIMER_INTERRUPT2 = 3U,
    eTIMER_INTERRUPT3 = 4U,
    eTIMER_INTERRUPT4 = 5U,
    eTIMER_INTERRUPT5 = 6U,
    eTIMER_INTERRUPT6 = 7U,
    eTIMER_INTERRUPT7 = 8U,
    eTIMER_INTERRUPT8 = 9U
};

enum TimerControlSource     // Timer Control Source options
{
    eTIMER_CONTROL_SRC_MASK =            0xFFF0U,
    eTIMER_CONTROL_SRC_GPI0 =            0x00U,
    eTIMER_CONTROL_SRC_GPI1 =            0x01U,
    eTIMER_CONTROL_SRC_GPI2 =            0x02U,
    eTIMER_CONTROL_SRC_GPI3 =            0x03U,
    eTIMER_CONTROL_SRC_GPI4 =            0x04U,
    eTIMER_CONTROL_SRC_GPI5 =            0x05U,
    eTIMER_CONTROL_SRC_GPI6 =            0x06U,
    eTIMER_CONTROL_SRC_GPI7 =            0x07U,
    eTIMER_CONTROL_SRC_SCI0_RX0 =        0x08U,
    eTIMER_CONTROL_SRC_SCI1_RX1 =        0x09U,
    eTIMER_CONTROL_SRC_PWM_CENTRE =      0x0AU,
    eTIMER_CONTROL_SRC_BEMF_COMP_TACHO = 0x0BU,
    eTIMER_CONTROL_SRC_LCRX_SYNC =       0x0CU,
    eTIMER_CONTROL_SRC_T5_TRIGGER =      0x0DU,
    eTIMER_CONTROL_SRC_T6_TRIGGER =      0x0EU,
    eTIMER_CONTROL_SRC_T7_TRIGGER =      0x0FU
};

enum TimerControlTClk       // Timer Control TClk prescale options
{
    eTIMER_CONTROL_TCLK_MASK =           0xFE0FU,
    eTIMER_CONTROL_TCLK_PRESCALE_1 =     0x0000U,
    eTIMER_CONTROL_TCLK_PRESCALE_2 =     0x0010U,
    eTIMER_CONTROL_TCLK_PRESCALE_4 =     0x0020U,
    eTIMER_CONTROL_TCLK_PRESCALE_8 =     0x0030U,
    eTIMER_CONTROL_TCLK_PRESCALE_16 =    0x0040U,
    eTIMER_CONTROL_TCLK_PRESCALE_32 =    0x0050U,
    eTIMER_CONTROL_TCLK_PRESCALE_64 =    0x0060U,
    eTIMER_CONTROL_TCLK_PRESCALE_128 =   0x0070U,
    eTIMER_CONTROL_TCLK_PRESCALE_256 =   0x0080U,
    eTIMER_CONTROL_TCLK_PRESCALE_512 =   0x0090U,
    eTIMER_CONTROL_TCLK_PRESCALE_1024 =  0x00A0U,
    eTIMER_CONTROL_TCLK_PRESCALE_2048 =  0x00B0U,
    eTIMER_CONTROL_TCLK_PRESCALE_4096 =  0x00C0U,
    eTIMER_CONTROL_TCLK_PRESCALE_8192 =  0x00D0U,
    eTIMER_CONTROL_TCLK_PRESCALE_16384 = 0x00E0U,
    eTIMER_CONTROL_TCLK_PRESCALE_32768 = 0x00F0U,
    eTIMER_CONTROL_TCLK_PRESCALE_65536 = 0x0100U,
};

enum TimerControlTMode      // Timer Control TMode options
{
    eTIMER_CONTROL_TMODE_MASK =                               0xE1FFU,
    eTIMER_CONTROL_TMODE_HI_TIME =                            0x0000U,
    eTIMER_CONTROL_TMODE_LO_TIME =                            0x0200U,
    eTIMER_CONTROL_TMODE_PERIOD_RISING_TO_RISING =            0x0400U,
    eTIMER_CONTROL_TMODE_PERIOD_FALLING_TO_FALLING =          0x0600U,
    eTIMER_CONTROL_TMODE_EVENT_CNTR_RISING_EDGES =            0x0800U,
    eTIMER_CONTROL_TMODE_EVENT_CNTR_FALLING_EDGES =           0x0A00U,
    eTIMER_CONTROL_TMODE_EVENT_CNTR_RISING_OR_FALLING_EDGES = 0x0C00U,
    eTIMER_CONTROL_TMODE_FDCTG =                              0x0E00U,
    eTIMER_CONTROL_TMODE_START_STOP =                         0x1000U,
    eTIMER_CONTROL_TMODE_VDCTG =                              0x1200U,
};

enum TimerControlEnable     // Timer Control Enable options
{    
    eTIMER_CONTROL_IEN_OVF =  ( 0b0010000000000000U ),
    eTIMER_CONTROL_IEN_DVL =  ( 0b0100000000000000U ),
    eTIMER_CONTROL_TIMER_EN = ( 0b1000000000000000U )
};

enum TimerStartStop         // Timer Start-Stop control bit
{
    eTIMER_STARTSTOP_FREEZE = ( 0b0000000000000000U ),
    eTIMER_STARTSTOP_RUN =    ( 0b0000000000000001U )
};

enum TimerStatus            // Timer Status flags
{
    eTIMER_STATUS_OVF =       ( 0b0000000000000001U ),
    eTIMER_STATUS_DVL =       ( 0b0000000000000010U ),
    eTIMER_STATUS_DOR =       ( 0b0000000000000100U )
};


// A structure to represent the register set of one timer 
typedef struct
{
    uint32 Control;     // offset:  0x00U 
    uint32 Threshold;   // offset:  0x04U 
    uint32 StartStop;   // offset:  0x08U 
    uint32 CountMax;    // offset:  0x0CU 
    uint32 Result;      // offset:  0x10U   
    uint32 Status;      // offset:  0x14U   
} GtuRegs_t;


#define  stGtu1Regs (*( volatile GtuRegs_t *)eADDRESS_TIMER1_BASE)
#define  stGtu2Regs (*( volatile GtuRegs_t *)eADDRESS_TIMER2_BASE)
#define  stGtu3Regs (*( volatile GtuRegs_t *)eADDRESS_TIMER3_BASE)
#define  stGtu4Regs (*( volatile GtuRegs_t *)eADDRESS_TIMER4_BASE)
#define  stGtu5Regs (*( volatile GtuRegs_t *)eADDRESS_TIMER5_BASE)
#define  stGtu6Regs (*( volatile GtuRegs_t *)eADDRESS_TIMER6_BASE)
#define  stGtu7Regs (*( volatile GtuRegs_t *)eADDRESS_TIMER7_BASE)   
#define  stGtu8Regs (*( volatile GtuRegs_t *)eADDRESS_TIMER8_BASE)
    
extern GtuRegs_t* const apGtuRegs[]; /* Extern only required for inline function */

/**
 * @brief Clear one of the timers Status bits
 * @param[in] eTimerNum Timer number (0-7) 
 * @param[in] eStatusFlag - flag to be cleared
 */
__attribute__( ( always_inline ) ) static inline void Gtu_ClearStatusFast( enum Timer const eTimerNum, enum TimerStatus eStatusFlag )
{
    apGtuRegs[eTimerNum]->Status |= (uint32)(eStatusFlag);
}

/**
 * @brief Sets a timers Control source
 * @details Sets the (start-stop) control signal a Timer will use. (Input source to timer)
 * @param[in] eTimerNum Timer number (0-7)
 * @param[in] eSource Timer Control Source
 */
void Gtu_SetControlSource( enum Timer const eTimerNum, const enum TimerControlSource eSource );

/**
 * @brief Get a timers Control source
 * @param[in] eTimerNum Timer number (0-7)
 * @returns Gets the external source input signal of the current timer instance.
 */
enum TimerControlSource Gtu_GetControlSource( enum Timer const eTimerNum );

/**
 * @brief Sets a timers Control TCLK
 * @details Sets the TCLK Pre-Scaler a Timer will use
 * @param[in] eTimerNum Timer number (0-7)
 * @param[in] eTClk A Timer Clock Pre-Scaler Option
 */
void Gtu_SetControlTClk( enum Timer const eTimerNum, const enum TimerControlTClk eTClk );

/**
 * @brief Gets a timers Control TCLK
 * @param[in] eTimerNum Timer number (0-7)
 * @returns TClk Pre-Scaler Option
 */
enum TimerControlTClk Gtu_GetControlTClk( enum Timer const eTimerNum );

/**
 * @brief Set a timers Control mode
 * @param[in] eTimerNum Timer number (0-7) 
 * @param[in] eMode - a Timer-Counter mode
 */
void Gtu_SetControlTMode( enum Timer const eTimerNum, const enum TimerControlTMode eMode );
    
/**
 * @brief Get a timers Control mode
 * @param[in] eTimerNum Timer number (0-7) 
 * @returns Timer-Counter mode
 */
enum TimerControlTMode Gtu_GetControlTMode( enum Timer const eTimerNum );

/**
 * @brief Set one of the timers Control enable bits
 * @param[in] eTimerNum Timer number (0-7) 
 * @param[in] eEnableFlag - flag to be altered
 * @param[in] bFlag - state of enable bit
 */
void Gtu_SetControlEnable( enum Timer const eTimerNum, enum TimerControlEnable const eEnableFlag, boolean bFlag );

/**
 * @brief Get one of the timers Control enable bits
 * @param[in] eTimerNum Timer number (0-7) 
 * @param[in] eEnableFlag - flag to be read
 * @returns state of enable bit
 */
boolean Gtu_GetControlEnable( enum Timer const eTimerNum, enum TimerControlEnable const eEnableFlag );

/**
 * @brief Set the threshold value of a timer
 * @param[in] eTimerNum Timer number (0-7) 
 * @param[in] u16Threshold The threshold value
 */
void Gtu_SetThreshold( const enum Timer eTimerNum, const uint16 u16Threshold );

/**
 * @brief Get the threshold value of a timer
 * @param[in] eTimerNum Timer number (0-7) 
 * @returns the threshold value
 */
uint16 Gtu_GetThreshold( const enum Timer eTimerNum );

/**
 * @brief Set the StartStop flag of a timer
 * @param[in] eTimerNum Timer number (0-7) 
 * @param[in] RUN_NSTOP value
 */
void Gtu_SetStartStop( enum Timer const eTimerNum, enum TimerStartStop eStartStop );

/**
 * @brief Get the StartStop flag of a timer
 * @param[in] eTimerNum Timer number (0-7) 
 * @returns the RUN_NSTOP value
 */
enum TimerStartStop Gtu_GetStartStop( enum Timer const eTimerNum );

/**
 * @brief Set the CountMax value of a timer
 * @param[in] eTimerNum Timer number (0-7) 
 * @param[in] u16CountMax the CountMax value
 */
void Gtu_SetCountMax( const enum Timer eTimerNum, const uint16 u16CountMax );

/**
 * @brief Get the CountMax value of a timer
 * @param[in] eTimerNum Timer number (0-7) 
 * @returns the CountMax value
 */
uint16 Gtu_GetCountMax( const enum Timer eTimerNum );

/**
 * @brief Get the Result value of a timer
 * @param[in] eTimerNum Timer number (0-7) 
 * @returns the Result
 */
uint16 Gtu_GetResult( enum Timer const eTimerNum );

/**
 * @brief Clear one of the timers Status bits
 * @param[in] eTimerNum Timer number (0-7) 
 * @param[in] eStatusFlag - flag to be cleared
 */
void Gtu_ClearStatus( enum Timer const eTimerNum, enum TimerStatus eStatusFlag );

/**
 * @brief Get one of the timers Status bits
 * @param[in] eTimerNum Timer number (0-7) 
 * @param[in] eStatusFlag - flag to be altered
 * @returns state of status bit
 */
boolean Gtu_GetStatus( enum Timer const eTimerNum, enum TimerStatus eStatusFlag );
   
/**
 * @brief Set timer Irq Handlers to required function
 * @param[in] pfnTimer0IrqHandler - function pointer to Irq Handler
 */

void Timer1_SetIrqHandler( void (pfnTimer1IrqHandler)( void ) );
void Timer2_SetIrqHandler( void (pfnTimer2IrqHandler)( void ) );
void Timer3_SetIrqHandler( void (pfnTimer3IrqHandler)( void ) );
void Timer4_SetIrqHandler( void (pfnTimer4IrqHandler)( void ) );
void Timer5_SetIrqHandler( void (pfnTimer5IrqHandler)( void ) );
void Timer6_SetIrqHandler( void (pfnTimer6IrqHandler)( void ) );
void Timer7_SetIrqHandler( void (pfnTimer7IrqHandler)( void ) );
void Timer8_SetIrqHandler( void (pfnTimer8IrqHandler)( void ) );

/**
 * @brief Irq Handlers for each timer
 */
void Timer1_IrqHandler( void );
void Timer2_IrqHandler( void );
void Timer3_IrqHandler( void );
void Timer4_IrqHandler( void );
void Timer5_IrqHandler( void );
void Timer6_IrqHandler( void );
void Timer7_IrqHandler( void );    
void Timer8_IrqHandler( void );


#endif //HEADER_TIMER_H
