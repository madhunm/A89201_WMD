/**
 * @file         UserParamters.c
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

#include "UserParameters.h"
#include "foc_control_api.h"

/**
 * @details     Apply all parameters to foc_control_api (conversions + range checks inside)				
 * @remarks     
 * @param[in] 	None
 * @returns 		None
 */
void UserParameters_Init( void )
{
		/* Based on the provided GUI tool, populate each of the following functions with the corresponding calculated values. */	
		MotorControl_SetDiffAmpGain (DIFF_GAIN_X10);																			/* refer to Section 4.0  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetPwmPeriod(4000);																			/* refer to Section 4.1  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetDeadtime(40);																					/* refer to Section 4.2  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetRun(0);																								/* refer to Section 4.3  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetDir(1);																								/* refer to Section 4.4  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetIdleBlank(10);																				/* refer to Section 4.5  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetAlignCurrent_Q15(1638);																/* refer to Section 4.6  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetAlignAngle_Q15(0);																		/* refer to Section 4.7  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetAlignDuration(10000);																	/* refer to Section 4.8  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetRampCurrent_Q15(2458);																/* refer to Section 4.9  in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetRampOmegaTarget_Q15(6553);														/* refer to Section 4.10 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetRampDuration(20000);																	/* refer to Section 4.11 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetRampOmegaStep_Q31(5369);															/* refer to Section 4.12 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetOmega2Theta_Q15(3277);																/* refer to Section 4.13 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSwitchMinIq_Q15(819);																	/* refer to Section 4.14 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSwitchIqStep_Q31(26890);															/* refer to Section 4.15 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSwitchThetaErr_Q15(1820);															/* refer to Section 4.16 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSwitchObsReadyTicks(100);															/* refer to Section 4.17 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetErrTimeoutTicks(10000);																/* refer to Section 4.18 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSwitchSpdLpRdyCount(10000);														/* refer to Section 4.19 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetCurrentLimit_Q15(32767);															/* refer to Section 4.20 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetOverCurrDebounce(3);																	/* refer to Section 4.21 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetVdqMax_Q15(18919);																		/* refer to Section 4.22 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetAdc2Ibase_Q15(24);																		/* refer to Section 4.23 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetDauAdc2VbbMicroV(12310);															/* refer to Section 4.24 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetAdc2Vbase_Q15(32767);																	/* refer to Section 4.25 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSpdCmd_Q15(0);																				/* refer to Section 4.26 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetOmega2Rpm_Q15(32767);																	/* refer to Section 4.27 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetIdKp_Q15(17157);// auto Q15 (Kp_pu=0.4189)  					/* refer to Section 4.28 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetIdKi_Q15(1029); // auto Q15 (Ki_pu=0.01257) 					/* refer to Section 4.28 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetIqKp_Q15(17157);// auto Q15 (Kp_pu=0.4189)  					/* refer to Section 4.28 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetIqKi_Q15(1029);	// auto Q15 (Ki_pu=0.01257)	 					/* refer to Section 4.28 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetIdPiVoutMax_Q15(18919);																/* refer to Section 4.29 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetIqPiVoutMax_Q15(18919);																/* refer to Section 4.29 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSpeedPiKp_Q15(25960);	// auto Q15 (Kp_pu=0.04871)  		/* refer to Section 4.30 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSpeedPiKi_Q15(3);	// auto Q15 (Ki_pu=0.0002165)				/* refer to Section 4.30 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSpeedPiOutMax_Q15(32767);															/* refer to Section 4.31 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSpeedPiOutMin_Q15(-32767);														/* refer to Section 4.31 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSpeedAcc_Q31(536871);																	/* refer to Section 4.32 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSpeedDec_Q31(715828);																	/* refer to Section 4.32 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetMinSampleTime_Q15(2500);															/* refer to Section 4.33 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetVBBmiliVolt(12000);																		/* refer to Section 4.34 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSmoDecay_Q15(31785);																	/* refer to Section 4.35 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSmoVgain_Q15(4915);																		/* refer to Section 4.35 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSmoKslide_Q15(11469);																	/* refer to Section 4.36 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSmoMaxErr_Q15(5459);																	/* refer to Section 4.37 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetSmoEmfK_Q15(5459);																		/* refer to Section 4.38 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetPllKp_Q15(18192);	// auto Q15 (Kp_pu=0.1414)   				/* refer to Section 4.39 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetPllKi_Q15(808);		// auto Q15 (Ki_pu=0.001)    				/* refer to Section 4.39 in UserParameters_Calc_and_Norm.txt */
		MotorControl_SetKTimeDelay_Q15(24576);																/* refer to Section 4.40 in UserParameters_Calc_and_Norm.txt */

}


