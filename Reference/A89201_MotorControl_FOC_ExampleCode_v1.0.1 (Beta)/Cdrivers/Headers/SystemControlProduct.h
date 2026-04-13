#ifndef HEADER_SYSTEM_CONTROL_PRODUCT_H
#define HEADER_SYSTEM_CONTROL_PRODUCT_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file       SystemControlProduct.h
 * @author     G. Sloan
 * @version    1.0
 *
 * @brief      MCU system control register interface
 *
 * @copyright (c) 2024 Allegro MicroSystems, Inc. All Rights Reserved.
 * The information contained in this file is confidential and proprietary.
 *
 * The information contained in this file is governed by Software License
 * acknowledged during the download from Allegro's Software Download
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
// ScuDiagnostic register
#define SCU_DIAG_POR_REG     ( 0b0000000000000001U )
#define SCU_DIAG_SYS_REG     ( 0b0000000000000010U )
#define SCU_DIAG_WDT_REG     ( 0b0000000000000100U )
#define SCU_DIAG_LKUP_REG    ( 0b0000000000001000U )
#define SCU_DIAG_MCLR_REG    ( 0b0000000000010000U )
#define SCU_DIAG_PCFG_REG    ( 0b0000000000100000U )
#define SCU_DIAG_PLLOK_REG   ( 0b0000100000000000U )
#define SCU_DIAG_PLLTO_REG   ( 0b0001000000000000U )
#define SCU_DIAG_SRCTO_REG   ( 0b0010000000000000U )

#define SCU_DIAG_POR_REG_BITPOS   (0U)
#define SCU_DIAG_SYS_REG_BITPOS   (1U)
#define SCU_DIAG_WDT_REG_BITPOS   (2U)
#define SCU_DIAG_LKUP_REG_BITPOS  (3U)
#define SCU_DIAG_MCLR_REG_BITPOS  (4U)
#define SCU_DIAG_PCFG_REG_BITPOS  (5U)
#define SCU_DIAG_PLLOK_REGBITPOS  (11U)
#define SCU_DIAG_PLLTO_REG_BITPOS (12U)
#define SCU_DIAG_SRCTO_REG_BITPOS (13U)

// ScuDiagnostic cached RAM mask
#define SCU_DIAG_POR_CACHE     ( 0b0000000000000001U )
#define SCU_DIAG_SYS_CACHE     ( 0b0000000000000010U )
#define SCU_DIAG_WDT_CACHE     ( 0b0000000000000100U )
#define SCU_DIAG_LKUP_CACHE    ( 0b0000000000001000U )
#define SCU_DIAG_MCLR_CACHE    ( 0b0000000000010000U )
#define SCU_DIAG_PLLOK_CACHE   ( 0b0000000000100000U )
#define SCU_DIAG_PLLTO_CACHE   ( 0b0000000001000000U )
#define SCU_DIAG_SRCTO_CACHE   ( 0b0000000010000000U )

#define SCU_CONTROL_MCLRN_RESET       ( 0b0000000010000000U )
#define SCU_CONTROL_MCLRN_RESET_EN    ( 0b0000000100000000U )


/** Enumerate bit ranges in the diagnostic register */
enum Diagnostic
{
    /** The power on reset bit is set after a POR */
    eDIAGNOSTIC_POR = SCU_DIAG_POR_CACHE,
    /**The system reset request bit is set by a system reset request */
    eDIAGNOSTIC_SYS = SCU_DIAG_SYS_CACHE,
    /** The watchdog timer bit is set after a reset */
    eDIAGNOSTIC_WDT = SCU_DIAG_WDT_CACHE,
    /** The CPU lockup bit is set after a lockup reset */
    eDIAGNOSTIC_LOCKUP = SCU_DIAG_LKUP_CACHE,
    /** The master clear is set if the GDU caused a reset or NMI */
    eDIAGNOSTIC_MCLR = SCU_DIAG_MCLR_CACHE,
    /** The powerup pin configuration */
    eDIAGNOSTIC_PINCFG = SCU_DIAG_PCFG_REG,
    /** PLL locked and ok status */
    eDIAGNOSTIC_PLLLOCKED = SCU_DIAG_PLLOK_REG,
    /** PLL timeout */
    eDIAGNOSTIC_PLLTIMEOUT = SCU_DIAG_PLLTO_CACHE,
    /** PLL source timeout */
    eDIAGNOSTIC_PLLSRCTIMEOUT = SCU_DIAG_SRCTO_CACHE
};

/**
 * @brief  Enable Warm-reset from MCLRn (signal from the GDU) else generate NMI.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WarmResetOnMclrnEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_MCLRN_RESET);
}
 
/**
 * @brief  Disable Warm-reset from MCLRn (signal from the GDU) else generate NMI.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_WarmResetOnMclrnDisable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_MCLRN_RESET);
}

/**
 * @brief  Enable MCLRn signal from the GDU.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_MclrEnable(void)
{
    SetRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_MCLRN_RESET_EN);
}

/**
 * @brief  Disable MCLRn signal from the GDU.
 * @returns    None
 */
__attribute__( ( always_inline ) ) static inline void Scu_MclrDisable(void)
{
    ClearRegisterField((volatile uint32*)&stScuRegs.ctrl, SCU_CONTROL_MCLRN_RESET_EN);
}



#endif // HEADER_SYSTEM_CONTROL_PRODUCT_H

