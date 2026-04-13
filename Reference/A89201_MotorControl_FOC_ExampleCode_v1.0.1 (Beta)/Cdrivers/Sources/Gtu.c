/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/*
 * @file         Gtu.c
 * @author       M. Shankey
 * @version      1.0
 *
 * @brief        Timer interface: Timer
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
#include "ARMCM4_FP.h"

#include "Std_Types.h"
#include "Gtu.h"

// Pointer to array of the 8 timer structure addresses
GtuRegs_t* const apGtuRegs[NUM_TIMERS] = { (GtuRegs_t*)&stGtu1Regs,
                                           (GtuRegs_t*)&stGtu2Regs,
                                           (GtuRegs_t*)&stGtu3Regs,
                                           (GtuRegs_t*)&stGtu4Regs,
                                           (GtuRegs_t*)&stGtu5Regs,
                                           (GtuRegs_t*)&stGtu6Regs,
                                           (GtuRegs_t*)&stGtu7Regs,
                                           (GtuRegs_t*)&stGtu8Regs };
                                             
void Gtu_SetControlSource( enum Timer const eTimerNum, const enum TimerControlSource eSource )
{
    apGtuRegs[eTimerNum]->Control &= (uint32)eTIMER_CONTROL_SRC_MASK;
    apGtuRegs[eTimerNum]->Control |= (uint32)eSource;
}
    
enum TimerControlSource Gtu_GetControlSource( enum Timer const eTimerNum )
{
    const uint32 u32ControlValue = apGtuRegs[eTimerNum]->Control & ~(uint32)eTIMER_CONTROL_SRC_MASK;
    return (enum TimerControlSource)(u32ControlValue);
}
   

void Gtu_SetControlTClk( enum Timer const eTimerNum, const enum TimerControlTClk eTClk )
{
    apGtuRegs[eTimerNum]->Control &= (uint32)eTIMER_CONTROL_TCLK_MASK;
    apGtuRegs[eTimerNum]->Control |= (uint32)eTClk;
}
    
enum TimerControlTClk Gtu_GetControlTClk( enum Timer const eTimerNum )
{
    const uint32 u32ControlValue = apGtuRegs[eTimerNum]->Control & ~(uint32)eTIMER_CONTROL_TCLK_MASK;
    return (enum TimerControlTClk)(u32ControlValue);
}


void Gtu_SetControlTMode( enum Timer const eTimerNum, const enum TimerControlTMode eMode )
{
    apGtuRegs[eTimerNum]->Control &= (uint32)eTIMER_CONTROL_TMODE_MASK;
    apGtuRegs[eTimerNum]->Control |= (uint32)eMode;
}
    
enum TimerControlTMode Gtu_GetControlTMode( enum Timer const eTimerNum )
{
    const uint32 u32ControlValue = apGtuRegs[eTimerNum]->Control & ~(uint32)eTIMER_CONTROL_TMODE_MASK;
    return (enum TimerControlTMode)(u32ControlValue);
}

void Gtu_SetControlEnable( enum Timer const eTimerNum, enum TimerControlEnable const eEnableFlag, boolean bFlag )
{
    if( bFlag == True )
    {
        apGtuRegs[eTimerNum]->Control |= (uint32)eEnableFlag;
    }
    else
    {
        apGtuRegs[eTimerNum]->Control &= ~(uint32)eEnableFlag;
    }
}

boolean Gtu_GetControlEnable( enum Timer const eTimerNum, enum TimerControlEnable const eEnableFlag )
{
    boolean bRetVal = False;
    const uint32 u32ControlValue =  apGtuRegs[eTimerNum]->Control & (uint32)eEnableFlag;
    if( u32ControlValue != 0 )
    {
        bRetVal = True;
    }
    return bRetVal;    
}
    

void Gtu_SetThreshold( enum Timer const eTimerNum, uint16 const u16Threshold )
{
    apGtuRegs[eTimerNum]->Threshold = u16Threshold;
}

uint16 Gtu_GetThreshold( enum Timer const eTimerNum )
{
    return (uint16)apGtuRegs[eTimerNum]->Threshold;
}

void Gtu_SetStartStop( enum Timer const eTimerNum, enum TimerStartStop eStartStop )
{
    apGtuRegs[eTimerNum]->StartStop = (uint32)eStartStop;
}

enum TimerStartStop Gtu_GetStartStop( enum Timer const eTimerNum )
{
    return (enum TimerStartStop)(apGtuRegs[eTimerNum]->StartStop);
}

void Gtu_SetCountMax( enum Timer const eTimerNum, uint16 const u16CountMax )
{
    apGtuRegs[eTimerNum]->CountMax = u16CountMax;
}

uint16 Gtu_GetCountMax( enum Timer const eTimerNum )
{
    return (uint16)apGtuRegs[eTimerNum]->CountMax;
}

uint16 Gtu_GetResult( enum Timer const eTimerNum )
{
    return (uint16)apGtuRegs[eTimerNum]->Result;
}

void Gtu_ClearStatus( enum Timer const eTimerNum, enum TimerStatus eStatusFlag )
{
    apGtuRegs[eTimerNum]->Status |= (uint32)(eStatusFlag);
}

boolean Gtu_GetStatus( enum Timer const eTimerNum, enum TimerStatus eStatusFlag )
{
    boolean bRetVal = False;
    if( ( apGtuRegs[eTimerNum]->Status & (uint32)(eStatusFlag) ) != 0 )
    {
        bRetVal = True;
    }
    return bRetVal;
}
    

// Timer1 interrupt code
// ---------------------

// Declare a function pointer for use by Timer1 Irq
static void (*fpTimer1IrqHandler)( void ) = NULLPTR;

/**
 * @internal Set the Timer1 Irq Handler to required function
 *
 * @details
 */
void Timer1_SetIrqHandler( void (pfnTimer1IrqHandler)( void ) )
{
    fpTimer1IrqHandler = *pfnTimer1IrqHandler;
}

/**
 * @internal Execute Timer1 interrupt function 
 *
 * @details
 */
void Timer1_IrqHandler( void )
{
    NVIC_DisableIRQ( (IrqType_t)( eIRQNUM_TIMER1 ) );
    NVIC_ClearPendingIRQ( (IrqType_t)( eIRQNUM_TIMER1 ) );

    if( fpTimer1IrqHandler != NULLPTR )
    {
        fpTimer1IrqHandler();
    }
    
    NVIC_EnableIRQ( (IrqType_t)( eIRQNUM_TIMER1 ) );
}


// Timer2 interrupt code
// ---------------------

// Declare a function pointer for use by Timer2 Irq
static void (*fpTimer2IrqHandler)( void ) = NULLPTR;

/**
 * @internal Set the Timer2 Irq Handler to required function
 *
 * @details
 */
void Timer2_SetIrqHandler( void (pfnTimer2IrqHandler)( void ) )
{
    fpTimer2IrqHandler = *pfnTimer2IrqHandler;
}

/**
 * @internal Execute Timer2 interrupt function 
 *
 * @details
 */
void Timer2_IrqHandler( void )
{
    NVIC_DisableIRQ( (IrqType_t)( eIRQNUM_TIMER2 ) );
    NVIC_ClearPendingIRQ( (IrqType_t)( eIRQNUM_TIMER2 ) );

    if( fpTimer2IrqHandler != NULLPTR )
    {
        fpTimer2IrqHandler();
    }
    
    NVIC_EnableIRQ( (IrqType_t)( eIRQNUM_TIMER2 ) );
}


// Timer3 interrupt code
// ---------------------

// Declare a function pointer for use by Timer3 Irq
static void (*fpTimer3IrqHandler)( void ) = NULLPTR;

/**
 * @internal Set the Timer3 Irq Handler to required function
 *
 * @details
 */
void Timer3_SetIrqHandler( void (pfnTimer3IrqHandler)( void ) )
{
    fpTimer3IrqHandler = *pfnTimer3IrqHandler;
}

/**
 * @internal Execute Timer3 interrupt function 
 *
 * @details
 */
void Timer3_IrqHandler( void )
{
    NVIC_DisableIRQ( (IrqType_t)( eIRQNUM_TIMER3 ) );
    NVIC_ClearPendingIRQ( (IrqType_t)( eIRQNUM_TIMER3 ) );

    if( fpTimer3IrqHandler != NULLPTR )
    {
        fpTimer3IrqHandler();
    }
    
    NVIC_EnableIRQ( (IrqType_t)( eIRQNUM_TIMER3 ) );
}


// Timer4 interrupt code
// ---------------------

// Declare a function pointer for use by Timer4 Irq
static void (*fpTimer4IrqHandler)( void ) = NULLPTR;

/**
 * @internal Set the Timer4 Irq Handler to required function
 *
 * @details
 */
void Timer4_SetIrqHandler( void (pfnTimer4IrqHandler)( void ) )
{
    fpTimer4IrqHandler = *pfnTimer4IrqHandler;
}

/**
 * @internal Execute Timer4 interrupt function 
 *
 * @details
 */
void Timer4_IrqHandler( void )
{
    NVIC_DisableIRQ( (IrqType_t)( eIRQNUM_TIMER4 ) );
    NVIC_ClearPendingIRQ( (IrqType_t)( eIRQNUM_TIMER4 ) );

    if( fpTimer4IrqHandler != NULLPTR )
    {
        fpTimer4IrqHandler();
    }
    
    NVIC_EnableIRQ( (IrqType_t)( eIRQNUM_TIMER4 ) );
}


// Timer5 interrupt code
// ---------------------

// Declare a function pointer for use by Timer5 Irq
static void (*fpTimer5IrqHandler)( void ) = NULLPTR;

/**
 * @internal Set the Timer5 Irq Handler to required function
 *
 * @details
 */
void Timer5_SetIrqHandler( void (pfnTimer5IrqHandler)( void ) )
{
    fpTimer5IrqHandler = *pfnTimer5IrqHandler;
}

/**
 * @internal Execute Timer5 interrupt function 
 *
 * @details
 */
void Timer5_IrqHandler( void )
{
    NVIC_DisableIRQ( (IrqType_t)( eIRQNUM_TIMER5 ) );
    NVIC_ClearPendingIRQ( (IrqType_t)( eIRQNUM_TIMER5 ) );

    if( fpTimer5IrqHandler != NULLPTR )
    {
        fpTimer5IrqHandler();
    }

    NVIC_EnableIRQ( (IrqType_t)( eIRQNUM_TIMER5 ) );
}


// Timer6 interrupt code
// ---------------------

// Declare a function pointer for use by Timer6 Irq
static void (*fpTimer6IrqHandler)( void ) = NULLPTR;

/**
 * @internal Set the Timer6 Irq Handler to required function
 *
 * @details
 */
void Timer6_SetIrqHandler( void (pfnTimer6IrqHandler)( void ) )
{
    fpTimer6IrqHandler = *pfnTimer6IrqHandler;
}

/**
 * @internal Execute Timer6 interrupt function 
 *
 * @details
 */
void Timer6_IrqHandler( void )
{
    NVIC_DisableIRQ( (IrqType_t)( eIRQNUM_TIMER6 ) );
    NVIC_ClearPendingIRQ( (IrqType_t)( eIRQNUM_TIMER6 ) );

    if( fpTimer6IrqHandler != NULLPTR )
    {
        fpTimer6IrqHandler();
    }
    
    NVIC_EnableIRQ( (IrqType_t)( eIRQNUM_TIMER6 ) );
}


// Timer7 interrupt code
// ---------------------

// Declare a function pointer for use by Timer7 Irq
static void (*fpTimer7IrqHandler)( void ) = NULLPTR;

/**
 * @internal Set the Timer7 Irq Handler to required function
 *
 * @details
 */
void Timer7_SetIrqHandler( void (pfnTimer7IrqHandler)( void ) )
{
    fpTimer7IrqHandler = *pfnTimer7IrqHandler;
}

/**
 * @internal Execute Timer7 interrupt function 
 *
 * @details
 */
void Timer7_IrqHandler( void )
{
    NVIC_DisableIRQ( (IrqType_t)( eIRQNUM_TIMER7 ));
    NVIC_ClearPendingIRQ( (IrqType_t)( eIRQNUM_TIMER7 ) );

    if( fpTimer7IrqHandler != NULLPTR )
    {
        fpTimer7IrqHandler();
    }
    
    NVIC_EnableIRQ( (IrqType_t)( eIRQNUM_TIMER7 ) );
}

// Timer8 interrupt code
// ---------------------

// Declare a function pointer for use by Timer8 Irq
static void (*fpTimer8IrqHandler)( void ) = NULLPTR;

/**
 * @internal Set the Timer8 Irq Handler to required function
 *
 * @details
 */
void Timer8_SetIrqHandler( void (pfnTimer8IrqHandler)( void ) )
{
    fpTimer8IrqHandler = *pfnTimer8IrqHandler;
}

/**
 * @internal Execute Timer8 interrupt function 
 *
 * @details
 */
void Timer8_IrqHandler( void )
{
    NVIC_DisableIRQ( (IrqType_t)( eIRQNUM_TIMER8 ));
    NVIC_ClearPendingIRQ( (IrqType_t)( eIRQNUM_TIMER8 ) );

    if( fpTimer8IrqHandler != NULLPTR )
    {
        fpTimer8IrqHandler();
    }
    
    NVIC_EnableIRQ( (IrqType_t)( eIRQNUM_TIMER8 ) );
}
