#ifndef HEADER_DRIVER_GPIO_H
#define HEADER_DRIVER_GPIO_H
/**
 * @file         driver_gpio.h
 * @author       B. Shao
 * @version      1.0
 *
 * @brief        Gpio module configuration and initialisation
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
 */

#include "Platform_Types.h"
#include "Std_Types.h"
#include "Gpio.h"


/**
 * @brief GPIO module initialise function
*/ 
void GPIO_Initialization();


#endif //HEADER_DRIVER_GPIO_H