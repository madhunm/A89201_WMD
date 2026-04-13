/**
 * @file         driver_gpio.c
 * @author       B. Shao
 * @version      1.0
 * @copyright (c) 2025 Allegro MicroSystems, Inc. All Rights Reserved.
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
 * ---------------------------------------------------------------------------------------------------------------------------- 
 * This Software License Agreement (“Agreement”) is made between                                                              
 * Allegro MicroSystems, LLC (“Allegro”) and the user identified in                                                           
 * the signature block below ("Licensee") (each a "Party" and collectively, the "Parties")                                    
 * on the date of the signature below.      

 * LICENSE TERMS:
 *    a. Exclusively in conjunction with Licensee’s development and sale of products 
 *       containing the Application, Allegro grants Licensee a limited, non-exclusive, 
 *       non-transferable, revocable, and royalty-free license under the copyright rights 
 *       of Allegro or its licensor to (i) use the Software Deliverables to develop the Application,
 *       (ii) use the Application in conjunction with the Library Files, solely in a non-production 
 *       environment for the exclusive purpose of evaluating the Application.   
 * 
 *   b. Licensee recognizes that the Software (i) has not been released by Allegro for general distribution or sale,
 *      and that Allegro has not completed its testing and quality assurances, and (ii) is not qualified and has not 
 *      been certified to meet any industry standards. Licensee shall not use the Software provided hereunder in 
 *      any production environment. Licensee will not disclose or provide the Software or Application to any 
 *      third party, and will defend, indemnify and hold Allegro harmless from all  damages, costs, liabilities 
 *      and expenses related to any third party claims arising from Licensee's breach of this Agreement. 
 *      Further, Licensee will not redistribute the Software or Application to any party, 
 *      including individuals within Licensee’s organization.
 *
 *   c.  In no event will Licensee use the Software in conjunction with any devices (including competitor devices) 
 *       other than the Allegro device identified in the Allegro documentation accompanying the Software or notified
 *       to Licensee by Allegro in writing.
 *
 *   d. Licensee acknowledges that the Software and all related technical information, documents and materials
 *      are subject to export controls under the U.S. Export Administration Regulations.  Licensee will (i) comply 
 *      strictly with all legal requirements established under these controls, (ii) cooperate fully with Allegro in 
 *      any official or unofficial audit or inspection that relates to these controls and (iii) not export, re-export,
 *      divert or transfer, directly or indirectly, any such item or direct products thereof to Crimea, Iran, North Korea,
 *      Sudan, Syria or any country that is embargoed by Executive order, unless Licensee has obtained the prior written 
 *      authorization of Allegro and the U.S. Commerce Department.  
 *
 * CONFIDENTIAL TERMS:
 *      a. Licensee acknowledges and agrees that all information regarding the Software, the results of testing and 
 *         evaluation hereunder, the Feedback provided hereunder, the characteristics and performance of the Software 
 *         and Application (including all related documentation) and this Agreement (collectively, the “Confidential Information”) 
 *         are all confidential information and contain trade secrets of Allegro.  Licensee acknowledges that the Agreement will 
 *         induce Allegro to make such Confidential Information available to Licensee.
 *
 *     This distribution is designated as EAR99.  
 * ---------------------------------------------------------------------------------------------------------------------------- 
 *
 */
 

#include "driver_gpio.h"

/**
 * @brief GPIO module initialise function
*/
void GPIO_Initialization()
{
		/* Set up GPIO 1 as the FOC exectution trigger */
		Gpio_SetPinActive( eGPIO_INSTANCE_ID1 );
		Gpio_SetMode( eGPIO_INSTANCE_ID1,eGPIO_MODE_OUTPUT_PUSH_PULL );
		Gpio_SetSource( eGPIO_INSTANCE_ID1,eGPIO_SOURCE_DOUT);
		Gpio_Clear( eGPIO_INSTANCE_ID1 );
		
		/* Set up GPIO 2 as the FG terminal */
		Gpio_SetPinActive( eGPIO_INSTANCE_ID2 );
		Gpio_SetMode( eGPIO_INSTANCE_ID2,eGPIO_MODE_OUTPUT_PUSH_PULL );
		Gpio_SetSource( eGPIO_INSTANCE_ID2,eGPIO_SOURCE_DOUT);
		Gpio_Set( eGPIO_INSTANCE_ID2 );
	
		/* Configure GPIO3 for Fault detection */
		Gpio_SetPinActive(eGPIO_INSTANCE_ID3);
		Gpio_SetMode(eGPIO_INSTANCE_ID3,eGPIO_MODE_OUTPUT_PUSH_PULL);
		Gpio_SetSource( eGPIO_INSTANCE_ID3,eGPIO_SOURCE_DOUT);
		Gpio_Clear(eGPIO_INSTANCE_ID3);	
		
		/* Set up GPIO 6 as the uart transmit terminal */
		Gpio_SetPinActive( eGPIO_INSTANCE_ID6 );
		Gpio_SetMode( eGPIO_INSTANCE_ID6,eGPIO_MODE_OUTPUT_PUSH_PULL);
		Gpio_SetSource( eGPIO_INSTANCE_ID6,eGPIO_ASRC0);

		/* Set up GPIO 7 as the uart receive terminal */
		Gpio_SetPinActive( eGPIO_INSTANCE_ID7 );
		Gpio_SetRxPinPort( eGPIO_INSTANCE_ID7, 0,(uint8)eGPIO_INSTANCE_ID7);// For Uart Connection Only
		Gpio_SetMode( eGPIO_INSTANCE_ID7,eGPIO_MODE_DIGITAL_INPUT );

		/* Set Up Gpio Pins for die to die Pwm Interface */
		/* Create GPIO 10-15 Objects for inter die to die Pwm Connection */
		/* These Gpio has to be configured if gate drive signals are required*/
		Gpio_SetPinActive( eGPIO_INSTANCE_ID10 );
		Gpio_SetPinActive( eGPIO_INSTANCE_ID11 );
		Gpio_SetPinActive( eGPIO_INSTANCE_ID12 );
		Gpio_SetPinActive( eGPIO_INSTANCE_ID13 );
		Gpio_SetPinActive( eGPIO_INSTANCE_ID14 );
		Gpio_SetPinActive( eGPIO_INSTANCE_ID15 );
		
		Gpio_SetMode( eGPIO_INSTANCE_ID10, eGPIO_MODE_OUTPUT_PUSH_PULL );
		Gpio_SetMode( eGPIO_INSTANCE_ID11, eGPIO_MODE_OUTPUT_PUSH_PULL );
		Gpio_SetMode( eGPIO_INSTANCE_ID12, eGPIO_MODE_OUTPUT_PUSH_PULL );
		Gpio_SetMode( eGPIO_INSTANCE_ID13, eGPIO_MODE_OUTPUT_PUSH_PULL );
		Gpio_SetMode( eGPIO_INSTANCE_ID14, eGPIO_MODE_OUTPUT_PUSH_PULL );
		Gpio_SetMode( eGPIO_INSTANCE_ID15, eGPIO_MODE_OUTPUT_PUSH_PULL );
		
		Gpio_SetSource( eGPIO_INSTANCE_ID10,eGPIO_ESRC0 );
		Gpio_SetSource( eGPIO_INSTANCE_ID11,eGPIO_ESRC0 );
		Gpio_SetSource( eGPIO_INSTANCE_ID12,eGPIO_ESRC0 );
		Gpio_SetSource( eGPIO_INSTANCE_ID13,eGPIO_ESRC0 );
		Gpio_SetSource( eGPIO_INSTANCE_ID14,eGPIO_ESRC0 );
		Gpio_SetSource( eGPIO_INSTANCE_ID15,eGPIO_ESRC0 );
}