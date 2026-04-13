/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/*
 * @file         Cau.c
 * @author       G. Sloan
 * @version      1.0
 *
 * @brief        Current Acquisition Unit interface: CAU
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
#include "A89201RegisterDefinitions.h"
#include "Cau.h"

static uint8 const ku8CauConvSpeedReset = 0xF9U;
static uint16 const ku16CauTrigSahTrigReset = 0xFF87U;
static uint8 const ku8CauTrigSahTrigBitMask = 0x78U;
static uint16 const ku16CauTrigSahPeriodReset = 0xFFF8U;
static uint16 const ku16CauTrigSahEdgeReset = 0xFE7F;
static uint16 const ku16CauCalTrigSourceSelectReset = 0xFFF0U;
static uint16 const ku16CauCalEdgeSelectReset = 0xFFCFU;
static uint16 const ku16CauIirCoefficientReset = 0xFFFCU;
 
// CAU interrupt data
static void (*spfnCauIrqHandler)(void) = NULLPTR;
static uintptr suPrCauData;
static IrqType_t const skIrqCau = ARM_IRQ_NUM(eIRQNUM_CAU);

void Cau_SetEnable( boolean const kbState )
{
    stCauRegs.CauControl = (uint32)kbState;
}


boolean Cau_GetEnable( void )
{
    boolean bRetVal = False;
    if ( stCauRegs.CauControl != 0 )
    {
        bRetVal = True;
    }
    return bRetVal;
}


void Cau_ClearAdcConfig( enum CauAdcSelect const keAdcSelect )
{
    switch(keAdcSelect)
    {
        case eCAU_A:
            stCauRegs.CauAdcA = 0;
            break;
        case eCAU_B:
            stCauRegs.CauAdcB = 0;
            break;
        case eCAU_C:
            stCauRegs.CauAdcC = 0;
            break;
        default:
             /* Do nothing */
            break;
    }
}


void Cau_SetAdcConfigCalibEnable( enum CauAdcSelect const keAdcSelect, enum CauAdcConfigCalibrationEnable const keAdcConfigCalibrationEnable )
{
    if ( keAdcConfigCalibrationEnable == eCAU_ADCCFGCAL_ENABLE )
    {
        switch(keAdcSelect)
        {
            case eCAU_A:
                stCauRegs.CauAdcA |= (uint32)eCAU_ADCCFGCAL_ENABLE;
                break;
            case eCAU_B:
                stCauRegs.CauAdcB |= (uint32)eCAU_ADCCFGCAL_ENABLE;
                break;
            case eCAU_C:
                stCauRegs.CauAdcC |= (uint32)eCAU_ADCCFGCAL_ENABLE;
                break;
            default:
                /* Do nothing */
                break;
        }
    }
    else
    {
        switch(keAdcSelect)
        {
            case eCAU_A:
                stCauRegs.CauAdcA &= (uint32)eCAU_ADCCFGCAL_DISABLE;
                break;
            case eCAU_B:
                stCauRegs.CauAdcB &= (uint32)eCAU_ADCCFGCAL_DISABLE;
                break;
            case eCAU_C:
                stCauRegs.CauAdcC &= (uint32)eCAU_ADCCFGCAL_DISABLE;
                break;
            default:
                /* Do nothing */
                break;
        }
    }
}


void Cau_SetAdcConfigConvSpeed( enum CauAdcSelect const keAdcSelect, enum CauAdcConfigConvSpeed const keAdcConfigConvSpeed )
{
    switch(keAdcSelect)
    {      
        case eCAU_A:
            stCauRegs.CauAdcA &= (uint32)ku8CauConvSpeedReset;
            stCauRegs.CauAdcA |= (uint32)keAdcConfigConvSpeed;
            break;
        case eCAU_B:
            stCauRegs.CauAdcB &= (uint32)ku8CauConvSpeedReset;
            stCauRegs.CauAdcB |= (uint32)keAdcConfigConvSpeed;
            break;
        case eCAU_C:
            stCauRegs.CauAdcC &= (uint32)ku8CauConvSpeedReset;
            stCauRegs.CauAdcC |= (uint32)keAdcConfigConvSpeed;
            break;
        default:
            /* Do nothing */
            break;
    }
}


void Cau_SetAdcConfigAdcEnable( enum CauAdcSelect const keAdcSelect, enum CauAdcConfigAdcEnable const keAdcConfigAdcEnable )
{  
    if ( keAdcConfigAdcEnable == eCAU_ADCCFGADC_ENABLE )
    {
        switch(keAdcSelect)
        {
            case eCAU_A:
                stCauRegs.CauAdcA |= (uint32)eCAU_ADCCFGADC_ENABLE;
                break;
            case eCAU_B:
                stCauRegs.CauAdcB |= (uint32)eCAU_ADCCFGADC_ENABLE;
                break;
            case eCAU_C:
                stCauRegs.CauAdcC |= (uint32)eCAU_ADCCFGADC_ENABLE;
                break;
            default:
                /* Do nothing */
                break;
        }
    }
    else
    {
        switch(keAdcSelect)
        {
            case eCAU_A:
                stCauRegs.CauAdcA &= (uint32)eCAU_ADCCFGADC_DISABLE;
                break;
            case eCAU_B:
                stCauRegs.CauAdcB &= (uint32)eCAU_ADCCFGADC_DISABLE;
                break;
            case eCAU_C:
                stCauRegs.CauAdcC &= (uint32)eCAU_ADCCFGADC_DISABLE;
                break;
            default:
                /* Do nothing */
                break;
        }
    }
}


void Cau_SetAdcConfigOverwritePermission( enum CauAdcSelect const keAdcSelect, enum CauAdcConfigOverwritePermission const keAdcConfigOverwritePermission )
{
    if ( keAdcConfigOverwritePermission == eCAU_ADCCFG_PERMITTED )
    {
        switch(keAdcSelect)
        {
          case eCAU_A:
              stCauRegs.CauAdcA |= (uint32)eCAU_ADCCFG_PERMITTED;
              break;
          case eCAU_B:
              stCauRegs.CauAdcB |= (uint32)eCAU_ADCCFG_PERMITTED;
              break;
          case eCAU_C:
              stCauRegs.CauAdcC |= (uint32)eCAU_ADCCFG_PERMITTED;
              break;
          default:
              /* Do nothing */
              break;
        }
    }
    else
    {
        switch(keAdcSelect)
        {
          case eCAU_A:
              stCauRegs.CauAdcA &= (uint32)eCAU_ADCCFG_NOTPERMITTED;
              break;
          case eCAU_B:
              stCauRegs.CauAdcB &= (uint32)eCAU_ADCCFG_NOTPERMITTED;
              break;
          case eCAU_C:
              stCauRegs.CauAdcC &= (uint32)eCAU_ADCCFG_NOTPERMITTED;
              break;
          default:
              /* Do nothing */
              break;
        }
    }
}


uint16 Cau_GetAdcConfig( enum CauAdcSelect const keAdcSelect )
{
    uint16 u16AdcConfig;
  
    switch(keAdcSelect)
    {
        case eCAU_A:
            u16AdcConfig = (uint16)stCauRegs.CauAdcA;
            break;
        case eCAU_B:
            u16AdcConfig = (uint16)stCauRegs.CauAdcB;
            break;
        case eCAU_C:
            u16AdcConfig = (uint16)stCauRegs.CauAdcC;
            break;
        default:
            u16AdcConfig = 0;
            break;    
    }
    return u16AdcConfig;
}


void Cau_ClearTrig( enum CauTrigSelect const keAdcSelect )
{
    switch(keAdcSelect)
    {
        case eCAU_TRIGSEL_A:
            stCauRegs.CauTrigA = 0;
            break;
        case eCAU_TRIGSEL_B:
            stCauRegs.CauTrigB = 0;
            break;
        case eCAU_TRIGSEL_C:
            stCauRegs.CauTrigC = 0;
            break;
        default:
            /* Do nothing */
            break;
    }
}


void Cau_SetTrigSahPeriod( enum CauTrigSelect const keAdcSelect, enum CauTrigSahPeriod const keTrigSahPeriod )
{
    switch(keAdcSelect)
    {
        case eCAU_TRIGSEL_A:
            stCauRegs.CauTrigA &= (uint32)ku16CauTrigSahPeriodReset;
            stCauRegs.CauTrigA |= (uint32)keTrigSahPeriod;
            break;
        case eCAU_TRIGSEL_B:
            stCauRegs.CauTrigB &= (uint32)ku16CauTrigSahPeriodReset;
            stCauRegs.CauTrigB |= (uint32)keTrigSahPeriod;
            break;
        case eCAU_TRIGSEL_C:
            stCauRegs.CauTrigC &= (uint32)ku16CauTrigSahPeriodReset;
            stCauRegs.CauTrigC |= (uint32)keTrigSahPeriod;
            break;
        default:
            /* Do nothing */
            break;
    }
}


void Cau_SetTrigSahTrig( enum CauTrigSelect const keAdcSelect, enum CauTrigSahTrig const keTrigSahTrig )
{
    switch(keAdcSelect)
    {
        case eCAU_TRIGSEL_A:
            stCauRegs.CauTrigA &= (uint32)ku16CauTrigSahTrigReset;
            stCauRegs.CauTrigA |= (uint32)keTrigSahTrig;
            break;
        case eCAU_TRIGSEL_B:
            stCauRegs.CauTrigB &= (uint32)ku16CauTrigSahTrigReset;
            stCauRegs.CauTrigB |= (uint32)keTrigSahTrig;
            break;
        case eCAU_TRIGSEL_C:
            stCauRegs.CauTrigC &= (uint32)ku16CauTrigSahTrigReset;
            stCauRegs.CauTrigC |= (uint32)keTrigSahTrig;
            break;
        default:
            /* Do nothing */
            break;
    }
}


void Cau_SetTrigSahTrigEdge( enum CauTrigSelect const keAdcSelect, enum CauTrigSahTrigEdge const keTrigSahTrigEdge )
{
    switch(keAdcSelect)
    {
        case eCAU_TRIGSEL_A:
            stCauRegs.CauTrigA &= (uint32)ku16CauTrigSahEdgeReset;
            stCauRegs.CauTrigA |= (uint32)keTrigSahTrigEdge;
            break;
        case eCAU_TRIGSEL_B:
            stCauRegs.CauTrigB &= (uint32)ku16CauTrigSahEdgeReset;
            stCauRegs.CauTrigB |= (uint32)keTrigSahTrigEdge;
            break;
        case eCAU_TRIGSEL_C:
            stCauRegs.CauTrigC &= (uint32)ku16CauTrigSahEdgeReset;
            stCauRegs.CauTrigC |= (uint32)keTrigSahTrigEdge;
            break;
        default:
            /* Do nothing */
            break;
    }
}


uint16 Cau_GetTrig( enum CauTrigSelect const keAdcSelect )
{
    uint16 u16Trig;
  
    switch(keAdcSelect)
    {
        case eCAU_TRIGSEL_A:
            u16Trig = (uint16)stCauRegs.CauTrigA;
            break;
        case eCAU_TRIGSEL_B:
            u16Trig = (uint16)stCauRegs.CauTrigB;
            break;
        case eCAU_TRIGSEL_C:
            u16Trig = (uint16)stCauRegs.CauTrigC;
            break;
        default:
            u16Trig = 0;
            break;
    }
    return u16Trig;
}


void Cau_SetSwTrig( enum CauSwTrig const keSwTrig )
{
    stCauRegs.CauSwtrig = (uint32)keSwTrig;
}


enum CauSwTrig Cau_GetSwTrig( void )
{
    return (enum CauSwTrig)stCauRegs.CauSwtrig;
}


void Cau_ClearCal( enum CauCalSelect const keAdcSelect )
{
    switch(keAdcSelect)
    {
        case eCAU_CALSELECT_A:
            stCauRegs.CauCalibrationA = 0;
            break;
        case eCAU_CALSELECT_B:
            stCauRegs.CauCalibrationB = 0;
            break;
        case eCAU_CALSELECT_C:
            stCauRegs.CauCalibrationC = 0;
            break;
        default:
            /* Do nothing */
            break;
    }
}


void Cau_SetCalTrigSourceSel( enum CauCalSelect const keAdcSelect, enum CauCalTrigSourceSel const keCalTrigSourceSel )
{
    switch(keAdcSelect)
    {
        case eCAU_CALSELECT_A:
            stCauRegs.CauCalibrationA &= (uint32)ku16CauCalTrigSourceSelectReset;
            stCauRegs.CauCalibrationA |= (uint32)keCalTrigSourceSel;
            break;
        case eCAU_CALSELECT_B:
            stCauRegs.CauCalibrationB &= (uint32)ku16CauCalTrigSourceSelectReset;
            stCauRegs.CauCalibrationB |= (uint32)keCalTrigSourceSel;
            break;
        case eCAU_CALSELECT_C:
            stCauRegs.CauCalibrationC &= (uint32)ku16CauCalTrigSourceSelectReset;
            stCauRegs.CauCalibrationC |= (uint32)keCalTrigSourceSel;
            break;
        default:
            /* Do nothing */
            break;
    }
}


void Cau_SetCalEdgeSel( enum CauCalSelect const keAdcSelect, enum CauCalEdgeSel const keCalEdgeSel )
{
    switch(keAdcSelect)
    {
        case eCAU_CALSELECT_A:
            stCauRegs.CauCalibrationA &= (uint32)ku16CauCalEdgeSelectReset;
            stCauRegs.CauCalibrationA |= (uint32)keCalEdgeSel;
            break;
        case eCAU_CALSELECT_B:
            stCauRegs.CauCalibrationB &= (uint32)ku16CauCalEdgeSelectReset;
            stCauRegs.CauCalibrationB |= (uint32)keCalEdgeSel;
            break;
        case eCAU_CALSELECT_C:
            stCauRegs.CauCalibrationC &= (uint32)ku16CauCalEdgeSelectReset;
            stCauRegs.CauCalibrationC |= (uint32)keCalEdgeSel;
            break;
        default:
            /* Do nothing */
            break;
    }
}


uint16 Cau_GetCal( enum CauCalSelect const keAdcSelect )
{
    uint16 u16CalTrig;
  
    switch(keAdcSelect)
    {
        case eCAU_CALSELECT_A:
            u16CalTrig = (uint16)stCauRegs.CauCalibrationA;
            break;
        case eCAU_CALSELECT_B:
            u16CalTrig = (uint16)stCauRegs.CauCalibrationB;
            break;
        case eCAU_CALSELECT_C:
            u16CalTrig = (uint16)stCauRegs.CauCalibrationC;
            break;
        default:
            u16CalTrig = 0;
            break;
    }
    return u16CalTrig;
  
}


void Cau_SetSwTrigCal( enum CauSwTrigCal const keSwTrigCal )
{
    stCauRegs.CauSwtrigCal = (uint32)keSwTrigCal;
}


enum CauSwTrigCal Cau_GetSwTrigCal( void )
{
    return (enum CauSwTrigCal)( stCauRegs.CauSwtrigCal );
}


void Cau_SetIir( enum CauIirSelect const keAdcSelect, enum CauIir const keIir )
{
    switch ( keIir )
    {
    case eCAU_IIR_COEFFICIENT_DIV1:
    case eCAU_IIR_COEFFICIENT_DIV2:
    case eCAU_IIR_COEFFICIENT_DIV4:
    case eCAU_IIR_COEFFICIENT_DIV8:
        switch(keAdcSelect)
        {
            case eCAU_IIRSELECT_A:
                stCauRegs.CauIirA &= (uint32) ku16CauIirCoefficientReset;
                stCauRegs.CauIirA |= (uint32) keIir;
                break;
            case eCAU_IIRSELECT_B:
                stCauRegs.CauIirB &= (uint32) ku16CauIirCoefficientReset;
                stCauRegs.CauIirB |= (uint32) keIir;
                break;
            case eCAU_IIRSELECT_C:
                stCauRegs.CauIirC &= (uint32) ku16CauIirCoefficientReset;
                stCauRegs.CauIirC |= (uint32) keIir;
                break;
            default:
                /* Do nothing */
                break;
        }
        break;

    case eCAU_IIR_ENABLE:
        switch(keAdcSelect)
        {
            case eCAU_IIRSELECT_A:
                stCauRegs.CauIirA |= (uint32) eCAU_IIR_ENABLE;
                break;
            case eCAU_IIRSELECT_B:
                stCauRegs.CauIirB |= (uint32) eCAU_IIR_ENABLE;
                break;
            case eCAU_IIRSELECT_C:
                stCauRegs.CauIirC |= (uint32) eCAU_IIR_ENABLE;
                break;
            default:
                /* Do nothing */
                break;
        }
        break;

    case eCAU_IIR_DISABLE:
        switch(keAdcSelect)
        {
            case eCAU_IIRSELECT_A:
                stCauRegs.CauIirA &= ~(uint32)eCAU_IIR_DISABLE;
                break;
            case eCAU_IIRSELECT_B:
                stCauRegs.CauIirB &= ~(uint32)eCAU_IIR_DISABLE;
                break;
            case eCAU_IIRSELECT_C:
                stCauRegs.CauIirC &= ~(uint32)eCAU_IIR_DISABLE;
                break;
            default:
                /* Do nothing */
                break;
        }
        break;

    default:
        /* Do nothing */
        break;
    }
}


enum CauIir Cau_GetIir( enum CauIirSelect const keAdcSelect )
{
    enum CauIir eCauIir;
  
    switch(keAdcSelect)
    {
        case eCAU_IIRSELECT_A:
            eCauIir = (enum CauIir)stCauRegs.CauIirA;
            break;
        case eCAU_IIRSELECT_B:
            eCauIir = (enum CauIir)stCauRegs.CauIirB;
            break;
        case eCAU_IIRSELECT_C:
            eCauIir = (enum CauIir)stCauRegs.CauIirC;
            break;
        default:
            eCauIir = (enum CauIir)stCauRegs.CauIirA;
            break;
    }
    return eCauIir;
}


void Cau_SetStatusError(enum CauStatusErr const keStatus )
{
    stCauRegs.CauErrStatus |= (uint32)keStatus;
}


boolean Cau_GetStatusError( enum CauStatusErr const keStatus )
{
    boolean bRetVal = False;

    uint32 const u32ErrorStatus = (uint32)stCauRegs.CauErrStatus;
    if ( (u32ErrorStatus & (uint32)keStatus) != 0 )
    {
        bRetVal = True;
    }
    return bRetVal;
}


void Cau_SetIrqErrorEnable( enum CauIrqErrorEnable const keIntErr )
{
    if ( keIntErr == eCAU_IRQERRORENABLE_NONE )
    {
        stCauRegs.CauErrInten = 0;
    }
    else
    {
        stCauRegs.CauErrInten |= (uint32)keIntErr;
    }

}


enum CauIrqErrorEnable Cau_GetIrqErrorEnable( void )
{
    return (enum CauIrqErrorEnable)( stCauRegs.CauErrInten );
}


enum CauPwrUpStatus Cau_GetPwrUpStatus( void )
{
    return (enum CauPwrUpStatus)( stCauRegs.CauPwrupStatus );
}


void Cau_SetConvStatus( enum CauConvStatus const keStatus )
{
    if ( keStatus == eCAU_CONVSTATUS_ANY )
    {
        stCauRegs.CauConvStatus = (uint32)eCAU_CONVSTATUS_ANY;
    }
    else
    {
        stCauRegs.CauConvStatus |= (uint32)keStatus;
    }
}


boolean Cau_GetConvStatus( enum CauConvStatus const keStatus )
{
    boolean bRetVal = False;

    uint32 const u32ErrorStatus = (uint32)stCauRegs.CauConvStatus;
    if ( (u32ErrorStatus & (uint32)keStatus ) != 0 )
    {
        bRetVal = True;
    }
    return bRetVal;
}


void Cau_SetConvIrqEnable( enum CauConvIrqEnable const keIrqEnable )
{
    if ( keIrqEnable == eCAU_CONVIRQENABLE_NONE )
    {
        stCauRegs.CauConvInten = 0;
    }
    else
    {
        stCauRegs.CauConvInten |= (uint32)keIrqEnable;
    }
}


enum CauConvIrqEnable Cau_GetConvIrqEnable( void )
{
    return (enum CauConvIrqEnable)( stCauRegs.CauConvInten );
}


uint16 Cau_GetOffset ( enum CauOffsetSelect const keAdcSelect )
{
    uint16 u16Offset;
  
    switch(keAdcSelect)
    {
        case eCAU_OFFSETSELECT_A:
            u16Offset = (uint16)stCauRegs.CauOffsetA;
            break;
        case eCAU_OFFSETSELECT_B:
            u16Offset = (uint16)stCauRegs.CauOffsetB;
            break;
        case eCAU_OFFSETSELECT_C:
            u16Offset = (uint16)stCauRegs.CauOffsetC;
            break;
        default:
            u16Offset = 0;
            break;
    }
    return u16Offset;
}


sint16 Cau_GetResult ( enum CauResultSelect const keAdcSelect )
{
    sint16 s16Result;
  
    switch(keAdcSelect)
    {
        case eCAU_RESULTSELECT_A:
            s16Result = (sint16)stCauRegs.CauResultA;
            break;
        case eCAU_RESULTSELECT_B:
            s16Result = (sint16)stCauRegs.CauResultB;
            break;
        case eCAU_RESULTSELECT_C:
            s16Result = (sint16)stCauRegs.CauResultC;
            break;
        default:
            s16Result = 0;
            break;
    }
    return s16Result;

}


void Cau_SetDmaConfig ( enum CauDmaConfig const keDmaConfig )
{
    switch ( keDmaConfig )
    {
        case eCAU_DMACONFIG_ACQUISITIONS_BEFORE_DMA_0:
        case eCAU_DMACONFIG_ACQUISITIONS_BEFORE_DMA_1:
        case eCAU_DMACONFIG_ACQUISITIONS_BEFORE_DMA_2:
        case eCAU_DMACONFIG_ACQUISITIONS_BEFORE_DMA_3:
            stCauRegs.CauDmaConfig |= (uint32)keDmaConfig;
            break;

        case eCAU_DMACONFIG_ENABLE_A:
        case eCAU_DMACONFIG_ENABLE_B:
        case eCAU_DMACONFIG_ENABLE_C:
            stCauRegs.CauDmaConfig |= (uint32)keDmaConfig;
            break;

        case eCAU_DMACONFIG_ENABLE_RESET:
            stCauRegs.CauDmaConfig &= (uint32)eCAU_DMACONFIG_ENABLE_RESET;
            break;
        default:
            /* Do nothing */
            break;
    }
}


enum CauDmaConfig Cau_GetDmaConfig( void )
{
    return (enum CauDmaConfig)( stCauRegs.CauDmaConfig );
}



void Cau_SetIrqHandler( void (pfnIrqHandler)(void), const uintptr uPrData )
{
    spfnCauIrqHandler = *pfnIrqHandler;
    suPrCauData = uPrData;

    // Handler set, so now safe to clear and enable interrupt
    NVIC_ClearPendingIRQ( (IrqType_t)( eIRQNUM_CAU ) );
    NVIC_EnableIRQ( (IrqType_t)( eIRQNUM_CAU ) );
}


__attribute__( ( interrupt( "IRQ" ) ) ) void Cau_IrqHandler(void)
{
    NVIC_DisableIRQ( skIrqCau );
    NVIC_ClearPendingIRQ( skIrqCau );

    if ( spfnCauIrqHandler != NULLPTR )
    {
        spfnCauIrqHandler();
    }

    NVIC_EnableIRQ( skIrqCau );
}

