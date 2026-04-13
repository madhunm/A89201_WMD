#ifndef HEADER_SPEED_LOOP_H
#define HEADER_SPEED_LOOP_H

/**
 * @file         speed_loop.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Speed loop - PI controller and Rampup function
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

/**
 * @brief Speed PI controller data structure (Q15-based).
 */
typedef struct
{
    sint16 Spd_Kp_q15;         /**< Proportional gain (Q15) */
    sint16 Spd_Ki_q15;         /**< Integral gain (Q15) */
    sint16 Spd_Ref_q15;        /**< Speed reference input (Q15) */
    sint16 Spd_Fb_q15;         /**< Speed feedback (Q15) */
    sint32 Spd_I_Term_q31;     /**< Integral term accumulator (Q31) */
    sint16 Out_Max_q15;    		/**< Output upper saturation limit (Q15) */
    sint16 Out_Min_q15;    		/**< Output lower saturation limit (Q15) */
    sint16 Id_Ref_q15;         /**< Output d-axis current reference (Q15) */
    sint16 Iq_Ref_q15;         /**< Output q-axis current reference (Q15) */
	
} SPEED_LOOP_PI;

/**
 * @brief Speed ramp configuration structure (Q15-based).
 */
typedef struct
{
    sint16 Spd_Ref_In_q15;                 /**< Commanded speed input (Q15) */
	  sint16 Spd_Ref_Out_q15;                /**< Ramp-limited speed output (Q15) */
	  sint32 Spd_Ref_Out_q31;                /**< Ramp-limited speed output (Q31) */
    sint32 Spd_Acc_Slope_per_tick_q31;     /**< Acceleration slope (Q31 per control tick) */
    sint32 Spd_Deacc_Slope_per_tick_q31;   /**< Deceleration slope (Q31 per control tick) */

} SPEED_RAMP_UP;

 /**
 * @brief Initialize speed loop PI controller and ramp components with default (zero) values.
 *
 * This version initializes all gains (Kp, Ki) and limits to 0.
 * Use it during startup or when parameters will be configured later.
 *
 * @param[out] pi   Pointer to SPEED_LOOP_PI_Q15 structure to initialize.
 * @param[out] ramp Pointer to SPEED_RAMP_UP_Q15 structure to initialize.
 */
void Speed_Loop_Init(SPEED_LOOP_PI *pi, SPEED_RAMP_UP *ramp);

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
__attribute__((always_inline)) void Speed_Loop_PI_Control(SPEED_LOOP_PI *SPD);

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
__attribute__((always_inline)) void Speed_Ramp_Up(SPEED_RAMP_UP *SpdRamp);

/**
 * @brief Reset speed loop controller and ramp state.
 *
 * @param[in,out] pi   Pointer to SPEED_LOOP_PI_Q15 structure to reset.
 * @param[in,out] ramp Pointer to SPEED_RAMP_UP_Q15 structure to reset.
 */
/**
 * @brief Reset speed loop controller and ramp state.
 *
 * Synchronizes ramp output and PI reference, clears integral state and outputs.
 *
 * @param[in,out] pi   Pointer to SPEED_LOOP_PI_Q15 structure to reset.
 * @param[in,out] ramp Pointer to SPEED_RAMP_UP_Q15 structure to reset.
 */
void Speed_Loop_Reset(SPEED_LOOP_PI *pi, SPEED_RAMP_UP *ramp);

#endif // HEADER_SPEED_LOOP_H