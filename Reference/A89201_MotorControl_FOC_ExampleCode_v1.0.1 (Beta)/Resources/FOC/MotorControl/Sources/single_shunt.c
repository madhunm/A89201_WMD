/**
 * @file         single_shunt.c
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
 
#include "single_shunt.h"

/**
 * @brief Initializes all SVPWM module structures in one call.
 *
 * This unified initializer sets safe default values for all module structures:
 *
 * Use this at system startup to ensure all structures are valid before control begins.
 *
 * @param[in,out] in     Pointer to SVPWM_Input
 * @param[in,out] norm   Pointer to SVPWM_Normalized
 * @param[in,out] out    Pointer to SVPWM_Output
 * @param[in,out] cur    Pointer to SVPWM_Current
 */
void SingleShunt_Init(SingleShunt_SVPWM_Input *in, SingleShunt_SVPWM_Normalized *norm,
                   SingleShunt_SVPWM_Output *out, SingleShunt_Current *cur)
{
    // Input defaults (Q15 format, no float)
    in->Valpha_q15  = 0;
    in->Vbeta_q15   = 0;
    in->Tmin_q15    = 1638;   // 5% in Q15 -> 1638
    in->Period  		= 4000;   // PWM period in timer ticks

    // Normalized intermediates
    norm->T1_q15 = norm->T2_q15 = 0;
    norm->Ta_q15 = norm->Tb_q15 = norm->Tc_q15 = 0;
    norm->sector = 0;

    // Output duty and triggers
    out->DutyA = out->DutyB = out->DutyC = 0;
    out->Duty1A = out->Duty2A = 0;
    out->Duty1B = out->Duty2B = 0;
    out->Duty1C = out->Duty2C = 0;
    out->ADCTrig1 = out->ADCTrig2 = 0;

    // Current sensing values
    cur->Ibus1_q15 = cur->Ibus2_q15 = 0;
    cur->Ia_q15 = cur->Ib_q15 = cur->Ic_q15 = 0;
    cur->sector = 0;
}

/**
 * @brief Computes the SVPWM sector and T1/T2 vector durations (normalized).
 *
 * This function determines which sector the voltage vector lies in, and then calculates
 * the corresponding T1 and T2 durations for the active vectors. The result is used
 * for further PWM timing and phase duty calculations.
 */
static inline __attribute__((always_inline))
	void SingleShunt_SVPWM_CalcNormalizedDuty(const SingleShunt_SVPWM_Input *in, SingleShunt_SVPWM_Normalized *norm)
{

    // Step 1: Determine sector from a-ß voltages
    sint32 Va_q15 = in->Valpha_q15;  
    sint32 Vb_q15 = (- (in->Valpha_q15 >> 1)) + ((Q15_SQRT3_OVER_2 * in->Vbeta_q15) >> Q15_SHIFT); 
    sint32 Vc_q15 = (- (in->Valpha_q15 >> 1)) - ((Q15_SQRT3_OVER_2 * in->Vbeta_q15) >> Q15_SHIFT);

    if (Va_q15 >= Vb_q15) {
        if (Vb_q15 >= Vc_q15)        norm->sector = 1;   // Va > Vb > Vc
        else if (Va_q15 >= Vc_q15)   norm->sector = 6;   // Va > Vc > Vb
        else                 				 norm->sector = 5;   // Vc > Va > Vb
    } 
		else {
        if (Va_q15 >= Vc_q15)        norm->sector = 2;   // Vb > Va > Vc
        else if (Vb_q15 >= Vc_q15)   norm->sector = 3;   // Vb > Vc > Va
        else                 				 norm->sector = 4;   // Vc > Vb > Va
    }
			
    // Step 2: Compute T1/T2 based on sector (Q15)

    sint32 T1_q15 = 0, T2_q15 = 0;
    switch (norm->sector) {
        case 1: T1_q15 = Va_q15 - Vb_q15; T2_q15 = Vb_q15 - Vc_q15; break; // Va > Vb > Vc
        case 2: T1_q15 = Vb_q15 - Va_q15; T2_q15 = Va_q15 - Vc_q15; break; // Vb > Va > Vc
        case 3: T1_q15 = Vb_q15 - Vc_q15; T2_q15 = Vc_q15 - Va_q15; break; // Vb > Vc > Va
        case 4: T1_q15 = Vc_q15 - Vb_q15; T2_q15 = Vb_q15 - Va_q15; break; // Vc > Vb > Va
        case 5: T1_q15 = Vc_q15 - Va_q15; T2_q15 = Va_q15 - Vb_q15; break; // Vc > Va > Vb
        case 6: T1_q15 = Va_q15 - Vc_q15; T2_q15 = Vc_q15 - Vb_q15; break; // Va > Vc > Vb
    }
	
		// Clamp T1 and T2
		sint32 Tsum = T1_q15 + T2_q15; 
		if ( Tsum > Q15_ONE )
		{
				T1_q15 = (sint16)(((sint32)T1_q15 * Q15_ONE) / Tsum );
				T2_q15 = (sint16)(((sint32)T2_q15 * Q15_ONE) / Tsum );
		}
		// Clamp T0
    sint32 T0_q15 = Q15_ONE - T1_q15 - T2_q15;
    if (T0_q15 < 0) T0_q15 = 0;

    // Step 3: Base duty calculation (Q15)
    sint32 halfT0_q15 = T0_q15 >> 1U;
    sint32 Ta_q15 = 0, Tb_q15 = 0, Tc_q15 = 0;

    switch (norm->sector) {
			
        case 1: Ta_q15 = T1_q15 + T2_q15 + halfT0_q15; 
								Tb_q15 = T2_q15 + halfT0_q15;       
								Tc_q15 = halfT0_q15; break; // Va > Vb > Vc
        case 2: Ta_q15 = T2_q15 + halfT0_q15;      
								Tb_q15 = T1_q15 + T2_q15 + halfT0_q15;  
								Tc_q15 = halfT0_q15; break; // Vb > Va > Vc
        case 3: Ta_q15 = halfT0_q15;           
								Tb_q15 = T1_q15 + T2_q15 + halfT0_q15;  
								Tc_q15 = T2_q15 + halfT0_q15; break; // Vb > Vc > Va
        case 4: Ta_q15 = halfT0_q15;           
								Tb_q15 = T2_q15 + halfT0_q15;       
								Tc_q15 = T1_q15 + T2_q15 + halfT0_q15; break; // Vc > Vb > Va
        case 5: Ta_q15 = T2_q15 + halfT0_q15;      
								Tb_q15 = halfT0_q15;            
								Tc_q15 = T1_q15 + T2_q15 + halfT0_q15; break; // Vc > Va > Vb
        case 6: Ta_q15 = T1_q15 + T2_q15 + halfT0_q15; 
								Tb_q15 = halfT0_q15;            
								Tc_q15 = T2_q15 + halfT0_q15; break; // Va > Vc > Vb
    }

    // Clamp duty to Q15 range
    if (Ta_q15 > Q15_ONE) Ta_q15 = Q15_ONE; if (Ta_q15 < 0) Ta_q15 = 0;
    if (Tb_q15 > Q15_ONE) Tb_q15 = Q15_ONE; if (Tb_q15 < 0) Tb_q15 = 0;
    if (Tc_q15 > Q15_ONE) Tc_q15 = Q15_ONE; if (Tc_q15 < 0) Tc_q15 = 0;
		
		//Output vector times
		norm->Ta_q15 = (sint16)Ta_q15;
		norm->Tb_q15 = (sint16)Tb_q15;
		norm->Tc_q15 = (sint16)Tc_q15;
		norm->T1_q15 = (sint16)T1_q15;
		norm->T2_q15 = (sint16)T2_q15;
}

/**
 * @brief Converts normalized duty cycles into actual timer compare values.
 *
 * This function applies bus voltage compensation and converts the normalized
 * duty cycles (Q15 format) into timer ticks using the PWM period.
 * This ensures the PWM output adapts to bus voltage fluctuations without
 * requiring changes to control loop PI parameters.
 *
 * @param[in]  in   Pointer to SVPWM input structure (contains Vbase/Vactual)
 * @param[in]  norm Pointer to normalized duty structure
 * @param[out] out  Pointer to output structure containing duty cycles
 */
static inline __attribute__((always_inline))
	void SingleShunt_SVPWM_ConvertDuty(const SingleShunt_SVPWM_Input *in, const SingleShunt_SVPWM_Normalized *norm, SingleShunt_SVPWM_Output *out)
{
    // Convert to timer ticks
    out->DutyA = (uint16)(((sint32)norm->Ta_q15) * in->Period >> Q15_SHIFT);
    out->DutyB = (uint16)(((sint32)norm->Tb_q15) * in->Period >> Q15_SHIFT);
    out->DutyC = (uint16)(((sint32)norm->Tc_q15) * in->Period >> Q15_SHIFT);
}


/**
 * @brief   Apply phase-shift (vector thickening) for single-shunt sampling
 *          and place ADC triggers at the centers of the *effective* small vectors.
 *
 * This routine serves two tightly coupled purposes:
 *   1) Enforce the minimal sampleable width Tmin on the two active vectors (T1/T2)
 *      by borrowing time from the zero vector T0 (a.k.a. phase-shift/thickening).
 *   2) Place the two ADC trigger compare values at the centers of the *effective*
 *      vectors T1' and T2' to avoid sampling near switching edges / deadtime.
 *
 * Pattern reference (symmetric 7-segment SVPWM within one PWM period P):
 *      [ T0'/2 ]  -- T1' --  -- T2' --  [ T0'/2 ]
 *  where T1' = max(T1, Tmin), T2' = max(T2, Tmin),
 *        T0' = P - T1' - T2' (clamped to >= 0 for safety).
 *
 * @param[in]  in    SVPWM scalar inputs: Period (ticks), Tmin (Q15 of period)
 * @param[in]  norm  Raw normalized results: Ta/Tb/Tc (Q15 duties), T1/T2 (Q15), sector (1..6)
 * @param[out] out   Shifted PWM compares (Duty1X/Duty2X), base duties, and ADC trig ticks
 */
static inline __attribute__((always_inline))
	void SingleShunt_SVPWM_ApplyPhaseShift(const SingleShunt_SVPWM_Input *in, const SingleShunt_SVPWM_Normalized *norm, SingleShunt_SVPWM_Output *out)
{
    sint32 T1_ticks = ((sint32)norm->T1_q15 * in->Period) >> Q15_SHIFT;
    sint32 T2_ticks = ((sint32)norm->T2_q15 * in->Period) >> Q15_SHIFT;
    sint32 Tmin_ticks = ((sint32)in->Tmin_q15 * in->Period) >> Q15_SHIFT;
		sint32 Tmin_DoubleTicks = (sint32)Tmin_ticks << 1U;
	
    sint32 baseTa = (sint32)( in->Period - out->DutyA );
    sint32 baseTb = (sint32)( in->Period - out->DutyB );
    sint32 baseTc = (sint32)( in->Period - out->DutyC );

    sint32 shift_T1 = 0;
    sint32 shift_T2 = 0;

    if ( T1_ticks < Tmin_DoubleTicks && T2_ticks >= Tmin_DoubleTicks ) {
        shift_T1 = Tmin_DoubleTicks - T1_ticks;
				shift_T2 = 0;	} 
		else if ( T2_ticks < Tmin_DoubleTicks && T1_ticks >= Tmin_DoubleTicks ) {
				shift_T1 = 0;
				shift_T2 = Tmin_DoubleTicks - T2_ticks ; } 
		else if ( T1_ticks < Tmin_DoubleTicks && T2_ticks < Tmin_DoubleTicks ) {
        shift_T1 =  Tmin_DoubleTicks - T1_ticks ;
        shift_T2 =  Tmin_DoubleTicks - T2_ticks ; }



    switch (norm->sector)
    {
        case 1:
            out->Duty1A = baseTa - shift_T1 - shift_T2;
            out->Duty2A = baseTa + shift_T1 + shift_T2;
            out->Duty1B = baseTb - shift_T2;
            out->Duty2B = baseTb + shift_T2;
            out->Duty1C = baseTc;
            out->Duty2C = baseTc;
//						out->ADCTrig1 = ( out->Duty1A + out->Duty1B ) >> 2U;
//						out->ADCTrig2 = ( out->Duty1B + out->Duty1C ) >> 2U;
						out->ADCTrig1 = ( out->Duty1B >> 1U ) - 10;
						out->ADCTrig2 = ( out->Duty1C >> 1U ) - 10;
            break;
        case 2:
            out->Duty1A = baseTa - shift_T2;
            out->Duty2A = baseTa + shift_T2;
            out->Duty1B = baseTb - shift_T1 - shift_T2;
            out->Duty2B = baseTb + shift_T1 + shift_T2;
            out->Duty1C = baseTc;
            out->Duty2C = baseTc;
//						out->ADCTrig1 = ( out->Duty1B + out->Duty1A ) >> 2U;
//						out->ADCTrig2 = ( out->Duty1A + out->Duty1C ) >> 2U;
						out->ADCTrig1 = ( out->Duty1A >> 1U ) - 10;
						out->ADCTrig2 = ( out->Duty1C >> 1U ) - 10;
            break;
        case 3:
            out->Duty1A = baseTa;
            out->Duty2A = baseTa;
            out->Duty1B = baseTb - shift_T1 - shift_T2;
            out->Duty2B = baseTb + shift_T1 + shift_T2;
            out->Duty1C = baseTc - shift_T2;
            out->Duty2C = baseTc + shift_T2;
//						out->ADCTrig1 = ( out->Duty1B + out->Duty1C ) >> 2U;
//						out->ADCTrig2 = ( out->Duty1C + out->Duty1A ) >> 2U;
						out->ADCTrig1 = ( out->Duty1C >> 1U ) - 10;
						out->ADCTrig2 = ( out->Duty1A >> 1U ) - 10;

            break;
        case 4:
            out->Duty1A = baseTa;
            out->Duty2A = baseTa;
            out->Duty1B = baseTb - shift_T2;
            out->Duty2B = baseTb + shift_T2;
            out->Duty1C = baseTc - shift_T1 - shift_T2;
            out->Duty2C = baseTc + shift_T1 + shift_T2;
//						out->ADCTrig1 = ( out->Duty1C + out->Duty1B ) >> 2U;
//						out->ADCTrig2 = ( out->Duty1B + out->Duty1A ) >> 2U;
						out->ADCTrig1 = ( out->Duty1B >> 1U ) - 10;
						out->ADCTrig2 = ( out->Duty1A >> 1U ) - 10;

            break;
        case 5:
            out->Duty1A = baseTa - shift_T2;
            out->Duty2A = baseTa + shift_T2;
            out->Duty1B = baseTb;
            out->Duty2B = baseTb;
            out->Duty1C = baseTc - shift_T1 - shift_T2;
            out->Duty2C = baseTc + shift_T1 + shift_T2;
//						out->ADCTrig1 = ( out->Duty1C + out->Duty1A ) >> 2U;
//						out->ADCTrig2 = ( out->Duty1A + out->Duty1B ) >> 2U;
						out->ADCTrig1 = ( out->Duty1A >> 1U ) - 10;
						out->ADCTrig2 = ( out->Duty1B >> 1U ) - 10;

            break;
        case 6:
            out->Duty1A = baseTa - shift_T1 - shift_T2;
            out->Duty2A = baseTa + shift_T1 + shift_T2;
            out->Duty1B = baseTb;
            out->Duty2B = baseTb;
            out->Duty1C = baseTc - shift_T2;
            out->Duty2C = baseTc + shift_T2;
//						out->ADCTrig1 = ( out->Duty1A + out->Duty1C ) >> 2U;
//						out->ADCTrig2 = ( out->Duty1C + out->Duty1B ) >> 2U;
						out->ADCTrig1 = ( out->Duty1C >> 1U ) - 10;
						out->ADCTrig2 = ( out->Duty1B >> 1U ) - 10;
			
            break;
        default:
            out->Duty1A = out->Duty2A = out->Duty1B = out->Duty2B = 
						out->Duty1C = out->Duty2C = out->ADCTrig1 = out->ADCTrig2 = 0;
            break;
    }

    // Clamp to bounds
    if (out->Duty1A > in->Period) out->Duty1A = in->Period;
    if (out->Duty2A > in->Period) out->Duty2A = in->Period;
    if (out->Duty1B > in->Period) out->Duty1B = in->Period;
    if (out->Duty2B > in->Period) out->Duty2B = in->Period;
    if (out->Duty1C > in->Period) out->Duty1C = in->Period;
    if (out->Duty2C > in->Period) out->Duty2C = in->Period;
		if ( out->ADCTrig1 > (in->Period - 4)) out->ADCTrig1 = in->Period - 4;
		if ( out->ADCTrig2 > (in->Period - 4)) out->ADCTrig2 = in->Period - 4;
		
		// Output sector info
		out->sector = norm->sector;
}

/**
 * @brief   Reconstruct phase currents from single-shunt current samples.
 *
 * Uses the two sampled bus currents (Ibus1, Ibus2) and the active SVPWM sector
 * to reconstruct the three-phase currents Ia, Ib, Ic.
 *
 *  - Works with dual-sample single-shunt topology (two ADC samples per PWM period).
 *  - Each SVPWM sector determines which two phases are switching and which one is clamped.
 *
 * @param[in,out] ss Pointer to the SingleShunt_Handle structure.
 *                   The reconstructed currents will be written to ss->Ia, ss->Ib, ss->Ic.
 */
__attribute__((always_inline))
	void SingleShunt_ReconstructCurrents(SingleShunt_Current *current)
{
    sint16 Ia_q15 = 0, Ib_q15 = 0, Ic_q15 = 0;

    switch (current->sector)
    {
			  case 1:
						Ia_q15 = current->Ibus1_q15;
            Ib_q15 = current->Ibus2_q15 - current->Ibus1_q15;
            Ic_q15 = -current->Ibus2_q15;
            break;

        case 2:
            Ia_q15 = current->Ibus2_q15 - current->Ibus1_q15;
            Ib_q15 = current->Ibus1_q15;
            Ic_q15 = -current->Ibus2_q15;
            break;

        case 3:
            Ia_q15 = -current->Ibus2_q15;
            Ib_q15 = current->Ibus1_q15;
            Ic_q15 = current->Ibus2_q15 - current->Ibus1_q15;
            break;

        case 4:
            Ia_q15 = -current->Ibus2_q15;
            Ib_q15 = current->Ibus2_q15 - current->Ibus1_q15;
            Ic_q15 = current->Ibus1_q15;
            break;

        case 5:
            Ia_q15 = current->Ibus2_q15 - current->Ibus1_q15;
            Ib_q15 = -current->Ibus2_q15;
            Ic_q15 = current->Ibus1_q15;
            break;

        case 6:
            Ia_q15 = current->Ibus1_q15;
            Ib_q15 = -current->Ibus2_q15;
            Ic_q15 = current->Ibus2_q15 - current->Ibus1_q15;
            break;

        default:
            // Invalid sector (safety fallback)
            Ia_q15 = Ib_q15 = Ic_q15 = 0;
            break;
    }

    // Write reconstructed currents back to handle
    current->Ia_q15 = Ia_q15;
    current->Ib_q15 = Ib_q15;
    current->Ic_q15 = Ic_q15;
}

/**
 * @brief Runs the full SVPWM process including duty, compensation, and current reconstruction.
 *
 * This wrapper function performs all SVPWM steps:
 *   - Space vector sector detection
 *   - Normalized duty calculation
 *   - Bus voltage compensated output duty
 *   - Phase shift insertion for single-shunt ADC sampling
 *   - ADC trigger generation
 *   - Current reconstruction
 *
 * It is designed for use inside the FOC ISR (typically at 50us loop).
 *
 * @param[in]  in      Pointer to SVPWM input
 * @param[in]  meas    Pointer to current structure with Ibus1/Ibus2
 * @param[out] out     Pointer to PWM output structure
 * @param[out] current Pointer to structure for reconstructed currents
 * @param[out] norm    Pointer to intermediate normalized structure (T1/T2/Ta/Tb/Tc/sector)
 */
__attribute__((always_inline))
	void SingleShunt_Run(const SingleShunt_SVPWM_Input *in, SingleShunt_SVPWM_Output *out, SingleShunt_SVPWM_Normalized *norm)
{
    SingleShunt_SVPWM_CalcNormalizedDuty(in, norm);
    SingleShunt_SVPWM_ConvertDuty(in, norm, out);
    SingleShunt_SVPWM_ApplyPhaseShift(in, norm, out);
}

/**
 * @brief Resets dynamic fields of all SVPWM structures to safe zero state.
 *
 * This function does not reset static configuration like Vbase/Vactual/Tmin/Period,
 * but only clears fields that may change per cycle or during runtime operation.
 *
 * Suitable for fault recovery or pre-loop resets.
 *
 * @param[in,out] in     Pointer to SVPWM_Input (Valpha/Vbeta reset only)
 * @param[in,out] norm   Pointer to SVPWM_Normalized
 * @param[in,out] out    Pointer to SVPWM_Output
 * @param[in,out] cur    Pointer to SVPWM_Current
 */
void SingleShunt_Reset(SingleShunt_SVPWM_Input *in, SingleShunt_SVPWM_Normalized *norm,
                    SingleShunt_SVPWM_Output *out, SingleShunt_Current *cur)
{
    // Reset only runtime-changing fields
    in->Valpha_q15 = 0;
    in->Vbeta_q15 = 0;

    norm->T1_q15 = norm->T2_q15 = 0;
    norm->Ta_q15 = norm->Tb_q15 = norm->Tc_q15 = 0;
    norm->sector = 0;

    out->DutyA = out->DutyB = out->DutyC = 0;
    out->Duty1A = out->Duty2A = 0;
    out->Duty1B = out->Duty2B = 0;
    out->Duty1C = out->Duty2C = 0;
    out->ADCTrig1 = out->ADCTrig2 = 0;

    cur->Ibus1_q15 = cur->Ibus2_q15 = 0;
    cur->Ia_q15 = cur->Ib_q15 = cur->Ic_q15 = 0;
    cur->sector = 0;
}