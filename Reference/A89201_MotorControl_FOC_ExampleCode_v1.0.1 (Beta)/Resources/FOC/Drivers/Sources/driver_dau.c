/**
 * @file         driver_dau.c
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
 
#include "driver_dau.h"
#include "foc_control_api.h"

/**
 * @brief DAU module initialise function
*/
void Dau_Initialization()
{
		NVIC_DisableIRQ( eIRQNUM_DAU );
		NVIC_ClearPendingIRQ( eIRQNUM_DAU );
		Dau_SetControlMode( eSINGLE_SRC ); // Set Dau control mode as single source mode
		Dau_SetControlTrigEdgeSel ( eRISING ); // Set Dau trigger source edge as rising edge
		Dau_SetControlTrigSel ( eGTU0 ); // Set Dau trigger source as Timer 1, i.e., timer 0 in MCAL
		Dau_SetControlCnvSpeed ( e1P0US ); // Set Dau conversion speed as 1us
		Dau_SetControlExtRef ( eINT ); // Set Dau reference voltage as internal reference
		Dau_SetControlExtGnd ( eGND_INT ); // Set Dau reference ground as internal ground
		Dau_SetSlotConfigChannel ( eSELECT_SLOT0, eCHANNEL14 ); // Select AOUT (channel 14) as the input of slot 0 
		Dau_SetSlotConfigSlotEnable ( eSELECT_SLOT0, eSLOT_ENABLE ); // Enable slot 0
		Dau_SetSlotConfigSAHPeriod ( eSELECT_SLOT0, e1US_SAH_PERIOD ); // Set slot 0 S&H time to 1us
		Dau_SetConvCompIrqEnable ( eSLOT0_CONV_COMP_IRQ_EN ); // Enable conversion complete interrupt of slot 0
		Dau_SetIrqHandler ( VbbComp_Dau_Handler ); // Interrupt handler pointer
		Dau_SetControlDauEnable ( eENABLE_DAU ); // Enable DAU
		NVIC_EnableIRQ( eIRQNUM_DAU );
}