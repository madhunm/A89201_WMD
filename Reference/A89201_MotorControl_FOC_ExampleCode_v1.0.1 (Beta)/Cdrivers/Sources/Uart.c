/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @internal
 *
 * @file         Uart.c
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
 * @details      Implementation of the EnSillica UART v5.0.7
 */
 /* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
 #include "ARMCM4_FP.h"

#include "Std_Types.h"
#include "Uart.h"
#include "Sci.h"
#include "system_ARMCM4.h"

// UART registers
typedef struct
{
    uint32 uartTx;             // offset:  0x00U 
    uint32 uartRx;             // offset:  0x04U 
    uint32 uartStatus;         // offset:  0x08U 
    uint32 uartControl;        // offset:  0x0CU 
    uint32 uartCyclePerBit;    // offset:  0x10U   
    uint32 reserved0;          // offset:  0x14U   
    uint32 uartTxAeThreshold;  // offset:  0x18U        
    uint32 uartRxAfThreshold;  // offset:  0x1CU 
    uint32 reserved1;          // offset:  0x20U  
    uint32 reserved2;          // offset:  0x24U 
    uint32 reserved3;          // offset:  0x28U 
    uint32 reserved4;          // offset:  0x2CU 
    uint32 uartTxCount;        // offset:  0x30U 
    uint32 uartRxCount;        // offset:  0x34U 
    uint32 uartTxGuard;        // offset:  0x38U 
} Uart_t;

static volatile Uart_t* pstUartRegister = (volatile Uart_t*)UART0_ADDRESS;
static uint8 m_u8UartPortNum;

void Uart_SetPort(enum UartId ePortId)
{
    switch(ePortId)
    {
        case PORTID_0:
            pstUartRegister = (volatile Uart_t*)UART0_ADDRESS;
            m_u8UartPortNum = (uint8) PORTID_0;
            Sci_Use(eUART,m_u8UartPortNum);
            break;
        
        case PORTID_1:
            pstUartRegister = (volatile Uart_t*)UART1_ADDRESS;
            m_u8UartPortNum = (uint8) PORTID_1;
            Sci_Use(eUART,m_u8UartPortNum);
            break;
        
        default:    
            pstUartRegister = (volatile Uart_t*)UART0_ADDRESS;
            break;
    }
}

void Uart_Open(void)
{
    pstUartRegister->uartControl |= (uint32) eENABLE; 
}

void Uart_Close(void)
{
    pstUartRegister->uartControl &= (uint32) eDISABLE; 
}

boolean Uart_IsOpen(void)
{
    boolean bIsOpen = False;
    
    uint32 const u16UartOpen = pstUartRegister->uartControl; 
	
    if ((u16UartOpen & (uint32) eENABLE) != 0)
    {
        bIsOpen = True;
    }
    return bIsOpen;
}

void Uart_SetStopBits(enum UartStopBits const keUartStopBits) 
{    
     switch(keUartStopBits)
    {
        case eONESTOP:
            pstUartRegister->uartControl &= (uint32) eONESTOP;
        break;
        
        case eTWOSTOPS:
            pstUartRegister->uartControl |= (uint32) eTWOSTOPS;
        break;
                       
        default:    // No update
        break;
    }
}

boolean Uart_GetStopBits(void)
{
    boolean bIsSet = False;

    uint32 const ku32StopBits = pstUartRegister->uartControl &(uint32) eTWOSTOPS; 
	
    if ((ku32StopBits & (uint32) eTWOSTOPS) != 0)
    {
        bIsSet = True;
    }
    return bIsSet;
}

void Uart_SetDataBits(enum UartDataBits const keUartDataBits) 
{    
     switch(keUartDataBits)
    {
        case eEIGHT:
            pstUartRegister->uartControl &= (uint32) eEIGHT;
        break;
        
        case eSEVEN:
            pstUartRegister->uartControl |= (uint32)eSEVEN;
        break;
                       
        default:    // No update       
        break;
    }
}

boolean Uart_GetDataBits(void)
{
    boolean bIsSet = False;

    uint32 const ku32DataBits = pstUartRegister->uartControl & (uint32) eSEVEN; 
	
    if ((ku32DataBits & (uint32) eSEVEN) != 0)
    {
        bIsSet = True;
    }
    return bIsSet;
}

void Uart_SetParityBits(enum UartParityBits const keUartParityBits) 
{    
     switch(keUartParityBits)
    {
        case eNOPARITY:
            pstUartRegister->uartControl &= (uint32) eNOPARITY;
        break;
        
        case eEVEN:
            pstUartRegister->uartControl &= (uint32) ePARITY_MASK;
            pstUartRegister->uartControl |= (uint32) eEVEN;
        break;
                               
        case eODD:
            pstUartRegister->uartControl &= (uint32) ePARITY_MASK;
            pstUartRegister->uartControl |= (uint32) eODD;
        break;
        
        case eMARK:
            pstUartRegister->uartControl &= (uint32) ePARITY_MASK;
            pstUartRegister->uartControl |= (uint32) eMARK;
        break;
                
        case eSPACE:
            pstUartRegister->uartControl &= (uint32) ePARITY_MASK;
            pstUartRegister->uartControl |= (uint32) eSPACE;
        break;
                
        default:    // No update
        break;
    }
}

boolean Uart_GetNoParityBit(void)
{
    boolean bIsSet = True;
    uint32 const ku32UartNoParityBit = pstUartRegister->uartControl; 
    uint32 const u32ParityBitExtract = ((ku32UartNoParityBit >> 3) &(uint16) ePARITY_MASK);
    if (u32ParityBitExtract != 0)
    {
        bIsSet = False;
    }
    return bIsSet;
}

boolean Uart_GetEvenParityBit(void)
{
    boolean bIsSet = False;
    uint32 const ku32UartEvenParityBit = pstUartRegister->uartControl; 

    if ((ku32UartEvenParityBit & (uint32) eEVEN)!= 0)
    {
        bIsSet = True;
    }
    return bIsSet;
}

boolean Uart_GetOddParityBit(void)
{
    boolean bIsSet = False;
    uint32 const ku32UartOddParityBit = pstUartRegister->uartControl; 

    if ((ku32UartOddParityBit & (uint32) eODD)!= 0)
    {
        bIsSet = True;
    }
    return bIsSet;
}

boolean Uart_GetMarkParityBit(void)
{
    boolean bIsSet = False;

    uint32 const ku32UartOddParityMask = pstUartRegister->uartControl &(uint32) eODD; 
    uint32 const ku32UartEvenParityMask = pstUartRegister->uartControl &(uint32) eEVEN;
    
    if((ku32UartOddParityMask !=0 && ku32UartEvenParityMask !=0))
    {
        bIsSet = True;
    }
    return bIsSet;
}

boolean Uart_GetSpaceParityBit(void)
{
    boolean bIsSet = False;
    uint32 const ku32UartSpaceParityBit = pstUartRegister->uartControl; 

    if ((ku32UartSpaceParityBit &(uint32) eSPACE) !=0)
    {
        bIsSet = True;
    }
    return bIsSet;
}

void Uart_SetEnableTransmitBreak(void)
{
    pstUartRegister->uartControl |= (uint32) eENABLE_UART_TX_BREAK;    
}

void Uart_SetDisableTransmitBreak(void)
{
    pstUartRegister->uartControl &= (uint32) eDISABLE_UART_TX_BREAK;   
}

boolean Uart_GetTransmitBreak(void)
{
    boolean bIsSet = False;
    uint32 const ku32UartTransmitBreak = pstUartRegister->uartControl; 

    if ((ku32UartTransmitBreak & (uint32) eENABLE_UART_TX_BREAK) != 0)
    {
        bIsSet = True;
    }
    return bIsSet;
}

void Uart_Clear(void)
{
    pstUartRegister->uartControl |= (uint32) eUART_RESET;   
}

void Uart_SetBaudRate( enum BaudRateIndex const keBaudRate)
{   
    pstUartRegister->uartCyclePerBit = 0;
    
    uint32 const uFrequency  = SYSTEM_CLOCK;

    uint32 const uSpeed =  (uFrequency  / (uint32) keBaudRate) -1;

    pstUartRegister->uartCyclePerBit =  uSpeed;
}

uint32 Uart_GetBaudRate(void)
{
    uint32 const ku32UartBaudRate = pstUartRegister->uartCyclePerBit; 
      
    return ku32UartBaudRate;
}

void Uart_SetIrqEnable(enum UartIrqIndex const keUartIrqIndex) 
{    
     switch(keUartIrqIndex)
    {
        case eUART_RX_BREAK:
            pstUartRegister->uartControl |= (uint32) eENABLE_UART_RX_BREAK;          
        break;
        
        case eUART_RX_RECEIVED:
            pstUartRegister->uartControl |= (uint32) eENABLE_UART_RX_RECEIVED;
        break;
                               
        case eUART_TX_COMPLETE:
            pstUartRegister->uartControl |= (uint32) eENABLE_UART_TX_COMPLETE;
        break;
        
        case eUART_TX_AE:
            pstUartRegister->uartTxAeThreshold |= (uint32) eENABLE_UART_TX_AE;
        break;
                
        case eUART_RX_AF:
            pstUartRegister->uartRxAfThreshold |= (uint32) eENABLE_UART_RX_AF;
        break;
                
        default:    // No update
        break;
    }
}

void Uart_SetIrqDisable(enum UartIrqIndex const keUartIrqIndex) 
{    
     switch(keUartIrqIndex)
    {
        case eUART_RX_BREAK:
            pstUartRegister->uartControl &= (uint32) eDISABLE_UART_RX_BREAK;  
        break;
        
        case eUART_RX_RECEIVED:
            pstUartRegister->uartControl &= (uint32) eDISABLE_UART_RX_RECEIVED;
        break;
                               
        case eUART_TX_COMPLETE:
            pstUartRegister->uartControl &= (uint32) eDISABLE_UART_TX_COMPLETE;
        break;
        
        case eUART_TX_AE:
            pstUartRegister->uartTxAeThreshold &= (uint32) eDISABLE_UART_TX_AE;
        break;
                
        case eUART_RX_AF:
            pstUartRegister->uartRxAfThreshold &= (uint32) eDISABLE_UART_RX_AF;
        break;
                
        default:    // No update
        break;
    }
}

boolean Uart_IsIrqEnabled(enum UartIrqIndex const keUartIrqIndex )
{
    boolean bRetVal = False;
    uint32 u16UartIrq = pstUartRegister->uartControl; 

    uint32 u16MaskBit;
    
    switch(keUartIrqIndex)
    {
        case eUART_RX_BREAK:
            u16MaskBit =  (uint32) eENABLE_UART_RX_BREAK;       
        break;
        
        case eUART_RX_RECEIVED:
            u16MaskBit =  (uint32) eENABLE_UART_RX_RECEIVED;       

        break;
                               
        case eUART_TX_COMPLETE:
            u16MaskBit =  (uint32) eENABLE_UART_TX_COMPLETE;       
        break;
        
        case eUART_TX_AE:
            u16MaskBit =  (uint32) eENABLE_UART_TX_AE;       
            u16UartIrq = pstUartRegister-> uartTxAeThreshold;
        break;
                
        case eUART_RX_AF:
            u16MaskBit =  (uint32) eENABLE_UART_RX_AF;    
            u16UartIrq = pstUartRegister->uartRxAfThreshold;       
        break;
                
        default:    // No update
        break;
    }
    if ((u16UartIrq &u16MaskBit)!=0)
    {
        bRetVal = True;
    } 
    return bRetVal;
}

void Uart_SetTxAeThreshold( uint16 u16Count)
{   
    pstUartRegister->uartTxAeThreshold &= (uint32) eTX_THRESHOLD;      

    pstUartRegister->uartTxAeThreshold |= u16Count;      
}

uint32 Uart_GetTxAeThreshold(void)
{
    uint32 const ku32UartTxAeInterrupt = pstUartRegister->uartTxAeThreshold; 
      
    return ku32UartTxAeInterrupt;
}

void Uart_SetRxAfThreshold( uint16 u16Count)
{   
    pstUartRegister->uartRxAfThreshold &= (uint32) eRX_THRESHOLD;    

    pstUartRegister->uartRxAfThreshold |= u16Count;  
}

uint32 Uart_GetRxAfThreshold(void)
{
    uint32 const ku32UartRxAfThreshold = pstUartRegister->uartRxAfThreshold; 
        
    return ku32UartRxAfThreshold;
}

uint32 Uart_GetTxCount(void)
{
    uint32 const ku32UartTxCount = pstUartRegister->uartTxCount; 
        
    return ku32UartTxCount;
}

uint32 Uart_GetRxCount(void)
{
    uint32 const ku32UartRxCount = pstUartRegister->uartRxCount; 
        
    return ku32UartRxCount;
}

void Uart_SetTxGuardTime( uint16 u16NumBitPeriods)
{   
    pstUartRegister->uartTxGuard |= u16NumBitPeriods;   
}

uint32 Uart_GetTxGuardTime(void)
{
    uint32 const ku32UartTxGuardTime = pstUartRegister->uartTxGuard; 
      
    return ku32UartTxGuardTime;
}

boolean Uart_GetStatusRegFlag(enum UartStatus const keStatus )
{
    boolean bRetVal = False;
    uint32 const ku32UartStatus = pstUartRegister->uartStatus; 

    uint32 const ku32MaskBit = (uint32) 1<<((uint32) keStatus);
    if ((ku32UartStatus &ku32MaskBit)!=0)
    {
        bRetVal = True;
    } 
    return bRetVal;
}

void Uart_ClearStatusRegFlag(enum UartStatus const keStatus )
{
    switch(keStatus)
    {
        case eTXOVERFLOW:
            pstUartRegister->uartStatus |= (uint32) 1 << (uint32) eTXOVERFLOW;
        break;                           
                                         
        case eRXOVERFLOW:                
            pstUartRegister->uartStatus |= (uint32) 1 << (uint32) eRXOVERFLOW;
        break;                           
                                         
        case eRXPARITYERROR:             
            pstUartRegister->uartStatus |= (uint32) 1 << (uint32) eRXPARITYERROR;       
        break;                           
                                         
        case eRXFRAMINGERROR:            
            pstUartRegister->uartStatus |= (uint32) 1 << (uint32) eRXFRAMINGERROR;
        break;                           
                                         
        case eRXBREAK:                   
            pstUartRegister->uartStatus |= (uint32) 1 << (uint32) eRXBREAK;
        break;
        
        default:    // No update
        break;
    }
}

void Uart_Write( const uint16 ku16Data)
{      
    volatile uint32* const pu32UartWrite = &pstUartRegister->uartTx;
    *pu32UartWrite = ku16Data ;                                                                     
}

uint32 Uart_Read(void)
{
    uint32 const ku32UartRead = pstUartRegister->uartRx; 
      
    return ku32UartRead;
}

// Set the UART Irq Handler to required function
void Uart_SetIrqHandler( void (UartIrqHandler)( void ) )
{
    Sci_SetPortIrqHandler(m_u8UartPortNum, UartIrqHandler);
}
