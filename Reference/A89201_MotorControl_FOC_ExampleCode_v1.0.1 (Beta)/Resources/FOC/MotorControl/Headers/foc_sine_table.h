#ifndef HEADER_FOC_SINE_TABLE_H
#define HEADER_FOC_SINE_TABLE_H
/**
 * @file         foc_sine_table.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Sine function look-up table
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

// =============================================================================
//  Lookup Table (LUT) Setup
// =============================================================================

/**
 * @brief Lookup table size (must be power of 2)
 *
 * Example: 4096 points for full sine wave (0–2p).
 */
#define LUT_SIZE        4096

/**
 * @brief Quarter-wave offset for cosine lookup
 *
 * Cosine can be obtained from sine table by adding a quarter-wave offset.
 */
#define LUT_QUARTER     (LUT_SIZE / 4)

/**
 * @brief Global sine lookup table (Q15)
 *
 * - Table length: @ref LUT_SIZE
 * - Format: Q15 (-32768 to 32767)
 * - Range: -pi to pi mapped over LUT_SIZE indices
 */
extern const sint16 sineTableQ15[LUT_SIZE];

/**
 * @brief Q15 sine lookup
 *
 * @param idx LUT index (0–LUT_SIZE-1)
 * @return Q15 sine value for given index
 */
static inline __attribute__((always_inline))
sint16 FOC_SinLutQ15(uint16 idx)
{
    return sineTableQ15[idx & (LUT_SIZE - 1)];
}

/**
 * @brief Q15 cosine lookup (derived from sine table)
 *
 * Uses quarter-wave phase shift to compute cosine from sine LUT.
 *
 * @param idx LUT index (0–LUT_SIZE-1)
 * @return Q15 cosine value for given index
 */
static inline __attribute__((always_inline))
	sint16 FOC_CosLutQ15(uint16 idx)
{
    return sineTableQ15[(idx + LUT_QUARTER) & (LUT_SIZE - 1)];
}

/**
 * @brief Convert Q15 theta (-32768..32767 ˜ -pi..pi) to 4096-point index.
 *
 * Equivalent to: ((Theta + 32768) >> 4) & 0x0FFF.
 *
 * @param[in] theta_q15 Electrical angle in Q15 format.
 * @return Electrical angle index [0..4095].
 */
static inline __attribute__((always_inline)) 
	uint16 thetaQ15_to_idx(sint16 theta_q15)
{
    sint32 u_q15 = (sint32)theta_q15 + 32768;     /* Shift to 0..65535 unsigned range */
    return (uint16)(((uint32)u_q15 >> 4U) & (LUT_SIZE - 1));
}


#endif //HEADER_FOC_SINE_TABLE_H