#ifndef HEADER_SINGLE_SHUNT_H
#define HEADER_SINGLE_SHUNT_H
/**
 * @file         single_shunt.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Single shunt algorithm with phase shifting
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
#include "iq_math.h"

/**
 * @brief SVPWM input signals (from FOC controller or modulation block)
 */
typedef struct
{
    sint16     Valpha_q15;			/**< alpha-axis voltage reference (Q15) */ 
    sint16     Vbeta_q15;      	/**< beta-axis voltage reference (Q15) */
    sint16     Tmin_q15;       	/**< minimum active vector time (Q15) */
    uint16 		 Period;   				/**< PWM period (timer ticks) */ 
} SingleShunt_SVPWM_Input;

/**
 * @brief Intermediate normalized duty and sector info
 */
typedef struct
{
    sint16     Ta_q15;         /**< duty for phase A (Q15) */
    sint16     Tb_q15;         /**< duty for phase B (Q15) */  
    sint16     Tc_q15;         /**< duty for phase C (Q15) */ 
    sint16     T1_q15;         /**< vector 1 duration (Q15) */ 
    sint16     T2_q15;         /**< vector 2 duration (Q15) */ 
    uint8 		 sector;     		 /**< Sector number (1-6) */ 
} SingleShunt_SVPWM_Normalized;

/**
 * @brief SVPWM final output values
 */
typedef struct
{
		uint16  DutyA;      /**< Base duty for phase A (no shift) */
    uint16  DutyB;      /**< Base duty for phase B (no shift)	*/
    uint16  DutyC;      /**< Base duty for phase C (no shift)	*/

    uint16  Duty1A;     /**< Phase A compare value 1 (after phase shift)	*/
    uint16  Duty2A;     /**< Phase A compare value 2 (after phase shift)	*/
    uint16  Duty1B;     /**< Phase B compare value 1	*/
    uint16  Duty2B;     /**< Phase B compare value 2	*/
    uint16  Duty1C;     /**< Phase C compare value 1	*/
    uint16  Duty2C;     /**< Phase C compare value 2	*/

    uint16  ADCTrig1;   /**< First ADC trigger point (timer tick)	*/
    uint16  ADCTrig2;   /**< Second ADC trigger point (timer tick)	*/
	
	  uint8 	sector;     /**< Sector number (1-6) */ 
} SingleShunt_SVPWM_Output;

/**
 * @brief Bus current samples and reconstructed phase currents
 */ 
typedef struct
{
    sint16 	Ibus1_q15;        /**< First current sample */ 
    sint16 	Ibus2_q15;        /**< Second current sample */
    uint8 	sector;     			/**< Sector from SVPWM */ 
    sint16 	Ia_q15;           /**< Reconstructed phase A current */
    sint16 	Ib_q15;           /**< Reconstructed phase B current */
    sint16 	Ic_q15;           /**< Reconstructed phase C current */
} SingleShunt_Current;

/**
 * @brief Initializes all SVPWM module structures in one call.
 *
 * @param[in,out] in     Pointer to SVPWM_Input
 * @param[in,out] norm   Pointer to SVPWM_Normalized
 * @param[in,out] out    Pointer to SVPWM_Output
 * @param[in,out] cur    Pointer to SVPWM_Current
 */
void SingleShunt_Init(SingleShunt_SVPWM_Input *in, SingleShunt_SVPWM_Normalized *norm,
                   SingleShunt_SVPWM_Output *out, SingleShunt_Current *cur);

/**
 * @brief Runs the full single shunt process including duty, compensation, and current reconstruction.
 *
 * @param[in]  in      Pointer to SVPWM input
 * @param[in]  meas    Pointer to current structure with Ibus1/Ibus2
 * @param[out] out     Pointer to PWM output structure
 * @param[out] current Pointer to structure for reconstructed currents
 * @param[out] norm    Pointer to intermediate normalized structure (T1/T2/Ta/Tb/Tc/sector)
 */
__attribute__((always_inline)) void SingleShunt_Run(const SingleShunt_SVPWM_Input *in, 
										SingleShunt_SVPWM_Output *out, SingleShunt_SVPWM_Normalized *norm);
													
													/**
 * @brief Resets dynamic fields of all SVPWM structures to safe zero state.
 *
 * @param[in,out] in     Pointer to SVPWM_Input (Valpha/Vbeta reset only)
 * @param[in,out] norm   Pointer to SVPWM_Normalized
 * @param[in,out] out    Pointer to SVPWM_Output
 * @param[in,out] cur    Pointer to SVPWM_Current
 */
void SingleShunt_Reset(SingleShunt_SVPWM_Input *in, SingleShunt_SVPWM_Normalized *norm,
                    SingleShunt_SVPWM_Output *out, SingleShunt_Current *cur);

/**
 * @brief   Reconstruct phase currents from single-shunt current samples.
 *
 * @param[in,out] ss Pointer to the SingleShunt_Handle structure.
 *                   The reconstructed currents will be written to ss->Ia, ss->Ib, ss->Ic.
 */
__attribute__((always_inline)) void SingleShunt_ReconstructCurrents(SingleShunt_Current *current);

#endif //HEADER_SINGLE_SHUNT_H