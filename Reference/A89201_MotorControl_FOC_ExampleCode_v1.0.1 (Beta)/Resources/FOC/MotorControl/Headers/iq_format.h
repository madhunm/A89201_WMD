#ifndef HEADER_IQ_FORMAT_H
#define HEADER_IQ_FORMAT_H
/**
 * @file         iq_format.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Q format definition
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
 
#include "Std_Types.h"


// =============================================================================
// Q-format definitions
// =============================================================================

/**
 * @brief Bit shift count for Q5.10 format (1.0 = 67108864)
 */
#define Q10_SHIFT      	10U

/**
 * @brief Bit shift count for Q0.15 format (1.0 = 32767)
 */
#define Q15_SHIFT       15U

/**
 * @brief Bit shift count for Q5.26 format (1.0 = 67108864)
 */
#define Q26_SHIFT      	26U

/**
 * @brief Bit shift count for Q0.31 format (1.0 = 2147483647)
 */
#define Q31_SHIFT       31U

/**
 * @brief Bit shift count from Q5.10 format to Q0.15
 */
#define Q15_FROM_Q10_SHIFT   (Q15_SHIFT - Q10_SHIFT)   /* 5 */

/**
 * @brief Bit shift count from Q5.10 format to Q5.26
 */
#define Q26_FROM_Q10_SHIFT   (Q26_SHIFT - Q10_SHIFT)   /* 16 */

/**
 * @brief Bit shift count from Q5.10 format to Q0.31
 */
#define Q31_FROM_Q10_SHIFT   (Q31_SHIFT - Q10_SHIFT)   /* 21 */

/**
 * @brief Bit shift count from Q0.15 format to Q5.26
 */
#define Q26_FROM_Q15_SHIFT	(Q26_SHIFT - Q15_SHIFT)		 /* 11 */

/**
 * @brief Bit shift count from Q0.15 format to Q0.31
 */
#define Q31_FROM_Q15_SHIFT	(Q31_SHIFT - Q15_SHIFT)		 /* 16 */

/**
 * @brief Bit shift count from Q5.26 format to Q0.31
 */
#define Q31_FROM_Q26_SHIFT	(Q31_SHIFT - Q26_SHIFT)		 /* 5 */


// =============================================================================
// Precomputed Q10/Q15/Q26/Q31 constants
// =============================================================================

/**
 * @brief Q10 (Q5.10) representation of 1.0 (˜ 1023/1024)
 *
 * Using symmetric range like Q15 macros:
 *  1.0 -> 1023, -1.0 -> -1024
 */
#define Q10_ONE               1023

/**
 * @brief Q10 (Q5.10) representation of -1.0
 */
#define Q10_MINUS_ONE        -1024

/**
 * @brief Q15 representation of 1.0
 */
#define Q15_ONE         		32767    

/**
 * @brief Q15 representation of -1.0
 */
#define Q15_MINUS_ONE       -32768 

/**
 * @brief Q15 constant for 1/v3 (˜ 0.5773503)
 *
 * Computation: 0.5773503 × 32768 ˜ 18919
 */
#define Q15_ONE_OVER_SQRT3   18919

/**
 * @brief Q15 constant for 2/3 (˜ 0.6666667)
 *
 * Computation: 0.6666667 × 32768 ˜ 21845
 */
#define Q15_TWO_OVER_THREE   21845

/**
 * @brief Q15 constant for -1/2 (-0.5)
 *
 * Computation: -0.5 × 32768 = -16384
 */
#define Q15_MINUS_HALF       -16384

/**
 * @brief Q15 constant for v3/2 (˜ 0.8660254)
 *
 * Computation: 0.8660254 × 32768 ˜ 28378
 */
#define Q15_SQRT3_OVER_2     28378

/**
 * @brief Q15 constant for 1/2 (0.5)
 *
 * Computation: 0.5 × 32768 = 16384
 */
#define Q15_HALF             16384

/**
 * @brief Q26 representation of 1.0
 */
#define Q26_ONE         		 67108864    

/**
 * @brief Q26 representation of -1.0
 */
#define Q26_MINUS_ONE      	 -67108864

/**
 * @brief Q31 representation of 1.0
 */
#define Q31_ONE         		 2147483647    

/**
 * @brief Q31 representation of -1.0
 */
#define Q31_MINUS_ONE      	 -2147483647

/**
 * @brief Q31 representation of -1.0
 */
#define Q15_90DEG   				((sint16)16384)


// =============================================================================
// Extreme values of Q10/Q15/Q26/Q31 format
// =============================================================================

/**
 * @brief Max value in Q10 (Q5.10) format
 */
#define Q10_MAX             INT16_MAX

/**
 * @brief Min value in Q10 (Q5.10) format
 */
#define Q10_MIN             INT16_MIN

/**
 * @brief Max value in Q15 format
 */
#define Q15_MAX             INT16_MAX   

/**
 * @brief Min value in Q15 format
 */
#define Q15_MIN             INT16_MIN  

/**
 * @brief Q26 values corresponding to Q15 max 
 */
#define Q26_FROM_Q15_MAX    67106816

/**
 * @brief Q26 values corresponding to Q15 min 
 */
#define Q26_FROM_Q15_MIN    -67108864

/**
 * @brief Max value in Q26 format
 */
#define Q26_MAX             INT32_MAX   

/**
 * @brief Min value in Q15 format
 */
#define Q26_MIN             INT32_MIN  

/**
 * @brief Max value in Q31 format
 */
#define Q31_MAX             INT32_MAX   

/**
 * @brief Min value in Q31 format
 */
#define Q31_MIN             INT32_MIN  

#endif //HEADER_IQ_FORMAT_H