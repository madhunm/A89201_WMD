#ifndef HEADER_SYSTEM_TIMER_H
#define HEADER_SYSTEM_TIMER_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file       SystemTimer.h
 * @author     G. Sloan
 * @version    1.0
 *
 * @brief      CPU system timer interface
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
#include "A89201RegisterDefinitions.h"

#define CSR_ENABLE_MASK            0x1U
#define CSR_ENABLE_BITPOS          0U
#define CSR_TICKINT_MASK           0x2U
#define CSR_TICKINT_BITPOS         1U
#define CSR_CLKSOURCE_MASK         0x4U
#define CSR_CLKSOURCE_BITPOS       2U
#define CSR_COUNTFLAG_MASK         0x10000U
#define CSR_COUNTFLAG_BITPOS       16U

#define SVR_RELOAD_MASK            0x00FFFFFFU
#define SVR_RELOAD_BITPOS          0U

#define CVR_CURRENT_MASK           0x00FFFFFFU
#define CVR_CURRENT_BITPOS         0U

#define CALIB_TENMS_MASK           0x00FFFFFFU
#define CALIB_TENMS_BITPOS         0U
#define CALIB_SKEW_MASK            0x40000000U
#define CALIB_SKEW_BITPOS          30U
#define CALIB_NOREF_MASK           0x80000000U
#define CALIB_NOREF_BITPOS         31U

/**
 * @internal
 * @brief Define register and base addresses
 */
// type definition for the system timer registers
typedef struct {
    uint32 systCsr;              //  0xE000E010 : SysTick Control and Status Register
    uint32 systRvr;              //  0xE000E014 : SysTick Reload Value Register
    uint32 systCvr;              //  0xE000E018 : SysTick Current Value Register
    uint32 systCalib;            //  0xE000E01C : SysTick Calibration Value Register
} SystemTimerRegs_t;

#define  stSystemTimerRegs (*( volatile SystemTimerRegs_t*)eADDRESS_SYSTEM_TIMER_BASE)


/**
 * @brief Initialize the timer
 * @details Cache the system timer calibration data.
 */
void SystemTimer_Initialize(void);


/**
 * @brief Get the Skew flag
 * @details When set means an inexact 10ms calib value can affect the suitability of SysTick as a software real time clock.
 */
boolean SystemTimer_GetSkew(void);


/**
 * @brief Get the No Reference flag
 * @returns True Indicates no reference clock provided to the processor:
 */
boolean SystemTimer_GetNoRef(void);


/**
 * @brief Get the ten millisecon tick
 * @returns Returns the number of tickts in 10 milliseconds.
 */
uint32 SystemTimer_GetTenMs( void);


/**
 * @brief Get the clock source is processor flag
 * @details Indicates the processor is the clock source otherwise its an external clock source.
 */
boolean SystemTimer_GetIsClockSourceProcessor(void);


/**
 * @brief Enable the timer
 * @details Enable the system timer to run.
 */
void SystemTimer_Enable(void);


/**
 * @brief Disable the timer
 * @details Disable the system timer.
 */
void SystemTimer_Disable(void);


/**
 * @brief Get the count flag.
 * @returns True Returns 1 if timer counted to 0 since last time this was read.
 */
boolean SystemTimer_GetCountFlag( void);

  
/**
 * @brief Set the reload value
 * @details  Sets the 24 bit reload value used to initialise the counter.
 * The system counter counts down from the value set by this method to zero.
 * This value is then loaded into the counter and decrementing begins again.
 * @param[in] The 24 bit counter value;
 */
void SystemTimer_SetReload( uint32 const ku32Count );

/**
 * @brief Gets the reload value
 * @returns uint32 The 24 bit reload value of the system timer.
 */
__attribute__( ( always_inline ) ) static inline uint32 SystemTimer_GetReloadValueFast(void)
{
    // Read the reload current value
    return stSystemTimerRegs.systRvr & SVR_RELOAD_MASK;
}


/**
 * @brief Gets the current value
 * @returns uint32 The 24 bit value of the system timer.
 */
__attribute__( ( always_inline ) ) static inline uint32 SystemTimer_GetValueFast(void)
{
    // Read the current value
    return stSystemTimerRegs.systCvr & CVR_CURRENT_MASK;
}


/**
 * @brief Set the IRQ enable
 * @param[in] True to enable the IRQ else false
 */
void SystemTimer_SetIrqEnable( boolean const kbEnable );


/**
 * @brief Gets if an IRQ handler has been registered
 * @returns bool True if a handler has been registered else false
 */
boolean SystemTimer_HasIrqHandler(void);


/**
 * @brief    Set system tick IRQ handler
 * @param[in]  pfnIrqHandler The system tick IRQ handler function pointer
 * @param[in]  uPrData The handler context data
 * @returns    NA
 */
void SystemTimer_SetSysTickIrqHandler( void (pfnIrqHandler)(void* pContext), uintptr uPrData );


/**
 * @brief    The system tick handler
 * @returns    None
 */
void SystemTimer_SysTickIrqHandler(void);

#endif // HEADER_SYSTEM_TIMER_H
