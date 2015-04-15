HELLO WORLD
===

nRF24L01 interface with R5F102AA 

Hardware required: nRF24L01, UART0, 

 **Device** : R5F102AA

 **Tools** : e2 studio v3.0.0.22

   KPIT GNU RL78-ELF ToolChain v14.02


Hardware Connections
--------------------


nRF24L01 Module PinOut:

    ----------------- 
   | |-| |-| |-| |---| <-- Antenna
   | | |-| |-| |-|   |
   |                 |
   |             |-| | <- XTAL
   |             |X| |
   |              -  |
   |  2   4  6  8    |
   || 1 | 3  5  7    |
    ----------------- 

    | Pins| Function|
    |-----|---------|
    |  1  | GND     |
    |  2  | VDD     | 3.3V
    |  3  | CE      |
    |  4  | CSN     |
    |  5  | SCK     |
    |  6  | MOSI    |
    |  7  | MISO    |
    |  8  | IRQ     |
           
R5F102AA and nRF24L01 Pin connections:

  ---------------------------------------------
  | R5F102AA                | nRF24L01        |
  ---------------------------------------------
  |  Pin  | Pin   |  Alt    |    Pin  | Pin   |
  |  Name | No    |         |    Name | No    |
  ---------------------------------------------
  | P30   | 16    |         | CSN     | 4     |
  | P15   | 21    |SCK20    | SCK     | 5     |
  | P14   | 22    |SI20     | MISO    | 7     |
  | P13   | 23    |SO20     | MOSI    | 6     |

VCC for R5F102AA +5V
VCC for nRF24L01 +3.3V
No level shifters used on pins. Direct connection




Code was generated using Applilet Code generator from within e2 studio

Settings used for Code generation
---------------------------------
  * PIOR0 = 0, PIOR1 = 0, PIOR2 = 0

  * VDD = 5V
  
  * Main system clock (fMAIN) = High-speed Onchip Clock Osc (fIH)
  * Internal Oscillator Freq: 24MHz

  * Interval Timer working on 15KHz 

  * SAU0 is already used up by UART0 and UART1

    * UART0 on CH0 and CH1
    * Single transfer mode
    * Data Lenght = 8 bits
    * Transfer direction setting : LSB first
    * Parity : None
    * Stop Bit : 1
    * Baud Rate : 9600
    
    * For using SPI we will use SAU1
    * Channel0 in SAU1 will be used as CSI20 and the corresponding pins are
    * SO20  (Pin 23)
    * SI20  (Pin 22)
    * SCK20 (Pin 21)
    * Single transfer mode
    * Data Lenght = 8 bits
    * Transfer direction setting : MSB first
    * Timing : Type 4
    * Mode : Master
    * Baud Rate : 153600
  
After generating the code Call the required functions in *R_CSI20_Start()*

```
 void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    R_CSI20_Start();
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
call *R_UART0_Receive()*  before receiving a new message. 
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
leave it here.**

A simple solution was to just call *R_UART0_Receive(&g_Uart0RxBuf,1);* to
reinitialize the sw rx buffer for the next reception

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
  * Forgot to mention volatile qualifier for this global
    Specified extern MD_STATUS g_Uart0TxEnd;		/* UART0 transmission end */
    instead of 
    extern volatile MD_STATUS g_Uart0TxEnd;		/* UART0 transmission end */
    The driver kept waiting as it could never find this variable set to 1

    /* Send rcvd char on UART */
    g_Uart0TxEnd = R_UART0_Send(tx_buf, tx_num);
    
    /* Wait for the final transmit */
    while(g_Uart0TxEnd == 0);		

    This read got optimized out

  * MISO Input needs to be configured to TTL for voltage compatibility.

  * First SPI read/write is not working 






http://cp.literature.agilent.com/litweb/pdf/5988-9489EN.pdf
nRF51 SDK https://devzone.nordicsemi.com/documentation/nrf51/4.4.0/html/group__spi__master__example.html


