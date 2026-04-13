/**
 * @file         driver_cau.c
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
 

#include "driver_cau.h"
#include "foc_control_api.h"

/**
 * @brief CAU module initialise function
*/ 
void Cau_Initialization()
{
		Cau_SetEnable( 0U );// Disable the CAU module
		NVIC_DisableIRQ( eIRQNUM_CAU );// Disable the CAU IRQ
		NVIC_ClearPendingIRQ( eIRQNUM_CAU );// Disable the CAU pending IRQ
		Cau_SetAdcConfigCalibEnable ( eCAU_A, eCAU_ADCCFGCAL_DISABLE ); // Disable ADC_A automatic calibration mode
		Cau_SetAdcConfigCalibEnable ( eCAU_B, eCAU_ADCCFGCAL_DISABLE ); // Disable ADC_A automatic calibration mode
		Cau_SetAdcConfigConvSpeed ( eCAU_A, eCAU_CONVSPEED_1P0US); // Set ADC_A conversion speed to 1us
		Cau_SetAdcConfigConvSpeed ( eCAU_B, eCAU_CONVSPEED_1P0US); // Set ADC_B conversion speed to 1us
		Cau_SetAdcConfigOverwritePermission ( eCAU_A, eCAU_ADCCFG_PERMITTED ); // Permit ADC_A conversion overwrite
		Cau_SetAdcConfigOverwritePermission ( eCAU_B, eCAU_ADCCFG_PERMITTED ); // Permit ADC_B conversion overwrite
		Cau_SetTrigSahPeriod ( eCAU_TRIGSEL_A, eCAU_TRIGSAHPERIOD_0P0US ); // Set ADC_A S&H time to 0us
		Cau_SetTrigSahPeriod ( eCAU_TRIGSEL_B, eCAU_TRIGSAHPERIOD_0P0US ); // Set ADC_B S&H time to 0us
		Cau_SetTrigSahTrig ( eCAU_TRIGSEL_A, eCAU_TRIGSAHTRIG_PWM ); // Set ADC_A normal trigger source as ADCxNT from PGU
		Cau_SetTrigSahTrig ( eCAU_TRIGSEL_B, eCAU_TRIGSAHTRIG_PWM ); // Set ADC_B normal trigger source as ADCxNT from PGU
		Cau_SetTrigSahTrigEdge ( eCAU_TRIGSEL_A, eCAU_TRIGSAHTRIGEDGE_RISING ); // Set ADC_A normal trigger source edge selection as rising edge
		Cau_SetTrigSahTrigEdge ( eCAU_TRIGSEL_B, eCAU_TRIGSAHTRIGEDGE_RISING ); // Set ADC_B normal trigger source edge selection as rising edge
		Cau_SetSwTrig ( eCAU_SWTRIG_DISABLED ); //All  software triggers disabled
		Cau_SetSwTrigCal ( eCAU_SWTRIGCAL_DISABLED ); // All software calibration triggers disabled
		Cau_SetIir ( eCAU_IIRSELECT_A, eCAU_IIR_DISABLE ); // Disbale ADC_A IIR filter
		Cau_SetIir ( eCAU_IIRSELECT_B, eCAU_IIR_DISABLE ); // Disbale ADC_B IIR filter
		Cau_SetAdcConfigAdcEnable ( eCAU_A, eCAU_ADCCFGADC_ENABLE ); // Enable ADC_A channel
		Cau_SetAdcConfigAdcEnable ( eCAU_B, eCAU_ADCCFGADC_ENABLE ); // Enable ADC_B channel
		Cau_SetEnable( 1U ); // Enable the CAU module
		uint16 u16PollingTimeOut = 500U;
		while ( Cau_GetPwrUpStatus()!= eCAU_PWRUPSTATUS_ALL_PWRED && u16PollingTimeOut){u16PollingTimeOut--;} // Polling wait all ADC channels powered up, ~200 cycles
		Cau_SetConvIrqEnableFast ( eCAU_CONVIRQENABLE_CONVERSION_COMPLETE_IRQ_B ); // Enable 
		Cau_SetConvStatusFast( eCAU_CONVSTATUS_ANY );
		Cau_SetIrqHandler ( Foc_Cau_Handler, 0 );	// Interrupt Handler Pointer
		NVIC_EnableIRQ( eIRQNUM_CAU );
}