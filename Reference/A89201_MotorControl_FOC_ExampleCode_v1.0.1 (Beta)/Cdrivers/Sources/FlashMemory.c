/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/*
 * @file         FlashMemory.c
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

#include "ARMCM4_FP.h"

#include "Std_Types.h"
#include "FlashMemory.h"

/* polyspace-begin MISRA-C3:11.1 [Justified:Unset] "Function pointer conversion necessary." */
/* polyspace-begin MISRA-C3:11.6 [Justified:Unset] "Function pointer casting necessary." */

// Function pointers for the 3 operations: Erase Page, Write and Read.
static const boolean (*fpBootRomFunctionEraseFlashPage)( uint8 u8Page, uint8 u8Mode ) = (void *)( ERASE_FLASH_PAGE_BOOTROM_FUNCTION_ADDRESS );  
static const boolean (*fpBootRomFunctionFlashWrite)( uint32 u32Address, uint8* pu8Buffer, uint16 u16Length, uint8 u8Mode ) = (void *)( WRITE_FLASH_BOOTROM_FUNCTION_ADDRESS );
static const void (*fpBootRomFunctionFlashRead)( uint32 u32Address, uint8* pu8Buffer, uint16 u16Length, uint8 u8Mode ) =  (void *)( READ_FLASH_BOOTROM_FUNCTION_ADDRESS );

/* polyspace-end MISRA-C3:11.1 [Justified:Unset] "Function pointer conversion necessary." */
/* polyspace-end MISRA-C3:11.6 [Justified:Unset] "Function pointer casting necessary." */

boolean FlashMemory_ErasePage( const uint8 ku8Page )
{
    boolean bRetVal = False;
    
    // If Page number is OK
    if( ku8Page <= FLASH_PAGE_MAX )
    {    
        // Then call BootROM function to erase this page Flash.
        bRetVal = fpBootRomFunctionEraseFlashPage( ku8Page, FLASH_PAGE_MODE );
    }
    
    return bRetVal;
}

boolean FlashMemory_Write( const uint32 ku32Address, uint8* kpu8WriteData, const uint16 ku16DataLength )
{
    boolean bRetVal = False;
    
    // If Start Address and Length are OK
    if( ( ku32Address >= (uint32)(eADDRESS_FLASHMIN) ) && ( ku32Address <= (uint32)(eADDRESS_FLASHMAX) ) && ( ku16DataLength <= FLASH_PAGE_SIZE ) )
    {
        // Then call BootROM function to write this part of Flash.
        bRetVal = fpBootRomFunctionFlashWrite( ku32Address, kpu8WriteData, ku16DataLength, FLASH_PAGE_MODE );
    }
        
    return bRetVal;
}

boolean FlashMemory_Read ( const uint32 ku32Address, uint8* kpu8ReadData, const uint16 ku16DataLength )
{
    boolean bRetVal = False;
    
    // If Start Address and Length are OK
    if( ( ku32Address >= (uint32)(eADDRESS_FLASHMIN) ) && ( ku32Address <= (uint32)(eADDRESS_FLASHMAX) ) && ( ku16DataLength <= FLASH_PAGE_SIZE ) )
    {
        // Then call BootROM function to read this part of Flash.
        fpBootRomFunctionFlashRead( ku32Address, kpu8ReadData, ku16DataLength, FLASH_PAGE_MODE );
        bRetVal = True;
    }
    
    return bRetVal;
}

