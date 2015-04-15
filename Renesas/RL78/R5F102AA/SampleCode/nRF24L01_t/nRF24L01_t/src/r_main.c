/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G12 V2.02.00.02 [11 Feb 2014]
* Device(s)    : R5F102AA
* Tool-Chain   : GCCRL78
* Description  : This file implements main function.
* Creation Date: 4/10/2015
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
#include "pins.h"
#include "nRF24L01.h"
#define DUMMY_SPI_BYTE 0xFF
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile MD_STATUS g_Uart0TxEnd = 0;	 /* Signals end of Tx 	*/
volatile MD_STATUS g_CSI20TxEnd = 0;	 /* Signals end of Tx 	*/

const uint8_t messageHelloWorld[13] = {"Hello World\r\n"};	/* Message for "T" */

const uint8_t g_testArray[10] = {"1234567890"};

extern const uint8_t TX_ADDRESS[TX_ADR_LENGTH];
extern volatile uint8_t g_UartBuff[UART_BUF_MAX_LENGTH];
extern volatile uint8_t g_sbufHead;
extern volatile uint8_t g_sbufTail;
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
    #define delay(); { volatile int i; for(i=0; i<100; i++); }

    {
        uint8_t rx_buf;

        uint8_t ch=0;
        uint8_t ch_bak;

        uint8_t repeat = 0;
        #define MAX_ADDR 10
        uint8_t addr[MAX_ADDR] = {0x0, 0x1, 0x2, 0x3, 0x4,
                                  0x5, 0x6, 0x7, 0x8, 0x9};
        uint8_t count=0;
        uint8_t status;
        uint8_t payload[5];
        uint8_t *ptr_payload = &payload[0];


    	  g_Uart0TxEnd = R_UART0_Send((uint8_t * const)&g_testArray[0], 10);
	      while(g_Uart0TxEnd == 0);		/* Wait for final transmit */
    	  g_Uart0TxEnd = R_UART0_Send((uint8_t * const)&messageHelloWorld[0], 13);
	      while(g_Uart0TxEnd == 0);		/* Wait for final transmit */
        
        while(1);

        while(1)
        {   
            ///*Check Serial FIFO */
            //if (g_sbufTail != g_sbufHead)
            //{
            //    rx_buf = g_UartBuff[g_sbufTail++];
            //    //uart0_send( (uint8_t * const)&rx_buf, 1);
            //    uart0_send(p_cmsg, 13);
            //    g_sbufTail = g_sbufTail & UART_BUF_MASK;
            //}

        };
        

        uart0_send((uint8_t * const)&messageHelloWorld[0], 13);
        
        /* Test SPI interface with nRF24L01 */
        status = nRF24L01_testSPI();

        /* Hang here if SPI interface not responding */
        while(status == STATUS_FAIL);

        /* Test SPI buffer writes and reads */
        nRF24L01_WriteBuffer(CMD_WRITE + REG_TX_ADDR, (uint8_t *)&TX_ADDRESS, 5);

        nRF24L01_ReadBuffer(CMD_READ + REG_TX_ADDR, ptr_payload, 5);
        uart0_send(payload, 5);
        uart0_crlf();



        for(count=0; count<MAX_ADDR; count++)
        {
            unsigned char rx_char;
            rx_char = nRF24L01_ReadReg(count);
            uart0_send(&rx_char, 1);

            rx_char = ' ';
            uart0_send(&rx_char, 1);
        }

 while(1);

        //do
        //{
        //  if (SWITCH == 0)
        //    LED = 0;
        //  if (SWITCH == 1)
        //    LED = 1;
        //}while(1);

        //do
        //{
        //  while(SWITCH == 1);
        //  {
        //    volatile uint16_t i;
        //    for(i=0; i<1000; i++)
        //      delay();
        //  }
        //  //for(repeat=0; repeat<2; repeat++)
        //  {
        //    /* SPI Transmission */
        //    CSN = 0;
        //    LED = 1;
        //    delay();
        //    //ch = 0x05;
        //    g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
        //    while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

        //    //delay();
        //    
        //    ch_bak = ch;
        //    ch = DUMMY_SPI_BYTE;
        //    g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
        //    while(g_CSI20TxEnd == 0);		/* Wait for final transmit */
        //    ch = ch_bak;

        //    CSN = 1;
        //    LED = 0;
        //    delay();
        //    ch = ch + 1;
        //  }
        //} while(1);

        /* Send rcvd char on UART */
    	  //g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
	      //while(g_Uart0TxEnd == 0);		/* Wait for final transmit */

        for(count=0; count<MAX_ADDR; count++)
        {
            for(repeat=0; repeat<3; repeat++)
            {
              /* SPI Transmission */
              CSN = 0;
              LED = 1;
              delay();
              ch = addr[count];
              g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
              while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

              delay();
                
              ch = DUMMY_SPI_BYTE;
              g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
              while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

              CSN = 1;
              LED = 0;
              delay();

              //rx_buf = rx_buf^0xFF;

              /* Send rcvd char on UART */
              g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
              while(g_Uart0TxEnd == 0);		/* Wait for final transmit */
            }

            rx_buf='\n';
            /* Send rcvd char on UART */
            g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
            while(g_Uart0TxEnd == 0);		/* Wait for final transmit */
            rx_buf='\r';
            /* Send rcvd char on UART */
            g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
            while(g_Uart0TxEnd == 0);		/* Wait for final transmit */

        }
        while(1);

        for(repeat=0; repeat<2; repeat++)
        {
          /* SPI Transmission */
          CSN = 0;
          LED = 1;
          delay();
          ch = 0x01;
          g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
          while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

          delay();
            
          ch = DUMMY_SPI_BYTE;
          g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
          while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

          CSN = 1;
          LED = 0;
          delay();

          /* Send rcvd char on UART */
    	    g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
	        while(g_Uart0TxEnd == 0);		/* Wait for final transmit */
        }
        

        for(repeat=0; repeat<2; repeat++)
        {
          /* SPI Transmission */
          CSN = 0;
          LED = 1;
          delay();
          ch = 0x02;
          g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
          while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

          delay();
            
          ch = DUMMY_SPI_BYTE;
          g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
          while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

          CSN = 1;
          LED = 0;
          delay();

          /* Send rcvd char on UART */
    	    g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
	        while(g_Uart0TxEnd == 0);		/* Wait for final transmit */
        }
        while(1);

        /* Send rcvd char on UART */
    	  //g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
	      //while(g_Uart0TxEnd == 0);		/* Wait for final transmit */



        for(repeat=0; repeat<2; repeat++)
        {
          /* SPI Transmission */
          CSN = 0;
          LED = 1;
          delay();
          ch = 0x02;
          g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
          while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

          delay();
            
          ch = DUMMY_SPI_BYTE;
          g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
          while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

          CSN = 1;
          LED = 0;
          delay();
        }

        /* Send rcvd char on UART */
    	  //g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
	      //while(g_Uart0TxEnd == 0);		/* Wait for final transmit */

        for(repeat=0; repeat<2; repeat++)
        {
          /* SPI Transmission */
          CSN = 0;
          LED = 1;
          delay();
          ch = 0x03;
          g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
          while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

          delay();
            
          ch = DUMMY_SPI_BYTE;
          g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
          while(g_CSI20TxEnd == 0);		/* Wait for final transmit */

          CSN = 1;
          LED = 0;
          delay();
        }

        /* Send rcvd char on UART */
    	  //g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
	      //while(g_Uart0TxEnd == 0);		/* Wait for final transmit */


        ///* SPI Transmission */
        //for(ch=0; ch<8;ch++)
        //{
        //  CSN = 0;
        //  //ch = 0x01;
        //  g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
        //  
        //  while(g_CSI20TxEnd == 0);		/* Wait for final transmit */
        //  CSN = 1;
        //}

        while(1);

        
        LED = 1;
    	  g_Uart0TxEnd = R_UART0_Send((uint8_t * const)&messageHelloWorld[0], 13);
	      while(g_Uart0TxEnd == 0);		/* Wait for final transmit */
        LED = 0;

        //{
        //  volatile int i,j;
        //  while(1)
        //  {
        //        CSN = 1;
        //        for(j=0; j<10; j++)
        //        {
        //          //for(i=0; i<5000; i++);
        //        }
        //        CSN = 0;
        //        for(j=0; j<10; j++)
        //        {
        //          //for(i=0; i<5000; i++);
        //        }
        //  }

        //}
        
        /* Send ( on UART */
        ch = '(';
    	  g_Uart0TxEnd = R_UART0_Send(&ch, 1);
	      while(g_Uart0TxEnd == 0);		/* Wait for final transmit */

        /* SPI Transmission */
        CSN = 0;
        LED = 1;
        ch = 0x07;
        g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
	      while(g_CSI20TxEnd == 0);		/* Wait for final transmit */
        CSN = 1;
        LED = 0;

        while(1);

        /* Send rcvd char on UART */
    	  g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
	      while(g_Uart0TxEnd == 0);		/* Wait for final transmit */

        /* SPI Transmission */
        CSN = 0;
        ch = 0x0C;
        g_CSI20TxEnd = R_CSI20_Send_Receive(&ch, 1, &rx_buf);
	      while(g_CSI20TxEnd == 0);		/* Wait for final transmit */
        CSN = 1;

        /* Send rcvd char on UART */
    	  g_Uart0TxEnd = R_UART0_Send(&rx_buf, 1);
	      while(g_Uart0TxEnd == 0);		/* Wait for final transmit */
        /* Send )on UART */
        ch = ')';
    	  g_Uart0TxEnd = R_UART0_Send(&ch, 1);
	      while(g_Uart0TxEnd == 0);		/* Wait for final transmit */

        while (1U)
        {
            ;
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
    R_CSI20_Start();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
