#ifndef HEADER_GDU_SLAVE_H
#define HEADER_GDU_SLAVE_H
/* polyspace-begin MISRA-C3:3.1 [Justified:Unset] "Standard corporate header with URL." */
/**
 * @file         GduSlave.h
 * @author       G. Sloan
 * @version      1.0
 *
 * @brief        The implementation of the Gdu Slave interface
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
enum GduSlaveRegister
{
    /** Register 0 */
    eGDUSLAVE_DRIVER_CONFIG = 0,
    /** Register 1 */
    eGDUSLAVE_SENSE_SELECT = 1,
    /** Register 2 */
    eGDUSLAVE_BIAS_AND_AMUX_CONTROL = 2,
    /** Register 3 */
    eGDUSLAVE_DI_CSA = 3,
    /** Register 4 */
    eGDUSLAVE_SLEW_RISE = 4,
    /** Register 5 */
    eGDUSLAVE_SLEW_FALL = 5,
    /** Register 6 */
    eGDUSLAVE_CONFIG = 6,
    /** Register 7 */
    eGDUSLAVE_SOF = 7,
    /** Register 8 */
    eGDUSLAVE_MASK = 8,
    /** Register 9 */
    eGDUSLAVE_NVM0 = 9,
    /** Register 10 */
    eGDUSLAVE_NVM1 = 10,
    /** Register 11 */
    eGDUSLAVE_NVM2 = 11,
    /** Register 12 */
    eGDUSLAVE_NVM3 = 12,
    /** Register 13 */
    eGDUSLAVE_NVM4 = 13,
    /** Register 14 */
    eGDUSLAVE_NVM5 = 14,
    /** Register 15 */
    eGDUSLAVE_NVM6 = 15,
    /** Register 16 */
    eGDUSLAVE_NVM7 = 16,
    /** Register 17 */
    eGDUSLAVE_NVM8 = 17,
    /** Register 18 */
    eGDUSLAVE_NVM9 = 18,
    /** Register 19 */
    eGDUSLAVE_NVM10 = 19,
    /** Register 20 */
    eGDUSLAVE_NVM11 = 20,
    /** Register 21 */
    eGDUSLAVE_NVM12 = 21,
    /** Register 22 */
    eGDUSLAVE_NVM13 = 22,
    /** Register 23 */
    eGDUSLAVE_NVM14 = 23,
    /** Register 24 */
    eGDUSLAVE_NVM15 = 24,
    /** Register 25 */
    eGDUSLAVE_NVM16 = 25,
    /** Register 26 */
    eGDUSLAVE_NVM17 = 26,
    /** Register 27 */
    eGDUSLAVE_NVM18 = 27,
    /** Register 28 */
    eGDUSLAVE_NVM19 = 28,
    /** Register 29 */
    eGDUSLAVE_NVM20 = 29,
    /** Register 30 */
    eGDUSLAVE_NVM21 = 30,
    /** Register 31 */
    eGDUSLAVE_NVM22 = 31,
    /** Register 32 */
    eGDUSLAVE_NVM23 = 32,
    /** Register 33 */
    eGDUSLAVE_NVM24 = 33,
    /** Register 34 */
    eGDUSLAVE_NVM25 = 34,
    /** Register 35 */
    eGDUSLAVE_NVM26 = 35,
    /** Register 36 */
    eGDUSLAVE_NVM27 = 36,
    /** Register 37 */
    eGDUSLAVE_NVM28 = 37,
    /** Register 38 */
    eGDUSLAVE_NVM29 = 38,
    /** Register 39 */
    eGDUSLAVE_NVM30 = 39,
    /** Register 40 */
    eGDUSLAVE_NVM31 = 40,
    /** Register 118 */
    eGDUSLAVE_TINY_TEMP_HIGH = 118,
    /** Register 119 */
    eGDUSLAVE_TINY_TEMP_LOW = 119,
    /** RegisteGDUSLAVE_r 120 */
    eTINY_TEMP_OUT = 120,
    /** Register 121 */
    eGDUSLAVE_NVM_CONTROL = 121,
    /** Register 122 */
    eGDUSLAVE_GD_CONTROL = 122,
    /** Register 123 */
    eGDUSLAVE_WATCHDOG = 123,
    /** Register 124 */
    eGDUSLAVE_QA_CONTROL = 124,
    /** Register 125 */
    eGDUSLAVE_DIAG0 = 125,
    /** Register 126 */
    eGDUSLAVE_DIAG1 = 126,
    /** Register 127 */
    eGDUSLAVE_STATUS = 127
};



#endif //HEADER_GDU_SLAVE_H