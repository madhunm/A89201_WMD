/**
 * @file         driver_timer.c
 * @author       B. Shao
 * @version      1.0
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

#include "driver_timer.h"
#include "foc_control_api.h"

void Timer_Initialization(void)
{
		NVIC_DisableIRQ( eIRQNUM_TIMER2 );
		NVIC_ClearPendingIRQ( eIRQNUM_TIMER2 );
		const uint16 ku16OneMilliSecond = 20000U;
	  /* Timer1 is used to generate periodic trigger to DAU channel to measure source signals */
    /* Timer1 Register Settings */
    Gtu_SetControlEnable(eTIMER1,eTIMER_CONTROL_TIMER_EN,False);
    /* Trigger Generation Mode for DAU unit */
		Gtu_SetControlTMode(eTIMER1,eTIMER_CONTROL_TMODE_FDCTG);
    Gtu_SetControlTClk(eTIMER1,eTIMER_CONTROL_TCLK_PRESCALE_2);
    /* Dau Sampling Period */
    Gtu_SetThreshold(eTIMER1,ku16OneMilliSecond);
    Gtu_SetControlEnable(eTIMER1,eTIMER_CONTROL_TIMER_EN,True);
	
	  /* Timer 2 Register Settings  for Speed Loop Control Interrupt Generation */
		/* Create Timer One Object used as interrupt for closed loop speed */
		const uint16 ku16TwoFiftyMicroSec = 5000U;
		const uint8 ku8TriggerGenerationMode = 7U;
		Gtu_SetControlEnable(eTIMER2,eTIMER_CONTROL_TIMER_EN,False);
		Gtu_SetControlEnable(eTIMER2,eTIMER_CONTROL_IEN_DVL,True);
		Gtu_SetControlTMode(eTIMER2,eTIMER_CONTROL_TMODE_FDCTG);
		Gtu_SetControlTClk(eTIMER2,eTIMER_CONTROL_TCLK_PRESCALE_2);
		/* Speed Loop Control Period */
		Gtu_SetThreshold(eTIMER2, ku16TwoFiftyMicroSec );
		Timer2_SetIrqHandler(Speed_Loop_Gtu_Handler);
		Gtu_SetControlEnable(eTIMER2,eTIMER_CONTROL_TIMER_EN,True);
		NVIC_EnableIRQ( eIRQNUM_TIMER2 );
		
		/*Timer 5,6 and 7 are initialised in comm_pwm.c for PWM command input using*/
}
