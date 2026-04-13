#ifndef HEADER_FLASH_H
#define HEADER_FLASH_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         FlashMemory.h
 * @author       M. Shankey
 * @version      1.0
 *
 * @brief        Flash Memory driver
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

#define FLASH_PAGE_MODE 0U
#define FLASH_PAGE_SIZE 0x1000U
#define FLASH_PAGE_MAX  63U

/**
 * Note: the Flash memory functions call BootROM functions to Read, Write, and Erase Flash. 
 * These BootROM functions cannot reside in Flash, as they operate on Flash.
 * The code address values are found in the map file of the BootROM: Product 00.
 * Search for these symbols:-
 * NsAllegroMicro::NsMemory::AccessorAllegroRom::FlashErasePage
 * NsAllegroMicro::NsMemory::AccessorAllegroRom::Write
 * NsAllegroMicro::NsMemory::AccessorAllegroRom::Read
 * The code resides in section "AccessorRom" which has been placed
 * deliberately at the end of the ROM.
 */
#define ERASE_FLASH_PAGE_BOOTROM_FUNCTION_ADDRESS 0x00007801U
#define WRITE_FLASH_BOOTROM_FUNCTION_ADDRESS      0x00007893U
#define READ_FLASH_BOOTROM_FUNCTION_ADDRESS       0x0000794FU

/**
 * @brief Erase the given page.
 * @param[in] ku8Page A flash page
 * @returns bool True if successful else false
 */
boolean FlashMemory_ErasePage( const uint8 ku8Page );

/**
 * @brief Write data to the given address.
 * @param[in] ku32Address The start address
 * @param[in] kpu8WriteData A pointer to the first location of the source data
 * @param[in] ku16DataLength The length of the source data to write
 * @returns bool True if successful else false
 */
boolean FlashMemory_Write( const uint32 ku32Address, uint8* kpu8WriteData, const uint16 ku16DataLength );

/**
 * @brief Read data from the given address.
 * @param[in] ku32Address The start address to read from
 * @param[out] kpu8ReadData A location receiving the data read
 * @param[in] ku16DataLength The length of the source data to read
 * @returns bool True if successful else false
 */
boolean FlashMemory_Read ( const uint32 ku32Address, uint8* kpu8ReadData, const uint16 ku16DataLength );

#endif //HEADER_FLASH_H