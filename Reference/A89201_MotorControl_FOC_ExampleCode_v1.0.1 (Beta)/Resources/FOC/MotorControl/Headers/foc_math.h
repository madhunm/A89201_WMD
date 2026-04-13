#ifndef HEADER_FOC_MATH_H
#define HEADER_FOC_MATH_H
/**
 * @file         foc_math.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Math functions for foc algorithm
 *
 * @copyright (c) 2024 Allegro MicroSystems, Inc. All Rights Reserved.
 * The information contained in this file is confidential and proprietary.
 *
 * The information contained in this file is governed by Software License
 * acknowledged during the download from Allegroís Software Download
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
#include "foc_sine_table.h"

/**
 * @brief Clarke transform (abc -> aﬂ) using Q15 fixed-point format.
 *
 * Converts three-phase currents or voltages (a, b, c) into the stationary
 * a-ﬂ coordinate system. Phase C is assumed to satisfy a+b+c=0 (balanced system).
 *
 * @note All values use Q15 format (-32768 to 32767).
 */
typedef struct {
    sint16 a_q15;       /**< Phase A input */
    sint16 b_q15;       /**< Phase B input */
    sint16 c_q15;       /**< Phase C input */
    sint16 alpha_q15;   /**< a-axis output */
    sint16 beta_q15;    /**< ﬂ-axis output */
} CLARKE_Q15;

static inline __attribute__((always_inline))
	void FOC_Clarke_Q15(CLARKE_Q15 *clarke)
{
    /* a = a */
    clarke->alpha_q15 = clarke->a_q15;

    /* ﬂ = (a + 2b) / sqrt(3) */
    sint32 temp_q15 = (sint32)clarke->a_q15 + ((sint32)clarke->b_q15 << 1U);
    temp_q15 = (temp_q15 * Q15_ONE_OVER_SQRT3) >> Q15_SHIFT;
    clarke->beta_q15 = (sint16)temp_q15;
}

/**
 * @brief Inverse Clarke transform (aﬂ -> abc) using Q15 fixed-point format.
 *
 * Converts stationary a-ﬂ voltages/currents back to three-phase (a, b, c).
 * Assumes balanced system: a+b+c=0.
 */
typedef struct {
    sint16 alpha_q15;   /**< a-axis input */
    sint16 beta_q15;    /**< ﬂ-axis input */
    sint16 a_q15;       /**< Phase A output */
    sint16 b_q15;       /**< Phase B output */
    sint16 c_q15;       /**< Phase C output */
} CLARKE_INV_Q15;

static inline __attribute__((always_inline))
void FOC_Clarke_Inv_Q15(CLARKE_INV_Q15 *clarkeInv)
{
    /* a = a */
    clarkeInv->a_q15 = clarkeInv->alpha_q15;

    /* b = -0.5*a + (v3/2)*ﬂ */
    sint32 temp_b_q30 = (sint32)clarkeInv->alpha_q15 * Q15_MINUS_HALF + (sint32)clarkeInv->beta_q15 * Q15_SQRT3_OVER_2;
    clarkeInv->b_q15 = (sint16)(temp_b_q30 >> Q15_SHIFT);

    /* c = -0.5*a - (v3/2)*ﬂ */
    sint32 temp_c_q30 = (sint32)clarkeInv->alpha_q15 * Q15_MINUS_HALF - (sint32)clarkeInv->beta_q15 * Q15_SQRT3_OVER_2;
    clarkeInv->c_q15 = (sint16)(temp_c_q30 >> Q15_SHIFT);
}

/**
 * @brief Park transform (aﬂ -> dq) using Q15 fixed-point format.
 *
 * Projects stationary a-ﬂ components into the rotating d-q reference frame
 * aligned with the rotor flux (angle ?).
 *
 * @note Uses sine/cosine lookup: FOC_SinLutQ15() / FOC_CosLutQ15().
 */
typedef struct {
    sint16 alpha_q15;        /**< a-axis input */
    sint16 beta_q15;         /**< ﬂ-axis input */
    uint16 thetaIdx;				 /**< Angle index for sin/cos LUT */
    sint16 d_q15;            /**< d-axis output */
    sint16 q_q15;            /**< q-axis output */
} PARK_Q15;

static inline __attribute__((always_inline))
	void FOC_Park_Q15(PARK_Q15 *park)
{
    /* d = a∑cos(theta) + ﬂ∑sin(theta) */
    sint32 temp_d_30 = (sint32)park->alpha_q15 * FOC_CosLutQ15(park->thetaIdx) +
                 (sint32)park->beta_q15  * FOC_SinLutQ15(park->thetaIdx);
    park->d_q15 = (sint16)(temp_d_30 >> Q15_SHIFT);

    /* q = -a∑sin(theta) + ﬂ∑cos(theta) */
    sint32 temp_q_30 = (sint32)park->alpha_q15 * (-FOC_SinLutQ15(park->thetaIdx)) +
                 (sint32)park->beta_q15  *  FOC_CosLutQ15(park->thetaIdx);
    park->q_q15 = (sint16)(temp_q_30 >> Q15_SHIFT);
}

/**
 * @brief Inverse Park transform (dq -> aﬂ) using Q15 fixed-point format.
 *
 * Converts rotating frame d-q components back into stationary a-ﬂ components.
 *
 * @note Uses sine/cosine lookup: FOC_SinLutQ15() / FOC_CosLutQ15().
 */
typedef struct {
    sint16 d_q15;            /**< d-axis input */
    sint16 q_q15;            /**< q-axis input */
    uint16 thetaIdx;				 /**< Angle index for sin/cos LUT */
    sint16 alpha_q15;        /**< a-axis output */
    sint16 beta_q15;         /**< ﬂ-axis output */
} PARK_INV_Q15;

static inline __attribute__((always_inline))
	void FOC_Park_Inv_Q15(PARK_INV_Q15 *parkInv)
{
    /* a = d∑cos(theta) - q∑sin(theta) */
    sint32 temp_alpha_q30 = (sint32)parkInv->d_q15 * FOC_CosLutQ15(parkInv->thetaIdx) - (sint32)parkInv->q_q15 * FOC_SinLutQ15(parkInv->thetaIdx);
    parkInv->alpha_q15 = (sint16)(temp_alpha_q30 >> Q15_SHIFT);

    /* ﬂ = d∑sin(theta) + q∑cos(theta) */
    sint32 temp_beta_q30  = (sint32)parkInv->d_q15 * FOC_SinLutQ15(parkInv->thetaIdx) + (sint32)parkInv->q_q15 * FOC_CosLutQ15(parkInv->thetaIdx);
		parkInv->beta_q15 = (sint16)(temp_beta_q30 >> Q15_SHIFT);
}

#endif //HEADER_FOC_MATH_H