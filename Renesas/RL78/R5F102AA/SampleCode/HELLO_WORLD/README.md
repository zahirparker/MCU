HELLO WORLD
===

Hello World project for R5F102AA 

Hardware required: USB 2 serial converter on COM0 

 **Device** : R5F102AA

 **Tools** : e2 studio v3.0.0.22

   KPIT GNU RL78-ELF ToolChain v14.02

Code was generated using Applilet Code generator from within e2 studio

Settings used for Code generation
---------------------------------
  * PIOR0 = 0, PIOR1 = 0, PIOR2 = 0

  * VDD = 5V
  
  * Main system clock (fMAIN) = High-speed Onchip Clock Osc (fIH)
  * Internal Oscillator Freq: 24MHz

  * Interval Timer working on 15KHz 
  * Interval Timer Configured to generate an interrupt every 100 ms
  
  * Serial Array Unit 0 is used as a UART 
    * Single transfer mode
    * Data Lenght = 8 bits
    * Transfer direction setting : LSB first
    * Parity : None
    * Stop Bit : 1
    * Baud Rate : 9600
  
 After generating the code Call the required functions in R_MAIN_UserInit() 

```
 void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    R_UART0_Start();
    /* End user code. Do not edit comment generated here */
}
```

Define the following globals:
 * static const uint8_t messageHelloWorld[13] = {"Hello World\r\n"};
 * uint8_t g_Uart0RxBuf;    // 1 byte RX Buffer
 * MD_STATUS g_Uart0TxEnd;	 // Signals end of Tx 	
 * extern volatile uint16_t  g_uart0_rx_count;           /* uart0 receive data number */
 * extern volatile uint16_t  g_uart0_rx_length;          /* uart0 receive data length */

 Set a Flag in the r_uart0_callback_sendend function
```
 static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
	g_Uart0TxEnd = 1U;		/* Set transmission end flag */
    /* End user code. Do not edit comment generated here */
}
```

 To transmit message on UART use the following
 ```
 g_Uart0TxEnd = R_UART0_Send(messageHelloWorld, 13);
 while(g_Uart0TxEnd == 0);
 ```
To receive data from UART use the following code
```
	/* Initialize the RX Buffer */
	R_UART0_Receive(&g_Uart0RxBuf,1);

    while (1U)
    {
    	if( g_uart0_rx_count >= g_uart0_rx_length)
    	{
    		/* Send the recieved char on console */
    		g_Uart0TxEnd = R_UART0_Send(&g_Uart0RxBuf, g_uart0_rx_length);
    		while(g_Uart0TxEnd == 0);		/* Wait for final transmit */

    		/* Initialize the RX Buffer for Next Reception */
    		R_UART0_Receive(&g_Uart0RxBuf,1);
    	}
        ;
    }
```


Building the project will generate HELLO_WORLD.mot in the HardwareDebug folder

Flashing 
---------------------------------
  * Done through Serial Port using Renesas Flash Programmer 
  * The microcontroller comes with a boot loader installed 
  * Open Renesas Flash Programmer 
  * Create New Workspace 
  * Select Microcontroller R5F102AA 
  * Select Communication Interface : COM<ID> 
  * Browse for .mot file 
  * Put the device in program mode by pressing the SW3 on board 
  * Hit the start button in Renesas Flash Programmer 
  * Release the SW3 button  



Issues Faced
-------------------------------------
  * Some characters are missed during reception
    To solve this issue, a FIFO based approach with RX_BUFFER length more than 1
    needs to be implemented






