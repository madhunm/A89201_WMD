#ifndef HEADER_FOC_CURRENT_LOOP_H
#define HEADER_FOC_CURRENT_LOOP_H
/**
 * @file         foc_current_loop.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Current loop function
 *
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
 *
 */
 
#include "iq_format.h"
#include "iq_math.h"


/**
 * @brief PI controller structure for Q15 fixed-point implementation.
 *
 * Stores proportional-integral (PI) controller parameters, state, and in&out.
 *
 * - Uses Q15 for Kp, Ki, OutMax, OutMin.
 * - Integral accumulator uses Q31 for higher precision.
 */
typedef struct {
	
		sint16 Current_ref_q15;					/**< Current reference input (Q15) */
		sint16 Current_fd_q15;					/**< Current reference input (Q15) */
    sint32 Current_Kp_q15;       		/**< Proportional gain (Q15) */
    sint32 Current_Ki_q15;       		/**< Integral gain (Q15) */
    sint16 OutMax_q15;   						/**< Output upper saturation limit (Q15) */
    sint16 OutMin_q15;   						/**< Output lower saturation limit (Q15) */
    sint32 Current_I_term_q31;   		/**< Integral term accumulator (Q31 precision) */
		sint16 Voltage_out_q15;					/**< Output voltage reference (Q15) */
	
} CURRENT_PI;


/**
 * @brief Initialize a Q15 current PI controller to safe zeros.
 *
 * Sets all fields to 0. Application must set Kp/Ki and output limits
 * (OutMin/OutMax) later before enabling the current loop.
 *
 * @param pi Pointer to CURRENT_PI instance.
 */
void Current_PI_Init(CURRENT_PI *pi);

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
__attribute__((always_inline)) void Current_PI_Parallel_Q15( CURRENT_PI *pi );

/**
 * @brief Reset the internal state of a Q15-format parallel PI controller.
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
__attribute__((always_inline)) void Current_PI_Reset(CURRENT_PI *pi);



#endif //HEADER_FOC_CURRENT_LOOP_H