README
========

UART Configuration
------------------

UART Driver
-----------

* Support for 9600, 19200, 38400, 115200 (good to have)
* R_UART0_Create() function is used 
* R_UART0_Start() function should be called to start the uart module
* For Interrupt driven buffered IO rx isr needs to be changed
* r_cg_vector_table.c needs to be changed to hook up the new ISR
* new ISR uart0_interrupt_isr in uart.c
* ISR should have interrupt attribute in the header file
  * void uart0_interrupt_isr(void) __attribute__ ((interrupt));


SPI Configuration
------------------

SPI Driver
------------------

* Check if nRF24L01 is connected or not
* If nRF24L01 is connected test SPI link 
  * Check SPI Read, by reading default value of a register and compare
  * Check SPI Write, by writing and reading back a register value

