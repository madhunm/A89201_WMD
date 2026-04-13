#ifndef HEADER_SYSTEM_ARMCM4_H
#define HEADER_SYSTEM_ARMCM4_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file       system_ARMCM4.h
 * @author     N. Moheeputh
 * @version    1.0
 *
 * @brief      System ARMCM4 interface
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
 
// Allegro 00 product uses 20MHz crystal 
#define  XTAL            (20000000UL)     /* Oscillator frequency */
// Phase lock loop doubles frequency to 40MHz
#define  SYSTEM_CLOCK    (XTAL * 2U) 
 
 #endif /* HEADER_SYSTEM_ARMCM4_H */