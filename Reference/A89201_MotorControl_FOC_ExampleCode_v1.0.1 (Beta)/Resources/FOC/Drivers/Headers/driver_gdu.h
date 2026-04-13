#ifndef HEADER_DRIVER_GDU_H
#define HEADER_DRIVER_GDU_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         driver_gdu.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Gate Driver Unit interface: GDU
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
 /* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */

#include "Platform_Types.h"
#include "Std_Types.h"
#include "GduSlave.h"
#include "GduMaster.h"


/* =====================================================================
 * REGISTER BIT DEFINITIONS
 * ===================================================================== */

/* ---------------- DRIVER (0x00) ----------------
 * bit 15:14  VCPM   Charge pump mode
 * bit 13    	RES    Reserved
 * bit 12    	VDQ    Blanking(1)/Debounce(0)
 * bit 11:6  	VQT    Qualifier time (0.1 µs/step)
 * bit 5:0   	VT     VDS threshold (50 mV/step)
 */
#define DRIVER_VT_Pos        0
#define DRIVER_VT_Msk        (0x3Fu << DRIVER_VT_Pos)

#define DRIVER_VQT_Pos       6
#define DRIVER_VQT_Msk       (0x3Fu << DRIVER_VQT_Pos)

#define DRIVER_VDQ_BLANK     (1u << 12)

#define DRIVER_VCPM_Pos      14
#define DRIVER_VCPM_Msk      (0x3u << DRIVER_VCPM_Pos)

/* ---------------- SENSESEL (0x01) ----------------
 * bit 15:9  RES    Reserved
 * bit 8     OSO    Offset-only mode
 * bit 7     SC     Calibration trigger
 * bit 6:3   SAO    Output pedestal code
 * bit 2:0   SAG    Gain code
 */
#define SENSE_SAG_Pos        0
#define SENSE_SAG_Msk        (0x7u << SENSE_SAG_Pos)

#define SENSE_SAO_Pos        3
#define SENSE_SAO_Msk        (0xFu << SENSE_SAO_Pos)

#define SENSE_SC_CALIB       (1u << 7)
#define SENSE_OSO_OFFSET     (1u << 8)

/* ---------------- AMUX_BIAS (0x02) ----------------
 * [ refer to the datasheet, reserved here ]
 */

/* ---------------- DIFFAMP (0x03) ----------------
 * bit 15:3  RES    Reserved
 * bit 2:0   SDG    Differential amplifier gain
 */
#define DIFF_GAIN_X5         	0		/* SDG=000 */
#define DIFF_GAIN_X8   				1 	/* SDG=001 */
#define DIFF_GAIN_X10  				2 	/* SDG=010 (reset default in many revs) */
#define	DIFF_GAIN_X13 				3 	/* SDG=011 */
#define DIFF_GAIN_X15 				4 	/* SDG=100 */
#define DIFF_GAIN_X18 				5 	/* SDG=101 */
#define DIFF_GAIN_X20 				6 	/* SDG=110 */
#define DIFF_GAIN_X25 				7   /* SDG=111 */


/* ---------------- SLEW_RISE (0x04), SLEW_FALL (0x05) ----------------
 * [ refer to the datasheet, reserved here ]
 */

/* ---------------- GDUCFG (0x06) ----------------
 * bit 15:12 RES    Reserved
 * bit 11    OPM    Mode (1=PWM, 0=LIN)
 * bit 10    LBB    LIN baud rate (1=20k, 0=10k)
 * bit 9     LEN    LIN enable
 * bit 8     DWL    Watchdog enable
 * bit 7:0   TTEN   Test mode enable (factory use)
 */
#define CFG_TTEN_Msk         0x00FFu
#define CFG_DWL              (1u << 8)
#define CFG_LEN              (1u << 9)
#define CFG_LBB_20k          (1u << 10)
#define CFG_OPM_PWM          (1u << 11)

/* ---------------- SOF (0x07) ----------------
 * [ refer to the datasheet, reserved here ]
 */


/* ---------------- FAULTMASK (0x08) ----------------
 * 1 = Mask (disable fault reporting)
 * 0 = Active (fault will trigger status/fault flag)
 *
 * bit15..8 */
#define FMASK_OT    (1u << 15)  /* Over temperature */
#define FMASK_TW    (1u << 14)  /* Temperature warning */
#define FMASK_VCPU  (1u << 13)  /* VCP undervoltage */
#define FMASK_VBU   (1u << 12)  /* VBB undervoltage */
#define FMASK_VRU   (1u << 11)  /* VREG undervoltage */
#define FMASK_RES10 (1u << 10)  /* reserved */
#define FMASK_SBOU  (1u << 9)   /* Sensor BIAS undervoltage */
#define FMASK_ETO   (1u << 8)   /* ENABLE time-out */

/* bit7..0 */
#define FMASK_WD    (1u << 7)   /* Q&A Watchdog error */
#define FMASK_BSU   (1u << 6)   /* Bootstrap undervoltage */
#define FMASK_CHO   (1u << 5)   /* Phase C high-side VDS over-voltage */
#define FMASK_CLO   (1u << 4)   /* Phase C low-side VDS over-voltage */
#define FMASK_BHO   (1u << 3)   /* Phase B high-side VDS over-voltage */
#define FMASK_BLO   (1u << 2)   /* Phase B low-side VDS over-voltage */
#define FMASK_AHO   (1u << 1)   /* Phase A high-side VDS over-voltage */
#define FMASK_ALO   (1u << 0)   /* Phase A low-side VDS over-voltage */

/* ---------------- GATEDRV (0x7A) ----------------
 * bit 15    FLT    FAULTn pin open-drain enable
 * bit 14:13 RES    Reserved
 * bit 12    GTS    Sleep trigger
 * bit 11:7  RES    Reserved
 * bit 6     ENBR   Enable bridge
 * bit 5:0   Phase control bits (AH, AL, BH, BL, CH, CL)
 */
#define GATE_AH              	(1u << 0)
#define GATE_AL              	(1u << 1)
#define GATE_BH              	(1u << 2)
#define GATE_BL              	(1u << 3)
#define GATE_CH              	(1u << 4)
#define GATE_CL              	(1u << 5)
#define GATE_ENBR            	(1u << 6)
#define GATE_GTS_SLEEP       	(1u << 12)
#define GATE_FLT_          		(1u << 15)

/* ---------------- DIAG1 (0x7E) ----------------
 * [ refer to the datasheet, reserved here ]
 */

/* ---------------- STATUS (0x7F) ----------------
 * Address : 0x7F
 * Reset   : 0x7800
 * Legend  : R   = Read-only
 *           R1C = Read & Write-1-to-Clear
 */

/* bit15..8 (upper byte) */
#define STAT_FF     (1u << 15)  /* R   : Fault flag (set when any fault is active; IG does not affect FF) */
#define STAT_POR    (1u << 14)  /* R1C : Power-on-reset */
#define STAT_SE     (1u << 13)  /* R   : Serial communication error (flagged by ETO, ENV, ESZ, EAC, ECRC) */
#define STAT_EE     (1u << 12)  /* R   : NVM error */
#define STAT_DSO    (1u << 11)  /* R   : VDS overvoltage (flagged when AHO/ALO/BHO/BLO/CHO/CLO are active) */
#define STAT_IG     (1u << 10)  /* R   : Ignition input state (0=low, 1=high) */
#define STAT_OT     (1u << 9)   /* R   : Overtemperature */
#define STAT_TW     (1u << 8)   /* R1C : Temperature warning */

/* bit7..0 (lower byte) */
#define STAT_BU     (1u << 7)   /* R   : Bootstrap undervoltage (flagged when VA, VB, VC are active) */
#define STAT_WD     (1u << 6)   /* R1C : Q&A watchdog fault */
#define STAT_ETO    (1u << 5)   /* R1C : ENABLE watchdog timeout */
#define STAT_SBOU   (1u << 4)   /* R1C : Sensor BIAS undervoltage */
#define STAT_VCPU   (1u << 3)   /* R1C : VCP undervoltage */
#define STAT_VBU    (1u << 2)   /* R1C : VBB undervoltage */
#define STAT_VRU    (1u << 1)   /* R1C : VREG undervoltage */
#define STAT_VLRU   (1u << 0)   /* R1C : VLR undervoltage */




extern volatile uint16 u16Gdu_diffAmpGain;
/**
 * @brief initialize I2C communication interface
 * @param[in] pclsGduMaster which provides baseline interface for die to die communication
 */
void Gdu_Initialization();
/**
 * @brief  clears GDU status register
 */
void Gdu_ClearGduStatus();
/**
 * @brief configures sense amplifier gain
 * @param[in] uint16Gain to define the sense amplifier gain
 */
void Gdu_SetDiffAmpGain();
/**
 * @brief receive status register information from GDU
 */
uint16  Gdu_GetStatus();	




#endif //HEADER_DRIVER_GDU_H