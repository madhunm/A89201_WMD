#ifndef HEADER_SPI_H
#define HEADER_SPI_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Spi.h
 * @author       N. Moheeputh
 * @version      1.0
 *
 * @brief        The implementation of a SPI_MASTER interface.
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
 */
/* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
#include "Std_Types.h"

#define    SPI0_ADDRESS  (0x4000A000U)
#define    SPI1_ADDRESS  (0x4000B000U)

// SCI Definitions
// ---------------

// SciRegisters
#define SPI0_BASE_ADDRESS       0x4000A000U
#define SPI0_TX                 0x00U
#define SPI0_RX                 0x04U
#define SPI0_STATUS             0x08U
#define SPI0_CONTROL            0x0CU
#define SPI0_CYCLES_PER_BIT     0x10U
#define SPI0_TXAE_THRESHOLD     0x14U
#define SPI0_RXAF_THRESHOLD     0x18U
#define SPI0_TX_COUNT           0x1CU
#define SPI0_RX_COUNT           0x20U
#define SPI0_AUTOTXRX_CONTROL   0x24U
#define SPI0_AUTOTXRX_COUNT     0x28U
#define SPI0_CS_CONTROL         0x2CU
#define SPI0_CS_ASSERT          0x30U
#define SPI0_CS_ASSERT_BITS     0x34U
#define SPI0_CS_DEASSERT_BITS   0x38U
#define SPI0_CD_IDLE_BITS       0x3CU
#define SPI1_BASE_ADDRESS       0x4000B000U
#define SPI1_TX                 0x00U
#define SPI1_RX                 0x04U
#define SPI1_STATUS             0x08U
#define SPI1_CONTROL            0x0CU
#define SPI1_CYCLES_PER_BIT     0x10U
#define SPI1_TXAE_THRESHOLD     0x14U
#define SPI1_RXAF_THRESHOLD     0x18U
#define SPI1_TX_COUNT           0x1CU
#define SPI1_RX_COUNT           0x20U
#define SPI1_AUTOTXRX_CONTROL   0x24U
#define SPI1_AUTOTXRX_COUNT     0x28U
#define SPI1_CS_CONTROL         0x2CU
#define SPI1_CS_ASSERT          0x30U
#define SPI1_CS_ASSERT_BITS     0x34U
#define SPI1_CS_DEASSERT_BITS   0x38U
#define SPI1_CD_IDLE_BITS       0x3CU

// SciSpiStatus
#define SCI_SPISTATUS_TXE   ( 0b0000000000000001U )
#define SCI_SPISTATUS_TXF   ( 0b0000000000000010U )
#define SCI_SPISTATUS_TXO   ( 0b0000000000000100U )
#define SCI_SPISTATUS_TXU   ( 0b0000000000001000U )
#define SCI_SPISTATUS_RXE   ( 0b0000000000010000U )
#define SCI_SPISTATUS_RXF   ( 0b0000000000100000U )
#define SCI_SPISTATUS_RXO   ( 0b0000000001000000U )
#define SCI_SPISTATUS_BUSY  ( 0b0010000000000000U )
#define SCI_SPISTATUS_TXAE  ( 0b0100000000000000U )
#define SCI_SPISTATUS_RXAF  ( 0b1000000000000000U )

// SciSpiControl
#define SCI_SPICONTROL_SPI_ENABLE   ( 0b0000000000000001U )
#define SCI_SPICONTROL_SPI_MODE     ( 0b0000000000000010U )
#define SCI_SPICONTROL_WS           ( 0b0000000000000100U )
#define SCI_SPICONTROL_BO           ( 0b0000000000001000U )
#define SCI_SPICONTROL_CLKPHA       ( 0b0000000000010000U )
#define SCI_SPICONTROL_CLKP         ( 0b0000000000100000U )
#define SCI_SPICONTROL_TXIE         ( 0b0000000001000000U )
#define SCI_SPICONTROL_RXIE         ( 0b0000000010000000U )
#define SCI_SPICONTROL_RFIFO        ( 0b0000000100000000U )
    
#define SCI_SPICONTROL_BPWWS        ( 0b0000000000000000U )
#define SCI_SPICONTROL_BPW01        ( 0b0000010000000000U )
#define SCI_SPICONTROL_BPW02        ( 0b0000100000000000U )
#define SCI_SPICONTROL_BPW03        ( 0b0000110000000000U )
#define SCI_SPICONTROL_BPW04        ( 0b0001000000000000U )
#define SCI_SPICONTROL_BPW05        ( 0b0001010000000000U )
#define SCI_SPICONTROL_BPW06        ( 0b0001100000000000U )
#define SCI_SPICONTROL_BPW07        ( 0b0001110000000000U )
#define SCI_SPICONTROL_BPW08        ( 0b0010000000000000U )
#define SCI_SPICONTROL_BPW09        ( 0b0010010000000000U )
#define SCI_SPICONTROL_BPW10        ( 0b0010100000000000U )
#define SCI_SPICONTROL_BPW11        ( 0b0010110000000000U )
#define SCI_SPICONTROL_BPW12        ( 0b0011000000000000U )
#define SCI_SPICONTROL_BPW13        ( 0b0011010000000000U )
#define SCI_SPICONTROL_BPW14        ( 0b0011100000000000U )
#define SCI_SPICONTROL_BPW15        ( 0b0011110000000000U )
#define SCI_SPICONTROL_BPW16        ( 0b0100000000000000U )


// SciSpiTxAeThresholdInterruptEnable
#define SCI_SPI_TXAETHRESHOLD_IE  ( 0b1000000000000000U )


// SciSpiRxAfThresholdInterruptEnable
#define SCI_SPI_RXAFTHRESHOLD_IE  ( 0b0100000000000000U )

// SciSpiAutoTxRxControl
#define SCI_SPI_AUTOTXCONTROL_ATR_DISABLED              0
#define SCI_SPI_AUTOTXCONTROL_AUTOTRANSMIT_ENABLED      1
#define SCI_SPI_AUTOTXCONTROL_AUTOTRANSMIT_DISABLED     2
   

// SciSpiCsControl
#define SCI_SPI_CSCONTROL_CSSENSE   ( 0b0000000000000001U ) 
#define SCI_SPI_CSCONTROL_CSMODE    ( 0b0000000000000010U )

// SciSpiCsAssert
#define SCI_SPI_CSASSERT   ( 0b0000000000000001U ) 

// End of SCI Definitions
// ----------------------

enum SpiIrqIndex
{
    eSPI_RX_RECEIVED,
    eSPI_TX_COMPLETE,
    eSPI_TX_AE,
    eSPI_RX_AF
};

/**
 * @brief Enumerates the Spi channels
 */
enum SpiId
{
    eSPI_PORTID_0 = 0x00, 
    eSPI_PORTID_1 = 0x01
};

/**
 * @brief Enumerates the status types
 */
enum SpiStatus
{
    /** Receive FIFO almost full */
    eSPI_RXALMOSTFULL = 15U,
    /** Transmit FIFO almost empty */
    eSPI_TXALMOSTEMPTY = 14U,
    /** Spi bus is Busy */
    eSPI_BUSY = 13U,
    /** Receive FIFO overflow */
    eSPI_RXOVERFLOW = 6U,
    /** Receive FIFO full */
    eSPI_RXFULL = 5U,
    /** Receive FIFO empty */
    eSPI_RXEMPTY = 4U,
    /** Transmit FIFO underrun */
    eSPI_TXUNDERRUN = 3U,
    /** Transmit FIFO overflow */
    eSPI_TXOVERFLOW = 2U,
    /** Transmit FIFO full */
    eSPI_TXFULL = 1U,
    /** Transmit FIFO empty */
    eSPI_TXEMPTY = 0U,
    /** Invalid */
    eSPI_INVALID = 0xFF
};

/**
 * @brief Enumerates the enable/disable bit
 */
enum SpiEnable
{
    eSPI_DISABLE_DEFAULT = 0xFFFE, 
    eSPI_ENABLE  = (0x0001) << 0 
};

/**
 * @brief Enumerates the Mode Bits
 */
enum SpiMode
{
    eMASTER_DEFAULT  = 0xFFFD,
    eSLAVE = (0x0001) << 1
};

/**
 * @brief Enumerates the Word Bits
 */
enum SpiWordSize
{
    e8_BITS_DEFAULT  =   0xFFFB,
    e16_BITS  = ( 0x0001 ) <<2
};

/**
 * @brief Enumerates the Bit Ordering Bits
 */
enum SpiBitOrdering
{
    eMSB_DEFAULT  = 0xFFF7,
    eLSB = (0x0001) << 3, 
};

/**
 * @brief Enumerates the Clock Phase Bits
 */
enum SpiClockPhase
{
    eSAMPLE_LEADING_EDGE_DEFAULT  = 0xFFEF,
    eSAMPLE_TRAILING_EDGE = (0x0001) << 4, 
};

/**
 * @brief Enumerates the Clock Polarity Bits
 */
enum SpiClockPolarity
{
    eCLOCK_LOW_DEFAULT  = 0xFFDF,
    eCLOCK_HIGH = (0x0001) << 5, 
};

/**
 * @brief Enumerates the TX Complete interrupt bit
 */
enum SpiTxComplete
{
    eDISABLE_SPI_TX_COMPLETE = 0xFFBF,
    eENABLE_SPI_TX_COMPLETE  = (uint16)(0x01) << 6
};

/**
 * @brief Enumerates the RX Riceived interrupt bit
 */
enum SpiRxReceived
{
    eDISABLE_SPI_RX_RECEIVED = 0xFF7F,
    eENABLE_SPI_RX_RECEIVED  = (uint16)(0x01) << 7
};

/**
 * @brief Enumerates the Reset bit
 */
enum SpiReset
{
    eSPI_DO_NOT_RESET_DEFAULT  = 0xFEFF,
    eSPI_RESET = (uint16)(0x0001) << 8
};

/**
 * @brief Enumerates the Bit Per Word bits
 */
enum BitsPerWord
{
    eWORD_SIZE_BY_WS_MASK          =    0x03FFU,
    eWORD_SIZE_BY_WS_BIT_DEFAULT   =   (uint32)(0x0000U) << 10,
    e1_BIT_WORD_TRANSFER           =   (uint32)(0x0001U) << 10,
    e2_BIT_WORD_TRANSFER           =   (uint32)(0x0002U) << 10,
    e3_BIT_WORD_TRANSFER           =   (uint32)(0x0003U) << 10,
    e4_BIT_WORD_TRANSFER           =   (uint32)(0x0004U) << 10,
    e5_BIT_WORD_TRANSFER           =   (uint32)(0x0005U) << 10,
    e6_BIT_WORD_TRANSFER           =   (uint32)(0x0006U) << 10,
    e7_BIT_WORD_TRANSFER           =   (uint32)(0x0007U) << 10,
    e8_BIT_WORD_TRANSFER           =   (uint32)(0x0008U) << 10,
    e9_BIT_WORD_TRANSFER           =   (uint32)(0x0009U) << 10,
    e10_BIT_WORD_TRANSFER          =   (uint32)(0x000AU) << 10,
    e11_BIT_WORD_TRANSFER          =   (uint32)(0x000BU) << 10,
    e12_BIT_WORD_TRANSFER          =   (uint32)(0x000CU) << 10,
    e13_BIT_WORD_TRANSFER          =   (uint32)(0x000DU) << 10,
    e14_BIT_WORD_TRANSFER          =   (uint32)(0x000EU) << 10,
    e15_BIT_WORD_TRANSFER          =   (uint32)(0x000FU) << 10,
    e16_BIT_WORD_TRANSFER          =   (uint32)(0x0010U) << 10
};

/**
 * @brief Enumerates the BaudRate levels
 */
enum SpiBaudRateIndex
{
    eSPI_BAUDRATEINDEX_MIN = 663U,
    eSPI_BAUDRATEINDEX_9600 = 9600U,
    eSPI_BAUDRATEINDEX_19200 = 19200U,
    eSPI_BAUDRATEINDEX_57600 = 57600U,
    eSPI_BAUDRATEINDEX_115200 = 115200U,
    eSPI_BAUDRATEINDEX_230400 = 230400U,
    eSPI_BAUDRATEINDEX_460800 = 460800U,
    eSPI_BAUDRATEINDEX_921600 = 921600U,
    eSPI_BAUDRATEINDEX_10000000 = 10000000U,
    eSPI_BAUDRATEINDEX_MAX = 21756864U
};

/**
 * @brief Enumerates the Sampling Edge Configuration
 */
enum SpiSampEdgeConfig
{
    eZERO  =  0U,
    eONE   =  1U,
    eTWO   =  2U,
    eTHREE =  3U
};
/**
 * @brief Enumerates the TX Almost Empty register
 */
enum SpiTxAe
{
    eSPI_TX_THRESHOLD = 0xFFF0,
    eSPI_DISABLE_SPI_TX_AE = 0x7FFF,
    eSPI_ENABLE_SPI_TX_AE  = (uint16)(0x01) << 15
};

/**
 * @brief Enumerates the RX Almost Full register
 */
enum SpiRxAf
{
    eSPI_RX_THRESHOLD = 0xFFF0,
    eSPI_DISABLE_SPI_RX_AF = 0x7FFF,
    eSPI_ENABLE_SPI_RX_AF  = (uint16)(0x01) << 15
};

/**
 * @brief Enumerates the Auto Tx - Rx bits
 */
enum SpiAutoTxRx
{
    eDISABLED_DEFAULT  = 0xFFFC,
    eAUTO_TX_ENABLED   = (0x0001) << 0, 
    eAUTO_RX_ENABLED   = (0x0002) << 0
};

/**
 * @brief Enumerates the CS Sense bits
 */
enum SpiCsSense
{
    eCS_SENSE_MASK       = 0xFFFE,
    eACTIVE_LOW_DEFAULT  = (0x0000) << 0,
    eACTIVE_HIGH         = (0x0001) << 0
};

/**
 * @brief Enumerates the CS Mode bits
 */
enum SpiCsMode
{
    eCS_MODE_MASK       = 0xFFFD,
    eAUTOMATIC_DEFAULT  = (0x0000) << 1,
    eMANUAL             = (0x0001) << 1
};

/**
 * @brief Enumerates the CS Assert bits
 */
enum SpiCsAssert
{
    eCS_ASSERT_MASK  = 0xFFFE,
    eDEASSERT  =          (0x0000) << 0,
    eASSERT_DEFAULT     = (0x0001) << 0
};

/**
 * @brief Enumerates the CS Timing bits
 */
enum SpiCsTiming
{
    /** Setup time */
    eSETUP,
    /** Hold time */
    eHOLD,
    /** Idle time */
    eIDLE
};

/**
 * @internal
 * @brief Define register and base addresses
 */
typedef struct
{
    uint32 spiTx;                 // offset:  0x00U 
    uint32 spiRx;                 // offset:  0x04U 
    uint32 spiStatus;             // offset:  0x08U 
    uint32 spiControl;            // offset:  0x0CU 
    uint32 spiCyclePerBit;        // offset:  0x10U   
    uint32 spiTxAeThreshold;      // offset:  0x14U   
    uint32 spiRxAfThreshold;      // offset:  0x18U        
    uint32 spiTxCount;            // offset:  0x1CU 
    uint32 spiRxCount;            // offset:  0x20U  
    uint32 spiAutoTxRxControl;    // offset:  0x24U 
    uint32 spiAutoTxRxCount;      // offset:  0x28U 
    uint32 spiCsControl;          // offset:  0x2CU 
    uint32 spiCsAssert;           // offset:  0x30U 
    uint32 spiCsAssertBits;       // offset:  0x34U
    uint32 spiCsDeassertBits;     // offset:  0x38U 
    uint32 spiCsIdleBits;         // offset:  0x3CU         
} SpiRegs_t;

#define  stSpiRegs (*( volatile SpiRegs_t *)SPI0_ADDRESS)

/**
 * @brief Set the SPI_MASTER Address
 * @param[in] ePortId An enumerated Spi port
 */
void Spi_SetPort(enum SpiId ePortId);

/**
 * @brief Open a port for use.
 * @details When a port is opened it is enabled and all settings
 * that have been configured become active.
 */
void Spi_Open(void);

/**
 * @brief Close a port.
 * @details When a port is closed it is disabled and all settings
 * that have been configured become inactive.
 */
void Spi_Close(void);

/**
 * @brief Gets whether a port is open.
 * @returns True if open else false
 */
boolean Spi_IsOpen(void);

/**
 * @brief Set SPI as Slave or Master.
 * @details This setting is necessary to be set in order to choose slave or master mode.
 */
void Spi_SetMode(enum SpiMode const keSpiMode); 

/**
 * @brief Get the operating mode.
 * @returns SpiMode The operating mode of SPI master or slave.
 */
boolean Spi_GetMode(void); 

/**
 * @brief Set the word size bits
 * @param[in] keSpiWordSize An enumerated number of word size bits
 */
void Spi_SetWordSize(enum SpiWordSize const keSpiWordSize); 

/**
 * @brief Get the word size bits.
 * @returns True if e16_BITS is set.
 */
boolean Spi_GetWordSize(void); 

/**
 * @brief Set the bit ordering
 * @param[in] keSpiBitOrdering An enumerated number of bit ordering bits
 */
void Spi_SetBitOrdering(enum SpiBitOrdering const keSpiBitOrdering); 

/**
 * @brief Get the bit ordering
 * @returns True if eLSB is set.
 */
boolean Spi_GetBitOrdering(void);

/**
 * @brief Set the sample edge configuration
 * @param[in] keSpiInterfaceSampEdgeConfig An enumerated number of the sample edge configuration
 */
void Spi_SetOperation( enum SpiSampEdgeConfig const keSpiInterfaceSampEdgeConfig );

/**
 * @brief Set the clock phase
 * @param[in] keSpiClockPhase An enumerated number of the clock phase
 */
void Spi_SetClockPhase(enum SpiClockPhase const keSpiClockPhase); 

/**
 * @brief Get the clock phase
 * @returns True if set
 */
boolean Spi_GetClockPhase(void);

/**
 * @brief Set the clock polarity
 * @param[in] keSpiClockPolarity An enumerated number of the clock polarity
 */
void Spi_SetClockPolarity(enum SpiClockPolarity const keSpiClockPolarity); 

/**
 * @brief Get the clock polarity
 * @returns True if set
 */
boolean Spi_GetClockPolarity(void);

/**
 * @brief Enable an interrupt bit
 * @param[in] keSpiIrqIndex An enumerated Irq index
 */
void Spi_SetIrqEnable(enum SpiIrqIndex const keSpiIrqIndex); 

/**
 * @brief Disable an interrupt bit
 * @param[in] keSpiIrqIndex An enumerated Irq index
 */
void Spi_SetIrqDisable(enum SpiIrqIndex const keSpiIrqIndex); 

/**
 * @brief Get the interrupt
 * @param[in] keSpiIrqIndex An enumerated Irq index 
 */
boolean Spi_IsIrqEnabled(enum SpiIrqIndex const keSpiIrqIndex );

/**
 * @brief Clears the Rx and Tx FIFOs.
 * @details Sets bit in SPI_MASTER FIFO to clear both Rx and Tx FIFOS
 */
void Spi_Clear(void) ;  

/**
 * @brief Get the reset bit.
 * @returns True if Fifo is clear else false
 */
boolean Spi_IsClear(void);

/**
 * @brief Set the bits per word
 * @param[in] keBitsPerWord An enumerated bits per word
 */
void Spi_SetBitsPerWord( enum BitsPerWord const keBitsPerWord);

/**
 * @brief Get the bits per word
 */
uint32 Spi_GetBitsPerWord(void);

/**
 * @brief Set the baudrate of the channel.
 * @details Master only
 * @param keBaudRate The baudrate of the channel.
 */
void Spi_SetClockGeneration( enum SpiBaudRateIndex const keBaudRate);

/**
 * @brief Get the baudrate of the channel.
 * @details Master only
 * @returns The baudrate of the channel.
 */
uint32 Spi_GetClockGeneration(void);

/**
 * @brief Set the TX threshold.
 * @details Set the TX Fifo size.
 * @param ku8Count is the size of the FIFO.
 */
void Spi_SetTxAeThreshold( uint16 u16Count );

/**
 * @brief Get the TX threshold.
 * @details get method to retrive the size of the TX FIFO.
 * @returns TX threshold FIFO size.
 */
uint32 Spi_GetTxAeThreshold(void);

/**
 * @brief Set the RX threshold.
 * @details Set the RX Fifo size.
 * @param u16Count is the size of the FIFO.
 */
void Spi_SetRxAfThreshold( uint16 u16Count);

/**
 * @brief Get the RX threshold.
 * @details get method to retrive the size of the RX FIFO.
 * @returns RX threshold FIFO size.
 */
uint32 Spi_GetRxAfThreshold(void);

/**
 * @brief Get the TX Count
 */
uint32 Spi_GetTxCount(void);

/**
 * @brief Get the RX Count
 */
uint32 Spi_GetRxCount(void);

/**
 * @brief Set the auto transmit/receive control.
 * @param keSpiAutoTxRx An enumerated auto transmit/receive setting.
 */
void Spi_SetAutoTrxControl( enum SpiAutoTxRx const keSpiAutoTxRx);

/**
 * @brief Get the auto transmit/receive control settings.
 * @returns AutoTrx The autotrx control setting.
 */
uint32 Spi_GetAutoTrxControl(void) ;

/**
 * @brief Set the auto transmit/receive counter.
 * @param ku8Count The number of the transmissions.
 */
void Spi_SetAutoTrxCount( uint16 u16Count);

/**
 * @brief Get the auto transmit/receive count settings.
 * @returns The number of autotrx count.
 */
uint32 Spi_GetAutoTrxCount(void) ;

/**
 * @brief Set the sense level of chip select.
 * @param[in] keSpiCsSense The active level of the chip select.
 */
void Spi_SetCsSense( enum SpiCsSense const keSpiCsSense);

/**
 * @brief Get the active level of chip select.
 * @returns The sense level of the chip select.
 */
uint32 Spi_GetCsSense(void) ;

/**
 * @brief Set the chip select mode for automatic or manual operation.
 * @details Master only
 * @param[in] keSpiCsMode The chip select mode.
 */
void Spi_SetCsMode( enum SpiCsMode const keSpiCsMode);

/**
 * @brief Get the chip select mode.
 * @details Master only
 * @returns CsMode The chip select mode.
 */
uint32 Spi_GetCsMode(void) ;

/**
 * @brief Set the chip select enable bit.
 * @details In slave mode this bit has to be enabled.
 * @param keSpiCsAssert In master mode with manual chip select operation use this bit to enable and disable the chip select
 * @returns The chip select enable bit.
 */
void Spi_SetCsAssert( enum SpiCsAssert const keSpiCsAssert);

/**
 * @brief Get the chip select enable bit.
 * @returns The chip select enable bit.
 */
uint32 Spi_GetCsAssert(void) ;

/**
 * @brief Set the chip select timing options.
 * @details Master only
 * @param keSpiCsTiming The chip select timing option to be adjusted.
 * @param u16NumBitPeriods The delay of clock cycles.
 */
void Spi_SetCsTiming( enum SpiCsTiming const keSpiCsTiming, uint16 u16NumBitPeriods);

/**
 * @brief Get the chip select timing options.
 * @details Master only
 * @param keCsTiming The chip select timing option to be adjusted.
 * @returns The delay of clock cycles for the selected timing option.
 */
uint32 Spi_GetCsTiming(enum SpiCsTiming const keSpiCsTiming) ;

/**
 * @brief Get the Status flags
 * @param[in] keStatus An enumerated Status Index
 */
boolean Spi_GetStatusRegFlag( enum SpiStatus const keStatus );

/**
 * @brief Clear the Status flags
 * @param[in] keStatus An enumerated Status Index
 */
void Spi_ClearStatusRegFlag( enum SpiStatus const keStatus );

/**
 * @brief Write a byte to the SPI buffer.
 * @param[in] ku16Data A byte
 */
void Spi_Write(  const uint16 ku16Data ) ;

/**
 * @brief Read a byte from the SPI buffer
 * @returns uint16 A byte
 */
uint32 Spi_Read(void) ;

/**
 * @brief Set a handler for the IRQ
 * @param[in] SpiIrqHandler A reference to an IRQ handler method
 */
void Spi_SetIrqHandler(void (SpiIrqHandler)( void ));

#endif /* HEADER_SPI_H */