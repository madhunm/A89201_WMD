/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @internal
 *
 * @file         Spi.c
 *
 * @copyright (c) 2024 - 2025 Allegro MicroSystems, Inc. All Rights Reserved.
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
 * @details      Implementation of the EnSillica SPI v5.0.7
 */
 /* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
#include "Spi.h"
#include "Sci.h"
#include "system_ARMCM4.h"

static SpiRegs_t* pSpiRegs = (SpiRegs_t*)&stSpiRegs;                                    
static uint8 m_u8SpiPortNum;

void Spi_SetPort(enum SpiId ePortId)
{
    switch(ePortId)
    {
        case eSPI_PORTID_0:
            pSpiRegs = (SpiRegs_t*)SPI0_ADDRESS;
            m_u8SpiPortNum = (uint8) eSPI_PORTID_0;
            Sci_Use(eSPI,m_u8SpiPortNum);
            break;
        
        case eSPI_PORTID_1:
            pSpiRegs = (SpiRegs_t*)SPI1_ADDRESS;
            m_u8SpiPortNum = (uint8) eSPI_PORTID_1;
            Sci_Use(eSPI,m_u8SpiPortNum);
            break;
        
        default:
            pSpiRegs = (SpiRegs_t*)SPI0_ADDRESS;
            break;
    }
}


void Spi_Open(void)
{
    pSpiRegs->spiControl |= (uint16) eSPI_ENABLE; 
}

void Spi_Close(void)
{
    pSpiRegs->spiControl &= (uint16) eSPI_DISABLE_DEFAULT; 
}

boolean Spi_IsOpen(void)
{
    boolean bIsOpen = False;
    
    uint32 const u32SpiOpen = pSpiRegs->spiControl; 
	
    if ((u32SpiOpen & (uint16) eSPI_ENABLE) != 0)
    {
        bIsOpen = True;
    }
    return bIsOpen;
}

void Spi_SetMode(enum SpiMode const keSpiMode) 
{    
     switch(keSpiMode)
    {
        case eMASTER_DEFAULT:
            pSpiRegs->spiControl &= (uint16) eMASTER_DEFAULT;
        break;
        
        case eSLAVE:
            pSpiRegs->spiControl |= (uint16) eSLAVE;
        break;
                       
        default: // No update         
            break;
    }
}

boolean Spi_GetMode(void)
{
    boolean bMode = False;
    
    uint32 const u32SpiMode = pSpiRegs->spiControl; 
	
    if ((u32SpiMode & (uint16) eSLAVE) != 0)
    {
        bMode = True;
    }
    return bMode;
}

void Spi_SetWordSize(enum SpiWordSize const keSpiWordSize) 
{    
     switch(keSpiWordSize)
    {
        case e8_BITS_DEFAULT:
            pSpiRegs->spiControl &= (uint16) e8_BITS_DEFAULT;
        break;
        
        case e16_BITS:
            pSpiRegs->spiControl |= (uint16)e16_BITS;
        break;
                       
        default: // No update           
            break;
    }
}

boolean Spi_GetWordSize(void)
{       
   boolean bWordSize = False;
    
   uint32 const u32SpiWordSize = pSpiRegs->spiControl; 

   if ((u32SpiWordSize & (uint16) e16_BITS) != 0)
   {
       bWordSize = True;
   }
   return bWordSize;
}

void Spi_SetBitOrdering(enum SpiBitOrdering const keSpiBitOrdering) 
{    
     switch(keSpiBitOrdering)
    {
        case eMSB_DEFAULT:
            pSpiRegs->spiControl &= (uint16) eMSB_DEFAULT;
        break;
        
        case eLSB:
            pSpiRegs->spiControl &= (uint16) eMSB_DEFAULT;
            pSpiRegs->spiControl |= (uint16) eLSB;
        break;
        
        default: // No update          
            break;
    }
}

boolean Spi_GetBitOrdering(void)
{
   boolean bBitOrdering = False;
    
   uint32 const u32SpiBitOrdering = pSpiRegs->spiControl; 

   if ((u32SpiBitOrdering & (uint16) eLSB) != 0)
   {
       bBitOrdering = True;
   }
   return bBitOrdering;
}

void Spi_SetOperation(enum SpiSampEdgeConfig const keSpiInterfaceSampEdgeConfig) 
{    
    switch ( keSpiInterfaceSampEdgeConfig )
    {
    case eZERO:
        pSpiRegs->spiControl &= (uint16) eSAMPLE_LEADING_EDGE_DEFAULT;
        pSpiRegs->spiControl &= (uint16) eCLOCK_LOW_DEFAULT;
        break;

    case eONE:
        pSpiRegs->spiControl |= (uint16) eSAMPLE_TRAILING_EDGE;
        pSpiRegs->spiControl &= (uint16) eCLOCK_LOW_DEFAULT;
        break;

    case eTWO:
        pSpiRegs->spiControl &= (uint16) eSAMPLE_LEADING_EDGE_DEFAULT;
        pSpiRegs->spiControl |= (uint16) eCLOCK_HIGH;
        break;

    case eTHREE:
        pSpiRegs->spiControl |= (uint16) eSAMPLE_TRAILING_EDGE;
        pSpiRegs->spiControl |= (uint16) eCLOCK_HIGH;
        break;

    default: // No update
        break;
    }
}

void Spi_SetClockPhase(enum SpiClockPhase const keSpiClockPhase) 
{    
     switch(keSpiClockPhase)
    {
        case eSAMPLE_LEADING_EDGE_DEFAULT:
            pSpiRegs->spiControl &= (uint16) eSAMPLE_LEADING_EDGE_DEFAULT;
        break;
        
        case eSAMPLE_TRAILING_EDGE:
            pSpiRegs->spiControl &= (uint16) eSAMPLE_LEADING_EDGE_DEFAULT;
            pSpiRegs->spiControl |= (uint16) eSAMPLE_TRAILING_EDGE;
        break;
                
        default: // No update
            break;
    }
}

boolean Spi_GetClockPhase(void)
{   
   boolean bClockPhase = False;
    
   uint32 const u32SpiClockPhase = pSpiRegs->spiControl; 

   if ((u32SpiClockPhase & (uint16) eSAMPLE_TRAILING_EDGE) != 0)
   {
       bClockPhase = True;
   }
   return bClockPhase;
}

void Spi_SetClockPolarity(enum SpiClockPolarity const keSpiClockPolarity) 
{    
     switch(keSpiClockPolarity)
    {
        case eCLOCK_LOW_DEFAULT:
            pSpiRegs->spiControl &= (uint16) eCLOCK_LOW_DEFAULT;
        break;
        
        case eCLOCK_HIGH:
            pSpiRegs->spiControl &= (uint16) eCLOCK_LOW_DEFAULT;
            pSpiRegs->spiControl |= (uint16) eCLOCK_HIGH;
        break;
                               
        default: // No update          
            break;
    }
}

boolean Spi_GetClockPolarity(void)
{
    boolean bClockPolarity = False;    
   
    uint32 const u32SpiClockPolarity = pSpiRegs->spiControl; 

    if ((u32SpiClockPolarity & (uint16) eCLOCK_HIGH) != 0)
    {
        bClockPolarity = True;
    }
    return bClockPolarity;
}

void Spi_SetIrqEnable(enum SpiIrqIndex const keSpiIrqIndex) 
{    
     switch(keSpiIrqIndex)
    {
        case eSPI_RX_RECEIVED:
            pSpiRegs->spiControl |= (uint16) eENABLE_SPI_RX_RECEIVED;
        break;
                               
        case eSPI_TX_COMPLETE:
            pSpiRegs->spiControl |= (uint16) eENABLE_SPI_TX_COMPLETE;
        break;
        
        case eSPI_TX_AE:
            pSpiRegs->spiTxAeThreshold |= (uint16) eSPI_ENABLE_SPI_TX_AE;
        break;
                
        case eSPI_RX_AF:
            pSpiRegs->spiRxAfThreshold |= (uint16) eSPI_ENABLE_SPI_RX_AF;
        break;
                
        default: // No update          
        break;
    }
}

void Spi_SetIrqDisable(enum SpiIrqIndex const keSpiIrqIndex) 
{    
     switch(keSpiIrqIndex)
    {
        case eSPI_RX_RECEIVED:
            pSpiRegs->spiControl &= (uint16) eDISABLE_SPI_RX_RECEIVED;
        break;
                               
        case eSPI_TX_COMPLETE:
            pSpiRegs->spiControl &= (uint16) eDISABLE_SPI_TX_COMPLETE;
        break;
        
        case eSPI_TX_AE:
            pSpiRegs->spiTxAeThreshold &= (uint16) eSPI_DISABLE_SPI_TX_AE;
        break;
                
        case eSPI_RX_AF:
            pSpiRegs->spiRxAfThreshold &= (uint16) eSPI_DISABLE_SPI_RX_AF;
        break;
                
        default: // No update         
        break;
    }
}

boolean Spi_IsIrqEnabled(enum SpiIrqIndex const keSpiIrqIndex )
{
    boolean bRetVal = False;
    uint32 u32SpiIrq = pSpiRegs->spiControl; 

    uint16 u16MaskBit;
    
    switch(keSpiIrqIndex)
    {     
        case eSPI_RX_RECEIVED:
            u16MaskBit =  (uint16) eENABLE_SPI_RX_RECEIVED;       
        break;
                               
        case eSPI_TX_COMPLETE:
            u16MaskBit =  (uint16) eENABLE_SPI_TX_COMPLETE;       
        break;
        
        case eSPI_TX_AE:
            u16MaskBit =  (uint16) eSPI_ENABLE_SPI_TX_AE;       
            u32SpiIrq = pSpiRegs-> spiTxAeThreshold;
        break;
                
        case eSPI_RX_AF:
            u16MaskBit =  (uint16) eSPI_ENABLE_SPI_RX_AF;    
            u32SpiIrq = pSpiRegs->spiRxAfThreshold;       
        break;
                
        default: // No update          
        break;
    }
    if ((u32SpiIrq &u16MaskBit)!=0)
    {
        bRetVal = True;
    } 
    return bRetVal;
}

void Spi_Clear(void)
{
    pSpiRegs->spiControl |= (uint16) eSPI_RESET;   
}

boolean Spi_IsClear(void)
{
    boolean bIsClear = False;
    
    uint32 const u32SpiReset = pSpiRegs->spiControl; 
	
    if ((u32SpiReset & (uint16) eSPI_RESET) != 0)
    {
        bIsClear = True;
    }
    return bIsClear;
}

void Spi_SetBitsPerWord( enum BitsPerWord const keBitsPerWord )
{   
    uint32 const u16SpiBitsPerWord = pSpiRegs->spiControl;

    pSpiRegs->spiControl &= (uint16) eWORD_SIZE_BY_WS_MASK;

    pSpiRegs->spiControl |= (uint16) keBitsPerWord;
}

uint32 Spi_GetBitsPerWord(void)
{      
    uint32 const u32SpiBitsPerWord = pSpiRegs->spiControl;
    
    uint32 const u32SpiBitsPerWordExtract = ((u32SpiBitsPerWord >> 10) &(uint16) eWORD_SIZE_BY_WS_MASK);
        
    return u32SpiBitsPerWordExtract;
}

void Spi_SetClockGeneration( enum SpiBaudRateIndex const keBaudRate)
{   
    pSpiRegs->spiCyclePerBit = 0;
    
    uint32 const uFrequency  =  SYSTEM_CLOCK;

    uint32 const uSpeed =  (uFrequency  / (uint32) keBaudRate);

    pSpiRegs->spiCyclePerBit |=  uSpeed;
}


uint32 Spi_GetClockGeneration(void)
{
    uint32 const ku32SpiBaudRate = pSpiRegs->spiCyclePerBit; 
      
    return ku32SpiBaudRate;
}

void Spi_SetTxAeThreshold( uint16 u16Count)
{   
    pSpiRegs->spiTxAeThreshold &= (uint16) eSPI_TX_THRESHOLD;      

    pSpiRegs->spiTxAeThreshold |= u16Count;      
}

uint32 Spi_GetTxAeThreshold(void)
{
    uint32 const ku32SpiTxAeInterrupt = pSpiRegs->spiTxAeThreshold; 
      
    return ku32SpiTxAeInterrupt;
}

void Spi_SetRxAfThreshold( uint16 u16Count)
{   
    pSpiRegs->spiRxAfThreshold &= (uint16) eSPI_RX_THRESHOLD;    

    pSpiRegs->spiRxAfThreshold |= u16Count;  
}

uint32 Spi_GetRxAfThreshold(void)
{
    uint32 const ku32SpiRxAfThreshold = pSpiRegs->spiRxAfThreshold; 
        
    return ku32SpiRxAfThreshold;
}


uint32 Spi_GetTxCount(void)
{
    uint32 const ku32SpiTxCount = pSpiRegs->spiTxCount; 
        
    return ku32SpiTxCount;
}

uint32 Spi_GetRxCount(void)
{
    uint32 const ku32SpiRxCount = pSpiRegs->spiRxCount; 
        
    return ku32SpiRxCount;
}


void Spi_SetAutoTrxControl( enum SpiAutoTxRx const keSpiAutoTxRx)
{   
    pSpiRegs->spiAutoTxRxControl &= (uint16) eDISABLED_DEFAULT;
    
    pSpiRegs->spiAutoTxRxControl |= (uint16) keSpiAutoTxRx;   
}

uint32 Spi_GetAutoTrxControl(void)
{
    return pSpiRegs->spiAutoTxRxControl;      
}

void Spi_SetAutoTrxCount( uint16 u16Count)
{   
    pSpiRegs->spiAutoTxRxCount &= (uint16) 0x0000;
    pSpiRegs->spiAutoTxRxCount |= u16Count;   
}

uint32 Spi_GetAutoTrxCount(void)
{
    return pSpiRegs->spiAutoTxRxCount; 
}

void Spi_SetCsSense( enum SpiCsSense const keSpiCsSense)
{   
    pSpiRegs->spiCsControl &= (uint16) eCS_SENSE_MASK;

    pSpiRegs->spiCsControl |= (uint16) keSpiCsSense;   
}

uint32 Spi_GetCsSense(void)
{
    uint32 const u32SpiCsSense = ((pSpiRegs->spiCsControl ) & (uint16) eACTIVE_HIGH);
      
    return u32SpiCsSense;
}

void Spi_SetCsMode( enum SpiCsMode const keSpiCsMode)
{   
    pSpiRegs->spiCsControl &= (uint16) eCS_MODE_MASK;

    pSpiRegs->spiCsControl |= (uint16) keSpiCsMode;   
}

uint32 Spi_GetCsMode(void)
{
    return pSpiRegs->spiCsControl & (uint16) eMANUAL;
}

void Spi_SetCsAssert( enum SpiCsAssert const keSpiCsAssert)
{   
    pSpiRegs->spiCsAssert &= (uint16) eCS_ASSERT_MASK;

    pSpiRegs->spiCsAssert |= (uint16) keSpiCsAssert;   
}

uint32 Spi_GetCsAssert(void)
{    
    uint32 const u32SpiCsAssert = ((pSpiRegs->spiCsAssert ) & (uint16) eASSERT_DEFAULT);
      
    return u32SpiCsAssert;
}

void Spi_SetCsTiming( enum SpiCsTiming const keSpiCsTiming, uint16 u16NumBitPeriods)
{   
    switch ( keSpiCsTiming )
        {
        case eSETUP:
            pSpiRegs->spiCsAssertBits &= (uint16) 0x0000; 
            pSpiRegs->spiCsAssertBits |= (uint16) u16NumBitPeriods; 
            break;

        case eHOLD:
            pSpiRegs->spiCsDeassertBits &= (uint16) 0x0000; 
            pSpiRegs->spiCsDeassertBits |= (uint16) u16NumBitPeriods; 
            break;

        case eIDLE:
            pSpiRegs->spiCsIdleBits &= (uint16) 0x0000; 
            pSpiRegs->spiCsIdleBits |= (uint16) u16NumBitPeriods; 
            break;

        default: // No update       
            break;
        }    
}

uint32 Spi_GetCsTiming(enum SpiCsTiming const keSpiCsTiming)
{
    uint32 ku32CsTiming;
    switch ( keSpiCsTiming )
        {
        case eSETUP:
            ku32CsTiming = pSpiRegs->spiCsAssertBits; 
            break;

        case eHOLD:
            ku32CsTiming = pSpiRegs->spiCsDeassertBits; 
            break;

        case eIDLE:
            ku32CsTiming = pSpiRegs->spiCsIdleBits; 
            break;

        default: // No update       
            break;
        }          
    return ku32CsTiming;
}

boolean Spi_GetStatusRegFlag(enum SpiStatus const keStatus )
{
    boolean bRetVal = False;
    uint32 const ku32SpiStatus = pSpiRegs->spiStatus; 

    uint16 const ku16MaskBit = (uint16) 1<<((uint16) keStatus);
    if ((ku32SpiStatus &ku16MaskBit)!=0)
    {
        bRetVal = True;
    } 
    return bRetVal;
}

void Spi_ClearStatusRegFlag(enum SpiStatus const keStatus )
{
    switch(keStatus)
    {
        case eSPI_TXOVERFLOW:
            pSpiRegs->spiStatus |= (uint16) 1 << (uint16) eSPI_TXOVERFLOW;
        break;                           
                                         
        case eSPI_RXOVERFLOW:                
            pSpiRegs->spiStatus |= (uint16) 1 << (uint16) eSPI_RXOVERFLOW;
        break;                           
                                         
        case eSPI_TXUNDERRUN:             
            pSpiRegs->spiStatus |= (uint16) 1 << (uint16) eSPI_TXUNDERRUN;       
        break;                           
        
        default: // No update
            break;
    }
}

void Spi_Write( const uint16 ku16Data)
{      
    volatile uint32* const pu32SpiWrite = &pSpiRegs->spiTx;
    *pu32SpiWrite = ku16Data;                                                                     
}

uint32 Spi_Read(void)
{
    uint32 const ku32SpiRead = pSpiRegs->spiRx; 
      
    return ku32SpiRead;
}

// Set the SPI Irq Handler to required function
void Spi_SetIrqHandler( void (SpiIrqHandler)( void ) )
{
    Sci_SetPortIrqHandler(m_u8SpiPortNum, SpiIrqHandler);
}