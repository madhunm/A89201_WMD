#ifndef HEADER_SCI_H
#define HEADER_SCI_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Sci.h
 * @author       N. Moheeputh
 * @version      1.0
 *
 * @brief        The implementation of Serial Communication Interface (SCI) Interrupt interface for the shared interrupts between Uart and Spi product.
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

/**
 * @internal
 * @brief Enumerate the available SCI channels
 */
enum Channel 
{
    /** Channel 0 */
    eSCI0,
    /** Channel 1 */
    eSCI1
};

/**
 * @internal
 * @brief Enumerate the SCI channel types
 */
enum ChannelType 
{
    /** Channel type spi */
    eSPI,
    /** Channel type uart */
    eUART
};

/** The number of ports supported by the SCI implementation */
static uint8 k_u8NumPorts = 0x02U;

/** The number of IRQ handlers */
static uint8 k_u8NumIrqHandlers = 0x05;

/** The number of status bits */
static uint8 k_u8NumStatusBits = 0x0AU;

/**The value of an invalid SPI port number*/
//static uint8 ku8InvalidPortNum {ku8Invalid};

/** The SCI 0 */
static uint8 s_ku8SpiSci0 = 0x00U;

/** The SCI 1 */
static uint8 s_ku8SpiSci1 = 0x01U;

/**
 * @brief The IRQ handler for SCI 0.
 */
void Sci_IrqHandler0(void);
/**
 * @brief The IRQ handler for SCI 1.
 */
void Sci_IrqHandler1(void);

/**
 * @brief Set a handler for the SCI IRQ
 * @details Does nothing if object not initialized.
 * @param[in] nPortNum A reference to the port address.
 * @param[in] nIrqHandler A reference to a derived class which will handle the enumeration for the IRQ.
 * @param[in] rclsIrqHandler A reference to a derived class which will handle the SCI IRQ.
 */
void Sci_SetIrqHandler( void (SpiIrqHandler)( void )  );

/**
 * @brief The Use method initialises the instance of HdiSci of the given port number.
 * @param[in] keChannelType The type of SCI channel.
 * @param[in] nPortNum A reference to the port address.
 * @param[in] keRegister A reference to the status register of UART/SPI.
 */
void Sci_Use( const enum ChannelType keChannelType, const uint8 nPortNum);

/**
 * @brief The Use method initialises the instance of SPI of the given port number.
 * @param[in] nPortNum A reference to the port address.
 * @param[in] keRegister A reference to the status register of SPI.
 */
void Sci_UseSpi( const uint8 nPortNum);

/**
 * @brief The Use method initialises the instance of UART of the given port number.
 * @param[in] nPortNum A reference to the port address.
 * @param[in] keRegister A reference to the status register of UART.
 */
void Sci_UseUart( const uint8 nPortNum);

/**
 * @brief Set a handler for the SCI IRQ
 * @details Does nothing if object not initialized.
 * @param[in] nPortNum A reference to the port address.
 * @param[in] nIrqHandler A reference to a derived class which will handle the enumeration for the IRQ.
 * @param[in] rclsIrqHandler A reference to a derived class which will handle the SCI IRQ.
 */
void Sci_SetPortIrqHandler( const uint8 nPortNum, void (SciIrqHandler)( void )  );

#endif //HEADER_SCI_H