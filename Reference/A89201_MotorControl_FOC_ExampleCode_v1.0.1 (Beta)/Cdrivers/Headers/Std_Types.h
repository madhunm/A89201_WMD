/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         Std_Types.h
 * @author       O.Luna
 * @version      1.0
 *
 * @brief        Standard Types.
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

#ifndef __STD_TYPES_H__
#define __STD_TYPES_H__

#include "Platform_Types.h"


#ifndef NULLPTR
  /* Define a Null Pointer Macro */
  #define NULLPTR  (0)

#endif // !NULLPTR

/*------------------------------- Type Definitions ----------------------------------*/
typedef uint8 Std_ReturnType;

/*------------------------------- Structure Definitions -----------------------------*/
/** Standard SW Version Info Structure */
typedef struct __attribute__((packed))
{
  uint16 u16vendorID;              /* Vendor Identifier             */
  uint16 u16moduleID;              /* Module Identifier             */

  uint8 u8sw_major_version;        /* Major Software Version Number */
  uint8 u8sw_minor_version;        /* Minor Software Version Number */
  uint8 u8sw_patch_version;        /* Patch Software Version Number */
  
} Std_VersionInfoType;

/*------------------------------- Macro Definitions ---------------------------------*/
/** Create Std_VersionInfoType */
#define STD_GET_VERSION_INFO(_versionInfo,_module) \
    ((_versionInfo)->u16vendorID =  _module ## _VENDOR_ID);\
    ((_versionInfo)->u16moduleID =  _module ## _MODULE_ID);\
    ((_versionInfo)->u8sw_major_version =  _module ## _SW_MAJOR_VERSION);\
    ((_versionInfo)->u8sw_minor_version =  _module ## _SW_MINOR_VERSION);\
    ((_versionInfo)->u8sw_patch_version =  _module ## _SW_PATCH_VERSION);\


/** Standard Types for Error Messages         */
#define E_OK 					(Std_ReturnType)0x00U
#define E_NOT_OK 			(Std_ReturnType)0x01U

/** Standard Types for High and Low           */
#define STD_HIGH		  (Std_ReturnType)0x01U
#define STD_LOW			  (Std_ReturnType)0x00U

/** Standard Types for Active and Idle States */
#define STD_ACTIVE		(Std_ReturnType)0x01U
#define STD_IDLE		  (Std_ReturnType)0x00U

/** Standard Types for On and Off States     */
#define STD_ON			  (Std_ReturnType)0x01U
#define STD_OFF			  (Std_ReturnType)0x00U

#define ku8Invalid  (0xFFU)
#define ku16Invalid (0xFFFFU)
#define ku32Invalid (0xFFFFFFFFU)
#define ku64Invalid (0xFFFFFFFFFFFFFFFFU)

#endif /* Std_types end */
