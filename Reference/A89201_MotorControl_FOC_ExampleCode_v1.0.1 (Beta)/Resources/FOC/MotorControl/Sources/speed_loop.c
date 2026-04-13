/**
 * @file         speed_loop.c
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
 
 #include "speed_loop.h"
 
/**
 * @brief Initialize speed loop PI controller and ramp components with default (zero) values.
 *
 * This version initializes all gains (Kp, Ki) and limits to 0.
 * Use it during startup or when parameters will be configured later.
 *
 * @param[out] pi   Pointer to SPEED_LOOP_PI_Q15 structure to initialize.
 * @param[out] ramp Pointer to SPEED_RAMP_UP_Q15 structure to initialize.
 */
void Speed_Loop_Init(SPEED_LOOP_PI *pi, SPEED_RAMP_UP *ramp)
{
    // Initialize PI controller (all zeros)
    pi->Spd_Kp_q15       = 0;
    pi->Spd_Ki_q15       = 0;
    pi->Spd_Ref_q15      = 0;
    pi->Spd_Fb_q15       = 0;
    pi->Spd_I_Term_q31   = 0;
    pi->Out_Min_q15  = 0;
    pi->Out_Max_q15  = 0;
    pi->Id_Ref_q15       = 0;
    pi->Iq_Ref_q15       = 0;

    // Initialize ramp generator (all zeros)
    ramp->Spd_Ref_In_q15               = 0;
    ramp->Spd_Ref_Out_q15              = 0;
    ramp->Spd_Ref_Out_q31              = 0;
    ramp->Spd_Acc_Slope_per_tick_q31   = 0;
    ramp->Spd_Deacc_Slope_per_tick_q31 = 0;
}

/**
 * @brief Speed loop PI controller (Q15-based fixed-point implementation).
 *
 * This function computes the torque-producing current reference `Iq_Ref`
 * using a standard PI controller. Both the proportional and integral terms 
 * are implemented in Q15 fixed-point format, with internal accumulation in Q31.
 *
 * Anti-windup is handled via integral term saturation. Output is limited by `Out_Max` and `Out_Min`.
 *
 * @param[in,out] SPD Pointer to SPEED_LOOP_PI_Q15 structure containing controller parameters and state.
 */
__attribute__((always_inline)) void Speed_Loop_PI_Control(SPEED_LOOP_PI *SPD)
{
    sint16 Spd_Err_q15 = SPD->Spd_Ref_q15 - SPD->Spd_Fb_q15;

    // Proportional term: Q15 * Q15  => Q31
    sint32 P_Term_q31 = (sint32)(((sint32)SPD->Spd_Kp_q15 * Spd_Err_q15 ) << 2U );

    // Integral term update: Q15 * Q15  => Q31
    SPD->Spd_I_Term_q31 += (sint32)(((sint32)SPD->Spd_Ki_q15 * Spd_Err_q15) << 1U);

    // Integral saturation (anti-windup)
		sint32 outMax_q31 = (sint32)SPD->Out_Max_q15 * (1 << Q31_FROM_Q15_SHIFT);
		sint32 outMin_q31 = (sint32)SPD->Out_Min_q15 * (1 << Q31_FROM_Q15_SHIFT);
	
    if (SPD->Spd_I_Term_q31 > outMax_q31)	 			SPD->Spd_I_Term_q31 = outMax_q31;
    else if (SPD->Spd_I_Term_q31 < outMin_q31)	SPD->Spd_I_Term_q31 = outMin_q31 ;

    // Total controller output
    sint32 Spd_Out_q31 = P_Term_q31 + SPD->Spd_I_Term_q31;

    // Output saturation
    sint32 Iq_Ref_q15 = Spd_Out_q31 >> Q31_FROM_Q15_SHIFT;
		sint32 Id_Ref_q15 = 0U;
		
    if (Iq_Ref_q15 > SPD->Out_Max_q15) 			Iq_Ref_q15 = SPD->Out_Max_q15;
    else if (Iq_Ref_q15 < SPD->Out_Min_q15)	Iq_Ref_q15 = SPD->Out_Min_q15;

    // Update current references
    SPD->Iq_Ref_q15 = (sint16)Iq_Ref_q15;
    SPD->Id_Ref_q15 = (sint16)Id_Ref_q15;
}

 /**
 * @brief Speed reference ramping function (Q15 format).
 *
 * This function performs a ramp-up/down of the speed reference value (`Spd_Ref_Out_Q15`)
 * toward the commanded input (`Spd_Ref_In_Q15`) using specified acceleration and 
 * deceleration slopes. This ensures a smooth transition in speed commands, 
 * avoiding sudden jumps.
 *
 * @param[in,out] SpdRamp Pointer to SPEED_RAMP_UP_Q15 structure containing input and output references.
 */
__attribute__((always_inline)) void Speed_Ramp_Up(SPEED_RAMP_UP *SpdRamp)
{
		// Convert input from Q15 to Q26
		sint32 in_q31 = (sint32)SpdRamp->Spd_Ref_In_q15 * (1 << Q31_FROM_Q15_SHIFT);; 
	  sint32 out_q31 = SpdRamp->Spd_Ref_Out_q31;

		// Calculate error and Fast return
	  sint32 err_q31 = in_q31 - out_q31;
    if (err_q31 == 0) {
        SpdRamp->Spd_Ref_Out_q15 = (sint16)(out_q31 >> Q31_FROM_Q15_SHIFT); 
        return;
    }
		
		// Compute absolute values
		sint32 abs_in_q31  = (in_q31 >= 0) ? in_q31 : -in_q31;
    sint32 abs_out_q31 = (out_q31 >= 0) ? out_q31 : -out_q31;
		
		// Select slope: use deceleration during sign change, otherwise
    // acceleration if abs(input) > abs(output), else deceleration
    sint32 acc = SpdRamp->Spd_Acc_Slope_per_tick_q31;
    sint32 dec = SpdRamp->Spd_Deacc_Slope_per_tick_q31;
    sint32 step_sel  = ((in_q31 ^ out_q31) < 0) ? dec : ((abs_in_q31 > abs_out_q31) ? acc : dec);
		
    // Limit step so it does not exceed error magnitude
    sint32 abs_err = (err_q31 >= 0) ? err_q31 : -err_q31;
    if (step_sel > abs_err) step_sel = abs_err;

    // Update output toward input
    out_q31 += (err_q31 > 0) ? step_sel : -step_sel;

    // Store back results
    SpdRamp->Spd_Ref_Out_q31 = out_q31;
    SpdRamp->Spd_Ref_Out_q15 = (sint16)(out_q31 >> Q31_FROM_Q15_SHIFT);
}


/**
 * @brief Reset speed loop controller and ramp state.
 *
 * Synchronizes ramp output and PI reference, clears integral state and outputs.
 *
 * @param[in,out] pi   Pointer to SPEED_LOOP_PI_Q15 structure to reset.
 * @param[in,out] ramp Pointer to SPEED_RAMP_UP_Q15 structure to reset.
 */
void Speed_Loop_Reset(SPEED_LOOP_PI *pi, SPEED_RAMP_UP *ramp)
{
    // Reset PI control state
    pi->Spd_Ref_q15    = 0;
    pi->Spd_Fb_q15     = 0;
    pi->Spd_I_Term_q31 = 0;
    pi->Iq_Ref_q15     = 0;
    pi->Id_Ref_q15     = 0;

    // Reset ramp state
    ramp->Spd_Ref_In_q15  = 0;
    ramp->Spd_Ref_Out_q15 = 0;
    ramp->Spd_Ref_Out_q31 = 0;
}