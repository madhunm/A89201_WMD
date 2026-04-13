#ifndef HEADER_COMM_PWM_APP_H
#define HEADER_COMM_PWM_APP_H

/**
 * @internal
 *
 * @file         comm_pwm_app.h
 * @author       B.Shao
 * @version			 1.0
 * @copyright (c) 2020-2023 Allegro MicroSystems, Inc. All Rights Reserved.
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
 * @details   PwmMapping implementation. This code uses Timers 5,6 and 7. 
 */


#include "Platform_Types.h"
#include "comm_pwm.h"

/**
 * @brief     Configure the PWM input interface parameters.
 */
void Comm_PWM_Config(void);

#endif //HEADER_COMM_PWM_APP_H