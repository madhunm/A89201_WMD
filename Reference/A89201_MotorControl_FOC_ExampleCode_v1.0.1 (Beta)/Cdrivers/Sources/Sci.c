/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @internal
 *
 * @file         Sci.cpp
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
 * @details      Implementation of the EnSillica SCI v6.0.3
 */
/* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */

#include "Sci.h"

static uint8 m_u8Port;

/**
 * @internal
 * @details A table of the supported interrupt numbers
 */
static const uint32 s_kaeIrq[gku8MaxNumSciPorts] =
{
  (uint8) eSCI0,
  (uint8) eSCI1  
};

/**
 * @internal
 * @details Add the address of the IRQ specific handler to the IRQ handler table.
 * This implementation is protected against changes to the Irq enumeration
 */
void Sci_SetPortIrqHandler( const uint8 nPortNum, void (SciIrqHandler)( void ) )
{
    switch(nPortNum)
    {
        case 0:
            Sci_SetIrqHandler( SciIrqHandler );    
        break;
        
        case 1:
            Sci_SetIrqHandler( SciIrqHandler );
        break;
                
        default:    // No update
        break;
    } 
}

/**
 * @internal
 * @details Allocate a port to the object instance
 */
void Sci_Use(  const enum ChannelType keChannelType, const uint8 nPortNum)
{ 
    switch(keChannelType)
    {
        case eSPI:
            Sci_UseSpi( nPortNum);   
        break;
        
        case eUART:
            Sci_UseUart( nPortNum);
        break;
                
        default:    // No update
        break;
    } 
}
/**
 * @internal
 * @details Allocate a port to the Spi object instance
 */
void Sci_UseSpi( const uint8 nPortNum)
{ 
    switch(nPortNum)
    {
        case 0:
            m_u8Port = s_ku8SpiSci0;
        break;
        
        case 1:
            m_u8Port = s_ku8SpiSci1;
        break;
                
        default:    // No update
        break;
    } 
}

/**
 * @internal
 * @details Allocate a port to the Uart object instance
 */
void Sci_UseUart( const uint8 nPortNum)
{
    switch(nPortNum)
    {
        case 0:
            m_u8Port = s_ku8SpiSci0;
        break;
        
        case 1:
            m_u8Port = s_ku8SpiSci1;
        break;
                
        default:    // No update
        break;
    }
}

// Declare a function pointer for use by SPI Irq
static void (*fpSciIrqHandler)( void ) = NULLPTR;

/**
 * @internal
 * @details Add the address of the IRQ specific handler to the IRQ handler table.
 * This implementation is protected against changes to the Irq enumeration
 */
void Sci_SetIrqHandler( void (SpiIrqHandler)( void ) )
{
     fpSciIrqHandler = *SpiIrqHandler;
}


/**
 * @internal
 * @details This SPI_INTERRUPT0 handler is registered in the vector table.
 */
void Sci_IrqHandler0(void) 
{
    NVIC_DisableIRQ( ( IRQn_Type) s_kaeIrq[s_ku8SpiSci0] );
    NVIC_ClearPendingIRQ( ( IRQn_Type) s_kaeIrq[s_ku8SpiSci0]  );

    if(fpSciIrqHandler != NULLPTR)
    {
        fpSciIrqHandler();
    }
    NVIC_EnableIRQ( ( IRQn_Type) s_kaeIrq[s_ku8SpiSci0] );
}

/**
 * @internal
 * @details This SPI_INTERRUPT1 handler is registered in the vector table.
 */
void Sci_IrqHandler1(void)
{
    NVIC_DisableIRQ( ( IRQn_Type) s_kaeIrq[s_ku8SpiSci1] );
    NVIC_ClearPendingIRQ( ( IRQn_Type) s_kaeIrq[s_ku8SpiSci1]  );

    if(fpSciIrqHandler != NULLPTR)
    {
        fpSciIrqHandler();
    }
    NVIC_EnableIRQ( ( IRQn_Type) s_kaeIrq[s_ku8SpiSci1] );
}
