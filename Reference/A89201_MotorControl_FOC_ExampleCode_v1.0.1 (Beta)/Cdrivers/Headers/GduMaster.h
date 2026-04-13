#ifndef HEADER_GDU_MASTER_H
#define HEADER_GDU_MASTER_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         GduMaster.h
 * @author       G. Sloan
 * @version      1.0
 *
 * @brief        The implementation of the GDU master data for the 02 product.
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
#include "GduSlave.h"
#include "I2c.h"

// GDU Registers represented in a struct
typedef struct __attribute__((packed)) 
{
    uint32 Tcf;         // offset:  0x18U
    uint32 IgIntEn;     // offset:  0x1CU
    uint32 IgIntStat;   // offset:  0x20U
} GduRegs_t;

#define stGduRegs (*( volatile GduRegs_t* )eADDRESS_GDU_BASE)

/**
 * @brief A struct representing the send/received packets.
 */
typedef struct 
{
    /** The write/read field for write set to 1 for read set to 0.
     *  It is used only for sending purpose and not for receiving. */
    boolean bWrite;
    /** The 7-bit address to write/read */
    uint8 u8Address;
    /** The 16-bit data to write/read */
    uint16 u16Data;
} I2cPacket_t;

/**
 * @brief (Re)Initialise the GDU master, which clears FIFOs and initialises the I2C and enables interrupts.
 */
void GduMaster_Initialize(void);
	
/**
 * @brief Enable the GDU master module in the specified mode and frequency.
 * @param[in] keI2cMode The mode to start.
 * @param[in] keI2cClock The frequency to write/read the registers.
 */
void GduMaster_Enable( enum I2cMode const keI2cMode, enum I2cClock const keI2cClock );

/**
 * @brief Disable the GDU master module.
 */
void GduMaster_Disable(void);

/**
 * @brief Set the I2c mode.
 * @param[in] keI2cMode The target mode.
 */
void GduMaster_SetI2cMode( enum I2cMode const keI2cMode );

/**
 * @brief Set the I2c clock frequency.
 * @param[in] keI2cMode The target clock rate.
 */
void GduMaster_SetI2cClock( enum I2cClock const keI2cClock  );

/**
 * @brief Enable the Tx, Rx, Ack, Crc, Ovf Irqs.
 */
void GduMaster_EnableIrqs(void);

/**
 * @brief Disable the Tx, Rx, Ack, Crc, Ovf Irqs.
 */
void GduMaster_DisableIrqs(void);

/**
 * @brief Write one of the GDU master registers, block until the transmission is finished.
 * @param[in] keSlaveAddress The register to write.
 * @param[in] ku16Data The data to write in the register.
 */
void GduMaster_Write( enum GduSlaveRegister const keSlaveAddress, uint16 const ku16Data );
 

/**
 * @brief Write one of the GDU master registers without blocking until the transmission is finished.
 * @param[in] keSlaveAddress The register to write.
 * @param[in] ku16Data The data to write in the register.
 */
void GduMaster_NonBlockingWrite( enum GduSlaveRegister const keSlaveAddress, uint16 const ku16Data );

/**
 * @brief Read the GDU master register, blocks until it reads the data back.
 * @param[in] keSlaveAddress The register address to read.
 * @returns The read data of the above specified address.
 */
uint16 GduMaster_Read( enum GduSlaveRegister const keSlaveAddress );
 
/**
 * @brief Read the GDU master register, without blocking, it executes the read command and it is user's
 * responsibility to get the data from the received queue (RX queue).
 * @param[in] keSlaveAddress The register address to read.
 */
void GduMaster_NonBlockingRead( enum GduSlaveRegister const keSlaveAddress );
 

/**
 * @brief Returns the number of pending packets to be send(I2C write or read commands).
 * @returns U8 The number of pending packets to transmit.
 */
uint8 GduMaster_GetNumberOfPendingPackets(void);

/**
 * @brief Returns the number of received packets (I2C read commands).
 * @returns U8 The number of received packets.
 */
uint8 GduMaster_GetNumberOfReceivedPackets(void);

/**
 * @brief Returns first packet that was received and removes it.
 * @returns Packet The first received packet.
 */
I2cPacket_t GduMaster_GetReceivedPacket(void);

/**
 * @brief Returns the Tx overflow of the queue.
 * @returns boolean True for an attempt to overflow the queue, false any other case.
 */
boolean GduMaster_IsTxOverflow(void);

/**
 * @brief Clears the Tx Overflow flag.
 */
void GduMaster_ClearTxOverflow(void);

/**
 * @brief Returns the Rx overflow of the queue.
 * @returns boolean True for an attempt to overflow the queue, false any other case.
 */
boolean GduMaster_IsRxOverflow(void);

/**
 * @brief Clears the Rx Overflow flag.
 */
void GduMaster_ClearRxOverflow(void);

/**
 * @brief Return the IG status bit.
 * @returns True if it is set else false.
 */
boolean GduMaster_GetIg(void);

/**
 * @brief Clear to 0 the IG status flag.
 */
void GduMaster_ClearIg(void);

/**
 * @brief Clear to 0 the fault status flags.
 */
void GduMaster_ClearFaults(void);

/**
 * @brief Get whether an I2c fault detected from ACK, OVF, CRC, Fault fields from the status register.
 * @returns True if any are set else false.
 */
boolean GduMaster_GetFaults(void);

/**
 * @brief Return the VLR undervoltage flag: VLRU
 * @returns True if it is set else false.
 */
boolean GduMaster_GetVlruFault(void);

/**
 * @brief Return the VREG undervoltage flag: VRU
 * @returns True if it is set else false.
 */
boolean GduMaster_GetVruFault(void);

/**
 * @brief Return the VBB undervoltage flag: VBU
 * @returns True if it is set else false.
 */
boolean GduMaster_GetVbuFault(void);

/**
 * @brief Return the VCP undervoltage flag: VCPU
 * @returns True if it is set else false.
 */
boolean GduMaster_GetVcpuFault(void);

/**
 * @brief Return the sensor BIAS undervoltage flag: SBOU
 * @returns True if it is set else false.
 */
boolean GduMaster_GetSbouFault(void);

/**
 * @brief Return the ETO timeout fault flag: ETO
 * @returns True if it is set else false.
 */
boolean GduMaster_GetEtoFault(void);

/**
 * @brief Return the Wd watchdog fault flag: WD
 * @returns True if it is set else false.
 */
boolean GduMaster_GetWdFault(void);

/**
 * @brief Return the bootstrap undervoltage fault flag: BU
 * @returns True if it is set else false.
 */
boolean GduMaster_GetBuFault(void);

/**
 * @brief Return the temperature warning flag: TW
 * @returns True if it is set else false.
 */
boolean GduMaster_GetTwFault(void);

/**
 * @brief Return the over temperature flag: OT
 * @returns True if it is set else false.
 */
boolean GduMaster_GetOtFault(void);

/**
 * @brief Return the IG ignition flag: IG
 * @returns True if it is set else false.
 */
boolean GduMaster_GetIgState(void);

/**
 * @brief Return the VDS over voltage flag: DSO
 * @returns True if it is set else false.
 */
boolean GduMaster_GetDsoFault(void);

/**
 * @brief Return the EE EEPROM fault flag.
 * @returns True if it is set else false.
 */
boolean GduMaster_GetEeFault(void);

/**
 * @brief Return the SE serial error fault flag.
 * @returns True if it is set else false.
 */
boolean GduMaster_GetSeFault(void);

/**
 * @brief Return the POR power on reset flag
 * @returns True if it is set else false.
 */
boolean GduMaster_GetPorFault(void);

/**
 * @brief Return the FF general fault flag.
 * @returns True if it is set else false.
 */
boolean GduMaster_GetFfFault(void);

/**
 * @brief Return the TCF register value.
 * @returns Return the TCF register value.
 */
uint16 GduMaster_GetTcf(void);

/**
 * @brief Set the IRQ to be enabled or disabled
 * @param[in] kbEnable True to be enabled else false.
 */
void GduMaster_SetIgIrqEnable( boolean const kbEnable );

/**
 * @brief Set the handler for the IG IRQ
 * @details Does nothing if pointer is not initialized
 * @param[in] pfnIrqHandler A function pointer to handle the GDU master IRQ
 * @param[in] uintptr Handler context data 
 */
void GduMaster_SetIgIrqHandler( void (pfnIrqHandler)(void* pContext), uintptr uPrData );

/**
 * @brief Gets if the IG IRQ is enabled or disabled
 * @returns True if enabled else false.
 */
boolean GduMaster_IsIgIrqEnabled(void);

/**
 * @brief Set the handler for the Fault IRQ
 * @details Does nothing if pointer is not initialized
 * @param[in] pfnIrqHandler A function pointer to handle the Fault IRQ
 */
void GduMaster_SetFaultIrqHandler( void (pfnIrqHandler)(void* pContext) );

/**
 * @brief Gets if the Fault IRQs is registered.
 * @returns True if enabled else false.
 */
boolean GduMaster_IsFaultIrqRegistered(void);

/**
 * @brief    The GduMaster IRQ handler
 * @returns    None
 */
void GduMaster_IrqHandler(void);

#endif //HEADER_GDU_MASTER_H
