#ifndef HEADER_COMM_PWM_H
#define HEADER_COMM_PWM_H

/**
 * @internal
 *
 * @file         comm_pwm.c
 * @author       B.Shao
 * @version			 1.0
 * @copyright (c) 2020-2023 Allegro MicroSystems, Inc. All Rights Reserved.
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
 * @details   PwmMapping implementation. This code uses Timers 5,6 and 7. 
 */

#include "Platform_Types.h"
#include "Gtu.h"
#include "Gpio.h"

/**
 * @brief Enumeration of PWM duty mapping indices.
 * @details 
 * Defines reference points used in duty-to-speed mapping.
 * @remarks 
 * - DminStart : Minimum duty value at which motor start is allowed  
 * - Dmax      : Maximum duty value corresponding to full command  
 * - DmaxStop  : Duty threshold above which command is considered invalid (stop)
 */
enum Mapduty
{
    DminStart,
    Dmax,
		DmaxStop
};

/**
 * @brief Enumeration of PWM frequency mapping indices.
 * @details 
 * Defines the valid frequency range of the external PWM input.
 * @remarks 
 * - Fmin : Minimum detectable frequency  
 * - Fmax : Maximum detectable frequency
 */
enum MapFrequency
{
    Fmin,
    Fmax
};

/**
 * @brief PWM communication configuration parameters.
 * @details 
 * Holds configuration constants for decoding external PWM command signals.
 * @remarks 
 * - bPwmSense  : Input polarity (1 = Active Low, 0 = Active High)  
 * - bFrqRange  : Frequency range selector (0 = 5–200 Hz, 1 = 200–1 kHz)  
 * - au16Duty[] : Duty cycle mapping table (refer to Mapduty)  
 * - au16Freq[] : Frequency mapping table (refer to MapFrequency)
 */
typedef struct{	
    boolean bPwmSense;
    boolean bFrqRange;
    uint16 au16Duty[5];
    uint16 au16Freq[2];
}comm_pwm_config;

/**
 * @brief PWM signal processing data structure.
 * @details 
 * Stores runtime variables used during PWM capture and speed command calculation.
 * @remarks 
 * - u16HighPulse       : Captured high time of PWM signal  
 * - u16Period          : Captured total period of PWM signal  
 * - u16Duty            : Calculated duty (in same scale as ADC resolution)  
 * - m_u16SpeedDemand   : Computed speed demand from duty/frequency mapping  
 * - m_bPwmMappingRun   : Flag indicating if PWM mapping is currently active
 */
typedef struct{
		uint16 u16HighPulse;
		uint16 u16Period;
		uint16 u16Duty;
		uint16 m_u16SpeedDemand;
		boolean m_bPwmMappingRun;
}comm_pwm_process;

/**
 * @details     Configure the PWM input interface and related hardware resources.
 *              This function initializes the timer, capture channels, and GPIOs
 *              used to decode external PWM control signals.
 * @remarks     Must be called once during system initialization before reading
 *              any PWM-based commands or statuses.
 * @param[in]   None
 * @returns     None
 */
void Comm_PwmIn_Initialise( enum GpioInstanceId gpioId, enum TimerControlSource timerSource);

/**
 * @details     Return the speed command based on external PWM signa duty
 * @remarks     
 * @param[in] 	None
 * @returns 		unit16 m_u16SpeedDemand
 */
__attribute__((always_inline)) uint16 Comm_PwmIn_GetSpeedCommand(void);

/**
 * @details     PwmMapping settings setup by the user   
 */
volatile comm_pwm_config* Comm_PwmIn_GetConfig (void);


/**
 * @details     Return the external PWM signal valid flag
 * @remarks     
 * @param[in] 	None
 * @returns 		False: 0 speed command 0, True: non-0 speed command
 */
__attribute__((always_inline)) boolean Comm_PwmIn_GetPwmInStatus(void); 

#endif //HEADER_COMM_PWM_H