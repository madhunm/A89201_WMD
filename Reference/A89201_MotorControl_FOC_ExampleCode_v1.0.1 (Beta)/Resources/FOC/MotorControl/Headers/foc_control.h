#ifndef HEADER_FOC_CONTROL_H
#define HEADER_FOC_CONTROL_H
/**
 * @file         foc_control.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Implement FOC control (3steps: Align, Rampup, Closeloop)
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
#include "foc_math.h"
#include "foc_control_api.h"
#include "foc_sine_table.h"
#include "foc_current_loop.h"
#include "smo_pos.h"
#include "single_shunt.h"
#include "speed_loop.h"


/**
 * @brief High-level motor control states (FOC state machine).
 *
 */
typedef enum
{
    MOTOR_STATE_IDLE = 0,       /**< Stopped/standby: blank-time & ADC offset tracking.            */
    MOTOR_STATE_ALIGN,          /**< Flux alignment at a fixed electrical angle (Id?0, Iq=0).      */
    MOTOR_STATE_RAMPUP,         /**< Open-loop acceleration (I/F startup, theta integrated O.L.).  */
    MOTOR_STATE_SWITCHING,      /**< Hand-over: soften torque, gate checks, blend angle/commands.  */
    MOTOR_STATE_CLOSELOOP,      /**< Closed-loop FOC: theta from observer, Iq/Id from controllers. */
	
} MotorCtrlState;

/**
 * @brief Sub-phases used during the SWITCHING state.
 *
 * Notes:
 * - These phases smooth the transition from open-loop to closed-loop.
 * - Keep the order stable; code may rely on 0/1/2 values.
 */
typedef enum {
	
    SWITCH_DROP_IQ = 0,      		/* Step-1: reduce |Iq|, keep sign; use open-loop angle */
    SWITCH_WAIT_SPEED    				/* Step-2: delay a few ticks, then enable speed loop */
	
} MotorCtrlSwitchPhase;


/**
 * @brief ADC results and calibration data for three phases (A/B/C).
 *
 * Conventions:
 * - Type sint16 is assumed to be a 16-bit signed integer.
 */
typedef struct 
{
    /* Runtime-tracked ADC zero offsets (counts). Typically updated in IDLE. */
    sint16 s16offsetA;
    sint16 s16offsetB;
    sint16 s16offsetC;
    /* Latest ADC readings ("normal" operation). */
    sint16 s16normalA;
    sint16 s16normalB;
    sint16 s16normalC;
    /* Calibration values */
    sint16 s16calibA;
    sint16 s16calibB;
    sint16 s16calibC;
} AdcResults;



/**
 * @brief Motor controller runtime container for the entire FOC pipeline.
 *
 * Holds fast-loop scratch, state machine state, references, observers, PIs,
 * and SVPWM IO. All values are fixed-point and per-unit where applicable.
 */
struct MotorCtrl
{
		/* --- Fast path ADC & current reconstruction --- */
		AdcResults						foc_AdcResults;          	/**< ADC offsets (raw counts). */
		SingleShunt_Current		currentReconstruction;    /**< Single-shunt reconstruction. */

		/* --- Clarke/Park scratch --- */
		CLARKE_Q15        		currentClarke;        		/**< Currents for Clarke. */
		PARK_Q15          		currentPark;          		/**< Currents for Park. */
		PARK_INV_Q15       		voltageParkInv;       		/**< Voltages for Inv Park. */

		/* --- References & measured currents --- */
		sint16                idRef_q15;                /**< Id reference (Q15). */
		sint16                iqRef_q15;                /**< Iq reference (Q15). */
		sint32                iqRef_q31;                /**< Iq reference (Q31). */
		sint16                measuredId_q15;           /**< Measured Id (Q15). */
		sint16                measuredIq_q15;           /**< Measured Iq (Q15). */

		/* --- Angle / speed (open-loop & observer) --- */
		sint16                theta_ol_q15;             /**< Open-loop electrical angle (Q15). */
		sint16                theta_used_q15;           /**< Angle used for Park/SVPWM (Q15). */
		sint16               	theta_smo_q15 ;           /**< Observer/PLL electrical angle (Q15). */
		sint16                omega_ol_q15;             /**< Open-loop omega (Q15). */
		sint32                omega_ol_q31;             /**< Open-loop omega (Q31). */
		sint16								omega_smo_q15;						/**< Observer/PLL electrical omega (Q15). */
		sint16                omega_used_q15;           /**< Omega used by control (Q15). */
		sint16                dtheta_q15;              	/**< dtheta per tick from omega (Q15). */
		uint16             		thetaIdx_ol;              /**< LUT index for open-loop angle. */
		uint16             		thetaIdx_used;            /**< LUT index used for transforms. */
		sint16								thetaCmpErr_q15;					/**< Theta compare error between smo and open loop (Q15) */

		/* --- Observer & PLL --- */
		SMO              			smo;                      /**< Sliding Mode Observer. */
		PLL              			pll;                      /**< PLL for angle/speed. */

		/* --- PI controllers --- */
		CURRENT_PI     				pi_d;                 		/**< D-axis current PI. */
		CURRENT_PI     				pi_q;                 		/**< Q-axis current PI. */

		/* --- SVPWM IO --- */
		SingleShunt_SVPWM_Input    		svpwmInput;       /**< Valpha/Vbeta input. */
		SingleShunt_SVPWM_Output    	svpwmOutput;      /**< Duty outputs. */
		SingleShunt_SVPWM_Normalized  svpwmNorm;        /**< Normalization/state. */

		/* --- Speed loop (timer ISR) --- */
		SPEED_LOOP_PI  				speedLoopPi;              /**< Speed loop PI. */
		SPEED_RAMP_UP  				speedLoopRampup;          /**< Speed command ramp. */
		boolean             	bSpeedLpEnFlag;           /**< Speed loop enable flag. */
		boolean             	bSpdInitPending;          /**< Speed loop I-term init pending flag. */
		sint16                iqSpdLoopInit_q15;        /**< Handover Iq seed for speed loop. */

		/* --- State machine & counters --- */
		MotorCtrlState      	state;                    /**< Current FOC state. */
		MotorCtrlSwitchPhase	switchPhase;              /**< Sub-state during SWITCHING. */
		uint8            			idlePhase;                /**< 0=blank, 1=offset track. */
		uint32             		u32IdleCounter;           /**< Idle blank/offset counter. */
		uint32             		u32AlignCounter;          /**< ALIGN duration counter. */
		uint32             		u32RampUpCounter;         /**< RAMP-UP duration counter. */
		uint32             		u32ObserveHoldCounter;    /**< Observer-ready hold counter. */
		uint32             		u32ErrTimeOutCounter;     /**< Theta error time-out counter. */
		uint32             		u32SpdLoopHoldCounter;    /**< Speed-loop-ready hold counter. */

		/* --- Switching blend helpers --- */
		sint16                iqBlendHold_q15;          /**< Frozen Iq during angle blend (Q15). */

		/* --- Protections --- */
		boolean               bfocOvrCurrFlag;          /**< Latched over-current flag. */
		uint32             		u32OvrCurrCounter;        /**< OC debounce counter. */
		
		/* --- Vbb compensation --- */
		uint32								u32Vbase_mV;							/**< Nominal bus voltage in millivolts (e.g., 12000 for 12 V) */
		uint32								u32Vactual_mV;						/**< Actual bus voltage in millivolts */
		sint32								Vgain_q10;								/**< Voltage compensation gain in Q5.10 format (= Vbase_mV / Vactual_mV). */
};


#endif //HEADER_FOC_CONTROL_H