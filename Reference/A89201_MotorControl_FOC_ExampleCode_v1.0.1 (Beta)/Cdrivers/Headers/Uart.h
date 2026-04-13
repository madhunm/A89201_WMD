#ifndef HEADER_UART_H
#define HEADER_UART_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Uart.h
 * @author       N. Moheeputh
 * @version      1.0
 *
 * @brief        The implementation of a UART interface.
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

#define    UART0_ADDRESS  (0x4000A000U)
#define    UART1_ADDRESS  (0x4000B000U)

// SCI Definitions
// ---------------

// SciRegisters
#define UART0_BASE_ADDRESS       0x4000A000U
#define UART0_TX                 0x00U
#define UART0_RX                 0x04U
#define UART0_STATUS             0x08U
#define UART0_CONTROL            0x0CU
#define UART0_CYCLES_PER_BIT     0x10U
#define UART0_TXAE_THRESHOLD     0x18U
#define UART0_RXAF_THRESHOLD     0x1CU
#define UART0_TX_COUNT           0x30U
#define UART0_RX_COUNT           0x34U
#define UART0_TX_GUARD           0x38U
#define UART1_BASE_ADDRESS       0x4000B000U
#define UART1_TX                 0x00U
#define UART1_RX                 0x04U
#define UART1_STATUS             0x08U
#define UART1_CONTROL            0x0CU
#define UART1_CYCLES_PER_BIT     0x10U
#define UART1_TXAE_THRESHOLD     0x18U
#define UART1_RXAF_THRESHOLD     0x1CU
#define UART1_TX_COUNT           0x30U
#define UART1_RX_COUNT           0x34U
#define UART1_TX_GUARD           0x38U

// SciUartStatus
#define SCI_UARTSTATUS_TXE   ( 0b0000000000000001U )
#define SCI_UARTSTATUS_TXF   ( 0b0000000000000010U )
#define SCI_UARTSTATUS_TXO   ( 0b0000000000000100U )
#define SCI_UARTSTATUS_RXE   ( 0b0000000000001000U )
#define SCI_UARTSTATUS_RXF   ( 0b0000000000010000U )
#define SCI_UARTSTATUS_RXO   ( 0b0000000000100000U )
#define SCI_UARTSTATUS_RXP   ( 0b0000000001000000U )
#define SCI_UARTSTATUS_RXB   ( 0b0000000010000000U )
#define SCI_UARTSTATUS_FE    ( 0b0000000100000000U )
#define SCI_UARTSTATUS_PE    ( 0b0000001000000000U )
#define SCI_UARTSTATUS_TXAE  ( 0b0100000000000000U )
#define SCI_UARTSTATUS_RXAF  ( 0b1000000000000000U )

// SciUartControl
#define SCI_UARTCONTROL_UART_EN      ( 0b0000000000000001U )
#define SCI_UARTCONTROL_SB           ( 0b0000000000000010U )
#define SCI_UARTCONTROL_DB           ( 0b0000000000000100U )
   
#define SCI_UARTCONTROL_NO_PARITY    ( 0b0000000000000000U )
#define SCI_UARTCONTROL_EVEN_PARITY  ( 0b0000000000001000U )
#define SCI_UARTCONTROL_ODD_PARITY   ( 0b0000000000010000U )
#define SCI_UARTCONTROL_MARK         ( 0b0000000000011000U )
#define SCI_UARTCONTROL_SPACE        ( 0b0000000000100000U )
    
#define SCI_UARTCONTROL_TXB          ( 0b0000000010000000U )
#define SCI_UARTCONTROL_TXIE         ( 0b0000000100000000U )
#define SCI_UARTCONTROL_RXIE         ( 0b0000001000000000U )
#define SCI_UARTCONTROL_RXBIE        ( 0b0000010000000000U )
#define SCI_UARTCONTROL_RF           ( 0b0010000000000000U )

// SciUartTxAeThreshold
#define UART_TXAE_THRESHOLD_IE         ( 0b1000000000000000U )
#define UART_TXAE_THRESHOLD_0_ENTRIES  ( 0b0000000000000000U )
#define UART_TXAE_THRESHOLD_1_ENTRY    ( 0b0000000000000001U ) 
#define UART_TXAE_THRESHOLD_2_ENTRIES  ( 0b0000000000000010U ) 
#define UART_TXAE_THRESHOLD_3_ENTRIES  ( 0b0000000000000011U ) 
#define UART_TXAE_THRESHOLD_4_ENTRIES  ( 0b0000000000000100U ) 
#define UART_TXAE_THRESHOLD_5_ENTRIES  ( 0b0000000000000101U ) 
#define UART_TXAE_THRESHOLD_6_ENTRIES  ( 0b0000000000000110U ) 
#define UART_TXAE_THRESHOLD_7_ENTRIES  ( 0b0000000000000111U ) 
#define UART_TXAE_THRESHOLD_8_ENTRIES  ( 0b0000000000001000U ) 
#define UART_TXAE_THRESHOLD_9_ENTRIES  ( 0b0000000000001001U ) 
#define UART_TXAE_THRESHOLD_10_ENTRIES ( 0b0000000000001010U ) 
#define UART_TXAE_THRESHOLD_11_ENTRIES ( 0b0000000000001011U ) 
#define UART_TXAE_THRESHOLD_12_ENTRIES ( 0b0000000000001100U ) 
#define UART_TXAE_THRESHOLD_13_ENTRIES ( 0b0000000000001101U ) 
#define UART_TXAE_THRESHOLD_14_ENTRIES ( 0b0000000000001110U ) 
#define UART_TXAE_THRESHOLD_15_ENTRIES ( 0b0000000000001111U ) 
#define UART_TXAE_THRESHOLD_16_ENTRIES ( 0b0000000000010000U ) 

// SciUartRxAfThreshold
#define UART_RXAF_THRESHOLD_IE         ( 0b1000000000000000U )
#define UART_RXAF_THRESHOLD_0_ENTRIES  ( 0b0000000000000000U )
#define UART_RXAF_THRESHOLD_1_ENTRY    ( 0b0000000000000001U ) 
#define UART_RXAF_THRESHOLD_2_ENTRIES  ( 0b0000000000000010U ) 
#define UART_RXAF_THRESHOLD_3_ENTRIES  ( 0b0000000000000011U ) 
#define UART_RXAF_THRESHOLD_4_ENTRIES  ( 0b0000000000000100U ) 
#define UART_RXAF_THRESHOLD_5_ENTRIES  ( 0b0000000000000101U ) 
#define UART_RXAF_THRESHOLD_6_ENTRIES  ( 0b0000000000000110U ) 
#define UART_RXAF_THRESHOLD_7_ENTRIES  ( 0b0000000000000111U ) 
#define UART_RXAF_THRESHOLD_8_ENTRIES  ( 0b0000000000001000U ) 
#define UART_RXAF_THRESHOLD_9_ENTRY    ( 0b0000000000001001U ) 
#define UART_RXAF_THRESHOLD_10_ENTRIES ( 0b0000000000001010U ) 
#define UART_RXAF_THRESHOLD_11_ENTRIES ( 0b0000000000001011U ) 
#define UART_RXAF_THRESHOLD_12_ENTRIES ( 0b0000000000001100U ) 
#define UART_RXAF_THRESHOLD_13_ENTRIES ( 0b0000000000001101U ) 
#define UART_RXAF_THRESHOLD_14_ENTRIES ( 0b0000000000001110U ) 
#define UART_RXAF_THRESHOLD_15_ENTRIES ( 0b0000000000001111U ) 

// End of SCI Definitions
// ----------------------

enum UartIrqIndex
{
    eUART_RX_BREAK,
    eUART_RX_RECEIVED,
    eUART_TX_COMPLETE,
    eUART_TX_AE,
    eUART_RX_AF
};

/**
 * @brief Enumerates the Uart channels
 */
enum UartId
{
    PORTID_0 = 0x00, 
    PORTID_1 = 0x01
};

/**
 * @brief Enumerates the status types
 */
enum UartStatus
{
    /** Receive FIFO almost full */
    eRXALMOSTFULL = 15U,
    /** Transmit FIFO almost empty */
    eTXALMOSTEMPTY = 14U,
    /** Receive parity error */
    eRXPARITYERROR = 9U,
    /** Receive framing error */
    eRXFRAMINGERROR = 8U,
    /** Receive break */
    eRXBREAK = 7U,
    /** Receive parity value */
    eRXPARITY = 6U,
    /** Receive FIFO overflow */
    eRXOVERFLOW = 5U,
    /** Receive FIFO full */
    eRXFULL = 4U,
    /** Receive FIFO empty */
    eRXEMPTY = 3U,
    /** Transmit FIFO overflow */
    eTXOVERFLOW = 2U,
    /** Transmit FIFO full */
    eTXFULL = 1U,
    /** Transmit FIFO empty */
    eTXEMPTY = 0U,
    /** Invalid */
    eINVALID = 0xFF
};

/**
 * @brief Enumerates the enable/disable bit
 */
enum UartEnable
{
    eDISABLE = 0xFFFE, 
    eENABLE  = (0x0001) << 0 
};

/**
 * @brief Enumerates the Stop Bits
 */
enum UartStopBits
{
    eONESTOP  = 0xFFFD,
    eTWOSTOPS = (0x0001) << 1
};

/**
 * @brief Enumerates the Data Bits
 */
enum UartDataBits
{
    eEIGHT  =   0xFFFB,
    eSEVEN  = ( 0x0001 ) <<2
};

/**
 * @brief Enumerates the Parity Bits
 */
enum UartParityBits
{
    ePARITY_MASK = 0xFFC7,
    eNOPARITY  = (0x0000) << 3,
    eEVEN      = (0x0001) << 3, 
    eODD       = (0x0002) << 3, 
    eMARK      = (0x0003) << 3,   
    eSPACE     = (0x0004) << 3 
};

/**
 * @brief Enumerates the BaudRate levels
 */
enum BaudRateIndex
{
    eBAUDRATEINDEX_MIN = 663U,
    eBAUDRATEINDEX_9600 = 9600U,
    eBAUDRATEINDEX_19200 = 19200U,
    eBAUDRATEINDEX_57600 = 57600U,
    eBAUDRATEINDEX_115200 = 115200U,
    eBAUDRATEINDEX_230400 = 230400U,
    eBAUDRATEINDEX_460800 = 460800U,
    eBAUDRATEINDEX_921600 = 921600U,
    eBAUDRATEINDEX_10000000 = 10000000U,
    eBAUDRATEINDEX_MAX = 21756864U
};

/**
 * @brief Enumerates the TX Break bit
 */
enum UartTxBreak
{
    eDISABLE_UART_TX_BREAK =  0xFF7F,
    eENABLE_UART_TX_BREAK  =  (uint32)(0x01) << 7
};

/**
 * @brief Enumerates the TX Complete interrupt bit
 */
enum UartTxComplete
{
    eDISABLE_UART_TX_COMPLETE = 0xFEFF,
    eENABLE_UART_TX_COMPLETE  = (uint32)(0x01) << 8
};

/**
 * @brief Enumerates the RX Riceived interrupt bit
 */
enum UartRxReceived
{
    eDISABLE_UART_RX_RECEIVED = 0xFDFF,
    eENABLE_UART_RX_RECEIVED  = (uint32)(0x01) << 9
};

/**
 * @brief Enumerates the RX Break interrupt bit
 */
enum UartRxBreak
{
    eDISABLE_UART_RX_BREAK = 0xFBFF,
    eENABLE_UART_RX_BREAK  = (uint32)(0x01) << 10
};

/**
 * @brief Enumerates the Reset bit
 */
enum UartReset
{
    eUART_RESET = (uint32)(0x01) << 13
};

/**
 * @brief Enumerates the TX Almost Empty register
 */
enum UartTxAe
{
    eTX_THRESHOLD = 0xFFE0,
    eDISABLE_UART_TX_AE = 0x7FFF,
    eENABLE_UART_TX_AE  = (uint32)(0x01) << 15
};

/**
 * @brief Enumerates the RX Almost Full register
 */
enum UartRxAf
{
    eRX_THRESHOLD = 0xFFE0,
    eDISABLE_UART_RX_AF = 0x7FFF,
    eENABLE_UART_RX_AF  = (uint32)(0x01) << 15
};

/**
 * @brief Set the UART Address
 * @param[in] ePortId An enumerated Uart port
 */
void Uart_SetPort(enum UartId ePortId);

/**
 * @brief Open a port for use.
 * @details When a port is opened it is enabled and all settings
 * that have been configured become active.
 */
void Uart_Open(void);
/**
 * @brief Close a port.
 * @details When a port is closed it is disabled and all settings
 * that have been configured become inactive.
 */
void Uart_Close(void);

/**
 * @brief Gets whether a port is open.
 * @returns True if set else false
 */
boolean Uart_IsOpen(void);

/**
 * @brief Set the stop bits
 * @param[in] keUartStopBits An enumerated number of stop bits
 */
void Uart_SetStopBits(enum UartStopBits const keUartStopBits); 

/**
 * @brief Gets the stop bits value.
 * @returns 0 if One Stop bit, 2 if Two Stop Bits
 */
boolean Uart_GetStopBits(void); 

/**
 * @brief Set the data bits to eight
 * @param[in] keUartDataBits An enumerated number of data bits
 */
void Uart_SetDataBits(enum UartDataBits const keUartDataBits); 

/**
 * @brief Get the data bits to eight
 * @returns True if 1 else false
 */
boolean Uart_GetDataBits(void); 

/**
 * @brief Set the parity bit to no parity
 * @param[in] keUartParityBits An enumerated number of parity bits
 */
void Uart_SetParityBits(enum UartParityBits const keUartParityBits); 

/**
 * @brief Get the parity bit
 * @returns True if set
 */
boolean Uart_GetNoParityBit(void);

/**
 * @brief Get the parity bit
 * @returns True if set
 */
boolean Uart_GetEvenParityBit(void);

/**
 * @brief Get the parity bit
 * @returns True if set
 */
boolean Uart_GetOddParityBit(void);

/**
 * @brief Get the parity bit
 * @returns True if set
 */
boolean Uart_GetMarkParityBit(void);

/**
 * @brief Get the parity bit
 * @returns True if set
 */
boolean Uart_GetSpaceParityBit(void);

/**
 * @brief Enable the transmit break
 */
void Uart_SetEnableTransmitBreak(void);

/**
 * @brief Disable the transmit break 
 */
void Uart_SetDisableTransmitBreak(void);

/**
 * @brief Get the transmit break 
 * @returns True if set else false
 */
boolean Uart_GetTransmitBreak(void);

/**
 * @brief Clears the Rx and Tx FIFOs.
 * @details Sets bit in UART FIFO to clear both Rx and Tx FIFOS
 */
void Uart_Clear(void) ;  

/**
 * @brief Set the baud rate
 * @param[in] keBaudRate An enumerated baud rate
 */
void Uart_SetBaudRate( enum BaudRateIndex const keBaudRate);

/**
 * @brief Get the baud rate
 * @returns BaudRate value
 */
uint32 Uart_GetBaudRate(void);

/**
 * @brief Enable an interrupt bit
 * @param[in] keUartIrqIndex An enumerated Irq index
 */
void Uart_SetIrqEnable(enum UartIrqIndex const keUartIrqIndex); 

/**
 * @brief Disable an interrupt bit
 * @param[in] keUartIrqIndex An enumerated Irq index
 */
void Uart_SetIrqDisable(enum UartIrqIndex const keUartIrqIndex); 

/**
 * @brief Get the interrupt
 * @param[in] keUartIrqIndex An enumerated Irq index
 * @returns True if set else false 
 */
boolean Uart_IsIrqEnabled(enum UartIrqIndex const keUartIrqIndex );

/**
 * @brief Set the TXAE Threshold
 * @param[in] u8Count threshold entry
 */
void Uart_SetTxAeThreshold( uint16 u16Count );

/**
 * @brief Get the TXAE Threshold
 * @returns TXAE Threshold value
 */
uint32 Uart_GetTxAeThreshold(void);

/**
 * @brief Set the RXAF Threshold
 * @param[in] u8Count threshold entry
 */
void Uart_SetRxAfThreshold( uint16 u16Count);

/**
 * @brief Get the RXAF Threshold
 * @returns RXAF Threshold value
 */
uint32 Uart_GetRxAfThreshold(void);

/**
 * @brief Get the TX Count
 * @returns TX Count value
 */
uint32 Uart_GetTxCount(void);

/**
 * @brief Get the RX Count
 * @returns RX Count value
 */
uint32 Uart_GetRxCount(void);

/**
 * @brief Set the TX Guard Time
 * @param[in] u8NumBitPeriods  Guard Time Period
 */
void Uart_SetTxGuardTime( uint16 u16NumBitPeriods);

/**
 * @brief Get the Transmit Guard Time
 * @returns uint8 The guard time expressed in bit periods
 */
uint32 Uart_GetTxGuardTime(void) ;

/**
 * @brief Get the Status flags
 * @param[in] keStatus An enumerated Status Index
 * @returns True if set else false
 */
boolean Uart_GetStatusRegFlag( enum UartStatus const keStatus );

/**
 * @brief Clear the Status flags
 * @param[in] keStatus An enumerated Status Index
 */
void Uart_ClearStatusRegFlag( enum UartStatus const keStatus );

/**
 * @brief Write a byte to the UART.
 * @param[in] ku16Data A byte
 */
void Uart_Write(  const uint16 ku16Data ) ;

/**
 * @brief Read a byte from the UART
 * @returns uint8 A byte
 */
uint32 Uart_Read(void) ;

/**
 * @brief Set a handler for the IRQ
 * @param[in] UartIrqHandler A reference to an IRQ handler method
 */
void Uart_SetIrqHandler(void (UartIrqHandler)( void ));

#endif /* HEADER_UART_H */