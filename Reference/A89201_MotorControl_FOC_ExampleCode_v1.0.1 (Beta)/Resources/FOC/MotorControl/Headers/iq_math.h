#ifndef HEADER_IQ_MATH_H
#define HEADER_IQ_MATH_H
/**
 * @file         iq_math.h
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
#include "iq_format.h"


// =============================================================================
// 1. Basic arithmetic functions
// =============================================================================

/* ----------------------- Saturation helpers ------------------------ */
/**
 * @brief Saturate a 32-bit integer to 16-bit range.
 *
 * @param[in] x  Value to saturate (sint32).
 * @return Saturated value in sint16.
 */
static inline __attribute__((always_inline)) sint16 sat16_from32(sint32 x)
{
    if (x >  32767) return  32767;
    if (x < -32768) return -32768;
    return (sint16)x;
}

/**
 * @brief Saturate a 64-bit integer to 32-bit range.
 *
 * @param[in] x  Value to saturate (sint64).
 * @return Saturated value in sint32.
 */
static inline __attribute__((always_inline)) sint32 sat32_from64(sint64 x)
{
    if (x >  (sint64)INT32_MAX) return  INT32_MAX;
    if (x <  (sint64)INT32_MIN) return  INT32_MIN;
    return (sint32)x;
}

/**
 * @brief Signed right shift with symmetric rounding to nearest.
 *
 * Performs y = round(x / 2^s) with symmetric rounding (ties away from zero).
 * If s <= 0, performs left shift by (-s) with saturation to sint32.
 *
 * @param[in] x  Numerator (sint64).
 * @param[in] s  Shift count (can be negative).
 * @return Rounded and shifted result in sint32.
 */
static inline __attribute__((always_inline)) sint32 shr_rnd_s64_to_s32(sint64 x, sint32 s)
{
    if (s <= 0) return sat32_from64(x << (-s));
    sint64 bias = (x >= 0) ? ( (sint64)1 << (s - 1) ) : -( (sint64)1 << (s - 1) );
    return sat32_from64( (x + bias) >> s );
}

/* ---------------------------------- Q15 ----------------------------------- */

/**
 * @brief Absolute value for Q15
 * @param x Q15 input value
 * @return Absolute value of x (Q15)
 */
static inline __attribute__((always_inline)) sint16 q15_Abs(sint16 x_q15) { return (x_q15 < 0) ? (sint16)(-x_q15) : x_q15; }


/**
 * @brief Multiply two Q15 numbers with symmetric rounding and saturation.
 *
 * @param[in] a_q15  Operand A (Q15).
 * @param[in] b_q15  Operand B (Q15).
 * @return Product in Q15 (sint16).
 */
static inline __attribute__((always_inline)) sint16 q15_mul(sint16 a_q15, sint16 b_q15)
{
    sint32 p = (sint32)a_q15 * (sint32)b_q15;                 /* Q30 */
    sint32 r = (p >= 0) ? (p + (1 << (Q15_SHIFT - 1)))        /* +2^14 */
                        : (p - (1 << (Q15_SHIFT - 1)));
    return sat16_from32(r >> Q15_SHIFT);
}

/**
 * @brief Divide two Q15 numbers with symmetric rounding and saturation.
 *
 * Computes (a/b) in Q15. If b == 0, returns signed max/min.
 *
 * @param[in] a_q15  Numerator (Q15).
 * @param[in] b_q15  Denominator (Q15).
 * @return Quotient in Q15 (sint16).
 */
static inline __attribute__((always_inline)) sint16 q15_div(sint16 a_q15, sint16 b_q15)
{
    if (b_q15 == 0) return (a_q15 >= 0) ? 32767 : -32768;
    sint32 num  = (sint32)a_q15 << Q15_SHIFT;                 /* Q30 */
    sint32 half = (b_q15 >= 0) ? (b_q15 >> 1) : -((sint32)(-b_q15) >> 1);
    sint32 q = ( ((num ^ b_q15) >= 0) ? (num + half) : (num - half) ) / b_q15;
    return sat16_from32(q);
}

/**
 * @brief Square a Q15 number with symmetric rounding and saturation.
 *
 * @param[in] x_q15  Input value (Q15).
 * @return x^2 in Q15 (sint16).
 */
static inline __attribute__((always_inline)) sint16 q15_square(sint16 x_q15)
{
    return q15_mul(x_q15, x_q15);
}

/**
 * @brief Square root for non-negative Q15 values.
 *
 * Uses integer Newton iterations on S = X * 2^15.
 *
 * @param[in] x_q15  Input (Q15), x >= 0.
 * @return sqrt(x) in Q15 (sint16).
 */
static inline __attribute__((always_inline)) sint16 q15_sqrt(sint16 x_q15)
{
    if (x_q15 <= 0) return 0;
    uint64 S = ((uint64)(uint16)x_q15) << Q15_SHIFT;          /* X * 2^15 */
    uint64 y = (S > 1) ? (S >> 1) : 1;
    for (sint32 i = 0; i < 8; ++i) {
        uint64 yn = (y + S / y) >> 1;
        if (yn == y) break;
        y = yn;
    }
    if (y > 32767) y = 32767;
    return (sint16)y;
}


/* ---------------------------------- Q31 ----------------------------------- */
/**
 * @brief Multiply two Q31 numbers with symmetric rounding and saturation.
 *
 * @param[in] a_q31  Operand A (Q31).
 * @param[in] b_q31  Operand B (Q31).
 * @return Product in Q31 (sint32).
 */
static inline __attribute__((always_inline)) sint32 q31_mul(sint32 a_q31, sint32 b_q31)
{
    sint64 p = (sint64)a_q31 * (sint64)b_q31;                 /* Q62 */
    return shr_rnd_s64_to_s32(p, Q31_SHIFT);
}

/**
 * @brief Divide two Q31 numbers with symmetric rounding and saturation.
 *
 * @param[in] a_q31  Numerator (Q31).
 * @param[in] b_q31  Denominator (Q31).
 * @return Quotient in Q31 (sint32). If b == 0, returns signed max/min.
 */
static inline __attribute__((always_inline)) sint32 q31_div(sint32 a_q31, sint32 b_q31)
{
    if (b_q31 == 0) return (a_q31 >= 0) ? INT32_MAX : INT32_MIN;
    sint64 num  = (sint64)a_q31 << Q31_SHIFT;                 /* Q62 */
    sint64 half = (b_q31 >= 0) ? ((sint64)b_q31 >> 1) : -((sint64)(-b_q31) >> 1);
    if ((num ^ b_q31) >= 0) num += half; else num -= half;
    return sat32_from64(num / b_q31);
}

/**
 * @brief Square a Q31 number with symmetric rounding and saturation.
 *
 * @param[in] x_q31  Input value (Q31).
 * @return x^2 in Q31 (sint32).
 */
static inline __attribute__((always_inline)) sint32 q31_square(sint32 x_q31)
{
    return q31_mul(x_q31, x_q31);
}

/**
 * @brief Square root for non-negative Q31 values.
 *
 * Uses integer Newton iterations on S = X * 2^31.
 *
 * @param[in] x_q31  Input (Q31), x >= 0.
 * @return sqrt(x) in Q31 (sint32).
 */
static inline __attribute__((always_inline)) sint32 q31_sqrt(sint32 x_q31)
{
    if (x_q31 <= 0) return 0;
    uint64 S = ((uint64)x_q31) << Q31_SHIFT;                  /* X * 2^31 */
    uint64 y = (S > 1) ? (S >> 1) : 1;
    for (sint32 i = 0; i < 12; ++i) {
        uint64 yn = (y + S / y) >> 1;
        if (yn == y) break;
        y = yn;
    }
    if (y > (uint64)INT32_MAX) return INT32_MAX;
    return (sint32)y;
}


// =============================================================================
// 2. Theta Wrap functions
// =============================================================================


/**
 * @brief Normalize a Q15 electrical angle to the symmetric range [-pi, +pi].
 *
 * @details
 *  - Input/Output are Q15 angles where -32768 maps to -pi and +32767 maps to +pi (exclusive).
 *  - Performs modulo-2pi wrapping (mod 65536) so the result always lies in [-32768, +32767].
 *  - This is NOT saturation/clamping; it preserves angle periodicity.
 *  - Typical use: after integrating angle each control tick, call this to keep the representation bounded.
 *
 * @param theta_q15  Angle in Q15 (can be any integer value; multi-turn values allowed).
 * @return           Normalized Q15 angle in [-32768, +32767].
 */
static inline __attribute__((always_inline))
	sint16 q15_Wrap_Theta(sint16 theta_q15)
{
    // Branchless modulo wrap to [-32768, +32767]
    uint32 u = (uint32)((sint32)theta_q15 + 32768) & 0xFFFFu; // 0..65535
    return (sint16)((sint32)u - 32768);
}

#endif //HEADER_IQ_MATH_H