/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @internal
 *
 * @file         GduMaster.c
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
 * @details      Implementation reflects I2C GDU Master Design Intent v0.9
 *
 * @remarks      This file implements private definitions.
 */
 /* polyspace-end MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
#include "GduMaster.h"

/**
 * @brief The I2C Fault interrupt handler.
 */
static void (*spfnI2cFaultIrqHandler)(void* pContext) = NULLPTR;

/**
 * @internal
 * @brief defines a mask for the fault Irqs, ack, crc, ovf, ff
 */
static uint32 const su32GduMasterFaultMask = 0x0000003CU;

/**
 * @brief The GDU ignition interrupt handler and context data.
 */
static void (*spfnGduIrqHandler)(void* pContext) = NULLPTR;
static uintptr suPrIgnitionData;
static IrqType_t const skIrqGdu = ARM_IRQ_NUM(eIRQNUM_GDU);

/**
 * @internal
 * @brief defines a mask for the ignition IRQ bit 0
 */
static uint32 const su32GduMasterIrqIgMask = 0x00000001U;

/**
 * @internal
 * @brief Enumerate the index for the Tcf bits
 */
enum GduMasterTcfMask
{
    eGDUMASTER_TCF_VLRU_MASK   = 0x1U,       /* VLR Undervoltage */
    eGDUMASTER_TCF_VRU_MASK    = 0x2U,       /* VREG Undervoltage */
    eGDUMASTER_TCF_VBU_MASK    = 0x4U,       /* VBB Undervoltage */
    eGDUMASTER_TCF_VCPU_MASK   = 0x8U,       /* VCP Undervoltage*/
    eGDUMASTER_TCF_SBOU_MASK   = 0x10U,      /* Sensor BIAS Under voltage */
    eGDUMASTER_TCF_ETO_MASK    = 0x20U,      /* Enable watchdog timeout */
    eGDUMASTER_TCF_WD_MASK     = 0x40U,      /* Q&A Watchdog fault */
    eGDUMASTER_TCF_BU_MASK     = 0x80U,      /* Bootstrap undervoltage */
    eGDUMASTER_TCF_TW_MASK     = 0x100U,     /* Temperature Warning */
    eGDUMASTER_TCF_OT_MASK     = 0x200U,     /* Over Temperature */
    eGDUMASTER_TCF_IG_MASK     = 0x400U,     /* Ignition Input State */
    eGDUMASTER_TCF_DSO_MASK    = 0x800U,     /* VDS over voltage */
    eGDUMASTER_TCF_EE_MASK     = 0x1000U,    /* NVM Error */
    eGDUMASTER_TCF_SE_MASK     = 0x2000U,    /* Serial Error */
    eGDUMASTER_TCF_POR_MASK    = 0x4000U,    /* Power-on reset */
    eGDUMASTER_TCF_FF_MASK     = 0x8000U     /* General Fault Flag */
};

/**
 * @brief The maximum number of Read commands to be stored in the Rx FIFO after the automatically receive.
 */
static uint16 const ksu16MaxNumRxPacketSize = 100U;

/**
 * @brief The maximum number of Write/Read commands to be stored in the Tx FIFO before automatically be sent.
 */
static uint16 const ksu16MaxNumTxPacketSize = 100U;
	
/**
 * @brief  The Tx and Rx packet .
 */
static I2cPacket_t astTxPacketFifo[ksu16MaxNumTxPacketSize] = { 0 };
static I2cPacket_t astRxPacketFifo[ksu16MaxNumRxPacketSize] = { 0 };

static I2cPacket_t sstPacket = { 0 };

/**
 * @brief  The Tx and Rx buffers indices.
 */
static volatile uint8 su8TxIndex = 0;
static volatile uint8 su8RxIndex = 0;
static volatile uint8 su8TxStartIndex = 0;
static volatile uint8 su8RxStartIndex = 0;

/**
 * @brief  Packet counters.
 */
static volatile uint8 su8TxNumPacketsPending = 0;
static volatile uint8 su8RxNumPacketsPending = 0;

/** 
 * @brief The Tx buffer overflow flag.
 */
static volatile boolean sbTxOverflow = False;

/**
 * @brief The Rx buffer overflow flag.
 */
static volatile boolean sbRxOverflow = False;

/**
 * @brief The Irq handler method for the transmission of packet data.
 * @param[in] kuPrData The Irq source.
 */
static void GduMaster_I2cTxIrqHandler( void* pContext )
{
    (void)pContext; // Polyspace requirement for unused variable
    // Not necessary to check the busy flag.
    // Make sure there are no packets pending transmission
    if ( !(su8TxNumPacketsPending == 0) )
    {
        sstPacket = astTxPacketFifo[su8TxStartIndex];

        if ( sstPacket.bWrite == True )
        {
            I2c_Write( sstPacket.u8Address, sstPacket.u16Data );
            // Packet transmitted, cylic shift the Tx FIFO on.
            su8TxStartIndex = (uint8)(( su8TxStartIndex + 1 ) % (uint8)ksu16MaxNumTxPacketSize);
            // Decrement the pending packet count.
            su8TxNumPacketsPending--;
        }
        else
        {
            I2c_NonBlockingRead( sstPacket.u8Address );
        }
    }
}


/**
 * @brief The Irq handler method for the reception of packet data.
 * @param[in] kuPrData The Irq source.
 */
static void GduMaster_I2cRxIrqHandler( void* pContext )
{
    // Store the address in the packet.
    sstPacket.u8Address = astTxPacketFifo[su8TxStartIndex].u8Address;
    // cylic shift the Tx FIFO on.
    su8TxStartIndex = (uint8)(( su8TxStartIndex + 1 ) % (uint8)ksu16MaxNumTxPacketSize);
    // Decrement the pending packet count.
    if(su8TxNumPacketsPending > 0)
    {
        su8TxNumPacketsPending--;
    }
    else
    {
        sbTxOverflow = True;
    }
  
    // Store the received data in the packet
    sstPacket.u16Data = I2c_GetRxValue();

    // Space available for a packet
    if ( su8RxNumPacketsPending != ksu16MaxNumRxPacketSize )
    {
        // Store the read packet value in the array.
        astRxPacketFifo[su8RxIndex] = sstPacket;
        // Cyclic shift the index
        su8RxIndex = (uint8)(( su8RxIndex + 1 ) % (uint8)ksu16MaxNumRxPacketSize);

        // Bump the number of pending packets.
        su8RxNumPacketsPending++;
    }
    else
    {
        sbRxOverflow = True;
    }
    // Process any pending transmission need
    GduMaster_I2cTxIrqHandler(pContext);
}


/**
 * @internal
 * @details Set the I2c clock frequency and then turn on the I2C module in the specified mode.
 */
void GduMaster_Enable( enum I2cMode const keI2cMode, enum I2cClock const keI2cClock )
{
    I2c_SetClock( keI2cClock );
    I2c_SetMode( keI2cMode );
}

/**
 * @internal
 * @details Set the I2c module off.
 */
void GduMaster_Disable(void)
{
    I2c_SetMode( eI2C_MODE_OFF );
}

/**
 * @internal
 * @details Set the I2c mode.
 */
void GduMaster_SetI2cMode( enum I2cMode const keI2cMode )
{
    // Set the target mode.
    I2c_SetMode( keI2cMode );
}

/**
 * @internal
 * @details Set the I2c clock frequency.
 */
void GduMaster_SetI2cClock( enum I2cClock const keI2cClock  )
{
    // Set the target clock rate.
    I2c_SetClock( keI2cClock );
}

/**
 * @internal
 * @details Set the GDU Master module in an initial state.
 */
void GduMaster_Initialize(void)
{
    NVIC_DisableIRQ( skIrqGdu );
    NVIC_ClearPendingIRQ( skIrqGdu );
  
      // Disable the IG Irq.
    GduMaster_SetIgIrqEnable( False );

    // Disable all the I2C Irqs.
    GduMaster_DisableIrqs();

    // Clear the user assigned IG Irq handler
    spfnGduIrqHandler = NULLPTR;
    
    // Clear the user assigned I2c fault handler
    spfnI2cFaultIrqHandler = NULLPTR;
  
    // Reset the internally assigned IRQ handlers.
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_TX, GduMaster_I2cTxIrqHandler );
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_RX, GduMaster_I2cRxIrqHandler );
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_ACK, spfnI2cFaultIrqHandler );
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_CRC, spfnI2cFaultIrqHandler );
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_OVF, spfnI2cFaultIrqHandler );
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_FF, spfnI2cFaultIrqHandler );
		
    // Clear FIFOs
    su8TxIndex = 0;
    su8RxIndex = 0;
    su8TxStartIndex = 0;
    su8RxStartIndex = 0;
    su8TxNumPacketsPending = 0;
    su8RxNumPacketsPending = 0;
    sbTxOverflow = False;
    sbRxOverflow = False;

    // Clear IG status flag.
    GduMaster_ClearIg();

    // Enable the I2c 
    I2c_Initialize();
     
    // Enable the I2c interrupts.
    GduMaster_EnableIrqs();
    
    // Enable MCU GDU interrupt
    NVIC_EnableIRQ( skIrqGdu );
    
}


/**
 * @internal
 * @details Enable the Tx, Rx, Ack, Crc, Ovf and FF Irqs.
 */
void GduMaster_EnableIrqs(void)
{
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_RX, True );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_TX, True );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_ACK, True );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_CRC, True );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_OVF, True );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_FF, True );
}

/**
 * @internal
 * @details Disable the Tx, Rx, Ack, Crc, Ovf and FF Irqs.
 */
void GduMaster_DisableIrqs(void)
{
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_RX, False );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_TX, False );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_ACK, False );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_CRC, False );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_OVF, False );
    I2c_SetIrqEnable( eI2C_IRQ_SOURCE_FF, False );
}

/**
 * @internal
 * @details Block until the I2C is not busy and write the register. After the write block again
 * until the I2C is not busy and the transmission is finished.
 */
void GduMaster_Write( enum GduSlaveRegister const keSlaveAddress, uint16 const ku16Data )
{
    // Read Tx interrupt enable and then disable it.
    const boolean bTxIrqEnable = I2c_GetTxIrqEnableFast();
    I2c_SetTxIrqEnableFast( False );

    // Block until the busy flag is off.
    while ( I2c_GetBusyFlagFast() == True )
    {
    }

    // Write data to GDU via I2C
    I2c_Write( (uint8)keSlaveAddress, ku16Data );


    // Block until the busy flag is off.
    while ( I2c_GetBusyFlagFast() == True )
    {
    }

    // After transmit done: clear Tx flag
    I2c_ResetTxFlagFast();

    // If Tx interrupt was enabled, re-enable it.
    if ( bTxIrqEnable == True )
    {
        I2c_SetTxIrqEnableFast( True );
    }
}

/**
 * @internal
 * @details Non-Blocking write.
 */
void GduMaster_NonBlockingWrite( enum GduSlaveRegister const keSlaveAddress, uint16 const ku16Data )
{
    // Beginning of critical section
    // critical section: 
    // if an I2C Irq happens.
    // Disable the I2C Irq
    NVIC_DisableIRQ( ARM_IRQ_NUM( eIRQNUM_I2C ) );

    // If the I2C is not busy and the Tx fifo is empty write direct over the I2C
    // without placing it in the Tx fifo.
    if ( ( I2c_GetBusyFlag() == False ) && (su8TxNumPacketsPending == 0) )
    {
        I2c_Write( (uint8)keSlaveAddress, ku16Data );
    }
    else // Check if the fifo is not empty or if the I2C is not busy
    {
        // Create the packet. It is a write operation so the first member of the struct has to be True.
        const I2cPacket_t kstPacket = {True, (uint8)keSlaveAddress, ku16Data};
        // Check if the Tx Packet buffer is full and append the next read command.
        if(su8TxIndex < ksu16MaxNumTxPacketSize)
        {
            astTxPacketFifo[su8TxIndex] = kstPacket;
            su8TxNumPacketsPending++;
            su8TxIndex = (uint8)((su8TxIndex + 1) % (uint8)ksu16MaxNumTxPacketSize);
        }
        else
        {
            sbTxOverflow = True;
        }
    }

    // End of critical section
    // Enable the I2C Irq
    NVIC_EnableIRQ( ARM_IRQ_NUM( eIRQNUM_I2C ) );

}

/**
 * @internal
 * @details Block until the I2C is not busy and read the register.
 */
uint16 GduMaster_Read( enum GduSlaveRegister const keSlaveAddress )
{
    // Read state of Rx interrupt enable and then disable it.
    const boolean bRxIrqEnable = I2c_GetRxIrqEnableFast();
    I2c_SetRxIrqEnableFast( False );

    // Block until the busy flag is off.
    while ( I2c_GetBusyFlagFast() == True )
    {
    }

    // Start an I2C read transfer.
    uint16 const ku16Rx = I2c_Read( (uint8)keSlaveAddress );

    // After receive done: clear Rx flag
    I2c_ResetRxFlagFast();

    // If Rx interrupt was enabled, re-enable it.
    if ( bRxIrqEnable == True )
    {
        I2c_SetRxIrqEnableFast( True );
    }

    return ku16Rx;
}

/**
 * @internal
 * @details Non-Blocking read.
 */
void GduMaster_NonBlockingRead( enum GduSlaveRegister const keSlaveAddress )
{
    // Beginning of critical section
    // critical section
    // if an I2C Irq happens.
    // Disable the I2C Irq
    NVIC_DisableIRQ( ARM_IRQ_NUM( eIRQNUM_I2C ) );

    // It means that there is not a user pending transfer so perform direct the trasmission.
    // There is a case where the transmission could be delayed because of the automatic transmission (if enabled).
    // But as soon as the auto-transmission finish the user request will be serviced directly by the hardware.
    if ( ( I2c_GetBusyFlag() == False ) && (su8TxNumPacketsPending == 0) )
    {
        // Perform the non blocking read
        I2c_NonBlockingRead( (uint8)keSlaveAddress);
    }

    // Create the packet to push in the Tx fifo. It is a read operation
    // so the first member of the struct has to be False.
    const I2cPacket_t kstPacket = {False, (uint8)keSlaveAddress, 0x0000};
    // Check for Tx overflow and
    // Push into the queue the next read command.
    if(su8TxIndex < ksu16MaxNumTxPacketSize)
    {
        astTxPacketFifo[su8TxIndex] = kstPacket;
        su8TxNumPacketsPending++;
        su8TxIndex = (uint8)((su8TxIndex + 1) % (uint8)ksu16MaxNumTxPacketSize);
    }
    else
    {
        sbTxOverflow = True;
    }
    // End of critical section
    // Enable the I2C Irq
    NVIC_EnableIRQ( ARM_IRQ_NUM( eIRQNUM_I2C ) );
}

/**
 * @brief Returns the number of pending packets to be sent (I2C write or read commands).
 * @returns uint8 The number of pending packets to transmit.
 */
uint8 GduMaster_GetNumberOfPendingPackets(void)
{
    return su8TxNumPacketsPending;
}

/**
 * @brief Returns the number of received packets (I2C read commands).
 * @returns uint8 The number of received packets.
 */
uint8 GduMaster_GetNumberOfReceivedPackets(void)
{
    return su8RxNumPacketsPending;
}

/**
 * @brief Returns first packet that was received and removes it.
 * @returns Packet The first received packet.
 */
I2cPacket_t GduMaster_GetReceivedPacket(void)
{
    I2cPacket_t stPacket = {0};
    if ( su8RxNumPacketsPending > 0 )
    {
        stPacket = astRxPacketFifo[su8RxStartIndex];
        su8RxStartIndex = (uint8)(( su8RxStartIndex + 1 ) % (uint8)ksu16MaxNumRxPacketSize);

        su8RxNumPacketsPending--;
    }
    else
    {
        sbRxOverflow = True;
    }
    return stPacket;
}

/**
 * @brief Returns the Tx overflow of the queue.
 * @returns boolean True for an attempt to overflow the queue, False any other case.
 */
boolean GduMaster_IsTxOverflow(void)
{
    return sbTxOverflow;
}

/**
 * @brief Clears the Tx Overflow flag.
 * @details Set the Tx Overflow flag to False.
 */
void GduMaster_ClearTxOverflow(void)
{
    sbTxOverflow = False;
}

/**
 * @brief Returns the Rx overflow of the queue.
 * @returns boolean True for an attempt to overflow the queue, False any other case.
 */
boolean GduMaster_IsRxOverflow(void)
{
    return sbRxOverflow;
}

/**
 * @brief Clears the Rx Overflow flag.
 * @details Set the Rx Overflow flag to False.
 */
void GduMaster_ClearRxOverflow(void)
{
    sbRxOverflow = False;
}

/**
 * @internal
 * @details The Ignition detected interrupt flag (IG) is in bit 0 of the status register.
 * Returns True if the IG is set else False.
 */
boolean GduMaster_GetIg(void)
{
    return (boolean)(stGduRegs.IgIntStat & su32GduMasterIrqIgMask);
}

/**
 * @internal
 * @details The I2c fault detected interrupt flag corresponds to ACK, OVF, CRC, Fault from the status register.
 * Returns True if any of these flags are set else False.
 */
boolean GduMaster_GetFaults(void)
{
    return (boolean)((stGduRegs.IgIntStat & su32GduMasterFaultMask) > 0);
}

/**
 * @internal
 * @details The Ig interrupt status flag (IG) is in bit 0 of the igintstatus register clear it out by writing 1.
 */
void GduMaster_ClearIg(void)
{
    stGduRegs.IgIntStat |= su32GduMasterIrqIgMask;
}


/**
 * @internal
 * @details The fault interrupt status flags corresponds to ACK, OVF, CRC, Fault and can clear upon by writing 1.
 */
void GduMaster_ClearFaults(void)
{
    stGduRegs.IgIntStat |= su32GduMasterFaultMask;
}


/**
 * @internal
 * @details The GDU VLR undervoltage flag (VLRU) is in bit 0 of the status register.
 * Returns True if the VLRU is set else False.
 */
boolean GduMaster_GetVlruFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_VLRU_MASK) > 0);
}

/**
 * @internal
 * @details The GDU VREG undervoltage flag (VRU) is in bit 1 of the status register.
 * Returns True if the VRU is set else False.
 */
boolean GduMaster_GetVruFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_VRU_MASK) > 0);
}

/**
 * @internal
 * @details The GDU VBB undervoltage flag (VBU) is in bit 2 of the status register.
 * Returns True if the VBU is set else False.
 */
boolean GduMaster_GetVbuFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_VBU_MASK) > 0);
}

/**
 * @internal
 * @details The GDU VCP undervoltage flag (VCPU) is in bit 3 of the status register.
 * Returns True if the VCPU is set else False.
 */
boolean GduMaster_GetVcpuFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_VCPU_MASK) > 0);
}

/**
 * @internal
 * @details The GDU sensor BIAS undervoltage flag (SBOU) is in bit 4 of the status register.
 * Returns True if the SBOU is set else False.
 */
boolean GduMaster_GetSbouFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_SBOU_MASK) > 0);
}

/**
 * @internal
 * @details The GDU enable watchdog timeout flag (ETO) is in bit 5 of the status register.
 * Returns True if the ETO is set else False.
 */
boolean GduMaster_GetEtoFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_ETO_MASK) > 0);
}

/**
 * @internal
 * @details The GDU watchdog fault flag (WD) is in bit 6 of the status register.
 * Returns True if the WD is set else False.
 */
boolean GduMaster_GetWdFault(void)
{
    return(boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_WD_MASK) > 0);
}

/**
 * @internal
 * @details The GDU bootstrap undervoltage flag (BU) is in bit 7 of the status register.
 * Returns True if the BU is set else False.
 */
boolean GduMaster_GetBuFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_BU_MASK) > 0);
}

/**
 * @internal
 * @details The GDU temperature warning flag (TW) is in bit 8 of the status register.
 * Returns True if the TW is set else False.
 */
boolean GduMaster_GetTwFault(void)
{
    return (boolean)(((stGduRegs.Tcf) & (uint32)eGDUMASTER_TCF_TW_MASK) > 0);
}

/**
 * @internal
 * @details The GDU over temperature flag (OT) is in bit 9 of the status register.
 * Returns True if the OT is set else False.
 */
boolean GduMaster_GetOtFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_OT_MASK) > 0);
}


/**
 * @internal
 * @details The GDU ignition input state flag (IG) is in bit 10 of the status register.
 * Returns True if the IG is set else False.
 */
boolean GduMaster_GetIgState(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_IG_MASK) > 0);
}

/**
 * @internal
 * @details The GDU VDS over voltage flag (DSO) is in bit 11 of the status register.
 * Returns True if the OC is set else False.
 */
boolean GduMaster_GetDsoFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_DSO_MASK) > 0);
}

/**
 * @internal
 * @details The GDU EEPROM error flag (EE) is in bit 12 of the status register.
 * Returns True if the EE is set else False.
 */
boolean GduMaster_GetEeFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_EE_MASK) > 0);
}

/**
 * @internal
 * @details The GDU serial error flag (SE) is in bit 13 of the status register.
 * Returns True if the SE is set else False.
 */
boolean GduMaster_GetSeFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_SE_MASK) > 0);
}

/**
 * @internal
 * @details The GDU power on reset flag (POR) is in bit 14 of the status register.
 * Returns True if the POR is set else False.
 */
boolean GduMaster_GetPorFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_POR_MASK) > 0);
}

/**
 * @internal
 * @details The GDU fault flag (FF) is in bit 15 of the status register.
 * Returns True if the FF is set else False.
 */
boolean GduMaster_GetFfFault(void)
{
    return (boolean)((stGduRegs.Tcf & (uint32)eGDUMASTER_TCF_FF_MASK) > 0);
}

/**
 * @internal
 * @details Get the TCF register value in order to avoid the individual calls for the TCF bits.
 * Returns the whole register.
 */
uint16 GduMaster_GetTcf(void)
{
    return (uint16)stGduRegs.Tcf;
}


/**
 * @internal
 * @details This IRQ service routine calls an interrupt specific handler
 * if one has been registered for Gdu master.
 */
static void GduMaster_ServiceIrq(void)
{

    // check the Status register if the interrupt is Set
    if (GduMaster_GetIg() == True)
    {
        // check the Control register if the interrupt is enabled
        if(GduMaster_IsIgIrqEnabled() == True)
        {
            // check if there is an assigned interrupt handler
            if ( spfnGduIrqHandler != NULLPTR )
            {
                // call user's interrupt handler
                spfnGduIrqHandler( (void*)&suPrIgnitionData );
            }
            // clear the Ig status bit.
            GduMaster_ClearIg();
        }
    }
}

/**
 * @internal
 * @details Set the GDU master IRQ handler and context data.
 */
void GduMaster_SetIgIrqHandler( void (pfnIrqHandler)(void* pContext), uintptr uPrData )
{
    spfnGduIrqHandler = *pfnIrqHandler;
    suPrIgnitionData = uPrData;
}

/**
 * @internal
 * @details Enables an IRQ
 */
void GduMaster_SetIgIrqEnable( boolean const kbEnable )
{
    NVIC_DisableIRQ( skIrqGdu );
    NVIC_ClearPendingIRQ( skIrqGdu );
    if(kbEnable == True)
    {
        stGduRegs.IgIntEn |= su32GduMasterIrqIgMask;
    }
    else
    {
        stGduRegs.IgIntEn &= ~su32GduMasterIrqIgMask;
    }
    NVIC_EnableIRQ( skIrqGdu );
}

/**
 * @internal
 * @details Gets whether an IRQ is enabled.
 * This implementation performs uninitialised error checking as it is not needed
 * for time critical code.
 */
boolean GduMaster_IsIgIrqEnabled(void)
{
    return (boolean)(stGduRegs.IgIntEn & su32GduMasterIrqIgMask);
}

/**
 * @internal
 * @details Set the I2c fault IRQ handler.
 */
void GduMaster_SetFaultIrqHandler( void (pfnIrqHandler)(void* pContext) )
{
    spfnI2cFaultIrqHandler = *pfnIrqHandler;
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_ACK, spfnI2cFaultIrqHandler );
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_CRC, spfnI2cFaultIrqHandler );
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_OVF, spfnI2cFaultIrqHandler );
    (void)I2c_SetIrqHandler( eI2C_IRQ_SOURCE_FF, spfnI2cFaultIrqHandler );
		
}

/**
 * @internal
 * @details Checks if an IRQ handler has been set for fault handling.
 */
boolean GduMaster_IsFaultIrqRegistered(void)
{
    return (boolean)(spfnI2cFaultIrqHandler != NULLPTR);
}

/**
 * @internal
 * @details This GDU_INTERRUPT handler is registered in the vector table.
 */
__attribute__( ( interrupt( "IRQ" ) ) ) void GduMaster_IrqHandler(void)
{
    NVIC_DisableIRQ( skIrqGdu );
    NVIC_ClearPendingIRQ( skIrqGdu );

    GduMaster_ServiceIrq();

    NVIC_EnableIRQ( skIrqGdu );
}