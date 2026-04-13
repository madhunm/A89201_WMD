#ifndef HEADER_FOC_SVPWM_H
#define HEADER_FOC_SVPWM_H
/**
 * @file         foc_svpwm.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Current loop PI controller function
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
 
#include "foc_iq_format.h"

/**
 * @brief SVPWM Handle structure
 *
 * Holds the state, parameters, and results of the SVPWM computation.
 */
typedef struct {
    Q31 Valpha;      /**< a-axis voltage reference (Q15 scaled in Q31) */
    Q31 Vbeta;       /**< ß-axis voltage reference (Q15 scaled in Q31) */
    Q31 Ta;          /**< Phase A duty cycle (Q15) */
    Q31 Tb;          /**< Phase B duty cycle (Q15) */
    Q31 Tc;          /**< Phase C duty cycle (Q15) */
    uint16_t ticksA; /**< Timer compare value for Phase A */
    uint16_t ticksB; /**< Timer compare value for Phase B */
    uint16_t ticksC; /**< Timer compare value for Phase C */
    uint16_t Period; /**< Timer period in ticks */
    uint8_t sector;  /**< SVPWM sector (1–6) */
} SVPWM_Handle;


/**
 * @brief Compute normalized SVPWM duty cycles (Q15) from a–ß voltages.
 *
 * This function implements the core SVPWM logic. It determines the active sector,
 * calculates vector times (T1, T2, T0), and generates normalized duty cycles Ta, Tb, Tc.
 *
 * The outputs Ta, Tb, and Tc are in Q15 format (0–32767), which later can be
 * converted into timer ticks for PWM hardware.
 *
 * @param sv Pointer to SVPWM_Handle structure containing a–ß inputs and result storage.
 */
static inline __attribute__((always_inline))
void SVPWM_Q15(SVPWM_Handle *sv)
{
    // ------------------------------------------------------------------------
    // Step 1: Determine sector from a–ß voltages
    // ------------------------------------------------------------------------
    Q31 Va = sv->Valpha;
    Q31 Vb = (-(sv->Valpha >> 1)) + ((Q15_SQRT3_OVER_2 * sv->Vbeta) >> Q15_SHIFT);
    Q31 Vc = (-(sv->Valpha >> 1)) - ((Q15_SQRT3_OVER_2 * sv->Vbeta) >> Q15_SHIFT);

    if (Va >= Vb) {
        if (Vb >= Vc)        sv->sector = 1;   // Va > Vb > Vc
        else if (Va >= Vc)   sv->sector = 6;   // Va > Vc > Vb
        else                 sv->sector = 5;   // Vc > Va > Vb
    } else {
        if (Va >= Vc)        sv->sector = 2;   // Vb > Va > Vc
        else if (Vb >= Vc)   sv->sector = 3;   // Vb > Vc > Va
        else                 sv->sector = 4;   // Vc > Vb > Va
    }

    // ------------------------------------------------------------------------
    // Step 2: Compute T1/T2 times (Q15)
    // ------------------------------------------------------------------------
    Q31 T1 = 0, T2 = 0;
    switch (sv->sector) {
        case 1: T1 = Va - Vb; T2 = Vb - Vc; break;
        case 2: T1 = Vb - Va; T2 = Va - Vc; break;
        case 3: T1 = Vb - Vc; T2 = Vc - Va; break;
        case 4: T1 = Vc - Vb; T2 = Vb - Va; break;
        case 5: T1 = Vc - Va; T2 = Va - Vb; break;
        case 6: T1 = Va - Vc; T2 = Vc - Vb; break;
    }

    if (T1 < 0) T1 = 0;
    if (T2 < 0) T2 = 0;

    Q31 T0 = Q15_ONE - T1 - T2;
    if (T0 < 0) T0 = 0;

    // ------------------------------------------------------------------------
    // Step 3: Calculate normalized duty cycles (Ta, Tb, Tc) in Q15
    // ------------------------------------------------------------------------
    Q31 halfT0 = T0 >> 1;

    switch (sv->sector) {
        case 1: sv->Ta = T1 + T2 + halfT0; sv->Tb = T2 + halfT0;       sv->Tc = halfT0; break;
        case 2: sv->Ta = T1 + halfT0;      sv->Tb = T1 + T2 + halfT0;  sv->Tc = halfT0; break;
        case 3: sv->Ta = halfT0;           sv->Tb = T1 + T2 + halfT0;  sv->Tc = T2 + halfT0; break;
        case 4: sv->Ta = halfT0;           sv->Tb = T1 + halfT0;       sv->Tc = T1 + T2 + halfT0; break;
        case 5: sv->Ta = T2 + halfT0;      sv->Tb = halfT0;            sv->Tc = T1 + T2 + halfT0; break;
        case 6: sv->Ta = T1 + T2 + halfT0; sv->Tb = halfT0;            sv->Tc = T1 + halfT0; break;
    }

    // Clamp duty cycles to valid Q15 range
    if (sv->Ta > Q15_ONE) sv->Ta = Q15_ONE; if (sv->Ta < 0) sv->Ta = 0;
    if (sv->Tb > Q15_ONE) sv->Tb = Q15_ONE; if (sv->Tb < 0) sv->Tb = 0;
    if (sv->Tc > Q15_ONE) sv->Tc = Q15_ONE; if (sv->Tc < 0) sv->Tc = 0;
}

/**
 * @brief Convert normalized duty cycles (Q15) to timer ticks.
 *
 * This function translates the Q15 duty cycles (Ta, Tb, Tc) into hardware timer
 * compare values (ticks). It should be called after SVPWM_Q15() to apply the
 * calculated duty cycles to the PWM hardware.
 *
 * @param sv Pointer to SVPWM_Handle structure containing duty cycles and timer info.
 */
static inline __attribute__((always_inline))
void SVPWM_ConvertToTicks(SVPWM_Handle *sv)
{
    sv->ticksA = (sv->Ta * sv->Period) >> Q15_SHIFT;
    sv->ticksB = (sv->Tb * sv->Period) >> Q15_SHIFT;
    sv->ticksC = (sv->Tc * sv->Period) >> Q15_SHIFT;
}

#endif //HEADER_FOC_SVPWM_H