/**
 * @file         foc_control_api.c
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
 
#include "foc_control_api.h"
#include "foc_control.h"

 
 /* Convenience macro to grab the volatile cfg pointer once per call */
#define CFG_PTR()  ( FOC_GetConfig() )
#define MC_PTR()   ( FOC_GetMotor() )

/** Set differential amplifier gain */
void MotorControl_SetDiffAmpGain (uint16 gain) { u16Gdu_diffAmpGain = gain; };

/** Set PWM period */
void MotorControl_SetPwmPeriod (uint16 period) { CFG_PTR()->pwmPeriod = period; };

/** Set deadtime */
void MotorControl_SetDeadtime (uint16 deadtime) { CFG_PTR()->pwmDeadtime = deadtime; };

/** Set run command (0=STOP, 1=RUN). */
void MotorControl_SetRun(boolean run) { CFG_PTR()->cmd = run; }

/** Set rotation direction (0=FWD, 1=REV). */
void MotorControl_SetDir(boolean rev) { CFG_PTR()->dir = rev; }

/** Set IDLE blank duration (ticks). */
void MotorControl_SetIdleBlank(uint32 idle) { CFG_PTR()->u32idleBlankDuration = idle; }

/** Set ALIGN d-axis current (Q15). */
void MotorControl_SetAlignCurrent_Q15(sint16 id_q15) { CFG_PTR()->alignCurrent_q15 = id_q15; }

/** Set ALIGN electrical angle (Q15). */
void MotorControl_SetAlignAngle_Q15(sint16 angle_q15) { CFG_PTR()->alignAngle_q15 = angle_q15; }

/** Set ALIGN duration (ticks). */
void MotorControl_SetAlignDuration(uint32 ticks) { CFG_PTR()->u32alignDuration = ticks; }

/** Set ramp-up target |Iq| (Q15). */
void MotorControl_SetRampCurrent_Q15(sint16 iq_q15) { CFG_PTR()->iqRampupTarget_q15 = iq_q15; }

/** Set ramp-up target |omega| (Q15). */
void MotorControl_SetRampOmegaTarget_Q15(sint16 omega_q15) { CFG_PTR()->omegaRampupTarget_q15 = omega_q15; }

/** Set ramp-up |omega| step per tick (Q31). */
void MotorControl_SetRampOmegaStep_Q31(sint32 step_q31) { CFG_PTR()->omegaRampupStep_q31 = step_q31; }

/** Set omega?dtheta gain per tick (Q15). */
void MotorControl_SetOmega2Theta_Q15(sint16 dtheta_q15) { CFG_PTR()->KoemgaToTheta_q15 = dtheta_q15; }

/** Set ramp-up duration (ticks). */
void MotorControl_SetRampDuration(uint32 ticks) { CFG_PTR()->u32rampupDuration = ticks; }

/** Set switching minimal |Iq| floor (Q15). */
void MotorControl_SetSwitchMinIq_Q15(sint16 iq_q15) { CFG_PTR()->iqSwitchMin_q15 = iq_q15; }

/** Set switching Iq ramp step per tick (Q31). */
void MotorControl_SetSwitchIqStep_Q31(sint32 step_q31) { CFG_PTR()->iqSwitchStep_q31 = step_q31; }

/** Set switching angle error threshold (Q15). */
void MotorControl_SetSwitchThetaErr_Q15(sint16 threshold_q15) { CFG_PTR()->thetaErrTh_q15 = threshold_q15; }

/** Set observer-ready hold time (ticks). */
void MotorControl_SetSwitchObsReadyTicks(uint32 ticks) { CFG_PTR()->u32ObsReadyTicks = ticks; }

/** Set theta error time-out time (ticks). */
void MotorControl_SetErrTimeoutTicks(uint32 ticks) { CFG_PTR()->u32ErrTimeoutTicks = ticks; }

/** Set speed-loop-ready hold time (ticks). */
void MotorControl_SetSwitchSpdLpRdyCount(uint32 ticks) { CFG_PTR()->u32SpdLpReadyTicks = ticks; }

/** Set per-phase current limit |I| (Q15). */
void MotorControl_SetCurrentLimit_Q15(sint16 imax_q15) { CFG_PTR()->ImaxLimit_q15 = imax_q15; }

/** Set over-current debounce (ticks). */
void MotorControl_SetOverCurrDebounce(uint32 ticks) { CFG_PTR()->u32OvrCurrDebounceN = ticks; }

/** Set |Vdq| circle limit (Q15). */
void MotorControl_SetVdqMax_Q15(sint16 voltage_q15) { CFG_PTR()->VmaxLimit_q15 = voltage_q15; }

/** Set ADC->current per-unit gain (Q15). */
void MotorControl_SetAdc2Ibase_Q15(sint16 gain_q15) { CFG_PTR()->AdcToCurrent_q15 = gain_q15; }

/** Set vbb (mV) -> ADC counts gain (uint32). */
void MotorControl_SetDauAdc2VbbMicroV(uint32 gain) { CFG_PTR()->u32DauToVbbMicroV = gain; }

/** Set ADC counts -> voltage per-unit gain (Q15). */
void MotorControl_SetAdc2Vbase_Q15(sint16 gain_q15) { CFG_PTR()->AdcToVoltage_q15 = gain_q15; }

/** Set external speed command (Q15). */
void MotorControl_SetSpdCmd_Q15(sint16 speed_q15) { CFG_PTR()->SpdCmd_q15 = speed_q15; }

/** Set omega->speed feedback scaling (Q15). */
void MotorControl_SetOmega2Rpm_Q15(sint16 coeff_q15){ CFG_PTR()->OmegaPuToRpmPu_q15 = coeff_q15; }

/** Set Id.Kp (Q15). */
void MotorControl_SetIdKp_Q15(sint16 kp_q15){ MC_PTR()->pi_d.Current_Kp_q15 = kp_q15; }

/** Set Id.Ki (Q15). */
void MotorControl_SetIdKi_Q15(sint16 ki_q15) { MC_PTR()->pi_d.Current_Ki_q15 = ki_q15; }

/** Set Iq.Kp (Q15). */
void MotorControl_SetIqKp_Q15(sint16 kp_q15) { MC_PTR()->pi_q.Current_Kp_q15 = kp_q15; }

/** Set Iq.Ki (Q15). */
void MotorControl_SetIqKi_Q15(sint16 ki_q15) { MC_PTR()->pi_q.Current_Ki_q15 = ki_q15; }

/** Set Id PI output limit |Vdq| (Q15). */
void MotorControl_SetIdPiVoutMax_Q15(sint16 vmax_q15){ MC_PTR()->pi_d.OutMax_q15 =  vmax_q15; 
		MC_PTR()->pi_d.OutMin_q15 = (sint16)(-vmax_q15);}

/** Set Iq PI output limit |Vdq| (Q15). */
void MotorControl_SetIqPiVoutMax_Q15(sint16 vmax_q15){	MC_PTR()->pi_q.OutMax_q15 =  vmax_q15; 
		MC_PTR()->pi_q.OutMin_q15 = (sint16)(-vmax_q15);}

/** Set speed PI Kp (Q15). */
void MotorControl_SetSpeedPiKp_Q15(sint16 spdKp_q15) { MC_PTR()->speedLoopPi.Spd_Kp_q15 = spdKp_q15; }

/** Set speed PI Ki (Q15). */
void MotorControl_SetSpeedPiKi_Q15(sint16 spdKi_q15) { MC_PTR()->speedLoopPi.Spd_Ki_q15 = spdKi_q15; }

/** Set speed PI output max (Q15). */
void MotorControl_SetSpeedPiOutMax_Q15(sint16 outmax_q15) { MC_PTR()->speedLoopPi.Out_Max_q15 = outmax_q15; }

/** Set speed PI output min (Q15). */
void MotorControl_SetSpeedPiOutMin_Q15(sint16 outmin_q15) { MC_PTR()->speedLoopPi.Out_Min_q15 = outmin_q15; }

/** Set speed ramp acceleration (Q31/tick). */
void MotorControl_SetSpeedAcc_Q31(sint32 acc_q31){ MC_PTR()->speedLoopRampup.Spd_Acc_Slope_per_tick_q31 = acc_q31; }

/** Set speed ramp deceleration (Q31/tick). */
void MotorControl_SetSpeedDec_Q31(sint32 deacc_q31){ MC_PTR()->speedLoopRampup.Spd_Deacc_Slope_per_tick_q31 = deacc_q31; }

/** Set ADC Sampling Minimum window time Tmin (Q15). */
void MotorControl_SetMinSampleTime_Q15(sint16 time_q15){ MC_PTR()->svpwmInput.Tmin_q15 = time_q15; }

/** Set the DC bus nominal voltage in milivolt unit (uint32). */
void MotorControl_SetVBBmiliVolt(uint32 voltage) { MC_PTR()->u32Vbase_mV = voltage; }

/** Set SMO current decay (Q15). */
void MotorControl_SetSmoDecay_Q15(sint16 smodecay_q15) { MC_PTR()->smo.currentDecay_q15 = smodecay_q15; }

/** Set SMO voltage gain (Q15). */
void MotorControl_SetSmoVgain_Q15(sint16 smovgain_q15)  { MC_PTR()->smo.voltageGain_q15 = smovgain_q15; }

/** Set SMO sliding gain (Q15). */
void MotorControl_SetSmoKslide_Q15(sint16 gain_q15) { MC_PTR()->smo.Kslide_q15 = gain_q15; }

/** Set SMO linear region threshold (Q15). */
void MotorControl_SetSmoMaxErr_Q15(sint16 max_q15) { MC_PTR()->smo.maxError_q15 = max_q15; }

/** Set SMO back-EMF LPF coefficient (Q15). */
void MotorControl_SetSmoEmfK_Q15(sint16 cutoff_q15){ MC_PTR()->smo.emfFilterK_q15 = cutoff_q15; }

/** Set PLL Kp (Q15). */
void MotorControl_SetPllKp_Q15(sint16 kpPll_q15)	{ MC_PTR()->pll.Kp_q15 = kpPll_q15; }

/** Set PLL Ki (Q15). */
void MotorControl_SetPllKi_Q15(sint16 kiPll_q15)	{ MC_PTR()->pll.Ki_q15 = kiPll_q15; }

/** Set time delay compensation factor (Q15). */
void MotorControl_SetKTimeDelay_Q15(sint16 k_td_q15)	{ CFG_PTR()->K_Td_q15 = k_td_q15; }

/** Get measured Id (Q15). */
sint16 MotorControl_GetId_Q15(void)    { return MC_PTR()->measuredId_q15; }

/** Get measured Iq (Q15). */
sint16 MotorControl_GetIq_Q15(void)   { return MC_PTR()->measuredIq_q15; }

/** Get Id reference (Q15). */
sint16 MotorControl_GetIdRef_Q15(void){ return MC_PTR()->idRef_q15; }

/** Get Iq reference (Q15). */
sint16 MotorControl_GetIqRef_Q15(void) { return MC_PTR()->iqRef_q15; }

/** Get Vd command (Q15). */
sint16 MotorControl_GetVd_Q15(void)   { return MC_PTR()->pi_d.Voltage_out_q15; }

/** Get Vq command (Q15). */
sint16 MotorControl_GetVq_Q15(void)  { return MC_PTR()->pi_q.Voltage_out_q15; }

/** Get normalized T1 (Q15). */
sint16 MotorControl_GetT1_Q15(void)   { return MC_PTR()->svpwmNorm.T1_q15; }

/** Get normalized T2 (Q15). */
sint16 MotorControl_GetT2_Q15(void)    { return MC_PTR()->svpwmNorm.T2_q15; }

/** Get SVPWM sector (1..6). */
uint8 MotorControl_GetSector(void)	{ return MC_PTR()->svpwmOutput.sector;	}

/** Get Phase Current Ia (Q15). */
sint16 MotorControl_GetIa_Q15(void)		{	return MC_PTR()->currentReconstruction.Ia_q15; }

/** Get Phase Current Ib (Q15). */
sint16 MotorControl_GetIb_Q15(void)		{	return MC_PTR()->currentReconstruction.Ib_q15; }

/** Get Phase Current Ic (Q15). */
sint16 MotorControl_GetIc_Q15(void)		{	return MC_PTR()->currentReconstruction.Ic_q15; }

/** Get open-loop theta (Q15). */
sint16 MotorControl_GetThetaOl_Q15(void)		{	return MC_PTR()->theta_ol_q15; }

/** Get smo theta (Q15). */
sint16 MotorControl_GetThetaSmo_Q15(void)	{	return MC_PTR()->theta_smo_q15; }

/** Get used theta (Q15). */
sint16 MotorControl_GetThetaUsed_Q15(void)	{	return MC_PTR()->theta_used_q15; }

/** Get open-loop omega (Q15). */
sint16 MotorControl_GetOmegaOl_Q15(void)	{	return MC_PTR()->omega_ol_q15; }

/** Get used omega (Q15). */
sint16 MotorControl_GetOmegaUsed_Q15(void)	{	return MC_PTR()->omega_used_q15; }

/** Get speed reference (Q15). */
sint16 MotorControl_GetSpeedRef_Q15(void)	{	return MC_PTR()->speedLoopPi.Spd_Ref_q15; }

/** Get speed value (Q15). */
sint16 MotorControl_GetSpeed_Q15(void)		{	return MC_PTR()->speedLoopPi.Spd_Fb_q15; }

/** Get measured bus voltage Vactual (uint32). */
uint32 MotorControl_GetVbus_MiliVolt(void)		{	return MC_PTR()->u32Vactual_mV;	}