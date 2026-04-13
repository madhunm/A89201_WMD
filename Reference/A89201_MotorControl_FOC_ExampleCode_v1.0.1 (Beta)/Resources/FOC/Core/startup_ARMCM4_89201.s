;/**************************************************************************//**
; * @file     startup_ARMCM4_89201.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM4 Device
; * @version  V5.4.0
; * @date     12. December 2018
; ******************************************************************************/
;/*
; * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
; * Copyright (c) 2023-2025 Allegro MicroSystems, Inc. All Rights Reserved. 
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------


;<h> Stack Configuration
;  <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>

Stack_Size      EQU      0x00001000

                AREA     STACK, NOINIT, READWRITE, ALIGN=3
__stack_limit
Stack_Mem       SPACE    Stack_Size
__initial_sp


;<h> Heap Configuration
;  <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>

Heap_Size       EQU      0x00002000

                IF       Heap_Size != 0                      ; Heap is provided
                AREA     HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE    Heap_Size
__heap_limit
                ENDIF


                PRESERVE8
                THUMB


; The CPP method symbols are extracted from the list files and map file produced by the ARMCLANG v6.7 compiler
; It takes the correct uVision, compiler and linker options combination to generate the symbols
; Right click on the Target withing the uVision IDE and select options for target
; On the Output tab all boxes are checked under create executable
; On the Listing tab all boxes arc checked
; On the C/C++ AC6 tab optimisation is set to 0 (none) and only the plain char is signed and short enums boxes are checked
; Warnings are set to MISRA, C++ to C03, C to C99. Under Misc controls "-fno-exceptions -g"
; On the Linker tab "--bestdebug --list_mapping_symbols --symbols"
;
; If you change a function / method in CPP then this will change the signature produced by the linker.



; Vector Table Mapped to Address 0 at Reset

                AREA     RESET, DATA, READONLY
                EXPORT   __Vectors
                EXPORT   __Vectors_End
                EXPORT   __Vectors_Size            					

				IMPORT  Cau_IrqHandler
				IMPORT  Dau_IrqHandler
				IMPORT 	Timer1_IrqHandler
				IMPORT 	Timer2_IrqHandler
				IMPORT 	Timer3_IrqHandler
				IMPORT 	Timer4_IrqHandler
				IMPORT 	Timer5_IrqHandler
				IMPORT 	Timer6_IrqHandler
				IMPORT 	Timer7_IrqHandler
				IMPORT 	Timer8_IrqHandler
				IMPORT  Sci_IrqHandler0
				IMPORT  Sci_IrqHandler1
				IMPORT  Gpio_IrqHandler
				IMPORT 	Amct_IrqHandler
				IMPORT  GduMaster_IrqHandler
				IMPORT  I2c_IrqHandler
				IMPORT  Lin_IrqHandler
                IMPORT  Scu_NmiIrqHandler
				IMPORT  Scu_HardFaultIrqHandler
                IMPORT  Scu_MemManageIrqHandler
                IMPORT  Scu_BusFaultIrqHandler
                IMPORT  Scu_UsageFaultIrqHandler
                IMPORT  Scu_FlashOrSystemFaultIrqHandler
                IMPORT  SystemTimer_SysTickIrqHandler
				IMPORT	Dma_IrqHandler
                    
__Vectors       DCD      __initial_sp                        ;     Top of Stack
                DCD      Reset_Handler                       ;     Reset Handler
                DCD      Scu_NmiIrqHandler                   ; -14 NMI Handler
                DCD      Scu_HardFaultIrqHandler;            ; -13 Hard Fault Handler           
                DCD      Scu_MemManageIrqHandler;            ; -12 MPU Fault Handler
                DCD      Scu_BusFaultIrqHandler;             ; -11 Bus Fault Handler
                DCD      Scu_UsageFaultIrqHandler;           ; -10 Usage Fault Handler
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      SVC_Handler                         ;  -5 SVCall Handler
                DCD      DebugMon_Handler                    ;  -4 Debug Monitor Handler
                DCD      0                                   ;     Reserved
                DCD      PendSV_Handler                      ;  -2 PendSV Handler
                DCD      SystemTimer_SysTickIrqHandler       ;  -1 SysTick Handler

                ; Interrupts
                DCD      Cau_IrqHandler;             	0 Interrupt 0
                DCD      Dau_IrqHandler;                1 Interrupt 1
                DCD      Timer1_IrqHandler;            	2 Interrupt 2
                DCD      Timer2_IrqHandler;            	3 Interrupt 3
                DCD      Timer3_IrqHandler;            	4 Interrupt 4
                DCD      Timer4_IrqHandler;            	5 Interrupt 5     
                DCD      Timer5_IrqHandler;            	6 Interrupt 6
                DCD      Timer6_IrqHandler;            	7 Interrupt 7
                DCD      Timer7_IrqHandler;            	8 Interrupt 8
                DCD      Timer8_IrqHandler;            	9 Interrupt 9
                DCD      Sci_IrqHandler0;				10 Interrupt 10
                DCD      Sci_IrqHandler1;				11 Interrupt 11
                DCD      Lin_IrqHandler;                12 Interrupt 12 
                DCD      GduMaster_IrqHandler;          13 Interrupt 13
                DCD      I2c_IrqHandler;                14 Interrupt 14
                DCD      Interrupt15_Handler;			15 Interrupt 15 
                DCD      Gpio_IrqHandler;           	16 Interrupt 16
                DCD      Amct_IrqHandler;           	17 Interrupt 17
                DCD      Dma_IrqHandler; 	            18 Interrupt 18
                DCD      Scu_FlashOrSystemFaultIrqHandler; 19 Interrupt 19
              
__Vectors_End
__Vectors_Size  EQU      __Vectors_End - __Vectors


                AREA     |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT   Reset_Handler             [WEAK]
                IMPORT   SystemInit
                IMPORT   __main

                LDR      R0, =SystemInit
                BLX      R0
                LDR      R0, =__main
                BX       R0
                ENDP
                    


; Macro to define default exception/interrupt handlers.
; Default handler are weak symbols with an endless loop.
; They can be overwritten by real handlers.
                MACRO
                Set_Default_Handler  $Handler_Name
$Handler_Name   PROC
                EXPORT   $Handler_Name             [WEAK]
                B        .
                ENDP
                MEND

; Macro to define default exception/interrupt handlers.
; Default handler are weak symbols with an endless loop.
; They can be overwritten by real handlers.

; The address of the NVIC interrupt control register.
NVIC_INT_CTRL_CONST     EQU     0xe000ed04

                

; Default exception/interrupt handler
; If you implement a handler you need to remove one of these
                Set_Default_Handler  SVC_Handler
                Set_Default_Handler  DebugMon_Handler
                Set_Default_Handler  PendSV_Handler

                Set_Default_Handler  Interrupt15_Handler
                Set_Default_Handler  Interrupt18_Handler

                ALIGN


; User setup Stack & Heap

                IF       :LNOT::DEF:__MICROLIB
                IMPORT   __use_two_region_memory
                ENDIF

                EXPORT   __stack_limit
                EXPORT   __initial_sp
                IF       Heap_Size != 0                      ; Heap is provided
                EXPORT   __heap_base
                EXPORT   __heap_limit
                ENDIF

                END