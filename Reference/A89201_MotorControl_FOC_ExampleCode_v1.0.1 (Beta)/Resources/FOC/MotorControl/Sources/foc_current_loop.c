/**
 * @file         current_loop.c
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
 
 #include "foc_current_loop.h"

/**
 * @brief Initialize a Q15 current PI controller to safe zeros.
 *
 * Sets all fields to 0. Application must set Kp/Ki and output limits
 * (OutMin/OutMax) later before enabling the current loop.
 *
 * @param pi Pointer to CURRENT_PI_Q15 instance.
 */
 
void Current_PI_Init(CURRENT_PI *pi)
{
    pi->Current_ref_q15   = 0;
    pi->Current_fd_q15    = 0;
    pi->Current_Kp_q15    = 0;
    pi->Current_Ki_q15    = 0;
    pi->OutMax_q15        = 0;
    pi->OutMin_q15        = 0;
    pi->Current_I_term_q31= 0;
    pi->Voltage_out_q15   = 0;
}

/**
 * @brief Parallel PI controller (standard form).
 *
 * Formula:  y = Kp * err + Ki * Integral(err)
 *
 * - Integral term is clamped to prevent windup.
 * - Output is saturated between OutMin and OutMax.
 *
 * @param pi   Pointer to PI controller structure
 */
__attribute__((always_inline)) 
	void Current_PI_Parallel_Q15( CURRENT_PI *pi )
{
    // Error
		sint16 err_q15 = pi->Current_ref_q15 - pi->Current_fd_q15;
		
		// Proportional term: Q15×Q15 = Q30 << 1 = Q31 
		sint32 P_q31 = ((sint32)pi->Current_Kp_q15 * err_q15) << 1U;

		// Integral term accumulation: Q15×Q15 = Q30 << 1 = Q31 
		pi->Current_I_term_q31 +=((sint32)pi->Current_Ki_q15 * err_q15) << 1U;

    // Integral clamping	
		if ( pi->Current_I_term_q31 > ((sint32)pi->OutMax_q15 << Q31_FROM_Q15_SHIFT) ) 
			pi->Current_I_term_q31 = (sint32)pi->OutMax_q15 << Q31_FROM_Q15_SHIFT;
		if (pi->Current_I_term_q31 < ((sint32)pi->OutMin_q15 << Q31_FROM_Q15_SHIFT)) 
			pi->Current_I_term_q31 = (sint32)pi->OutMin_q15 << Q31_FROM_Q15_SHIFT;
	
    // Output calculation (P+I, Q31 -> Q15)
    sint32 out_q15 = (P_q31 + pi->Current_I_term_q31) >> Q31_FROM_Q15_SHIFT;

    // Output saturation
		if ( out_q15 > (sint32)pi->OutMax_q15 ) out_q15 = (sint32)pi->OutMax_q15; 
		if ( out_q15 < (sint32)pi->OutMin_q15 ) out_q15 = (sint32)pi->OutMin_q15;
		
		pi->Voltage_out_q15 = (sint16)out_q15;
}


/**
 * @brief Reset the internal state of PI controller.
 *
 * This function clears the accumulated integral term and output voltage 
 * of the PI controller. It is typically used during control state transitions 
 * (e.g., from alignment to ramp-up, or open-loop to closed-loop) or during fault 
 * recovery to prevent residual integral windup from influencing future control cycles.
 *
 * @param[in,out] pi Pointer to the CURRENT_PI_Q15 structure representing the PI controller.
 *
 * @note This function does not reset the reference or feedback inputs.
 *       If required, those should be cleared separately by the application.
 */

void Current_PI_Reset(CURRENT_PI *pi)
{
    pi->Current_I_term_q31 = 0;
    pi->Voltage_out_q15 = 0;
}

 