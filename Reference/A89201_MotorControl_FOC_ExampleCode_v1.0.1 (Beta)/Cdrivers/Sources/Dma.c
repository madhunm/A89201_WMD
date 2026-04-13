/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/*
 * @file         Dma.c
 * @author       N. Moheeputh
 * @version      1.0
 *
 * @brief        Direct Memory Access interface: DMA 
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
#include "ARMCM4_FP.h"

#include "Std_Types.h"
#include "Dma.h"

static DmaRegs_t* pDmaRegs = (DmaRegs_t*)&stDmaRegs;                                    

void Dma_SetChannel(enum DmaChannelId eDmaChannelId)
{
    switch(eDmaChannelId)
    {
        case eDMA_CH_0:
            pDmaRegs = (DmaRegs_t*)DMA0_BASE_ADDRESS;
            break;
        
        case eDMA_CH_1:
            pDmaRegs = (DmaRegs_t*)DMA1_BASE_ADDRESS;
            break;
                
        case eDMA_CH_2:
            pDmaRegs = (DmaRegs_t*)DMA2_BASE_ADDRESS;
            break;
        
        default:
            pDmaRegs = (DmaRegs_t*)DMA0_BASE_ADDRESS;
            break;
    }
}

/**
 * @interna
 * @details Enable the DMA.
 */
void Dma_Enable(void) 
{
    pDmaRegs->Control |= (uint16) eE;  
}

/**
 * @internal
 * @details Disable & Clear the DMA: clears the internal
 * DMA logic and resets the Result and Status registers.
 */
void Dma_Disable(void) 
{
    pDmaRegs->Control &=~ (uint16) eE; 
}

/**
 * @internal
 * @details The Enable control bit (E) is in bit 0.
 * Returns true if the E is set else false
 */
boolean Dma_IsEnabled(void)  
{
    boolean bIsEnabled = False;
    
    uint32 const u32DmaEnable = pDmaRegs->Control; 
	
    if ((u32DmaEnable & (uint16) eE) != 0U)
    {
        bIsEnabled = True;
    }
    return bIsEnabled;    
}

/**
 * @internal
 * @details The SP is in bit 4 of the control register.
 * Sets the SP high to stop the DMA transfer.
 */
void Dma_Stop(void) 
{
    pDmaRegs->Control |= (uint16) eSP;
}

/**
 * @internal
 * @details The SP is in bit 4 of the control register.
 * Sets the SP low to continue the DMA transfer.
 */
void Dma_Continue(void)
{
    pDmaRegs->Control &=~ (uint16) eSP; 
}

/**
 * @internal
 * @details The SP is in bit 4 of the control register.
 * Returns true if the SP is not set as if it is set, it will mean the DMA has stopped.
 */
boolean Dma_GetSp(void) 
{
    boolean bStop = False;
    
    uint32 const u32DmaStop = pDmaRegs->Control; 
	
    if ((u32DmaStop & (uint16) eSP) != 0U)
    {
        bStop = True;
    }
    
    return bStop;    
}

/**
 * @internal
 * @details Clear the DMA status register.
 */
void Dma_SetClearStatus(  enum DmaStatusIndex const eStatusIndex ) 
{
    switch(eStatusIndex)
    {
        case eSTATUS_DC:
            pDmaRegs->Status |= (uint16) 1U << (uint16) eSTATUS_DC;
        break;                           
                                         
        case eSTATUS_DCO:                
            pDmaRegs->Status |= (uint16) 1U << (uint16) eSTATUS_DCO;
        break;                           
                                         
        case eSTATUS_ER:             
            pDmaRegs->Status |= (uint16) 1U << (uint16) eSTATUS_ER;       
        break;                           
        
        default: // No update
            break;
    }
}

/**
 * @internal
 * @details Checks if an IRQ is enabled.
 * This implementation performs uninitialised error checking as it is not needed
 * for time critical code.
 */
boolean Dma_IsStatusSet(  enum DmaStatusIndex const keStatus ) 
{
    boolean bRetVal = False;
    uint32 const ku32DmaStatus = pDmaRegs->Status; 

    uint16 const ku16MaskBit = (uint16) 1U <<((uint16) keStatus);
    if ((ku32DmaStatus &ku16MaskBit)!= 0U)
    {
        bRetVal = True;
    } 
    
    return bRetVal;
}

/**
 * @internal
 *
 * @details Set the tiny/small descriptor control register. 
 */
uint32 Dma_SetTinySmallDescControl(  enum DescriptorType keDescriptor, struct TinySmallDescControlStructure const* keDescControlStructure )  // polyspace AUTOSAR-CPP14:M9-3-3 [Justified:Unset] " Function cannot be made static because it has a member object"
{
    switch ( keDescriptor )
    {
    case eTINY:
        break;

    case eSMALL:
        break;

    default: // No update
         break;
    }
    if ( keDescControlStructure->DescriptorIndex == ((uint32) eTINY ) )
    {

        uint32 const TinyControl = ( ( keDescControlStructure->DescriptorIndex << ( 0U ) ) |
                                     ( keDescControlStructure->DescriptorInterrupt << ( 2U ) ) |
                                     ( keDescControlStructure->ModeIndex << ( 3U ) ) |
                                     ( keDescControlStructure->SizeIndex << ( 6U ) ) |
                                     ( keDescControlStructure->DescriptorBurstIndex << ( 8U ) ) |
                                     ( keDescControlStructure->PeripheralIndex << ( 10U ) ) );
        return TinyControl;
    }
    else if ( keDescControlStructure->DescriptorIndex == ((uint32) eSMALL ) )
    {
        uint32 const SmallControl = ( ( keDescControlStructure->DescriptorIndex << ( 0U ) ) |
                                      ( keDescControlStructure->DescriptorInterrupt << ( 2U ) ) |
                                      ( keDescControlStructure->ModeIndex << ( 3U ) ) |
                                      ( keDescControlStructure->SizeIndex << ( 6U ) ) |
                                      ( keDescControlStructure->DescriptorBurstIndex << ( 8U ) ) |
                                      ( keDescControlStructure->PeripheralIndex << ( 10U ) ) );
        return SmallControl;
    }
    else
    {
        return 0;
    }
}

/**
 * @internal
 *
 * @details Sets the full descriptor control register. 
 */
uint32 Dma_SetFullDescControl(  struct FullDescControlStructure const* keFullDescControlStructure ) 
{
    if ( keFullDescControlStructure->DescriptorIndex == ((uint32) eFULL ) )
    {
        uint32 const FullControl  = ( ( keFullDescControlStructure->DescriptorIndex << ( 0U ) ) |
                                (       keFullDescControlStructure->DescriptorInterrupt << ( 2U ) ) );
        return FullControl;
    }
    else
    {
      return 0;
    }
}

/**
 * @internal
 *
 * @details Sets the full descriptor source control register. 
 */
uint32 Dma_SetFullSrcControl(  struct FullDescControlStructure const* keFullDescSourceStructure )  // polyspace AUTOSAR-CPP14:M9-3-3 [Justified:Unset] " Function cannot be made static because it has a member object"
{
    if ( keFullDescSourceStructure->DescriptorIndex == ((uint32) eFULL ) )
    {
        uint32 const FullControl =( ( keFullDescSourceStructure->ModeIndex << ( 0U ) ) |
                                    ( keFullDescSourceStructure->SizeIndex << ( 2U ) ) |
                                    ( keFullDescSourceStructure->DescriptorBurstIndex << ( 4U ) ) |
                                    ( keFullDescSourceStructure->PeripheralIndex << ( 10U ) ) );
        return FullControl;
    }
    else
    {
      return 0;
    }
}

/**
 * @internal
 *
 * @details Sets the full descriptor destination control register. 
 */
uint32 Dma_SetFullDstControl( struct FullDescControlStructure const* keFullDescDestinationStructure ) 
{
    if ( keFullDescDestinationStructure->DescriptorIndex == ((uint32) eFULL ) )
    {
        uint32 const FullControl = ( ( keFullDescDestinationStructure->ModeIndex << ( 0U ) ) |
                                     ( keFullDescDestinationStructure->SizeIndex << ( 2U ) ) |
                                     ( keFullDescDestinationStructure->DescriptorBurstIndex << ( 4U ) ) |
                                     ( keFullDescDestinationStructure->PeripheralIndex << ( 10U ) ) );
        return FullControl;
    }
    else
    {
      return 0;
    }
}

/**
 * @internal
 *
 * @details Gets the address of the current descriptor being processed.
 */
uint32 Dma_GetDmaCurrent(void)  
{
    return pDmaRegs->Current;
}

/**
 * @internal
 *
 * @details Set the current register of the DMA by writing the address of the descriptor to be used. 
 */
void Dma_SetDmaCurrent( const uint32 ku32CurrentDescriptor ) 
{
    pDmaRegs->Current |= ku32CurrentDescriptor;  
}

/**
 * @internal
 * @details Enables the DMA  interrupts: eDCIE - ERIE.
 */
void Dma_SetIrqEnable(  enum Irq const keIrq ) 
{
    switch ( keIrq )
    {
    case eIRQ_DCIE:
        pDmaRegs->Control |= (uint16) eDCIE;
        break;

    case eIRQ_ERIE:
        pDmaRegs->Control |= (uint16) eERIE;
        break;

    default: // No update 
        break;
    }
}

/**
 * @internal
 * @details Disables the DMA  interrupts: eDCIE - ERIE.
 */
void Dma_SetIrqDisable(  enum Irq const keIrq ) 
{
    switch ( keIrq )
    {
    case eIRQ_DCIE:
        pDmaRegs->Control &=~ (uint16) eDCIE;
        break;

    case eIRQ_ERIE:
        pDmaRegs->Control &=~ (uint16) eERIE;
        break;

    default: // No update 
        break;
    }
}

/**
 * @internal
 * @details Checks if an IRQ is enabled.
 * This implementation performs uninitialised error checking as it is not needed
 * for time critical code.
 */
boolean Dma_IsIrqEnabled( enum Irq const keIrq ) 
{
    boolean bIsEnabled = False;
    uint32 const u32DmaIrq = pDmaRegs->Control; 

    uint16 const ku16MaskBit = (uint16) 1U <<((uint16) keIrq);
    if ((u32DmaIrq &ku16MaskBit)!= 0U)
    {
        bIsEnabled = True;
    } 
    return bIsEnabled;
}

// Declare a function pointer for use by DMA Irq
static void (*fpDmaIrqHandler)( void ) = NULLPTR;

void Dma_SetIrqHandler( void (DmaIrqHandler)( void ))
{
    fpDmaIrqHandler = *DmaIrqHandler;
}

/**
 * @internal
 * @details This handler is registered in the vector table.
 */
void Dma_IrqHandler(void)
{
    NVIC_DisableIRQ((IRQn_Type) eIRQNUM_DMA );
    NVIC_ClearPendingIRQ((IRQn_Type) eIRQNUM_DMA );

    if ( fpDmaIrqHandler != NULLPTR )
    {
        fpDmaIrqHandler();
    }
    NVIC_EnableIRQ((IRQn_Type)eIRQNUM_DMA );
}