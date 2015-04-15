/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "uart.h"
#include "pins.h"
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"



/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
// This should be modulo 2 number 
volatile uint8_t g_UartBuffer[UART_BUF_MAX_LENGTH];
volatile uint8_t g_UbuffHead = 0;
volatile uint8_t g_UbuffTail = 0;


extern volatile MD_STATUS g_Uart0TxEnd;		/* UART0 transmission end */
extern volatile MD_STATUS g_CSI20TxEnd;		/* CSI20 transmission end */


/***********************************************************************************************************************
* Function Name: uart0_isr
* Description  : This function is INTSR0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void uart0_interrupt_isr(void)
{
    uint8_t err_type;
    
    //LED = ~LED;
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        uart0_callback_error(err_type);
    }

    g_UartBuffer[g_UbuffHead++] = RXD0;
    g_UbuffHead = g_UbuffHead & UART_BUF_MASK;

}

void uart0_send(uint8_t * const tx_buf, uint16_t tx_num)
{
    /* Send rcvd char on UART */
    g_Uart0TxEnd = R_UART0_Send(tx_buf, tx_num);
    
    /* Wait for the final transmit */
    while(g_Uart0TxEnd == 0);		
}

void uart0_crlf(void)
{
    uint8_t * const buf = "\n\r";
    uart0_send(buf, 2);
}

void uart0_callback_error(uint8_t err_type)
{
}
