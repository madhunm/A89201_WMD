/**
 * @file         smo_pos.c
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
 
#include "smo_pos.h"
#include "foc_sine_table.h"
#include "iq_math.h"

/**
 * @brief Initialize SMO (Sliding Mode Observer) parameters and internal states.
 *
 * All structure members are set to zero. Use this before assigning real parameters,
 * or for safe default startup behavior.
 *
 * @param smo Pointer to SMO structure
 */
void SMO_Init(SMO *smo)
{
    /* --- Configuration parameters (Q15) --- */
    smo->currentDecay_q15 = 0;    ///< Current decay coefficient
    smo->voltageGain_q15  = 0;    ///< Voltage gain for motor model
    smo->Kslide_q15       = 0;    ///< Sliding mode gain
    smo->maxError_q15     = 0;    ///< Maximum error before saturation
    smo->emfFilterK_q15   = 0;    ///< Back-EMF low-pass filter gain

    /* --- Inputs --- */
    smo->Valpha_q15 = 0;          ///< Input voltage a-axis
    smo->Vbeta_q15  = 0;          ///< Input voltage ß-axis
    smo->Ialpha_q15 = 0;          ///< Measured current a-axis
    smo->Ibeta_q15  = 0;          ///< Measured current ß-axis

    /* --- Internal state estimates --- */
    smo->IalphaEst_q15 = 0;       ///< Estimated current a-axis
    smo->IbetaEst_q15  = 0;       ///< Estimated current ß-axis
    smo->Ealpha_q15    = 0;       ///< Estimated back-EMF a-axis
    smo->Ebeta_q15     = 0;       ///< Estimated back-EMF ß-axis
    smo->Zalpha_q15   = 0;       ///< Sliding mode output a-axis
    smo->Zbeta_q15     = 0;       ///< Sliding mode output ß-axis

    /* --- Internal error terms --- */
    smo->IalphaErr_q15 = 0;       ///< Current error a-axis
    smo->IbetaErr_q15  = 0;       ///< Current error ß-axis
}

/**
 * @brief Reset internal states of SMO (runtime only).
 *
 * Does not affect configuration parameters. Use when restarting observer.
 *
 * @param smo Pointer to SMO structure
 */
void SMO_Reset(SMO *smo)
{
    /* --- Inputs --- */
    smo->Valpha_q15 = 0;
    smo->Vbeta_q15  = 0;
    smo->Ialpha_q15 = 0;
    smo->Ibeta_q15  = 0;

    /* --- Estimates and errors --- */
    smo->IalphaEst_q15 = 0;
    smo->IbetaEst_q15  = 0;
    smo->Ealpha_q15    = 0;
    smo->Ebeta_q15     = 0;
    smo->Zalpha_q15    = 0;
    smo->Zbeta_q15     = 0;
    smo->IalphaErr_q15 = 0;
    smo->IbetaErr_q15  = 0;
}

/**
 * @brief Initialize PLL structure with zeroed parameters and state.
 *
 * This clears all control gains and internal states.
 *
 * @param pll Pointer to PLL structure
 */
void PLL_Init(PLL *pll)
{
    pll->Kp_q15        = 0;    ///< Proportional gain
    pll->Ki_q15        = 0;    ///< Integral gain
    pll->Omega_q15     = 0;    ///< Estimated speed (Q15)
    pll->OmegaInt_q31  = 0;    ///< PI integrator (Q31)
    pll->Theta_q15     = 0;    ///< Rotor electrical angle (Q15)
		pll->Ed_q15 = 0;					 ///< Estimated Ed (Q15)
}

/**
 * @brief Reset internal state of PLL (runtime only).
 *
 * Does not modify control gains. Use this during system reset.
 *
 * @param pll Pointer to PLL structure
 */
void PLL_Reset(PLL *pll)
{
    pll->Omega_q15     = 0;
    pll->OmegaInt_q31  = 0;
    pll->Theta_q15     = 0;
		pll->Ed_q15 = 0;
}

/**
 * @brief Saturation function for sliding mode control.
 *
 * Implements a piecewise function with saturation for sliding mode observer.
 * 
 *        |  +Kslide     if error > +maxErr
 *  f(x)= |  -Kslide     if error < -maxErr
 *        |  (Kslide * error) / maxErr    otherwise
 *
 * @param error     Input error signal (Q15)
 * @param Kslide    Slope gain (Q15)
 * @param maxErr    Saturation threshold (Q15)
 * @return Saturated output (Q15)
 */
static inline __attribute__((always_inline))
	sint16 smo_sat(sint16 error_q15, sint16 Kslide_q15, sint16 maxErr_q15)
{
    if (error_q15 > maxErr_q15) return Kslide_q15;
    if (error_q15 < -maxErr_q15) return -Kslide_q15;
    return (sint16)(((sint32)Kslide_q15 * error_q15) / (( maxErr_q15 == 0) ? 1 : maxErr_q15 ));
}

/**
 * @brief Sliding Mode Observer (SMO) core update function.
 *
 * This function runs one step of the current & back-EMF estimation using:
 *   - Discrete motor model for current prediction
 *   - Sliding mode saturation for nonlinearity
 *   - First-order IIR for EMF estimation
 *
 * Call at every PWM cycle.
 *
 * @param smo Pointer to SMO structure
 */
static inline void SMO_Update ( SMO *smo )
{
    /* Predict phase currents based on motor model
     * I_est[k+1] = currentDecay * I_est[k] + voltageGain * (V - Z)
		 */
    sint32 ialpha_pred_q15 = (sint32)smo->currentDecay_q15 * smo->IalphaEst_q15 >> Q15_SHIFT;
    ialpha_pred_q15 += ((sint32)smo->voltageGain_q15 *
                   ((sint32)smo->Valpha_q15 - smo->Zalpha_q15)) >> Q15_SHIFT;
    smo->IalphaEst_q15 = (sint16)ialpha_pred_q15;
	
	  /* -------- Current prediction (ß-axis) -------- */

    sint32 ibeta_pred_q15 = (sint32)smo->currentDecay_q15 * smo->IbetaEst_q15 >> Q15_SHIFT;
    ibeta_pred_q15 += ((sint32)smo->voltageGain_q15 *
                   ((sint32)smo->Vbeta_q15 - smo->Zbeta_q15)) >> Q15_SHIFT;
    smo->IbetaEst_q15 = (sint16)ibeta_pred_q15;

    /* -------- Compute current estimation error -------- */
	
    smo->IalphaErr_q15 = smo->IalphaEst_q15 - smo->Ialpha_q15;
    smo->IbetaErr_q15  = smo->IbetaEst_q15  - smo->Ibeta_q15;

    /* -------- Sliding control output (Z) -------- */

    smo->Zalpha_q15 = smo_sat(smo->IalphaErr_q15, smo->Kslide_q15, smo->maxError_q15);
    smo->Zbeta_q15  = smo_sat(smo->IbetaErr_q15,  smo->Kslide_q15, smo->maxError_q15);

    /* 
     * Estimate back-EMF using a first-order low-pass filter
     * E[k+1] = E[k] + Kslf * (Z - E[k])
     */
    sint32 ealpha_filt_q15 = (sint32)smo->emfFilterK_q15 * ((sint32)smo->Zalpha_q15 - smo->Ealpha_q15);
    smo->Ealpha_q15 += (sint16)(ealpha_filt_q15 >> Q15_SHIFT);

    sint32 ebeta_filt_q15 = (sint32)smo->emfFilterK_q15 * ((sint32)smo->Zbeta_q15 - smo->Ebeta_q15);
    smo->Ebeta_q15 += (sint16)(ebeta_filt_q15 >> Q15_SHIFT);
}

/**
 * @brief PLL update function to track rotor angle from BEMF.
 *
 * Uses a PI loop based on the q-axis BEMF projection.
 *
 * @param pll     Pointer to PLL structure
 * @param Ealpha  a-axis back-EMF estimate (Q15)
 * @param Ebeta   ß-axis back-EMF estimate (Q15)
 */

static inline void PLL_Update ( PLL *pll )
{
		/* ---- Convert -> to lookup table index ---- */
    uint16 idx = (uint16)((((sint32)pll->Theta_q15 + 32768 ) >> 4U) & (LUT_SIZE - 1));

    /* ----  Get sin(theta) & cos(theta) from LUT ---- */
    sint16 sin_q15 = FOC_SinLutQ15(idx);
    sint16 cos_q15 = FOC_CosLutQ15(idx);

    /* ---- Compute error between estimated and acutal theta ---- */ 
    sint32 temp1_q30 = (sint32)( -pll->Ealpha_q15 ) * cos_q15;
    sint32 temp2_q30 = (sint32)( pll->Ebeta_q15 ) * sin_q15;
    pll->Ed_q15 = (sint16)((temp1_q30 - temp2_q30) >> Q15_SHIFT);
	
		/* ---- Direction needs to be considered into Ed errors ---- */
		pll->Ed_q15 = pll->dir ? -pll->Ed_q15 : pll->Ed_q15;
	
		if (pll->Ed_q15 > Q15_ONE) pll->Ed_q15 = Q15_ONE;
		if (pll->Ed_q15 < Q15_MINUS_ONE) pll->Ed_q15 = Q15_MINUS_ONE;

    /* ---- PI controller ---- */
    sint32 p_term_q30 = (sint32)pll->Kp_q15 * pll->Ed_q15 ;      /* Q15×Q15=Q30 */

    pll->OmegaInt_q31 += (sint32)pll->Ki_q15 * pll->Ed_q15 ;  /* Q15×Q15=Q30 */

    /* ---- Anti-windup ---- */
    if (pll->OmegaInt_q31 > Q31_ONE) pll->OmegaInt_q31 = Q31_ONE;
    if (pll->OmegaInt_q31 < Q31_MINUS_ONE) pll->OmegaInt_q31 = Q31_MINUS_ONE;

    /* Combine P+I terms, shift down to Q15 */
    pll->Omega_q15 = (sint16)((p_term_q30 >> 15U) + (pll->OmegaInt_q31 >> 15U));
		if (pll->Omega_q15 > Q15_ONE) pll->Omega_q15 = Q15_ONE;
		if (pll->Omega_q15 < Q15_MINUS_ONE) pll->Omega_q15 = Q15_MINUS_ONE;

    /* ---- Angle integration ---- */
    pll->Theta_q15 +=  (sint16)((sint32)pll->Omega_q15 * pll->KomegaToTheta_q15 >> Q15_SHIFT);
		pll->Theta_q15 = q15_Wrap_Theta( pll->Theta_q15);	
}

/**
 * @brief System-level initialization of observer modules.
 *
 * Initializes both configuration parameters and internal state variables
 * for the SMO and PLL modules.
 *
 * @param smo Pointer to SMO structure
 * @param pll Pointer to PLL structure
 */
void SlidingMode_Observer_Init(SMO *smo, PLL *pll)
{
    SMO_Init(smo);  ///< Initialize SMO parameters and zero states
    PLL_Init(pll);  ///< Initialize PLL parameters and zero states
}

/**
 * @brief Top-level observer execution: SMO + PLL
 *
 * @param smo Pointer to SMO structure
 * @param pll Pointer to PLL structure
 */
__attribute__((always_inline)) void SlidingMode_Observer_Run(SMO *smo_para, PLL *pll_para)
{
		SMO_Update(smo_para);
		pll_para->Ealpha_q15 = smo_para->Ealpha_q15;
		pll_para->Ebeta_q15 = smo_para->Ebeta_q15;
		PLL_Update(pll_para);
}

/**
 * @brief Reset runtime state variables of observer modules.
 *
 * This clears internal estimation states while retaining configuration.
 * Useful during motor restart, fault recovery, or observer resync.
 *
 * @param smo Pointer to SMO structure
 * @param pll Pointer to PLL structure
 */
void SlidingMode_Observer_Reset(SMO *smo, PLL *pll)
{
    SMO_Reset(smo);  ///< Reset SMO estimation states
    PLL_Reset(pll);  ///< Reset PLL integrator and angle
}