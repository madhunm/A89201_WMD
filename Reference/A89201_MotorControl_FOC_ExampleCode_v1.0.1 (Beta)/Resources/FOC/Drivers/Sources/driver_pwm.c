/**
 * @file         driver_pwm.c
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
 

#include "driver_pwm.h"


/**
 * @brief PGU module initialise function
*/
void PwmGen_Initialization()
{
		PwmGen_SetPeriod( 0U );
		PwmGen_SetOutputSelection( ePWM_OUTPUT_SELECTION_ENABLE_ALL_OUTSEL_XX );
		PwmGen_SetOutputLogicLevel( 0 );
		PwmGen_SetDeadTime( 40 );
		PwmGen_SetControlRegister1Phase( ePWM_CONTROL_REGISTER_PHASE_ABC_INV  );
		PwmGen_SetControlRegister1Alignment( ePWM_CONTROL_REGISTER_ALIGNMENT_CENTRE_ALIGNED );
		PwmGen_SetControlRegister2AdcNormalTrig( ePWM_CONTROL_REGISTER_ADC_NORMAL_TRIG_AB_ENABLE  );
		PwmGen_SetControlRegister2AdcCalibTrig ( ePWM_CONTROL_REGISTER_ADC_CALIB_TRIGGER_NONE  );
		PwmGen_SetAdcTriggerRefAFast(  0 );
		PwmGen_SetAdcTriggerRefBFast ( 200 );
		PwmGen_SetPhaseDutyAFast ( 0 );
		PwmGen_SetPhaseDutyBFast ( 0 );
		PwmGen_SetPhaseDutyCFast ( 0 );
		PwmGen_SetPeriodFast( 4000 );
}

/**
 * @brief Disable all PWM outputs
*/
__attribute__((always_inline)) void DisablePWMOutputs()
{
		PwmGen_SetOutputSelection( ePWM_OUTPUT_SELECTION_NONE );
}

/**
 * @brief Update PWM Gen resisters based on Motor Control struct and Api struct
*/
__attribute__((always_inline)) void UpdatePWMRegisters( volatile MotorCtrlApiPwm* MotApiPwm, const volatile MotorCtrlApiConfig* MotApiConfig)
{
		/* Pwm Register Update */
		PwmGen_SetOutputSelection( ePWM_OUTPUT_SELECTION_ENABLE_ALL_OUTSEL_XX );
		PwmGen_SetDeadTime( (uint16)MotApiConfig->pwmDeadtime );
		PwmGen_SetAdcTriggerRefAFast( (uint16)MotApiPwm->ADCTrig1 );
		PwmGen_SetAdcTriggerRefBFast( (uint16)MotApiPwm->ADCTrig2 );
		PwmGen_SetPhaseDutyAFast ( (uint32)MotApiPwm->PWMA_Duty1 + ((uint32)MotApiPwm->PWMA_Duty2 << 16));
		PwmGen_SetPhaseDutyBFast ( (uint32)MotApiPwm->PWMB_Duty1 + ((uint32)MotApiPwm->PWMB_Duty2 << 16) );
		PwmGen_SetPhaseDutyCFast ( (uint32)MotApiPwm->PWMC_Duty1 + ((uint32)MotApiPwm->PWMC_Duty2 << 16) );
		PwmGen_SetPeriodFast( (uint16)MotApiConfig->pwmPeriod );
}