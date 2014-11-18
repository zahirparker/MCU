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
  
After generating the code Call the required functions in *R_MAIN_UserInit()*

```
 void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    R_UART0_Start();
    /* End user code. Do not edit comment generated here */
}
```

Define the following globals in r_main.c:
 * static const uint8_t messageHelloWorld[13] = {"Hello World\r\n"};
 * uint8_t g_Uart0RxBuf;     /*  1 byte RX Buffer */
 * MD_STATUS g_Uart0TxEnd;	 /* Signals end of Tx 	*/
 * extern volatile uint16_t  g_uart0_rx_count;           /* uart0 receive data number */
 * extern volatile uint16_t  g_uart0_rx_length;          /* uart0 receive data length */

Set a Flag in the *r_uart0_callback_sendend()* function to indicate transmission of
all bytes is complete
```
 static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
	g_Uart0TxEnd = 1U;		/* Set transmission end flag */
    /* End user code. Do not edit comment generated here */
}
```

*r_uart0_interrupt_send()* is an interrupt subroutine.
This gets triggered once the current byte transmission is complete
The ISR then reads the next data in the sw tx buffer and writes to the Hardware
register to start the transmission of the next byte.
Once all the bytes have been transmitted the callback function *r_uart0_callback_sendend()* is called
This function can the set a flag and let the application know that the
transmission of all bytes in the sw tx buffer is complete

```
void r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}
```

To transmit message on UART use the following
```
g_Uart0TxEnd = R_UART0_Send(messageHelloWorld, 13);
while(g_Uart0TxEnd == 0);
```
To receive data from UART use the following code
Call *R_UART0_Receive()*  before receiving a new message. 
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
*R_UART0_Receive()* function will 
* Clear the rx count
* Initialize the sw buffer address to first element
* Initialize the rx length 

```
MD_STATUS R_UART0_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (rx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_uart0_rx_count = 0U;
        g_uart0_rx_length = rx_num;
        gp_uart0_rx_address = rx_buf;
    }

    return (status);
}
```

*r_uart0_interrupt_receive()* is an interrupt subroutine.
This gets triggered whenever a byte is received on UART
The ISR then reads the data from hardware register and puts it in the sw rx buffer 
Once all the bytes have been received the callback function *r_uart0_callback_receiveend()* is called
This function can then set a flag and let the application know that the
reception of X bytes in the sw rx buffer is complete

 **Even though X bytes have been received in the sw rx buffer. The code here does
not read the sw rx buffer and decrement the number of bytes available in the sw
rx buffer. This kind of solution needs to be implemented in order to fully use
the HW capabilities. But this code is just used to receive a 1 byte and hence we
leave it here.

A simple solution was to just call *R_UART0_Receive(&g_Uart0RxBuf,1);* to
reinitialize the sw rx buffer for the next reception**

```
void r_uart0_interrupt_receive(void)
{
    uint8_t rx_data;
    uint8_t err_type;
    
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart0_callback_error(err_type);
    }
    
    rx_data = RXD0;

    if (g_uart0_rx_length > g_uart0_rx_count)
    {
        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            r_uart0_callback_receiveend();
        }
    }
    else
    {
        r_uart0_callback_softwareoverrun(rx_data);
    }
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






