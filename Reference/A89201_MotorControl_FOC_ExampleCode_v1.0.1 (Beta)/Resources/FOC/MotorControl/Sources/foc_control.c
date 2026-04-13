/**
 * @file         foc_control.c
 * @author       B. Shao
 * @version      1.0
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
 * ---------------------------------------------------------------------------------------------------------------------------- 
 * This Software License Agreement (“Agreement”) is made between                                                              
 * Allegro MicroSystems, LLC (“Allegro”) and the user identified in                                                           
 * the signature block below ("Licensee") (each a "Party" and collectively, the "Parties")                                    
 * on the date of the signature below.      

 * LICENSE TERMS:
 *    a. Exclusively in conjunction with Licensee’s development and sale of products 
 *       containing the Application, Allegro grants Licensee a limited, non-exclusive, 
 *       non-transferable, revocable, and royalty-free license under the copyright rights 
 *       of Allegro or its licensor to (i) use the Software Deliverables to develop the Application,
 *       (ii) use the Application in conjunction with the Library Files, solely in a non-production 
 *       environment for the exclusive purpose of evaluating the Application.   
 * 
 *   b. Licensee recognizes that the Software (i) has not been released by Allegro for general distribution or sale,
 *      and that Allegro has not completed its testing and quality assurances, and (ii) is not qualified and has not 
 *      been certified to meet any industry standards. Licensee shall not use the Software provided hereunder in 
 *      any production environment. Licensee will not disclose or provide the Software or Application to any 
 *      third party, and will defend, indemnify and hold Allegro harmless from all  damages, costs, liabilities 
 *      and expenses related to any third party claims arising from Licensee's breach of this Agreement. 
 *      Further, Licensee will not redistribute the Software or Application to any party, 
 *      including individuals within Licensee’s organization.
 *
 *   c.  In no event will Licensee use the Software in conjunction with any devices (including competitor devices) 
 *       other than the Allegro device identified in the Allegro documentation accompanying the Software or notified
 *       to Licensee by Allegro in writing.
 *
 *   d. Licensee acknowledges that the Software and all related technical information, documents and materials
 *      are subject to export controls under the U.S. Export Administration Regulations.  Licensee will (i) comply 
 *      strictly with all legal requirements established under these controls, (ii) cooperate fully with Allegro in 
 *      any official or unofficial audit or inspection that relates to these controls and (iii) not export, re-export,
 *      divert or transfer, directly or indirectly, any such item or direct products thereof to Crimea, Iran, North Korea,
 *      Sudan, Syria or any country that is embargoed by Executive order, unless Licensee has obtained the prior written 
 *      authorization of Allegro and the U.S. Commerce Department.  
 *
 * CONFIDENTIAL TERMS:
 *      a. Licensee acknowledges and agrees that all information regarding the Software, the results of testing and 
 *         evaluation hereunder, the Feedback provided hereunder, the characteristics and performance of the Software 
 *         and Application (including all related documentation) and this Agreement (collectively, the “Confidential Information”) 
 *         are all confidential information and contain trade secrets of Allegro.  Licensee acknowledges that the Agreement will 
 *         induce Allegro to make such Confidential Information available to Licensee.
 *
 *     This distribution is designated as EAR99.  
 * ---------------------------------------------------------------------------------------------------------------------------- 
 *
 */
 

/**
 * @brief Compile-time switch for enabling SEGGER RTT debug output.
 *
 * Values:
 *   - 0 : Disable SEGGER RTT module (no debug output)
 *   - 1 : Enable SEGGER RTT module (debug output via RTT)
 */
#ifndef Segger_RTT_Enable

#define Segger_RTT_Enable 	0

#endif

#include "Platform_Types.h"
#include "Cau.h"
#include "Dau.h"
#include "Gtu.h"
#include "Gpio.h"
#include "driver_pwm.h"
#include "foc_control.h"
#include "foc_control_api.h"

#if Segger_RTT_Enable
#include "SEGGER_RTT_TEST.h"
#endif

/**
 * @brief Module-private runtime container for the motor FOC.
 *
 */
static MotorCtrl          s_focMotor;

/**
 * @brief Module-private configuration block for the motor FOC.
 *
 */
static volatile MotorCtrlApiConfig s_focMotorApiConfig;

/**
 * @brief Module-private configuration block for the motor FOC.
 *
 */
static volatile MotorCtrlApiPwm s_focMotorPwmConfig;

/**
 * @brief Get a mutable handle to the module’s configuration singleton.
 *
 * @return Pointer to the module-private @ref MotorCtrlApiConfig instance.
 *
 */
volatile MotorCtrlApiConfig* FOC_GetConfig(void) { return &s_focMotorApiConfig; }

/**
 * @brief Get the opaque handle to the module’s runtime singleton.
 *
 * @return Pointer to the module-private @ref MotorCtrl instance.
 *
 */
MotorCtrl* FOC_GetMotor(void) { return &s_focMotor; }

/**
 * @brief One-time bring-up initializer for the FOC pipeline.
 *
 * Usage & policy:
 *  - Call exactly once at system startup (e.g., in main()).
 *  - Operates on the global objects `focMotor` and `focCfg` (no parameters).
 *  - Leaves `MotorCtrlApiConfig` at safe zero defaults (cmd=0, dir=0); the
 *    application is expected to fill the config later (gains, limits, timings).
 *  - Initializes all submodules to zero/idle state; does not start the motor.
 *  - ADC offsets are left at 0 here; your IDLE phase code will track them.
 *
 */
void Foc_Init(void)
{
    /* Clear runtime and configuration to deterministic zeros */
		s_focMotor = (MotorCtrl){0};
		s_focMotorPwmConfig= (MotorCtrlApiPwm){0};
		s_focMotorApiConfig= (MotorCtrlApiConfig){0};

    /* Safe-zero defaults for top-level config (application will override) */
    s_focMotorApiConfig.cmd = 0u;   /* STOP by default */
    s_focMotorApiConfig.dir = 0u;   /* FWD  by default */

    /* Initialize submodules (all zeros) */
    Speed_Loop_Init(&s_focMotor.speedLoopPi, &s_focMotor.speedLoopRampup);
    SingleShunt_Init(&s_focMotor.svpwmInput, &s_focMotor.svpwmNorm, &s_focMotor.svpwmOutput, &s_focMotor.currentReconstruction);
    SlidingMode_Observer_Init(&s_focMotor.smo, &s_focMotor.pll);
		
    /* Current PIs (Id/Iq) */
    Current_PI_Init(&s_focMotor.pi_d);
    Current_PI_Init(&s_focMotor.pi_q);

    /* Explicitly set a few runtime gates/markers (redundant with memset but clear intent) */
    s_focMotor.bSpeedLpEnFlag = False;
    s_focMotor.state          = MOTOR_STATE_IDLE;
    s_focMotor.switchPhase    = SWITCH_DROP_IQ;
    s_focMotor.idlePhase      = 0U;

    /* ADC offset seeds (tracked later in IDLE) */
    s_focMotor.foc_AdcResults.s16offsetA = 0;
    s_focMotor.foc_AdcResults.s16offsetB = 0;

    /* Make the math scratch structs explicitly zeroed for readability */
    s_focMotor.currentClarke  = (CLARKE_Q15){0};
    s_focMotor.currentPark    = (PARK_Q15){0};
    s_focMotor.voltageParkInv = (PARK_INV_Q15){0};
		
		#if Segger_RTT_Enable
		SEGGER_RTT_ConfigUpBuffer(JscopeChannel,"JScope_T4I4I4I4",(uint8_t*)&JS_RTT_UpBuffer[0],sizeof(JS_RTT_UpBuffer),SEGGER_RTT_MODE_NO_BLOCK_SKIP);
		#endif
}


/**
 * @brief Reset the entire FOC runtime to a safe, non-moving state.
 *
 * This routine:
 *  - Keeps ADC offsets and all tuning/config parameters intact.
 *  - Zeros references, measured values, angle/speed accumulators and LUT indices.
 *  - Resets Single-Shunt/SVPWM runtime, SMO, PLL, current PIs and the speed loop.
 *  - Returns the state machine to IDLE and clears all counters/flags.
 *
 * @param MotCtrlApi Pointer to the user configuration (not modified).
 * @param MotCtrl    Pointer to the motor runtime container to reset.
 *
 * @pre  Call from a context where the fast FOC ISR cannot observe a half-reset
 *       state (e.g., inside the ADC ISR branch that handles STOP/fault).
 * @post MotCtrl->state == MOTOR_STATE_IDLE and PWM outputs reflect zero commands.
 */
static inline void Foc_Reset( MotorCtrlApiConfig *MotCtrlApi, MotorCtrl *MotCtrl )
{
    (void)MotCtrlApi;			/* config not changed here */
		
    /* Gates & references */
    MotCtrl->bSpeedLpEnFlag = False;
    MotCtrl->idRef_q15 = MotCtrl->iqRef_q15 = MotCtrl->iqRef_q31 = 0;
    MotCtrl->measuredId_q15 = MotCtrl->measuredIq_q15 = 0;

    /* Angles, speeds, indices */
    MotCtrl->theta_ol_q15 = MotCtrl->theta_used_q15 = MotCtrl->theta_smo_q15 = MotCtrl->thetaCmpErr_q15 = 0;
    MotCtrl->omega_ol_q15 = MotCtrl->omega_ol_q31 = MotCtrl->omega_smo_q15 = MotCtrl->omega_used_q15 = MotCtrl->dtheta_q15 = 0;
    MotCtrl->thetaIdx_ol = MotCtrl->thetaIdx_used = 0u;

    /* Power stage & current reconstruction runtime */
    SingleShunt_Reset(&MotCtrl->svpwmInput, &MotCtrl->svpwmNorm, &MotCtrl->svpwmOutput, &MotCtrl->currentReconstruction);

    /* Observer / PLL (params kept) */
    SlidingMode_Observer_Reset(&MotCtrl->smo, &MotCtrl->pll);

    /* PI controllers & speed loop (gains/limits kept) */
    Current_PI_Reset(&MotCtrl->pi_d);
    Current_PI_Reset(&MotCtrl->pi_q);
    Speed_Loop_Reset(&MotCtrl->speedLoopPi, &MotCtrl->speedLoopRampup);
		
		/* ADC offset seeds (tracked later in IDLE) */
    MotCtrl->foc_AdcResults.s16offsetA = 0;
    MotCtrl->foc_AdcResults.s16offsetB = 0;

    /* Math scratch */
    MotCtrl->currentClarke  = (CLARKE_Q15){0};
    MotCtrl->currentPark    = (PARK_Q15){0};
    MotCtrl->voltageParkInv = (PARK_INV_Q15){0};

    /* FSM, counters, helpers, protections */
    MotCtrl->state = MOTOR_STATE_IDLE;
    MotCtrl->switchPhase = SWITCH_DROP_IQ;
    MotCtrl->idlePhase = 0u;

    MotCtrl->u32IdleCounter = MotCtrl->u32AlignCounter =
    MotCtrl->u32RampUpCounter = MotCtrl->u32ObserveHoldCounter =
    MotCtrl->u32ErrTimeOutCounter = MotCtrl->u32SpdLoopHoldCounter = 0u;

    MotCtrl->iqBlendHold_q15 = 0;
    MotCtrl->bfocOvrCurrFlag = False;
    MotCtrl->u32OvrCurrCounter = 0u;
}

/**
 * @brief IDLE state handler for FOC.
 *
 * Behavior (two phases):
 *  - Phase 0: Enforce a blank time so phase currents decay to ~0 before control starts.
 *  - Phase 1: Track ADC offsets with a simple 1st-order average; when run==1, enter ALIGN.
 *
 * @param[in]  MotCtrlApi  API/config (uses: u32idleBlankTime, run)
 * @param[in,out] MotCtrl  Control state (uses/updates: idlePhase, u32IdleCounter,
 *                         foc_AdcResults.s16offsetA/B, state)
 *
 * Notes:
 *  - Offset tracker uses (old + new) / 2. Keep calling in IDLE to converge.
 *  - Phase 0 -> 1 after u32idleBlankTime ticks. On exit to ALIGN, counters are reset.
 */
static inline void Foc_Idle(	MotorCtrlApiConfig *MotCtrlApi, MotorCtrl *MotCtrl	)
{
		/* Phase 0: Blank Time to Ensure Current Decays to zero before starting the Motor Control Cycle */
		if ( MotCtrl->idlePhase == 0 )
		{
				if (++MotCtrl->u32IdleCounter >= MotCtrlApi->u32idleBlankDuration)
				{
						MotCtrl->u32IdleCounter = 0;
						MotCtrl->idlePhase = 1;
				}
				return;
		}
		
		/* Phase 1: track ADC offsets with simple first-order averaging */
		MotCtrl->foc_AdcResults.s16offsetA = (sint16)( ( (sint32)( MotCtrl->foc_AdcResults.s16offsetA + (sint16)( Cau_GetResultAFast() ) ) ) >> 1U );
		MotCtrl->foc_AdcResults.s16offsetB = (sint16)( ( (sint32)( MotCtrl->foc_AdcResults.s16offsetB + (sint16)( Cau_GetResultBFast() ) ) ) >> 1U );
	
		/* Start ALIGN once run is requested (after blank time) */
		if ( MotCtrlApi->cmd ) 
		{ 	
				MotCtrl->u32IdleCounter = 0;
				MotCtrl->idlePhase = 0;			
				MotCtrl->state = MOTOR_STATE_ALIGN; 
		}
}

/**
 * @brief Flux alignment at a fixed electrical angle (open-loop).
 *
 * Behavior:
 *  - Forces Id reference to a configured alignment current; Iq is forced to 0.
 *  - Holds the electrical angle at a configured alignment angle (Q15, -pi..+pi).
 *  - Produces LUT indices from the open-loop angle for sin/cos lookup.
 *  - After a fixed duration (u32alignDuration, in control ticks), resets SMO/PLL
 *    and transitions to MOTOR_STATE_RAMPUP.
 */
static inline void Foc_Align( MotorCtrlApiConfig *MotCtrlApi, MotorCtrl *MotCtrl )
{
    /* Iq reference for alignment (Id forced to zero) */
   
	  MotCtrl->idRef_q15 = 0;
	  MotCtrl->iqRef_q15 =  MotCtrlApi->dir ? (-MotCtrlApi->alignCurrent_q15) : MotCtrlApi->alignCurrent_q15;

	  /* Omega setting for alignment (forced to zero) */
		MotCtrl->omega_ol_q15 = MotCtrl->omega_ol_q31 = 0;
		MotCtrl->omega_used_q15 = MotCtrl->omega_ol_q15;
	
    /* Fix open-loop electrical angle to the configured alignment angle */
    MotCtrl->theta_ol_q15  = MotCtrlApi->alignAngle_q15;
    MotCtrl->theta_used_q15 = MotCtrl->theta_ol_q15;

    /* LUT indices from theta(Q15); LUT is defined over (-pi..+pi) */
    MotCtrl->thetaIdx_ol   = thetaQ15_to_idx( MotCtrl->theta_ol_q15 );
    MotCtrl->thetaIdx_used = MotCtrl->thetaIdx_ol;

    /* Stay in ALIGN for the configured duration, then reset observers and proceed */
    if ( ++MotCtrl->u32AlignCounter >= MotCtrlApi->u32alignDuration ) 
		{
        MotCtrl->u32AlignCounter = 0;
        SlidingMode_Observer_Reset( &MotCtrl->smo, &MotCtrl->pll );
        MotCtrl->state = MOTOR_STATE_RAMPUP;
    }
}


/**
 * @brief I-F ramp-up stage (open-loop).
 *
 *	Forces Id=0; ramps Iq and open-loop omega toward targets by direction.
 *	_Q31omegaRampupStep is non-negative.
 *	_Q15KoemgaToTheta is Q15 gain converting omega(Q15) to dtheta(Q15)/tick.
 *	thetaQ15_to_idx() matches a (-pi..+pi) sine/cos LUT (size 4096 here).
 */
static inline void Foc_RampUp( MotorCtrlApiConfig *MotCtrlApi, MotorCtrl *MotCtrl )
{
    /* Id fixed to 0 in Ramp-Up */
    MotCtrl->idRef_q15 = 0;

    /* Iq target by direction (0=FWD, 1=REV) */
    MotCtrl->iqRef_q15 =  MotCtrlApi->dir ? -MotCtrlApi->iqRampupTarget_q15 : MotCtrlApi->iqRampupTarget_q15;
		
		/* Omega target by direction */ 
		sint16 omegaTarget_q15 = MotCtrlApi->dir ? -MotCtrlApi->omegaRampupTarget_q15 : MotCtrlApi->omegaRampupTarget_q15; 
		sint32 omegaTarget_q31 = (sint32)omegaTarget_q15 * (1 << Q31_FROM_Q15_SHIFT); 
		
		/* Ramp open-loop omega with Q31 step (step = 0) */
		if (MotCtrlApi->dir) 
		{
			MotCtrl->omega_ol_q31 -= MotCtrlApi->omegaRampupStep_q31; 
			if ( MotCtrl->omega_ol_q31 > omegaTarget_q31 ) 
				MotCtrl->omega_ol_q15 = (sint16)(MotCtrl->omega_ol_q31 >> Q31_FROM_Q15_SHIFT); 
			else
			{
				MotCtrl->omega_ol_q15 = omegaTarget_q15; 
				MotCtrl->omega_ol_q31	= omegaTarget_q31;
			}
		
		}
		else 
		{
			MotCtrl->omega_ol_q31 += MotCtrlApi->omegaRampupStep_q31;
			if ( MotCtrl->omega_ol_q31 < omegaTarget_q31 ) 
				MotCtrl->omega_ol_q15 = (sint16)(MotCtrl->omega_ol_q31 >> Q31_FROM_Q15_SHIFT); 
			else 
			{
				MotCtrl->omega_ol_q15 = omegaTarget_q15;
				MotCtrl->omega_ol_q31	= omegaTarget_q31;
			}
		}
		
		MotCtrl->omega_used_q15 = MotCtrl->omega_ol_q15;
	
		/* theta(k+1) = wrap{ theta(k) + (omega * K) } ; omega,K are Q15 */		
		MotCtrl->dtheta_q15 = (sint16)(((sint32)MotCtrl->omega_used_q15 * MotCtrlApi->KoemgaToTheta_q15 ) >> Q15_SHIFT);
		MotCtrl->theta_ol_q15 = q15_Wrap_Theta( (sint16)((sint32)MotCtrl->theta_ol_q15 + (sint32)MotCtrl->dtheta_q15));
		MotCtrl->theta_used_q15 = MotCtrl->theta_ol_q15;
	
    /* LUT index from theta(Q15); table is (-pi..+pi) */
    MotCtrl->thetaIdx_ol = thetaQ15_to_idx ( MotCtrl->theta_used_q15 );
    MotCtrl->thetaIdx_used = MotCtrl->thetaIdx_ol;
		
    /* Transition to SWITCHING after timeout */
    if ( ++MotCtrl->u32RampUpCounter >= MotCtrlApi->u32rampupDuration ) {
        MotCtrl->u32RampUpCounter = 0;
        MotCtrl->state = MOTOR_STATE_SWITCHING;
				MotCtrl->pll.OmegaInt_q31 = (sint32)MotCtrl->omega_used_q15 << Q15_SHIFT;
				MotCtrl->iqRef_q31 = MotCtrl->iqRef_q15 * (1 << Q31_FROM_Q15_SHIFT); 
    }
}

/**
 * @brief Single switching state (direction-aware).
 *  - Open-loop keep running; Id -> 0; Iq -> dir-signed floor (soft torque).
 *  - Check phase/speed/EMF gates with hold.
 *  - When gates hold: blend angle a (ADC ISR). Iq ß-blend is done in speed timer ISR.
 *  - Exit to CLOSED_LOOP when a==1 && ß==1.
 */
static inline void Foc_Switching( MotorCtrlApiConfig *MotCtrlApi, MotorCtrl *MotCtrl )
{
    /* ---- 1) Direction-aware minimal torque & Id = 0 ---- */
		MotCtrl->pll.KomegaToTheta_q15 = MotCtrlApi->KoemgaToTheta_q15;	
		sint16 ThetaError_q15 = (sint16)((sint32)MotCtrl->pll.Omega_q15 * MotCtrlApi->K_Td_q15 >> Q15_SHIFT);
		MotCtrl->theta_smo_q15 = q15_Wrap_Theta ( MotCtrl->pll.Theta_q15 + ThetaError_q15); 
		MotCtrl->thetaCmpErr_q15 = q15_Wrap_Theta((sint16)(MotCtrl->theta_smo_q15 - MotCtrl->theta_ol_q15));
	
		switch ( MotCtrl->switchPhase )
		{
				case
					SWITCH_DROP_IQ:
				{		
					MotCtrl->omega_used_q15 = MotCtrl->omega_ol_q15;
					MotCtrl->dtheta_q15 = (sint16)(((sint32)MotCtrl->omega_used_q15 * MotCtrlApi->KoemgaToTheta_q15 ) >> Q15_SHIFT);	
					MotCtrl->theta_ol_q15 = q15_Wrap_Theta( (sint16)((sint32)MotCtrl->theta_ol_q15 + (sint32)MotCtrl->dtheta_q15));	
					MotCtrl->theta_used_q15 = MotCtrl->theta_ol_q15;
					MotCtrl->thetaIdx_ol = thetaQ15_to_idx ( MotCtrl->theta_used_q15 );
					MotCtrl->thetaIdx_used = MotCtrl->thetaIdx_ol;
				
					/* Ramp down iq with Q31 step (step = 0) */
					sint16 iqMin_signed_q15 = ( MotCtrlApi->dir ) ? -MotCtrlApi->iqSwitchMin_q15 : MotCtrlApi->iqSwitchMin_q15;
					sint32 iqMin_signed_q31 = (sint32)iqMin_signed_q15 * (1 << Q31_FROM_Q15_SHIFT); 
					if (MotCtrlApi->dir) 
					{
							MotCtrl->iqRef_q31 += MotCtrlApi->iqSwitchStep_q31; 
							if ( MotCtrl->iqRef_q31 < iqMin_signed_q31 ) 
									MotCtrl->iqRef_q15 = (sint16)(MotCtrl->iqRef_q31 >> Q31_FROM_Q15_SHIFT); 
							else
							{
									MotCtrl->iqRef_q15 = iqMin_signed_q15;
									MotCtrl->iqRef_q31 = iqMin_signed_q31; 
							}								
					}
					else 
					{
							MotCtrl->iqRef_q31 -= MotCtrlApi->iqSwitchStep_q31;
							if ( MotCtrl->iqRef_q31 > iqMin_signed_q31 ) 
									MotCtrl->iqRef_q15 = (sint16)(MotCtrl->iqRef_q31 >> Q31_FROM_Q15_SHIFT); 
							else 
							{
									MotCtrl->iqRef_q15 = iqMin_signed_q15; 
									MotCtrl->iqRef_q31 = iqMin_signed_q31; 
							}
					}
											
						MotCtrl->u32ObserveHoldCounter = ( q15_Abs(MotCtrl->thetaCmpErr_q15) <= MotCtrlApi->thetaErrTh_q15 ) ? (MotCtrl->u32ObserveHoldCounter + 1) : 0;
						if( MotCtrl->u32ObserveHoldCounter > MotCtrlApi->u32ObsReadyTicks
							  || ++MotCtrl->u32ErrTimeOutCounter > MotCtrlApi->u32ErrTimeoutTicks )
						{
								MotCtrl->switchPhase = SWITCH_WAIT_SPEED;
								MotCtrl->iqBlendHold_q15 = MotCtrl->iqRef_q15;
								MotCtrl->u32ObserveHoldCounter = 0;
						}
				}
				break;
				
				case
					SWITCH_WAIT_SPEED:
				{
					
						MotCtrl->omega_smo_q15 = MotCtrl->pll.Omega_q15;
						MotCtrl->omega_used_q15 = MotCtrl->omega_smo_q15;
						MotCtrl->theta_used_q15 = MotCtrl->pll.Theta_q15;
						MotCtrl->thetaIdx_used  = thetaQ15_to_idx(MotCtrl->theta_used_q15);
						MotCtrl->iqRef_q15 = MotCtrl->iqBlendHold_q15;
						if ( ++MotCtrl->u32SpdLoopHoldCounter >= (uint32)MotCtrlApi->u32SpdLpReadyTicks )
						{
								MotCtrl->u32SpdLoopHoldCounter =0;
								MotCtrl->bSpeedLpEnFlag = True;
								MotCtrl->switchPhase = SWITCH_DROP_IQ;
								MotCtrl->state = MOTOR_STATE_CLOSELOOP;
								MotCtrl->iqSpdLoopInit_q15 = MotCtrl->iqBlendHold_q15;
						}
				}
				break;
				
				default:
						MotCtrl->switchPhase = SWITCH_DROP_IQ;
				break;
		}
}

/**
 * @brief Fully sensorless closed loop stage.
 *
 * Behavior:
 *  - Uses observer/PLL electrical angle for Park/SVPWM lookup.
 *  - Uses speed-loop outputs as current references (Iq/Id).
 */
static inline void Foc_CloseLoop( MotorCtrlApiConfig *MotCtrlApi, MotorCtrl *MotCtrl )
{    
		/* Electrical angle from observer/PLL */
		MotCtrl->theta_smo_q15 = MotCtrl->pll.Theta_q15;
	
		// Correct theta due to time delay
		sint16 ThetaError_q15 = (sint16)((sint32)MotCtrl->pll.Omega_q15 * MotCtrlApi->K_Td_q15 >> Q15_SHIFT);
		MotCtrl->theta_used_q15 = q15_Wrap_Theta ( MotCtrl->theta_smo_q15 + ThetaError_q15);
		MotCtrl->thetaIdx_used = thetaQ15_to_idx ( MotCtrl->theta_used_q15 );
		MotCtrl->omega_smo_q15 = MotCtrl->pll.Omega_q15;
		MotCtrl->omega_used_q15 = MotCtrl->omega_smo_q15;
		
		/* Current references from speed loop */

		MotCtrl->idRef_q15 = MotCtrl->speedLoopPi.Id_Ref_q15;
		MotCtrl->iqRef_q15 = MotCtrl->speedLoopPi.Iq_Ref_q15;
}

/**
 * @brief FOC state machine.
 *
 * Policy:
 *  - When run == 0 (STOP), force IDLE. Call Foc_Reset() only once on transition.
 *  - When run == 1, follow the normal pipeline: IDLE -> ALIGN -> RAMPUP -> SWITCHING -> CLOSELOOP.
 */
static inline __attribute__((always_inline))
	void FOC_StateMachine ( MotorCtrlApiConfig *MotCtrlApi, MotorCtrl *MotCtrl )
{
		/* STOP gate: force IDLE; do reset only once when leaving non-IDLE states */
		if ( !MotCtrlApi->cmd || MotCtrl->bfocOvrCurrFlag ) 
		{ 	    
				if (MotCtrl->state != MOTOR_STATE_IDLE) { Foc_Reset(MotCtrlApi, MotCtrl); }
				MotCtrl->state = MOTOR_STATE_IDLE; 
		}
		/* RUN: normal pipeline */
    switch (MotCtrl->state)
    {
				case 
					MOTOR_STATE_IDLE:      Foc_Idle( MotCtrlApi, MotCtrl );       break;
				case 
					MOTOR_STATE_ALIGN:     Foc_Align( MotCtrlApi, MotCtrl );   		break;
				case 
					MOTOR_STATE_RAMPUP:    Foc_RampUp( MotCtrlApi, MotCtrl );     break;
				case 
					MOTOR_STATE_SWITCHING: Foc_Switching( MotCtrlApi, MotCtrl );  break;
				case 
					MOTOR_STATE_CLOSELOOP: Foc_CloseLoop( MotCtrlApi, MotCtrl );  break;
				default:                   
																 MotCtrl->state = MOTOR_STATE_IDLE;    	break;
		}
}

/**
 * @brief Over-current (OC) software check (per-unit, Q15).
 *
 * Compares reconstructed phase currents Ia/Ib/Ic against a positive
 * per-phase limit (_Q15ImaxLimit). If any phase exceeds the limit,
 * sets bfocOvrCurrFlag = True.
 *
 */
static inline __attribute__((always_inline)) 
	void	Foc_OvrCurr_Protection( MotorCtrlApiConfig *MotCtrlApi, MotorCtrl *MotCtrl )
{	
	  /* already latched: keep flag; optional: early return */
    if (MotCtrl->bfocOvrCurrFlag == True) return;
	
	  sint16 ia_q15 = q15_Abs(MotCtrl->currentReconstruction.Ia_q15);
    sint16 ib_q15 = q15_Abs(MotCtrl->currentReconstruction.Ib_q15);
    sint16 ic_q15 = q15_Abs(MotCtrl->currentReconstruction.Ic_q15);

    sint16 amax_q15 = (ia_q15 > ib_q15 ? ia_q15 : ib_q15);
    if (ic_q15 > amax_q15) amax_q15 = ic_q15;
	
		if ( amax_q15 > MotCtrlApi->ImaxLimit_q15 )
		{
				if( ++MotCtrl->u32OvrCurrCounter > MotCtrlApi->u32OvrCurrDebounceN )
				{
						MotCtrl->bfocOvrCurrFlag = True;
						MotCtrl->u32OvrCurrCounter = MotCtrlApi->u32OvrCurrDebounceN;
				}
		}
		else
		{
				MotCtrl->u32OvrCurrCounter = 0;
		}
}

/**
 * @brief Fast-loop FOC handler body.
 *
 * Order of operations (per interrupt):
 *  0) State machine -> updates refs (Id/Iq, thetaIdx_used, etc.).
 *  1) Sample ADC -> remove offsets -> normalize to per-unit -> reconstruct phase currents.
 *  2) Over current protections. 
 *  3) Clarke -> provide Ia/ß to SMO.
 *  4) If running (RampUp/Switching/CloseLoop): feed Va/ß and Ia/ß to SMO/PLL and step observer.
 *  5) Park (use thetaIdx_used) -> Id/Iq measured.
 *  6) Current PI controllers (Id/Iq).
 *  7) Voltage vector circle limitation: clamp |Vdq| = Vmax (per-unit).
 *  8) Inverse Park -> Va/ß; SVPWM; PWM register update.
 *
 */
static inline __attribute__((always_inline)) void Foc_Handler_Body( void )
{
		/* ---- Snapshot config once for this tick (works with or without volatile) ---- */
    MotorCtrl *m = &s_focMotor;
		volatile MotorCtrlApiPwm *pwm = &s_focMotorPwmConfig;
    const volatile MotorCtrlApiConfig *cfgv = &s_focMotorApiConfig; /* or FOC_GetConfigRO() */
    MotorCtrlApiConfig cfg = *cfgv;  /* local non-volatile copy for this ISR */

	
    /* --- 0) State machine (updates refs/state/theta indices, etc.) --- */
    FOC_StateMachine(&cfg, m);
		if ( m->state == MOTOR_STATE_IDLE ) { DisablePWMOutputs();  return; }
	
    /* --- 1) Sample & normalize (counts -> per-unit), then reconstruct Ia/Ib/Ic --- */
		
		m->foc_AdcResults.s16normalA = (sint16)Cau_GetResultAFast();
		m->foc_AdcResults.s16normalB = (sint16)Cau_GetResultBFast();

		sint16 AdcrawA = (sint16)((m->foc_AdcResults.s16normalA - m->foc_AdcResults.s16offsetA ) >> 2U);
		sint16 AdcrawB = (sint16)((m->foc_AdcResults.s16normalB - m->foc_AdcResults.s16offsetB ) >> 2U);

		m->currentReconstruction.Ibus1_q15 = (sint16)((sint32)AdcrawA * cfg.AdcToCurrent_q15);
		m->currentReconstruction.Ibus2_q15 = (sint16)((sint32)AdcrawB * cfg.AdcToCurrent_q15);
		m->currentReconstruction.sector = m->svpwmOutput.sector;
		SingleShunt_ReconstructCurrents(&m->currentReconstruction);
		
		
    /* --- 2) Over-current protection --- */
    Foc_OvrCurr_Protection(&cfg, m);

    /* --- 3) Clarke transform --- */
    m->currentClarke.a_q15 = m->currentReconstruction.Ia_q15;
    m->currentClarke.b_q15 = m->currentReconstruction.Ib_q15;
    m->currentClarke.c_q15 = m->currentReconstruction.Ic_q15;
    FOC_Clarke_Q15(&m->currentClarke);
		

    /* --- 4) Observer/PLL update (when running) --- */
    if (  m->state >= MOTOR_STATE_RAMPUP )
    {
				m->pll.dir = cfg.dir;
        m->smo.Ialpha_q15 = m->currentClarke.alpha_q15;
        m->smo.Ibeta_q15  = m->currentClarke.beta_q15;
        m->smo.Valpha_q15 = m->svpwmInput.Valpha_q15;
        m->smo.Vbeta_q15  = m->svpwmInput.Vbeta_q15;
			
        SlidingMode_Observer_Run(&m->smo, &m->pll);
    }

    /* --- 5) Park transform -> Id/Iq measured --- */
    m->currentPark.alpha_q15   = m->currentClarke.alpha_q15;
    m->currentPark.beta_q15    = m->currentClarke.beta_q15;
    m->currentPark.thetaIdx= m->thetaIdx_used;
    FOC_Park_Q15(&m->currentPark);
    m->measuredId_q15 = m->currentPark.d_q15;
    m->measuredIq_q15 = m->currentPark.q_q15;

    /* --- 6) Current PI controllers (Id/Iq) --- */
    m->pi_d.Current_ref_q15 = m->idRef_q15;
    m->pi_d.Current_fd_q15  = m->measuredId_q15;
    Current_PI_Parallel_Q15(&m->pi_d);

    m->pi_q.Current_ref_q15 = m->iqRef_q15;
    m->pi_q.Current_fd_q15  = m->measuredIq_q15;
    Current_PI_Parallel_Q15(&m->pi_q);

    /* --- 7) Vdq compenstion with VBB change --- */
		sint32 vd_comp = ((sint32)m->pi_d.Voltage_out_q15 * m->Vgain_q10) >> Q10_SHIFT;
		sint32 vq_comp = ((sint32)m->pi_q.Voltage_out_q15 * m->Vgain_q10) >> Q10_SHIFT;
		// Clamp [-1,1]
		if (vd_comp > Q15_ONE)  vd_comp =  Q15_ONE;  
		if (vd_comp < Q15_MINUS_ONE) vd_comp = Q15_MINUS_ONE;
		if (vq_comp > Q15_ONE)  vq_comp =  Q15_ONE;  
		if (vq_comp < Q15_MINUS_ONE) vq_comp = Q15_MINUS_ONE;
		m->pi_d.Voltage_out_q15 = (sint16)vd_comp;
		m->pi_q.Voltage_out_q15 = (sint16)vq_comp;

    /* --- 8) Inverse Park -> Va/Vb; SVPWM; PWM update --- */
    m->voltageParkInv.d_q15 = m->pi_d.Voltage_out_q15;
    m->voltageParkInv.q_q15 = m->pi_q.Voltage_out_q15;
		m->voltageParkInv.thetaIdx = m->thetaIdx_used;
    FOC_Park_Inv_Q15(&m->voltageParkInv);
		
		m->svpwmInput.Valpha_q15 = m->voltageParkInv.alpha_q15;
    m->svpwmInput.Vbeta_q15  = m->voltageParkInv.beta_q15;

    SingleShunt_Run(&m->svpwmInput, &m->svpwmOutput, &m->svpwmNorm);
		
		/* --- 9) update PWM settings in pwm strcut --- */
		pwm->ADCTrig1 = m->svpwmOutput.ADCTrig1;
		pwm->ADCTrig2 = m->svpwmOutput.ADCTrig2;	
		pwm->PWMA_Duty1 = m->svpwmOutput.Duty1A;
		pwm->PWMA_Duty2 = m->svpwmOutput.Duty2A;
		pwm->PWMB_Duty1 = m->svpwmOutput.Duty1B;
		pwm->PWMB_Duty2 = m->svpwmOutput.Duty2B;
		pwm->PWMC_Duty1 = m->svpwmOutput.Duty1C;
		pwm->PWMC_Duty2 = m->svpwmOutput.Duty2C;
		
		/* --- 10) call UpdatePWMRegisters function in driver_pwm.c file --- */
		UpdatePWMRegisters( pwm,&cfg );
			
		/* --- 11) Segger J-Scope RTT Module script --- */
		#if Segger_RTT_Enable
		
		_Timestamp++;
		rtt_JsMsg.timestamp = _Timestamp;
		rtt_JsMsg.msg_i32_1 = (sint32) (m->foc_AdcResults.s16normalA);
		rtt_JsMsg.msg_i32_2 = (sint32) (m->foc_AdcResults.s16normalB);
		rtt_JsMsg.msg_i32_3 = (sint32) (m->currentReconstruction.Ia_q15);
		SEGGER_RTT_Write(JscopeChannel,&rtt_JsMsg,sizeof(rtt_JsMsg));
		
		#endif

}

/**
 * @brief Speed loop handler body.
 *
 * Flow (only when enabled):
 *  1) Command ramp: Spd_Ref_In_Q15 -> Spd_Ref_Out_Q15.
 *  2) Run speed PI -> updates Id/Iq reference inside speedLoopPi.
 *
 */
static inline	void Speed_Loop_Handler_Body( void )
{
    MotorCtrl *m = &s_focMotor;
    /* Snapshot cfg once per tick */
    const volatile MotorCtrlApiConfig *cfgv = &s_focMotorApiConfig;
    MotorCtrlApiConfig cfg = *cfgv;
	
    /* Disabled: reset loop and arm one-shot integrator seeding */
    if (!m->bSpeedLpEnFlag) {
        Speed_Loop_Reset(&m->speedLoopPi, &m->speedLoopRampup);
        m->bSpdInitPending = True;   /* next enable: apply bumpless seed once */
        return;
    }

    /* One-shot integrator seed on the first enabled tick */
    if (m->bSpdInitPending) {
        /* iqSpdLoopInit_q15 is Q15; integrator is Q26 -> shift by 11 */
				m->speedLoopPi.Spd_I_Term_q31 = (sint32)m->iqSpdLoopInit_q15 * ( 1 << Q31_FROM_Q15_SHIFT );
        m->bSpdInitPending = False;
    }

    /* 1) Command ramp with direction */
    m->speedLoopRampup.Spd_Ref_In_q15 = cfg.SpdCmd_q15;
    m->speedLoopRampup.Spd_Ref_In_q15 = cfg.dir ? -m->speedLoopRampup.Spd_Ref_In_q15 : m->speedLoopRampup.Spd_Ref_In_q15;   /* reverse direction */

    Speed_Ramp_Up(&m->speedLoopRampup);

    /* 2) PI control (same units for Ref and Fb) */
    
		m->speedLoopPi.Spd_Ref_q15 = m->speedLoopRampup.Spd_Ref_Out_q15;
    m->speedLoopPi.Spd_Fb_q15  = (sint16)((sint32)m->omega_used_q15 * cfg.OmegaPuToRpmPu_q15 >> Q15_SHIFT);

    Speed_Loop_PI_Control(&m->speedLoopPi);  /* updates Iq_Ref inside struct */
}

/**
 * @brief  Update bus voltage estimation and compute Q5.10 voltage gain.
 *
 * This ISR routine performs:
 *  1) Read raw ADC bus voltage sample.
 *  2) Apply an exponential moving average (EMA) filter on ADC counts.
 *  3) Convert filtered counts to millivolts using an integer calibration factor.
 *  4) Compute Vgain_q10 = Vbase_mV / Vactual_mV (Q5.10), with numerator/denominator
 *     in the same unit (mV). The gain is saturated to VGAIN_MAX_X to avoid excessive
 *     amplification under deep undervoltage.
 *
 */
static inline void VbbComp_Dau_Handler_Body( void )
{
    MotorCtrl *m = &s_focMotor;
	
    /* 1) Read raw ADC sample */
    uint16 raw = Dau_GetSlotResultFast(eSLOT0_RESULT) >> 2;
	
	  /* 2) Exponential moving average on ADC counts
     *    y += (x - y) >> 7, alpha = 1/128 (smooth, ms-level response at kHz rates)
     */
    static uint16 vbus_ema_counts = 0;
    static uint8  inited          = 0;
    if (!inited) {
        vbus_ema_counts = raw;  /* Initialize to first sample to avoid startup bias */
        inited = 1;
    } else {
        sint32 diff = (sint32)raw - (sint32)vbus_ema_counts;
        /**< Exponential moving average: alpha = 1 / ( 1 << 7 ) */
				vbus_ema_counts += (sint16)(diff >> 7); 
    }
		
		/* 3) Convert filtered ADC counts to millivolts (integer scaling) */
		const volatile MotorCtrlApiConfig *cfgv = &s_focMotorApiConfig; /* or FOC_GetConfigRO() */
    MotorCtrlApiConfig cfg = *cfgv;
    uint32 Vact_mV = ((uint64)vbus_ema_counts * (uint64)cfg.u32DauToVbbMicroV) >> 10;
    m->u32Vactual_mV = Vact_mV;  /* Publish filtered bus voltage in mV */

    /* 4) Compute voltage gain in Q5.10: Vgain = Vbase_mV / Vactual_mV */

		uint32 Vact_mV_nz = (Vact_mV > 0u) ? Vact_mV : 1u;
		uint32 gain_q10 = (uint32)(((uint64)m->u32Vbase_mV << Q10_SHIFT) / Vact_mV_nz); /* (Vbase << 10) / Vact */

    /* 5) Publish Q5.10 gain for SVPWM duty normalization */
		m->Vgain_q10 = (sint32)gain_q10;
}

/**
 * @brief ADC fast-loop ISR entry point.
 *
 * Addressable public symbol for ADC IRQ/callback registration.
 * Dispatches to the inlined fast-loop body.
 *
 * @note Call from ISR context only.
 */
__attribute__((always_inline)) void Foc_Cau_Handler(void) 
{ 
		Gpio_Set( eGPIO_INSTANCE_ID1 );
		Foc_Handler_Body(); 
		Cau_SetConvStatusFast( eCAU_CONVSTATUS_ANY );
		Gpio_Clear( eGPIO_INSTANCE_ID1 );
}

/**
 * @brief Timer speed-loop ISR entry point.
 *
 * Addressable public symbol for timer IRQ/callback registration.
 * Dispatches to the inlined speed-loop body.
 *
 * @note Call from ISR context only; not re-entrant.
 */
__attribute__((always_inline)) void Speed_Loop_Gtu_Handler(void) 
{ 
		Speed_Loop_Handler_Body(); 
		Gtu_ClearStatus( eTIMER2, eTIMER_STATUS_DVL );
}	


/**
 * @brief Dau voltage compensation ISR entry point.
 *
 * Addressable public symbol for DAU IRQ/callback registration.
 * Dispatches to the inlined speed-loop body.
 *
 * @note Call from ISR context only; not re-entrant.
 */
__attribute__((always_inline)) void VbbComp_Dau_Handler(void) 
{ 
		VbbComp_Dau_Handler_Body(); 
		Dau_SetConvCompStatusFast(eSLOT0_CONV_COMP_STATUS);
}