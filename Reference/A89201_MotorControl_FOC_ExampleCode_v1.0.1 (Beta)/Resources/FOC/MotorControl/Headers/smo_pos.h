#ifndef HEADER_SMO_POS_H
#define HEADER_SMO_POS_H
/**
 * @file         smo_pos.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Sliding mode observer
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
 *
 */
 
#include "iq_format.h"



#define PLL_PHASE_ACCUMULATOR 4
 
 /**
 * @brief PLL structure for sensorless motor control
 * 
 * - All values are in Q15 fixed-point format, unless noted otherwise.
 * - Theta is maintained as uint16_t [0–65535] representing [0–2p).
 * - Omega is Q15, representing electrical speed.
 */
typedef struct {

		sint16		Kp_q15;      				/**< PLL proportional gain [Q15] */
    sint16		Ki_q15;      				/**< PLL integral gain [Q15] */
	  sint16		Theta_q15;   				/**< Estimated electrical angle [-32768–32767 -> -pi–pi) */
    sint16		Omega_q15;   				/**< Estimated electrical angular speed [Q15] */
    sint32		OmegaInt_q31;				/**< Integrator accumulator for PI controller [Q31] */
		sint16   	KomegaToTheta_q15;	/**< Gain to convert omega->dtheta per tick (Q15). */
		sint16		Ed_q15;							/**< Estimated Ed (Q15) */
		sint16		dir;								/**< Motor direction  */
		sint16 		Ealpha_q15;        	/**< Estimated back-EMF a-axis [Q15] */
    sint16 		Ebeta_q15;         	/**< Estimated back-EMF ß-axis [Q15] */

} PLL;

/** 
 * @brief Sliding Mode Observer (SMO) structure
 *
 * - Inputs: measured currents (Ia/Iß) and applied voltages (Va/Vß)
 * - Outputs: estimated back-EMF (Ea/Eß)
 * - All math in Q15 fixed-point
 */
typedef struct {
		
		/* --- Inputs --- */
    sint16 Ialpha_q15;        /**< Measured phase current a [Q15] */
    sint16 Ibeta_q15;         /**< Measured phase current ß [Q15] */
    sint16 Valpha_q15;        /**< Applied phase voltage a [Q15] */
    sint16 Vbeta_q15;         /**< Applied phase voltage ß [Q15] */

    /* --- SMO Parameters --- */
    sint16 currentDecay_q15;  /**< Current model decay factor (1 - R·Ts/L) [Q15] */
    sint16 voltageGain_q15;   /**< Voltage to current gain factor (Ts/L) [Q15] */
    sint16 Kslide_q15;        /**< Sliding gain [Q15] */
    sint16 maxError_q15;      /**< Linear region threshold for sat function [Q15] */
    sint16 emfFilterK_q15;    /**< Back-EMF low-pass filter coefficient [Q15] */

    /* --- Internal States --- */
    sint16 IalphaEst_q15;     /**< Estimated current a [Q15] */
    sint16 IbetaEst_q15;      /**< Estimated current ß [Q15] */
    sint16 IalphaErr_q15;     /**< Current estimation error a [Q15] */
    sint16 IbetaErr_q15;      /**< Current estimation error ß [Q15] */
    sint16 Zalpha_q15;        /**< Sliding mode correction a [Q15] */
    sint16 Zbeta_q15;         /**< Sliding mode correction ß [Q15] */

		sint16 IalphaEst_prev_q15;
		sint16 IbetaEst_prev_q15;
		
		/* --- Outputs --- */		
		sint16 Ealpha_q15;        /**< Applied phase voltage a [Q15] */
    sint16 Ebeta_q15;         /**< Applied phase voltage ß [Q15] */
		
} SMO;

/**
 * @brief System-level initialization of observer modules.
 *
 * Initializes both configuration parameters and internal state variables
 * for the SMO and PLL modules.
 *
 * @param smo Pointer to SMO structure
 * @param pll Pointer to PLL structure
 */
void SlidingMode_Observer_Init(SMO *smo, PLL *pll);

/**
 * @brief Top-level sliding mode observer execution: SMO + PLL
 *
 * @param smo Pointer to SMO structure
 * @param pll Pointer to PLL structure
 */ 
__attribute__((always_inline)) void SlidingMode_Observer_Run(SMO *smo_para, PLL *pll);

/**
 * @brief Reset runtime state variables of observer modules.
 *
 * This clears internal estimation states (I, Z, EMF, ?, ?) while retaining configuration.
 * Useful during motor restart, fault recovery, or observer resync.
 *
 * @param smo Pointer to SMO structure
 * @param pll Pointer to PLL structure
 */
void SlidingMode_Observer_Reset(SMO *smo, PLL *pll);

 #endif //HEADER_SMO_POS_H