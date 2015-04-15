#include "r_cg_macrodriver.h"
#define UART_BUF_MAX_LENGTH  32
#define UART_BUF_MASK (UART_BUF_MAX_LENGTH -1) 

void uart0_interrupt_isr(void) __attribute__ ((interrupt));
void uart0_interrupt_isr(void);
void uart0_send(uint8_t * const tx_buf, uint16_t tx_num);
void uart0_crlf(void);
void uart0_callback_error(uint8_t err_type);

