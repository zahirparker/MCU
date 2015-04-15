/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G12 V2.03.00.03 [07 Aug 2014]
* Device(s)    : R5F102AA
* Tool-Chain   : GCCRL78
* Description  : This file implements main function.
* Creation Date: 4/12/2015
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "uart.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile MD_STATUS g_Uart0TxEnd = 0;	 /* Signals end of Tx 	*/
volatile MD_STATUS g_Uart0RxEnd = 0;	 /* Signals end of Rx 	*/
const uint8_t c_MsgHelloWorld[12] = {"nRF24L01_z\r\n"};

// Externs
extern volatile uint8_t g_UartBuffer[UART_BUF_MAX_LENGTH];
extern volatile uint8_t g_UbuffHead;
extern volatile uint8_t g_UbuffTail;
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    {
        uint8_t rxChar;
        g_Uart0TxEnd = R_UART0_Send((uint8_t * const)&c_MsgHelloWorld[0], 12);
        while(g_Uart0TxEnd == 0U);

        while (1U)
        {   

            /* 
             * Code to Poll UART 
             */

            #if 0
            /* Receive one char */
            g_Uart0RxEnd = R_UART0_Receive((uint8_t * const)&rxChar, 1);
            while(g_Uart0RxEnd == 0U);

            /* Transmit the received data */
            g_Uart0TxEnd = R_UART0_Send((uint8_t * const)&rxChar, 1);
            while(g_Uart0TxEnd == 0U);
            #endif


            /* 
             * Interrupt, Buffered Serial UART 
             */
            if (g_UbuffHead != g_UbuffTail)
            {
                /* Read Char from Uart Buffer */
                rxChar = g_UartBuffer[g_UbuffTail++];
                g_UbuffTail = g_UbuffTail & UART_BUF_MASK;

                /* Transmit it out */
                g_Uart0TxEnd = R_UART0_Send((uint8_t * const)&rxChar, 1);
                while(g_Uart0TxEnd == 0U);

            }
        }
    }
    /* End user code. Do not edit comment generated here */
}


/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    R_UART0_Start();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
