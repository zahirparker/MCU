#include "r_cg_macrodriver.h"
#include "spi.h"
extern volatile MD_STATUS g_Uart0TxEnd;		/* UART0 transmission end */
extern volatile MD_STATUS g_CSI20TxEnd;		/* CSI20 transmission end */

void csi20_transceive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf)
{
    /* Send out char on SPI & receive char on SPI */
    g_CSI20TxEnd = R_CSI20_Send_Receive(tx_buf, tx_num, rx_buf);
    
    /* Wait for the transmit to complete */
    while(g_CSI20TxEnd == 0);		
}

