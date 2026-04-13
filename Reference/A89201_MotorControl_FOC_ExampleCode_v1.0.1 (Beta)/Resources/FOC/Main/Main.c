/**
 * @file         Main.c
 * @author       B. Shao
 * @version      1.0
 * @brief        Main function
 *
 * @copyright (c) 2025 Allegro MicroSystems, Inc. All Rights Reserved.
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

#include "SystemControl.h"
#include "driver_cau.h"
#include "driver_dau.h"
#include "driver_gdu.h"
#include "driver_gpio.h"
#include "driver_pwm.h"
#include "driver_timer.h"
#include "comm_pwm_app.h"
#include "foc_control_api.h"
#include "UserParameters.h"


/**
 * @brief Generate FG (frequency generator) signal from motor electrical angle.
 *
 * The GPIO output toggles once per electrical revolution.
 * When the electrical angle is positive (bit15 = 0), the pin is cleared LOW;
 * when negative (bit15 = 1, i.e. angle < 0), the pin is set HIGH.
 * Effectively, this produces one FG pulse per electrical cycle.
 *
 * @param u16MotorAngle  Electrical angle in signed Q15 format (-32768 to +32767),
 *                       corresponding to -pi to +pi radians (or -180° to +180°).
 */
static inline void GenerateFg(const uint16 u16MotorAngle){
		if( u16MotorAngle >> 15 ) Gpio_Set(eGPIO_INSTANCE_ID2);
		else Gpio_Clear(eGPIO_INSTANCE_ID2);
}

/* ---------------------------------------------------------------------------
 * Fault management variables
 * --------------------------------------------------------------------------- */

/* Holds the current GDU fault status bits (masked and exported subset). */
static volatile uint16 su16FaultStatus;

/* Indicates a pending fault-clear request (set after a fault is detected). */
static volatile boolean m_FaultClearRequestFlag = False;

/* Indicates that fault clear should be executed (request + PWM input low). */
static volatile boolean FaultClearRequested = False;

/* ---------------------------------------------------------------------------
 * Main Function
 * --------------------------------------------------------------------------- */
__asm(".global __ARM_use_no_argv\n\t");
int main(int argc, char **argv)
{
		/* Disable Watchdog */
		Scu_WatchdogDisable();
	
		/* Enable Flash Prefetch mode to speed up execution */
		Scu_SetFlashControlPreFetchEnable(True);
	
		/* Interrupt Priority Level (CAU, Timer 2 (Speed loop),Timer7 (PWM input timer) 
			 and DAU units (VBB measurement) below should be highest priority level and should not be touched) */
		NVIC_SetPriority( eIRQNUM_CAU, 0 );
		NVIC_SetPriority( eIRQNUM_TIMER2,1 );
		NVIC_SetPriority( eIRQNUM_TIMER7,2);
		NVIC_SetPriority( eIRQNUM_DAU,3);
	
		/* Motor Control Parameters Initialization */
		Foc_Init();	
	
		/* Motor Control API Parameters Initialization */
		UserParameters_Init();
	
		/* PWM Input Parameter Initialization */
		Comm_PWM_Config();
	
		/* PWM Input Hardware Configuration Initialization */
		Comm_PwmIn_Initialise(eGPIO_INSTANCE_ID4, eTIMER_CONTROL_SRC_GPI4);
	
		/* Peripherals Initialization */ 
		Cau_Initialization();
		Dau_Initialization();
		Gdu_Initialization();
		GPIO_Initialization();
		PwmGen_Initialization();
		Timer_Initialization();
		
		while (1) 
		{
				/* Fault detection and handling */
				FaultClearRequested = m_FaultClearRequestFlag && (!Comm_PwmIn_GetPwmInStatus());
				
				if ( FaultClearRequested )
				{
						/* Clear GDU faults when PWM input is off */
						uint16 u16status_pre = Gdu_GetStatus();
						Gdu_ClearGduStatus();
					
						/* Initialization is needed in case of GDU POR */
						/* Only re-initialize if a real POR occurred */
						if ( u16status_pre & STAT_POR)	Gdu_Initialization(); 
						su16FaultStatus = 0;
						m_FaultClearRequestFlag = False;
				}
				else
				{
						/* Normal monitoring path: read and mirror selected GDU status bits */
						volatile uint16 u16gduStatus = Gdu_GetStatus();
						uint16 u16rawStatus = u16gduStatus;
						uint16 u16status = 0U;
					
						/* Export as a masked mirror (no arbitrary bit shifts) */
					  if ( u16rawStatus & STAT_POR )  u16status |= (1u << 0); 	/* POR */
						if ( u16rawStatus & STAT_SE )   u16status |= (1u << 1);		/* SE */
						if ( u16rawStatus & STAT_TW )   u16status |= (1u << 2);		/* TW -> TF */
						if ( u16rawStatus & STAT_BU )   u16status |= (1u << 3);		/* BU */
						if ( u16rawStatus & STAT_DSO )  u16status |= (1u << 4);		/* DSO -> VO */
						if ( u16rawStatus & STAT_VBU )  u16status |= (1u << 5);		/* VBU -> VS */
						if ( u16rawStatus & STAT_VLRU ) u16status |= (1u << 6);		/* VLRU -> RF */
						su16FaultStatus = u16status;
				}
	
				/* Motor control based on fault status */
				if( su16FaultStatus )
				{
						/* Fault detected -> stop motor, set fault output, request clear */
						MotorControl_SetRun( False );
						Gpio_Set( eGPIO_INSTANCE_ID3 );
						m_FaultClearRequestFlag = True;
				}
				else
				{
						/* No fault -> run motor only if speed command is valid */
						if (!Comm_PwmIn_GetSpeedCommand())
						{
								MotorControl_SetRun( False );
						}
						else
						{	
								MotorControl_SetRun( True );
								Gpio_Clear( eGPIO_INSTANCE_ID3 );
						}
					}
				
				/* Generate FG signal (one pulse per electrical revolution) */
				GenerateFg( MotorControl_GetThetaUsed_Q15() );
				
		}
		return 0;
}
