#ifndef HEADER_DMA_H
#define HEADER_DMA_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Dma.h
 * @author       N. Moheeputh
 * @version      1.0
 *
 * @brief        The implementation of the DMA interface.
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
 */
/* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
#include "Std_Types.h"

// DMA Definitions
// ---------------

// DmaRegisters
#define DMA0_BASE_ADDRESS     0x40070000U
#define DMA1_BASE_ADDRESS     0x40070080U
#define DMA2_BASE_ADDRESS     0x40070100U
#define DMA_STATUS            0x40U
#define DMA_CONTROL           0x44U
#define DMA_CURRENT           0x48U


// DmaStatus
#define DMA_STATUS_DC      ( 0b0000000000000010U )
#define DMA_STATUS_DCO     ( 0b0000000000000100U )
#define DMA_STATUS_ER      ( 0b0000000000001000U )

// DmaControl
#define DMA_CONTROL_E      ( 0b0000000000000001U )
#define DMA_CONTROL_DCIE   ( 0b0000000000000100U )
#define DMA_CONTROL_ERIE   ( 0b0000000000001000U )
#define DMA_CONTROL_SP     ( 0b0000000000010000U )

// End of DMA Definitions
// ----------------------

/**
 * @brief Enumerates the Dma channels
 */
enum DmaChannelId
{
    eDMA_CH_0 = 0x00, 
    eDMA_CH_1 = 0x01,
    eDMA_CH_2 = 0x02
};

/**
 * @internal
 * @brief Enumerate the index for the control switch statement.
 * eE    - Enables DMA.
 * eDCIE - Enable descriptor complete interrupt.
 * eERIE - Error interrupt enable.
 * eSP   - Stop transfer immediately. Data may be lost.
 */
enum ControlIndex 
{
    eE       = (0x01) << 0U,
    eDCIE    = (0x01) << 2U,
    eERIE    = (0x01) << 3U,
    eSP      = (0x01) << 4U
};

/**
 * @internal
 * @brief Enumerates the IRQ types
 * eIRQ_DCIE - Descriptor Complete.
 * eIRQ_ERIE - Error occured during a read/write process, or indicates an unsupported value.
 */
enum Irq 
{
    eIRQ_DCIE       = 2U,
    eIRQ_ERIE       = 3U
};

/**
 * @internal
 * @brief Enumerate the index for the status switch statement
 * eDC - Check descriptor completion process
 * eDCO - Check descriptor completion overflow. This indicates the DC flag was already set when a
 * descriptor was completed.
 * eER  - Indicated an error occured during a read/write process, or indicates an unsupported value.
 */
enum DmaStatusIndex 
{
    eSTATUS_DC         = 1U,
    eSTATUS_DCO        = 2U,
    eSTATUS_ER         = 3U
};

/**
 * @brief Enumerates the descriptor's type.
 */
enum DescriptorType 
{
    eFULL                 = 0x00U,
    eSMALL                = 0x01U,
    eTINY                 = 0x02U
};

 /**
 * @brief Enumerates the Full descriptor' source address type.
 */
enum  TypeIndex 
{
    eMEMORY       =       0x00U,
    ePERIPHERAL   =       0x01U,
    eNONE         =       0x02U,
};

/**
 * @brief Enumerates the Full descriptor's length burst.
 */
enum  FullBurstIndex 
{
    eINCR     =       0x00U,
    eSINGLE   =       0x01U,
    eINCR4    =       0x02U,
    eINCR8    =       0x03U,
    eINCR16   =       0x04U,
};

/**
 * @brief Enumerates the descriptor' size of each access.
 */
enum  SizeIndex 
{
    eONEBYTE    =       0x00U,
    eTWOBYTES   =       0x01U,
    eFOURBYTES  =       0x02U,
};

/**
 * @brief Enumerates the Tiny/Small's length burst.
 */
enum  DescriptorBurstIndex 
{
    eBEAT1   =       0x00U,
    eBEAT4   =       0x01U,
    eBEAT8   =       0x02U,
    eBEAT16  =       0x03U
};

/**
 * @brief Enumerates the different DMA peripheral sources.
 */
enum  PeripheralIndex 
{
    eSCI0     =       0x00U,
    eSCI1     =       0x01U,
    eCAU      =       0x02U,
    eDAU      =       0x03U,
    eLIN      =       0x04U,
};

/**
 * @brief Enumerates the Tiny/Small descriptor's transfer mode.
 */
enum  DescriptorMode 
{
    eMEMORY_TO_MEMORY     = 0x00U,
    eMEMORY_TO_PERIPHERAL = 0x01U,
    ePERIPHERAL_TO_MEMORY = 0x02U,
    eMEMORY_TO_FIFO       = 0x03U,
    eFIFO_TO_MEMORY       = 0x04U,
    eNONE_TO_MEMORY       = 0x05U,
    eMEMORY_TO_NONE       = 0x06U,
};

/**
 * @brief Enumerates the descriptor interrupt.
 */
enum DescriptorInterrupt 
{
    eDMA_DESCRIPTOR_DISABLE    =       0x00U,
    eDMA_DESCRIPTOR_ENABLE     =       0x01U
};

/**
 * @brief Define the Tiny descriptor structure with its variables.
 */
struct DmaTinyDescriptor
{
    /**used to set the descriptor control register.*/
    uint32 DescriptorControl;
    /**used to set the source address.*/
    uint32 SrcAddress;
    /**used to set the destination address.*/
    uint32 DstAddress;
    /**used to set the number of bytes to transmit.*/
    uint32 Count;
};

/**
 * @brief Define the Small descriptor structure with its variables.
 */
struct DmaSmallDescriptor
{
    /**used to set the descriptor control register.*/
    uint32 DescriptorControl;
    /**used to set the descriptor to be transmitted.*/
    uint32 NextAddress;
    /**used to set the source address.*/
    uint32 SrcAddress;
    /**used to set the destination address.*/
    uint32 DstAddress;
    /**used to set the number of bytes to transmit.*/
    uint32 Count;
};

/**
 * @brief Define the Full descriptor structure with its variables.
 */
struct DmaFullDescriptor
{
    /**used to set the descriptor control register.*/
    uint32 DescriptorControl;
    /**used to set the descriptor to be transmitted.*/
    uint32 NextAddress;
    /**used to set the source address.*/
    uint32 SrcAddress;
    /**used to set the destination address.*/
    uint32 DstAddress;
    /**used to set the source control register.*/
    uint32 SrcControl;
    /**used to set the destination control register.*/
    uint32 DstControl;
    /**contains the number of transfer that should occur in the X dimention.*/
    uint32 CountX;
    /**contains the number of iteration, minus 1, of the transfers in the X direction.*/
    uint32 CountY;
    /**contains a signed integer that is added to the src_addressregister after each transfer in the X dimension.*/
    uint32 SrcIncX;
    /**contains a signed integer that is added to the src_addressregister after each transfer in the Y dimension.*/
    uint32 SrcIncY;
    /**contains a signed integer that is added to the dst_addressregister after each transfer in the X dimension.*/
    uint32 DstIncX;
    /**contains a signed integer that is added to the dst_addressregister after each transfer in the Y dimension.*/
    uint32 DstIncY;
};

/**
 * @brief Define the Tiny/Small descriptor control register structure with its variables.
 */
struct TinySmallDescControlStructure
{
    /**used to set the descriptor type.*/
    uint32 DescriptorIndex;
    /**used to set the descriptor's interrupt.*/
    uint32 DescriptorInterrupt;
    /**used to set mode of operation.*/
    uint32 ModeIndex;
    /**used to set the size of trasfer.*/
    uint32 SizeIndex;
    /**used to set the maximum burst length.*/
    uint32 DescriptorBurstIndex;
    /**used to set source of flow control.*/
    uint32 PeripheralIndex;
};

/**
 * @brief Define the Full descriptor control register structure with its variables.
 */
struct FullDescControlStructure
{
    /**used to set the descriptor type.*/
    uint32 DescriptorIndex;
    /**used to set the descriptor's interrupt.*/
    uint32 DescriptorInterrupt;
    /**used to set mode of operation for the source control register.*/
    uint32 ModeIndex;
    /**used to set the size of transfer for the source control register.*/
    uint32 SizeIndex;
    /**used to set the maximum burst length for the source control register.*/
    uint32 DescriptorBurstIndex;
    /**used to set source of flow control for the source control register.*/
    uint32 PeripheralIndex;
};

/**
 * @internal
 * @brief Define register and base addresses
 */
typedef struct
{
    uint32 Reserved0[0x10];      //  0x00 - 0x3C (Not used)
    uint32 Status;               //  0x40  :(R1C)
    uint32 Control;              //  0x44  :(R/W)
    uint32 Current;              //  0x48  :(R/W)       
} DmaRegs_t;

#define  stDmaRegs (*( volatile DmaRegs_t *)DMA0_BASE_ADDRESS)



/**
 * @brief Set the DMA Address
 * @param[in] eDmaId An enumerated Dma channel
 */
void Dma_SetChannel(enum DmaChannelId eDmaChannelId);

/**
 * @brief Enable the DMA.
 * @details
 */
void Dma_Enable(void);

/**
 * @brief Disable the DMA.
 * @details Disable & Clear the DMA: clears the internal DMA logic and resets the Result and Status registers.
 */
void Dma_Disable(void);

/**
 * @brief Check if the DMA is enabled.
 * @returns  boolean - the enable bit (E) of the current DMA instance. True if it is enabled else false.
 */
boolean Dma_IsEnabled(void);

/**
 * @brief Stop the DMA.
 * @details Sets the SP high to stop the DMA transfer.
 */
void Dma_Stop(void);

/**
 * @brief Continue the DMA operation.
 * @details Sets the SP low to continue the DMA transfer.
 */
void Dma_Continue(void);

/**
 * @brief Checks SP of the DMA.
 * @details Checks the SP bit of the current DMA instance.
 * @returns Returns true if the SP is not set as if it is set, it will mean the DMA has stopped.
 */
boolean Dma_GetSp(void);

/**
 * @brief Clear the DMA status register.
 * @param[in] eStatusIndex status register mode
 */
void Dma_SetClearStatus( enum DmaStatusIndex const eStatusIndex );

/**
 * @brief Checks the DMA status register.
 * @param[in] keStatus status register mode
 * @returns the DMA status register.
 */
boolean Dma_IsStatusSet( enum DmaStatusIndex const keStatus );

/**
 * @brief Sets the address of the Descriptor control register of the Tiny/Small descriptors.
 * @param[in] keDescControlStructure Tiny/Small Descriptor Control Structure constant pointer.
 * @param[in] keDescriptor Descriptor mode.
 * @returns the address of the Descriptor register of the Tiny/Small descriptor.
 */
uint32 Dma_SetTinySmallDescControl( enum DescriptorType keDescriptor, struct TinySmallDescControlStructure const* keDescControlStructure );

/**
 * @brief Sets the address of the Descriptor control register of the Full descriptor.
 * @param[in] keFullDescControlStructure Full Descriptor Control Structure constant pointer.
 * @returns the address of the Descriptor register of the Full descriptor.
 */
uint32 Dma_SetFullDescControl(  struct FullDescControlStructure  const* keFullDescControlStructure ); // changed from &keFullDescControlStructure

/**
 * @brief Sets the address of the Source control register of the Full descriptor.
 * @param[in] keFullDescControlStructure Full Descriptor Control Structure constant pointer.
 * @returns the address of the Source register of the Full descriptor.
 */
uint32 Dma_SetFullSrcControl( struct FullDescControlStructure const* keFullDescSourceStructure );//changed from &keFullDescSourceStructure

/**
 * @brief Sets the address of the destination control register of the Full descriptor.
 * @param[in] keFullDescControlStructure Full Descriptor Control Structure constant pointer.
 * @returns the address of the Source register of the Full descriptor.
 */
uint32 Dma_SetFullDstControl( struct FullDescControlStructure const* keFullDescDestinationStructure );//changed from &keFullDescDestinationStructure


/**
 * @brief Gets the address of the current descriptor being processed.
 * @returns the current descriptor's address.
 */
uint32 Dma_GetDmaCurrent(void);

/**
 * @brief Sets the address of the current descriptor being processed.
 * @param[in] ku32CurrentDescriptor current descriptor.
 * @returns the address of the descriptor being processed.
 */
void Dma_SetDmaCurrent( const uint32 ku32CurrentDescriptor );

/**
 * @brief Set the Dma IRQ to be enabled or disabled
 * @remarks The data overrun (DOR) interrupt is not maskable: enabling or disabling it has no effect.
 * @param[in] keIrq The IRQ to set
 */
void Dma_SetIrqEnable( enum  Irq const keIrq);

/**
 * @brief Set the Dma IRQ to be disabled
 * @remarks The data overrun (DOR) interrupt is not maskable: enabling or disabling it has no effect.
 * @param[in] keIrq The IRQ to set
 */
void Dma_SetIrqDisable( enum  Irq const keIrq);

/**
 * @brief Read if Dma IRQ is enabled or disabled
 * @param[in] keIrq The IRQ to read
 * @returns true if enabled, false if not
 */
boolean Dma_IsIrqEnabled(  enum Irq const keIrq );

/**
 * @brief Set a handler for the IRQ
 * @param[in] SpiIrqHandler A reference to an IRQ handler method
 */
void Dma_SetIrqHandler(void (DmaIrqHandler)( void ));

/**
 * @brief The IRQ handler for DMA 0
 */
void Dma_IrqHandler( void );

#endif /* HEADER_DMA_H */