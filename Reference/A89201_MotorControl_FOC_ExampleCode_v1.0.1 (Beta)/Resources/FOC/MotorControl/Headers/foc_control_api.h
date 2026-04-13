#ifndef HEADER_FOC_CONTROL_API_H
#define HEADER_FOC_CONTROL_API_H

/**
 * @file         foc_control_api.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        External configuration API for foc control parameters
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
#include "driver_gdu.h"

/**
 * @brief Opaque runtime handle for the FOC module (forward declaration).
 *
 * The complete definition of @c struct MotorCtrl is kept private to the FOC
 * implementation. External modules may hold and pass @c MotorCtrl* handles,
 * but must not dereference members directly.
 */
typedef struct MotorCtrl MotorCtrl;


/* -------------------------------------------------------------------------- */
/* API configuration provided by upper layer                                  */
/* -------------------------------------------------------------------------- */
/**
 * @brief User/upper-layer configuration for the FOC pipeline.
 *
 * All numeric values are fixed-point (Q15/Q31) unless noted otherwise.
 * Default values set in @ref Foc_Init() are safe zeros (no motion).
 * Fill/adjust this structure before enabling RUN (i.e., before setting
 * @ref MotorCtrlApiConfig::cmd to 1).
 */
typedef struct
{
		uint16	pwmPeriod;    					/**< Pwm Period */
		uint16	pwmDeadtime;    				/**< Pwm Dead-time */
		/* --- Global gates --- */
		boolean cmd;                   	/**< Run command: 0=STOP, 1=RUN. */
		boolean dir;                   	/**< Direction: 0=FWD, 1=REV. */

		/* --- IDLE phase --- */
		uint32 	u32idleBlankDuration;   /**< Blank ticks to let currents decay before starting. */

		/* --- ALIGN phase --- */
		sint16  alignCurrent_q15;       /**< Id during ALIGN (Q15, per-unit). */
		sint16  alignAngle_q15;         /**< Electrical angle during ALIGN (Q15, -pi..+pi). */
		uint32 	u32alignDuration;       /**< ALIGN duration in control ticks. */

		/* --- RAMP-UP (open-loop) --- */
		sint16  iqRampupTarget_q15;     /**< Target Iq magnitude for ramp-up (Q15). */
		sint16  omegaRampupTarget_q15;  /**< Target |omega| for ramp-up (Q15). */
		sint32  omegaRampupStep_q31;    /**< |omega| step per tick (Q31, non-negative). */
		sint32  KoemgaToTheta_q15;      /**< Gain to convert omega->dtheta per tick (Q15). */
		uint32 	u32rampupDuration;      /**< Ramp-up duration in control ticks. */

		/* --- SWITCHING (handover) --- */
		sint16  iqSwitchMin_q15;        /**< Minimal |Iq| (floor) during handover (Q15). */
		sint32  iqSwitchStep_q31;       /**< Iq ramp step per tick (Q31, used by IQ15_Ramp). */
		sint16  thetaErrTh_q15;         /**< |theta_smo - theta_ol| threshold (Q15). */
		uint32 	u32ObsReadyTicks;       /**< Observer-ready hold ticks above threshold. */
		uint32 	u32ErrTimeoutTicks;     /**< Theta error timeout ticks above threshold. */
		uint32 	u32SpdLpReadyTicks;     /**< Speed-loop ready hold ticks. */

		/* --- Protections & scaling --- */
		sint16  ImaxLimit_q15;          /**< Per-phase current limit abs (Q15). */
		uint32 	u32OvrCurrDebounceN;    /**< OC debounce ticks (consecutive). */
		sint16  VmaxLimit_q15;          /**< Max |Vdq| circle limit (Q15). */
		uint32  u32miliAToAdcCount;   	/**< current (mA) -> ADC counts gain (CAU). */
		sint16  AdcToCurrent_q15;       /**< ADC counts -> current per-unit gain (Q15). */
		uint32  u32DauToVbbMicroV; 			/**< ADC counts gain (DAU) -> VBB (micro V). */
		sint16  AdcToVoltage_q15;       /**< ADC counts -> voltage per-unit gain (Q15). */
		
		/* --- Speed loop command & scaling --- */
		sint16  SpdCmd_q15;             /**< External speed command (Q15). */
		sint32  OmegaPuToRpmPu_q15; 		/**< Omega(p.u.) -> speed feedback (p.u.) (Q15 gain). */
		
		/* --- Theta error compensation due to time delay --- */
		sint16  K_Td_q15;             	/**< Time delay factor on theta correction (Q15). */	
		
} MotorCtrlApiConfig;


typedef struct
{
		sint16 ADCTrig1;								/**< ADC trigger point 1, defines the timing of the first current sampling trigger. */
		sint16 ADCTrig2;								/**< ADC trigger point 2, defines the timing of the second current sampling trigger. */
		sint16 PWMA_Duty1;							/**< PWM phase A duty cycle for high-side. */
		sint16 PWMA_Duty2;							/**< PWM phase A duty cycle for low-side. */
		sint16 PWMB_Duty1;							/**< PWM phase B duty cycle for high-side. */
		sint16 PWMB_Duty2;							/**< PWM phase B duty cycle for low-side. */
		sint16 PWMC_Duty1;							/**< PWM phase C duty cycle for high-side. */
		sint16 PWMC_Duty2;							/**< PWM phase C duty cycle for low-side. */
	
} MotorCtrlApiPwm;

/**
 * @brief Get a mutable handle to the module's configuration singleton.
 * @return Pointer to the module-private @ref MotorCtrlApiConfig instance.
 */
volatile	MotorCtrlApiConfig* FOC_GetConfig(void);

/**
 * @brief Get the opaque runtime handle (singleton) of the FOC module.
 * @return Pointer to the module-private @ref MotorCtrl instance.
 */
MotorCtrl*          FOC_GetMotor(void);

/**
 * @brief One-time bring-up for the FOC module.
 *
 * Initializes internal singletons and submodules (PI/SMO/PLL/SVPWM) and
 * leaves the state machine in @c IDLE with @c cfg->cmd = 0 (STOP).
 */
void                Foc_Init(void);

/**
 * @brief ADC fast-loop ISR entry point.
 * @note Call from ISR context only.
 */
void Foc_Cau_Handler(void);

/**
 * @brief Timer speed-loop ISR entry point.
 * @note Call from ISR context only; not re-entrant.
 */
void Speed_Loop_Gtu_Handler(void);

/**
 * @brief Dau Vbb Compensation ISR entry point.
 * @note Call from ISR context only; not re-entrant.
 */
void VbbComp_Dau_Handler(void);

/**
 * @brief Set differential amplifier gain.
 */
void MotorControl_SetDiffAmpGain (uint16 gain);

/**
 * @brief Set PWM period (ticks).
 */
void MotorControl_SetPwmPeriod(uint16 period);

/**
 * @brief Set PWM deadtime (ticks).
 */
void MotorControl_SetDeadtime(uint16 deadtime);

/**
 * @brief Set run command (0=STOP, 1=RUN).
 */
void MotorControl_SetRun(boolean run);

/**
 * @brief Set motor rotation direction (0=FWD, 1=REV).
 */
void MotorControl_SetDir(boolean rev);

/**
 * @brief Set IDLE blanking duration (ticks).
 */
void MotorControl_SetIdleBlank(uint32 idle);

/**
 * @brief Set ALIGN d-axis current (Q15).
 */
void MotorControl_SetAlignCurrent_Q15(sint16 id_q15);

/**
 * @brief Set ALIGN electrical angle (Q15).
 */
void MotorControl_SetAlignAngle_Q15(sint16 angle_q15);

/**
 * @brief Set ALIGN duration (ticks).
 */
void MotorControl_SetAlignDuration(uint32 ticks);

/**
 * @brief Set ramp-up target |Iq| (Q15).
 */
void MotorControl_SetRampCurrent_Q15(sint16 iq_q15);

/**
 * @brief Set ramp-up target |omega| (Q15).
 */
void MotorControl_SetRampOmegaTarget_Q15(sint16 omega_q15);

/**
 * @brief Set ramp-up |omega| step per tick (Q31).
 */
void MotorControl_SetRampOmegaStep_Q31(sint32 step_q31);

/**
 * @brief Set omega-to-theta conversion gain per tick (Q15).
 */
void MotorControl_SetOmega2Theta_Q15(sint16 dtheta_q15);

/**
 * @brief Set ramp-up duration (ticks).
 */
void MotorControl_SetRampDuration(uint32 ticks);

/**
 * @brief Set minimal Iq floor for switching (Q15).
 */
void MotorControl_SetSwitchMinIq_Q15(sint16 iq_q15);

/**
 * @brief Set Iq ramp step per tick during switching (Q31).
 */
void MotorControl_SetSwitchIqStep_Q31(sint32 step_q31);

/**
 * @brief Set switching angle error threshold (Q15).
 */
void MotorControl_SetSwitchThetaErr_Q15(sint16 threshold_q15);

/**
 * @brief Set observer-ready hold duration (ticks).
 */
void MotorControl_SetSwitchObsReadyTicks(uint32 ticks);

/**
 * @brief Set theta error time-out time (ticks).
 */
void MotorControl_SetErrTimeoutTicks(uint32 ticks);

/**
 * @brief Set speed-loop-ready hold duration (ticks).
 */
void MotorControl_SetSwitchSpdLpRdyCount(uint32 ticks);

/**
 * @brief Set per-phase current limit |I| (Q15).
 */
void MotorControl_SetCurrentLimit_Q15(sint16 imax_q15);

/**
 * @brief Set over-current debounce duration (ticks).
 */
void MotorControl_SetOverCurrDebounce(uint32 ticks);

/**
 * @brief Set |Vdq| circular limit (Q15).
 */
void MotorControl_SetVdqMax_Q15(sint16 voltage_q15);

/**
 * @brief Set ADC-to-current per-unit gain (Q15).
 */
void MotorControl_SetAdc2Ibase_Q15(sint16 gain_q15);

/**
 * @brief Set DAU ADC counts to VBB (microV) gain (uint32).
 */
void MotorControl_SetDauAdc2VbbMicroV(uint32 gain);

/**
 * @brief Set ADC-to-voltage per-unit gain (Q15).
 */
void MotorControl_SetAdc2Vbase_Q15(sint16 gain_q15);

/**
 * @brief Set external speed command (Q15).
 */
void MotorControl_SetSpdCmd_Q15(sint16 speed_q15);

/**
 * @brief Set omega-to-RPM scaling coefficient (Q15).
 */
void MotorControl_SetOmega2Rpm_Q15(sint16 coeff_q15);

/**
 * @brief Set Id PI proportional gain (Q15).
 */
void MotorControl_SetIdKp_Q15(sint16 kp_q15);

/**
 * @brief Set Id PI integral gain (Q15).
 */
void MotorControl_SetIdKi_Q15(sint16 ki_q15) ;

/**
 * @brief Set Iq PI proportional gain (Q15).
 */
void MotorControl_SetIqKp_Q15(sint16 kp_q15) ;

/**
 * @brief Set Iq PI integral gain (Q15).
 */
void MotorControl_SetIqKi_Q15(sint16 ki_q15);

/**
 * @brief Set Id PI output limit |Vdq| (Q15).
 */
void MotorControl_SetIdPiVoutMax_Q15(sint16 vmax_q15);

/**
 * @brief Set Iq PI output limit |Vdq| (Q15).
 */
void MotorControl_SetIqPiVoutMax_Q15(sint16 vmax_q15);

/**
 * @brief Set speed PI proportional gain (Q15).
 */
void MotorControl_SetSpeedPiKp_Q15(sint16 spdKp_q15);

/**
 * @brief Set speed PI integral gain (Q15).
 */
void MotorControl_SetSpeedPiKi_Q15(sint16 spdKi_q15);

/**
 * @brief Set speed PI output maximum (Q15).
 */
void MotorControl_SetSpeedPiOutMax_Q15(sint16 outmax_q15);

/**
 * @brief Set speed PI output minimum (Q15).
 */
void MotorControl_SetSpeedPiOutMin_Q15(sint16 outmin_q15);

/**
 * @brief Set speed ramp acceleration slope (Q31/tick).
 */
void MotorControl_SetSpeedAcc_Q31(sint32 acc_q31);

/**
 * @brief Set speed ramp deceleration slope (Q31/tick).
 */
void MotorControl_SetSpeedDec_Q31(sint32 deacc_q31);

/**
 * @brief Set ADC minimum sampling window (Q15).
 */
void MotorControl_SetMinSampleTime_Q15(sint16 time_q15);

/**
 * @brief Set the DC bus nominal voltage (mV).
 */
void MotorControl_SetVBBmiliVolt(uint32 voltage);

/**
 * @brief Set SMO current decay factor (Q15).
 */
void MotorControl_SetSmoDecay_Q15(sint16 smodecay_q15);

/**
 * @brief Set SMO voltage gain (Q15).
 */
void MotorControl_SetSmoVgain_Q15(sint16 smovgain_q15) ;

/**
 * @brief Set SMO sliding gain (Q15).
 */
void MotorControl_SetSmoKslide_Q15(sint16 gain_q15);

/**
 * @brief Set SMO linear region threshold (Q15).
 */
void MotorControl_SetSmoMaxErr_Q15(sint16 max_q15);

/**
 * @brief Set SMO back-EMF low-pass filter coefficient (Q15).
 */
void MotorControl_SetSmoEmfK_Q15(sint16 cutoff_q15);

/**
 * @brief Set PLL proportional gain (Q15).
 */
void MotorControl_SetPllKp_Q15(sint16 kpPll_q15);

/**
 * @brief Set PLL integral gain (Q15).
 */
void MotorControl_SetPllKi_Q15(sint16 kiPll_q15);

/**
 * @brief Set time delay compensation factor for theta correction(Q15).
 */
void MotorControl_SetKTimeDelay_Q15(sint16 k_td_q15);

/* ============================================================================
 * Motor Control - Getters (Runtime Monitoring APIs)
 * ==========================================================================*/

/**
 * @brief Get measured Id (Q15).
 */
sint16 MotorControl_GetId_Q15(void);

/**
 * @brief Get measured Iq (Q15).
 */
sint16 MotorControl_GetIq_Q15(void);

/**
 * @brief Get Id reference (Q15).
 */
sint16 MotorControl_GetIdRef_Q15(void);

/**
 * @brief Get Iq reference (Q15).
 */
sint16 MotorControl_GetIqRef_Q15(void);

/**
 * @brief Get commanded Vd (Q15).
 */
sint16 MotorControl_GetVd_Q15(void);

/**
 * @brief Get commanded Vq (Q15).
 */
sint16 MotorControl_GetVq_Q15(void);

/**
 * @brief Get normalized SVPWM duty T1 (Q15).
 */
sint16 MotorControl_GetT1_Q15(void);

/**
 * @brief Get normalized SVPWM duty T2 (Q15).
 */
sint16 MotorControl_GetT2_Q15(void);

/**
 * @brief Get SVPWM sector (1..6).
 */
uint8 MotorControl_GetSector(void);

/**
 * @brief Get reconstructed phase current Ia (Q15).
 */
sint16 MotorControl_GetIa_Q15(void);

/**
 * @brief Get reconstructed phase current Ib (Q15).
 */
sint16 MotorControl_GetIb_Q15(void);

/**
 * @brief Get reconstructed phase current Ic (Q15).
 */
sint16 MotorControl_GetIc_Q15(void);

/**
 * @brief Get open-loop electrical angle theta (Q15).
 */
sint16 MotorControl_GetThetaOl_Q15(void);

/**
 * @brief Get SMO-estimated angle theta (Q15).
 */
sint16 MotorControl_GetThetaSmo_Q15(void);

/**
 * @brief Get used angle theta (Q15).
 */
sint16 MotorControl_GetThetaUsed_Q15(void);

/**
 * @brief Get open-loop angular speed omega (Q15).
 */
sint16 MotorControl_GetOmegaOl_Q15(void);

/**
 * @brief Get used angular speed omega (Q15).
 */
sint16 MotorControl_GetOmegaUsed_Q15(void);

/**
 * @brief Get speed reference (Q15).
 */
sint16 MotorControl_GetSpeedRef_Q15(void);

/**
 * @brief Get measured speed feedback (Q15).
 */
sint16 MotorControl_GetSpeed_Q15(void);

/**
 * @brief Get measured DC bus voltage (mV).
 */
uint32 MotorControl_GetVbus_MiliVolt(void);

#endif // HEADER_FOC_CONTROL_API_H