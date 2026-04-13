/**
 * @internal
 *
 * @file         comm_pwm.c
 * @author       B.Shao
 * @version			 1.0
 * @brief        PwmMapping - used as external speed command. Duty cycle versus command frequency
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
 * ---------------------------------------------------------------------------------------------------------------------------- 
 * This Software License Agreement (“Agreement”) is made between                                                              
 * Allegro MicroSystems, LLC (“Allegro”) and the user identified in                                                           
 * the signature block below ("Licensee") (each a "Party" and collectively, the "Parties")                                    
 * on the date of the signature below.      

 * LICENSE TERMS:
 *    a. Exclusively in conjunction with Licensee’s development and sale of products 
 *       containing the Application, Allegro grants Licensee a limited, non-exclusive, 
 *       non-transferable, revocable, and royalty-free license under the copyright rights 
 *       of Allegro or its licensor to (i) use the Software Deliverables to develop the Application,
 *       (ii) use the Application in conjunction with the Library Files, solely in a non-production 
 *       environment for the exclusive purpose of evaluating the Application.   
 * 
 *   b. Licensee recognizes that the Software (i) has not been released by Allegro for general distribution or sale,
 *      and that Allegro has not completed its testing and quality assurances, and (ii) is not qualified and has not 
 *      been certified to meet any industry standards. Licensee shall not use the Software provided hereunder in 
 *      any production environment. Licensee will not disclose or provide the Software or Application to any 
 *      third party, and will defend, indemnify and hold Allegro harmless from all  damages, costs, liabilities 
 *      and expenses related to any third party claims arising from Licensee's breach of this Agreement. 
 *      Further, Licensee will not redistribute the Software or Application to any party, 
 *      including individuals within Licensee’s organization.
 *
 *   c.  In no event will Licensee use the Software in conjunction with any devices (including competitor devices) 
 *       other than the Allegro device identified in the Allegro documentation accompanying the Software or notified
 *       to Licensee by Allegro in writing.
 *
 *   d. Licensee acknowledges that the Software and all related technical information, documents and materials
 *      are subject to export controls under the U.S. Export Administration Regulations.  Licensee will (i) comply 
 *      strictly with all legal requirements established under these controls, (ii) cooperate fully with Allegro in 
 *      any official or unofficial audit or inspection that relates to these controls and (iii) not export, re-export,
 *      divert or transfer, directly or indirectly, any such item or direct products thereof to Crimea, Iran, North Korea,
 *      Sudan, Syria or any country that is embargoed by Executive order, unless Licensee has obtained the prior written 
 *      authorization of Allegro and the U.S. Commerce Department.  
 *
 * CONFIDENTIAL TERMS:
 *      a. Licensee acknowledges and agrees that all information regarding the Software, the results of testing and 
 *         evaluation hereunder, the Feedback provided hereunder, the characteristics and performance of the Software 
 *         and Application (including all related documentation) and this Agreement (collectively, the “Confidential Information”) 
 *         are all confidential information and contain trade secrets of Allegro.  Licensee acknowledges that the Agreement will 
 *         induce Allegro to make such Confidential Information available to Licensee.
 *
 *     This distribution is designated as EAR99.  
 * ---------------------------------------------------------------------------------------------------------------------------- 
 *
 */



#include "Platform_Types.h"
#include "comm_pwm.h"
#include "foc_control_api.h"

/** 
 * @brief Timeout threshold for low-frequency PWM input (˜4.9 Hz).
 * @details 
 * Defines the maximum allowed period count before PWM input is considered lost.
 */
const uint16 kTimeOutTHreshold_4P9Hz = 63750; 

/** 
 * @brief Timeout threshold for high-frequency PWM input (˜196 Hz).
 * @details 
 * Used when PWM operates in the high-frequency range (200 Hz–1 kHz mode).
 */
const uint16 kTimeOutTHreshold_196Hz = 51000;

/** 
 * @brief Maximum duty-cycle value corresponding to 100%.
 * @details 
 * PWM duty values are normalized in 0–1023 range.
 */
const uint16 ku16MaxDuty = 1023;// 100% Duty

/** 
 * @brief Active GPIO instance used for PWM input signal.
 * @details 
 * Default assignment is eGPIO_INSTANCE_ID0, can be reconfigured if needed.
 */
static volatile enum GpioInstanceId g_GpioInstance = eGPIO_INSTANCE_ID0;

/** 
 * @brief Runtime data of the PWM input process.
 * @details 
 * Holds latest measured period, high time, duty, and mapped speed command.
 */
volatile comm_pwm_process pwmIn_process;

/** 
 * @brief Configuration parameters for the PWM input interface.
 * @details 
 * Stores polarity, frequency range, and mapping constants.
 */
static volatile comm_pwm_config pwmIn_config;

/**
 * @brief Get pointer to the PWM input configuration structure.
 * @details 
 * Allows external modules to access or modify PWM configuration parameters.
 * @param[in] None
 * @returns Pointer to the global comm_pwm_config instance.
 */
volatile comm_pwm_config* Comm_PwmIn_GetConfig (void) { return &pwmIn_config; };

/**
 * @details     This function is to read the results from Timer 5 and 6 to get the high-level time measurement and period measurement
 * @param[in]   None
 * @returns 		None
 */
static inline void Comm_PwmIn_RegisterRead(void)
{
    if (Gtu_GetStatus(eTIMER6,eTIMER_STATUS_DVL))
    {
       pwmIn_process.u16HighPulse =	Gtu_GetResult(eTIMER5);
       pwmIn_process.u16Period 		=	Gtu_GetResult(eTIMER6);
    }
		else
    {
       pwmIn_process.u16Period    = ku16MaxDuty;      
       pwmIn_process.u16HighPulse = Gpio_Read(g_GpioInstance) ?  ku16MaxDuty : 0;
    }
}

/**
 * @details     This function is to calculate the external PWM signal duty 
 * @param[in]   None
 * @returns 		None
 */
static inline void Comm_PwmIn_DutyCalculate(void)
{
		 pwmIn_process.u16Duty = (sint16)( (sint32)pwmIn_process.u16HighPulse * ku16MaxDuty  / pwmIn_process.u16Period );
		 if(!pwmIn_config.bPwmSense)	pwmIn_process.u16Duty = pwmIn_process.u16Duty;
		 else													pwmIn_process.u16Duty = ku16MaxDuty - pwmIn_process.u16Duty;
}

/**
 * @details     This function is to decode the speed command from the user-defined duty-speed profile based on duty calculation
 * @param[in]   None
 * @returns None
 */
static inline void Comm_PwmIn_Decode(void)
{
    sint32 s32SpeedTemp = 0U;
    if  ( pwmIn_process.u16Duty <= pwmIn_config.au16Duty[(uint16)( DminStart )] ) 
    {
        pwmIn_process.m_u16SpeedDemand = 0;
        pwmIn_process.m_bPwmMappingRun = False;
    }
    else if (  pwmIn_process.u16Duty < pwmIn_config.au16Duty[(uint16)( Dmax )]  )
    {
        s32SpeedTemp = pwmIn_config.au16Freq[(uint16)(Fmax)] - pwmIn_config.au16Freq[(uint16)(Fmin)];
        s32SpeedTemp = s32SpeedTemp * ( pwmIn_process.u16Duty - pwmIn_config.au16Duty[(uint16)(DminStart)]);
        s32SpeedTemp = s32SpeedTemp / ( pwmIn_config.au16Duty[(uint16)(Dmax)] - pwmIn_config.au16Duty[(uint16)(DminStart)] );
        s32SpeedTemp = s32SpeedTemp  + (sint32) ( pwmIn_config.au16Freq[(uint16)(Fmin)] );
        pwmIn_process.m_u16SpeedDemand = (sint16)( s32SpeedTemp );
        pwmIn_process.m_bPwmMappingRun = True;
    }	
    else if (  pwmIn_process.u16Duty < pwmIn_config.au16Duty[(uint16)(DmaxStop)]  )
    {
				pwmIn_process.m_u16SpeedDemand = pwmIn_config.au16Freq[(uint16)(Fmax)];
				pwmIn_process.m_bPwmMappingRun = True;			
    }
		else
		{
        pwmIn_process.m_u16SpeedDemand = 0;
        pwmIn_process.m_bPwmMappingRun = False;		
		}
}

/**
 * @details     This function is to update the timer register for next measurement
 * @param[in]   None
 * @returns None
 */
static inline void Comm_PwmIn_RegisterUpdate(void)
{
    // 200 - 1000 Hz PWM Input Range
    if ( pwmIn_config.bFrqRange )
    {
			Gtu_SetControlTClk(eTIMER5,eTIMER_CONTROL_TCLK_PRESCALE_4);
			Gtu_SetControlTClk(eTIMER6,eTIMER_CONTROL_TCLK_PRESCALE_4);
			Gtu_SetControlTClk(eTIMER7,eTIMER_CONTROL_TCLK_PRESCALE_4);
			Gtu_SetThreshold(eTIMER7,kTimeOutTHreshold_196Hz);
    }
    // 5-200 Hz PWM Input Range
    else
    {
			Gtu_SetControlTClk(eTIMER5,eTIMER_CONTROL_TCLK_PRESCALE_128);
			Gtu_SetControlTClk(eTIMER6,eTIMER_CONTROL_TCLK_PRESCALE_128);
			Gtu_SetControlTClk(eTIMER7,eTIMER_CONTROL_TCLK_PRESCALE_128);
			Gtu_SetThreshold(eTIMER7,kTimeOutTHreshold_4P9Hz);
    }
		
    if (Gtu_GetStatus (eTIMER6,eTIMER_STATUS_DVL))	
			Gtu_ClearStatus( eTIMER6,eTIMER_STATUS_DVL );  
		else	{;}
}

/**
 * @details     This function is to process the external PWM duty/ Speed command conversion
 * @details     This function is also the Timer 7 OVL interrupt handler
 * @param[in]   None
 * @returns None
 */

__attribute__((always_inline)) void Comm_PwnIn_Run(void)
{
	  Comm_PwmIn_RegisterRead();
    Comm_PwmIn_DutyCalculate();
    Comm_PwmIn_Decode();
    Comm_PwmIn_RegisterUpdate();
		MotorControl_SetSpdCmd_Q15( pwmIn_process.m_u16SpeedDemand );
		Gtu_ClearStatus( eTIMER7,eTIMER_STATUS_DVL );
}

/**
 * @details     Initialise settings for PWM mapping functions
 * @remarks     
 * @param[in]		None
 * @returns 		None
 */

void Comm_PwmIn_Initialise( enum GpioInstanceId gpioId, enum TimerControlSource timerSource)
{
		g_GpioInstance = gpioId;
		/* Configure GPIO for PWM input terminal */	
		Gpio_SetPinActive(gpioId);
		Gpio_SetMode(gpioId,eGPIO_MODE_DIGITAL_INPUT);
		
		pwmIn_config.bPwmSense = False;
		pwmIn_config.bFrqRange = True;
		
		NVIC_DisableIRQ( eIRQNUM_TIMER7 );
		
		Gtu_SetControlEnable(eTIMER5,eTIMER_CONTROL_TIMER_EN,False);
		Gtu_SetControlEnable(eTIMER6,eTIMER_CONTROL_TIMER_EN,False);
		Gtu_SetControlEnable(eTIMER7,eTIMER_CONTROL_TIMER_EN,False);
		// PWM Input Connected to GPIO4
		Gtu_SetControlSource(eTIMER5,timerSource);
		Gtu_SetControlSource(eTIMER6,timerSource);
		Gtu_SetControlSource(eTIMER7,timerSource);

		//PWM High Pulse Measurement
		Gtu_SetControlTMode(eTIMER5,eTIMER_CONTROL_TMODE_HI_TIME);
		//PWM Period Measurement
		Gtu_SetControlTMode(eTIMER6,eTIMER_CONTROL_TMODE_PERIOD_RISING_TO_RISING);
		//PWM Timeout Measurement
		Gtu_SetControlTMode(eTIMER7,eTIMER_CONTROL_TMODE_FDCTG);

		// High Resolution PWM Input (Min - 200 Hz)
    if ( pwmIn_config.bFrqRange )
    {
			Gtu_SetControlTClk(eTIMER5,eTIMER_CONTROL_TCLK_PRESCALE_4);
			Gtu_SetControlTClk(eTIMER6,eTIMER_CONTROL_TCLK_PRESCALE_4);
			Gtu_SetControlTClk(eTIMER7,eTIMER_CONTROL_TCLK_PRESCALE_4);
			Gtu_SetThreshold(eTIMER7,kTimeOutTHreshold_196Hz);
    }
    // Low Resolution PWM Input (Min - 5 Hz)
    else
    {
			Gtu_SetControlTClk(eTIMER5,eTIMER_CONTROL_TCLK_PRESCALE_128);
			Gtu_SetControlTClk(eTIMER6,eTIMER_CONTROL_TCLK_PRESCALE_128);
			Gtu_SetControlTClk(eTIMER7,eTIMER_CONTROL_TCLK_PRESCALE_128);
			Gtu_SetThreshold(eTIMER7,kTimeOutTHreshold_4P9Hz);
    }
		

		Gtu_SetControlEnable(eTIMER5,eTIMER_CONTROL_TIMER_EN,True);
		Gtu_SetControlEnable(eTIMER6,eTIMER_CONTROL_TIMER_EN,True);
	
		Gtu_SetControlEnable(eTIMER7,eTIMER_CONTROL_IEN_DVL,True);
		Timer7_SetIrqHandler ( Comm_PwnIn_Run );
		Gtu_SetControlEnable(eTIMER7,eTIMER_CONTROL_TIMER_EN,True);
		NVIC_EnableIRQ( eIRQNUM_TIMER7 );		
}

/**
 * @brief Get the current speed command derived from PWM input.
 * @details 
 * Returns the latest mapped speed demand value calculated
 * from the captured PWM duty and frequency.
 * @param[in]  None
 * @returns    uint16  Current speed demand value
 */
__attribute__((always_inline)) uint16 Comm_PwmIn_GetSpeedCommand(void)
{
	return pwmIn_process.m_u16SpeedDemand;
}

/**
 * @brief Get the current PWM input activity status.
 * @details 
 * Indicates whether valid PWM input is being detected and mapped.
 * True means PWM mapping is active; False means timeout or inactive.
 * @param[in]  None
 * @returns    boolean  PWM input active flag (True/False)
 */
__attribute__((always_inline)) boolean Comm_PwmIn_GetPwmInStatus(void)
{
	return pwmIn_process.m_bPwmMappingRun;
}