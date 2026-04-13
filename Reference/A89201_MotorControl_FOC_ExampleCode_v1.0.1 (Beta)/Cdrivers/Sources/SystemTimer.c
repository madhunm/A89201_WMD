/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @internal
 *
 * @file         SystemTimer.c
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
 */
 /* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
#include "SystemTimer.h"
#include "system_ARMCM4.h"

// NMI
static void (*spfnSysTickIrqHandler)(void* pContext) = NULLPTR;
static uintptr suPrSysTickData;
static uint32 s_u32TenMs = 0; // Reload value for 10ms (100Hz) timing, subject to system clock skew errors.
static boolean s_bSkew = 0; // Clock skew errors. If the value reads as zero, the calibration value is not known.
static boolean s_bNoRef = 0; // Indicates whether the device provides a reference clock to the processor:


boolean SystemTimer_GetIsClockSourceProcessor(void)
{
    boolean bClockSourceIsProcessor;
    ReadField((volatile uint32*)&stSystemTimerRegs.systCsr, CSR_CLKSOURCE_MASK, CSR_CLKSOURCE_BITPOS, bClockSourceIsProcessor);
    return bClockSourceIsProcessor;
}  


boolean SystemTimer_GetCountFlag(void)
{
    return   (boolean)((stSystemTimerRegs.systCsr & CSR_COUNTFLAG_MASK) > 0);
}  


boolean SystemTimer_GetSkew(void)
{
    return s_bSkew;
}


boolean SystemTimer_GetNoRef(void)
{
    return s_bNoRef;
}


uint32 SystemTimer_GetTenMs(void)
{
    return s_u32TenMs;
}


void SystemTimer_Initialize(void)
{
    uint32 u32Val = stSystemTimerRegs.systCalib;
    ReadField((volatile uint32*)&stSystemTimerRegs.systCalib, CALIB_TENMS_MASK, CALIB_TENMS_BITPOS, s_u32TenMs);
    ReadField((volatile uint32*)&stSystemTimerRegs.systCalib, CALIB_SKEW_MASK, CALIB_SKEW_BITPOS, s_bSkew);
    ReadField((volatile uint32*)&stSystemTimerRegs.systCalib, CALIB_NOREF_MASK, CALIB_NOREF_BITPOS, s_bNoRef);
    if(s_u32TenMs == 0)
    {
        // Calibration value 0/unknown. Calculate the calibration value required from the frequency of the processor clock.
        // 40MHz clock, 40000 * 25ns = 10ms.
        s_u32TenMs = (uint32)(SYSTEM_CLOCK * 0.01);
    }
}


void SystemTimer_Enable()
{
    ReadModifyWriteField((volatile uint32*)&stSystemTimerRegs.systCsr, CSR_ENABLE_MASK, CSR_ENABLE_BITPOS, 0x1U);
}


void SystemTimer_Disable()
{
    ReadModifyWriteField((volatile uint32*)&stSystemTimerRegs.systCsr, CSR_ENABLE_MASK, CSR_ENABLE_BITPOS, 0x0U);
}


void SystemTimer_SetReload( uint32 const ku32Count )
{
    // Clear current value
    stSystemTimerRegs.systCvr = 0;

    // Set reload value
    stSystemTimerRegs.systRvr = ku32Count & SVR_RELOAD_MASK;
}


void SystemTimer_SetIrqEnable( boolean const kbEnable )
{
    if ( kbEnable == True )
    {
        ReadModifyWriteField((volatile uint32*)&stSystemTimerRegs.systCsr, CSR_TICKINT_MASK, CSR_TICKINT_BITPOS, 0x1U);
    }
    else
    {
        ReadModifyWriteField((volatile uint32*)&stSystemTimerRegs.systCsr, CSR_TICKINT_MASK, CSR_TICKINT_BITPOS, 0x0U);
    }
}


boolean SystemTimer_HasIrqHandler(void)
{
    return (boolean)(spfnSysTickIrqHandler != NULLPTR );
}


void SystemTimer_SetSysTickIrqHandler( void (pfnIrqHandler)(void* pContext), uintptr uPrData )
{
    spfnSysTickIrqHandler = *pfnIrqHandler;
    suPrSysTickData = uPrData;
}


__attribute__( ( interrupt( "IRQ" ) ) ) void SystemTimer_SysTickIrqHandler()
{
    if ( spfnSysTickIrqHandler != NULLPTR )
    {
        spfnSysTickIrqHandler( (void*)&suPrSysTickData );
    }
}
