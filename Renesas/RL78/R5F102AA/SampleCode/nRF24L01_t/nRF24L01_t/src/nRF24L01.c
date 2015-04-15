//#include<pins.h>
//#include<spi.h>
//#include<delay.h>
//#include<serial.h>
//#include<lcd.h>
#include "r_cg_macrodriver.h"
#include "pins.h"
#include "spi.h"
#include "nRF24L01.h"
#include "uart.h"
#include "delay.h"

#define TX_ADR_LENGTH     5 
#define RX_AW     5 
//#define DEBUG
extern volatile MD_STATUS g_Uart0TxEnd;		/* UART0 transmission end */
extern volatile MD_STATUS g_CSI20TxEnd;		/* CSI20 transmission end */

static uint8_t *p_msgSpiRdPass = "SPI Read Pass\n\r";
static uint8_t *p_msgSpiWrPass = "SPI Write Pass\n\r";
static uint8_t *p_msgSpiExp = "Expected: 5 Got: ";


const uint8_t TX_ADDRESS[TX_ADR_LENGTH] = {0xE7,0xE7,0xE7,0xE7,0xE7};                 // Predefined TX_Address 
//const uint8_t RX_ADDRESS_[RX_AW]   = {0xE7,0xE7,0xE7,0xE7,0xE7};  // Predefined RX_Address 
//uint8_t  xdata TX_PAYLOAD[TX_PLOAD_WIDTH];
//uint8_t   xdata PAYLOAD[RX_PLOAD_WIDTH];
//#ifdef DEBUG
//uint8_t g_pos=1; 
//#endif

/***************************************************************************
* Function: Transmit_Data(void) 
* Parameters: none            
* Returns: None 
* Purpose:  This function sends out the payload stored in the array .   
			Payload_array must be setup before calling this function 
			> First we need to clear all interupt by writing 0x7e to status reg.
			> Make PWR_UP = 1 in the config reg. to enable transmission.
			> Flush the TX_FIFO  by sending command 0xE1.
			> Write The  payload
			> Pulse CE pin of nRF24L01 to Start transmiision.
***************************************************************************/
void Transmit_Data(void)   
{   
    uint8_t cmd;
    uint8_t rx_buf;
   
    nRF24L01_WriteReg(CMD_WRITE + REG_STATUS,0x7E);	//clear previous ints
    nRF24L01_WriteReg(CMD_WRITE + REG_CONFIG,0x0A);  //PWR_UP = 1,enable tx intr.
    
        
    CSN = 0;
    /* Flush TX FIFO */
    cmd = CMD_FLUSH_TX;
    csi20_transceive(&cmd, 1, &rx_buf);
         
    CSN = 1;
    /* Write TX Payload */
    //nRF24L01_WriteBuffer(CMD_WR_TX_PLOAD, &TX_PAYLOAD, TX_PLOAD_WIDTH); 
       
    /* Pulse CE to start the transmission */
    CE = 1;  							    
    delayMs(1);   
    CE = 0;   
}   

/***************************************************************************
* Function: void nRF24L01_Configure_TX(void)   
* Parameters: none            
* Returns: None 
* Purpose:  This function sets up one RF-24G for shockburst transmission 
			This function initializes  nRF24L01 device to    
  			TX Mode, set TX address, writes TX payload width,    
  			select RF channel. 
***************************************************************************/   
   
//This sets up one RF-24G for shockburst transmission    
void nRF24L01_Configure_TX(void) 
{   
    uint8_t cmd;
    uint8_t rx_buf;

    CE = 0;   
    nRF24L01_WriteReg(CMD_WRITE +REG_CONFIG, 0X08);							// PTX, CRC enabled, mask a couple of ints
    nRF24L01_WriteReg(CMD_WRITE +REG_SETUP_RETR, 0X00);						//auto retransmit 10
    nRF24L01_WriteReg(CMD_WRITE +REG_SETUP_AW,0X03);	  						//address width = 5
    nRF24L01_WriteReg(CMD_WRITE +REG_RF_SETUP,0X07);	 						//dat rate = 1MB
    nRF24L01_WriteReg(CMD_WRITE +REG_RF_CH,0X02); 							//set channel 2, this is default . 
    //nRF24L01_WriteBuffer(CMD_WRITE +REG_TX_ADDR, &TX_ADDRESS,TX_ADR_LENGTH); // set transmitter addr.        
    nRF24L01_WriteReg(CMD_WRITE +REG_EN_AA, 0X00); 
    CSN = 0;
    cmd = CMD_FLUSH_TX;
    csi20_transceive(&cmd, 1, &rx_buf);
    CSN = 1; 
    
    CSN = 0;
    cmd = CMD_FLUSH_RX;
    csi20_transceive(&cmd, 1, &rx_buf);
    CSN = 1; 
}



/**************************************************************************************/    
void nRF24L01_Configure_RX(void) 
{ 
    uint8_t cmd;
    uint8_t rx_buf;
    CE = 0; 
    /* CRC enabled, PRX */
    nRF24L01_WriteReg(CMD_WRITE +REG_CONFIG,0x09);  
        
    CSN = 0;
    cmd = CMD_FLUSH_TX;
    csi20_transceive(&cmd, 1, &rx_buf);
    CSN = 1; 
    
    CSN = 0;
    cmd = CMD_FLUSH_RX;
    csi20_transceive(&cmd, 1, &rx_buf);
    CSN = 1; 
    
    /* Enable auto-ack for pipe 0 */
    nRF24L01_WriteReg(CMD_WRITE + REG_EN_AA ,0x01); 

    /* Enable RX data pipe 0 */
	  nRF24L01_WriteReg(CMD_WRITE + REG_EN_RXADDR, 0X01); 

    /* Address width of 5 bytes */
    nRF24L01_WriteReg(CMD_WRITE + REG_SETUP_AW ,0x03);

    /* Setup Automatic Re-Transmission
     * Delay before next auto-re-transmit 500 + 86 us
     * Number of retransmits = 10
     */
    nRF24L01_WriteReg(CMD_WRITE + REG_SETUP_RETR, 0x1a); 

    /* RF Settings
     * Data Rate = 1MB
     * RF Power = 0dBm
     * LNA Gain = Max Gain
     */
    nRF24L01_WriteReg(CMD_WRITE + REG_RF_SETUP  ,0x07);
    
    /* RX Payload Width in Data Pipe 0 */
    nRF24L01_WriteReg(CMD_WRITE + REG_RX_PW_P0,RX_PLOAD_WIDTH );
    
    /* Set RF Channel = 2 */
    nRF24L01_WriteReg(CMD_WRITE + REG_RF_CH ,0X02);  
    
    /* Setup TX Address for this device */
    //nRF24L01_WriteBuffer(CMD_WRITE + REG_TX_ADDR, &TX_ADDRESS,TX_ADR_LENGTH);	

    /* Setup RX Address for this device */
    //nRF24L01_WriteBuffer(CMD_WRITE + REG_RX_ADDR_P0, &TX_ADDRESS,TX_ADR_LENGTH);	

    /* Power up in RX Mode */
    nRF24L01_WriteReg(CMD_WRITE + REG_CONFIG,0x0B); 
    CE = 1; 
} 
///********************************************************************************    
//    
//*	Function: Receive_Data()   
//    
//*  Description:    
//  		This function Reads the payload and stored into an array.
//		After reading the payload , the data will send to hyperterminal.
//		finally it will clear the RX FIFO
//    
////**************************************************************************************/   
// 
// 
//void Receive_Data(void) 
//{ 
//    unsigned int j=0; 
//    //Serial_puts("REceived...\n");
//	CE=0; 
//	nRF24L01_ReadBuffer(RD_RX_PLOAD_CMD, &PAYLOAD,RX_PLOAD_WIDTH);//Read RX payload	 cmd 0x61
//
//	#ifdef DEBUG 
//	Serial_puts("R:");
//  	Lcd_cursorPos(2,g_pos);	 
//    	for (j = 0; j < RX_PLOAD_WIDTH; j++) 
//    	{ 
//			Serial_Wr(PAYLOAD[j]);
//			Lcd_putch(PAYLOAD[j]);
//			g_pos++;
//			if(g_pos>=16)
//			{
//	 	 		g_pos=1;
//		 	Lcd_cursorPos(2,g_pos);	 
//		 	}
//    	} 
//	#endif
//    CSN = 0; 
//    SPI_transceive(0xE2);//Flush RX FIFO
//    CSN = 1;
//
//	nRF24L01_WriteReg(CMD_WRITE +STATUS,0x7E);  //ReSET ALL INTR
//	CE=1;
//} 
//   
///************************************************************************************
//*	Function: void nRF24L01_Set_as_TX()
// 
//*  Description:    
//  		This function MAKES THE nRF24L01 to transmittmode.Most of the time the device is in recieve mode.
//		Whenever we want to transmitt a data, make the device into tx mode. 
//		It will clear PRIN_RX BIT in the COFIG register and set pwerup bit in the config reg.   
//    
//*************************************************************************************/
//void nRF24L01_Set_as_TX()
//{
//	CSN = 0; 
//    SPI_transceive(0xE1);  
//    CSN = 1; 
//
//	CSN = 0; 
//    SPI_transceive(0xE2);  
//    CSN = 1; 
//	nRF24L01_WriteReg(CONFIG,0x0A);//config);
// 	CE=0;
//}
// /************************************************************************************
//*	Function:nRF24L01_Set_as_RX())
// 
//*  Description:    
//  		This function MAKES THE nRF24L01 to Receiver mode. 
//		It will set PRIN_RX BIT, pwerup bit in the COFIG register.
//		Make CE =1, to enable reception    
//    
//*************************************************************************************/
//void nRF24L01_Set_as_RX()  
//{ 
//   	CE = 0; 
//    
//	CSN = 0; 
//    SPI_transceive(0xE1);  
//    CSN = 1; 
//
//	CSN = 0; 
//    SPI_transceive(0xE2);  
//    CSN = 1; 
//	nRF24L01_WriteReg(CMD_WRITE +CONFIG,0x0B); //PWR_UP = 1
//    CE = 1; 
//} 


uint8_t nRF24L01_testSPI(void)
{
    uint8_t dat = 0;
    uint8_t datBak = 0;
    uint8_t status = STATUS_PASS;
    
    /* Dummy read
     * First Read always fails
     */
    dat = nRF24L01_ReadReg(CMD_READ + REG_EN_AA);

    /* Read a default value of REG_EN_AA */
    dat = nRF24L01_ReadReg(CMD_READ + REG_EN_AA);

    if (dat != 0x3F)
    {
        return STATUS_FAIL;
    }

    /* Print SPI Read successful */
    uart0_send(p_msgSpiRdPass, 15);

    /* Test SPI Write and Read Back */
    datBak = nRF24L01_ReadReg(CMD_READ + REG_RF_CH);

    /* Write 5 to REG_RF_CH */
    nRF24L01_WriteReg(CMD_WRITE + REG_RF_CH, 5);

    /* Read back REG_RF_CH register */
    dat = nRF24L01_ReadReg(CMD_READ + REG_RF_CH);

    if (dat != 5)
    {
        /* Print message if comparison fails */
        uart0_send(p_msgSpiExp, 17);
        uart0_send(&dat, 1);
        return STATUS_FAIL;
    }

    nRF24L01_WriteReg(CMD_WRITE + REG_RF_CH, datBak);

    /* Print SPI Write successful */
    uart0_send(p_msgSpiWrPass, 16);

}

/**********************************************************   
    
*  Function: nRF24L01_ReadReg(uint8_t dat)  
*  Parameters: uint8_t             
*  Returns: uint8_t  
*  Description:    
  			This function reads one byte from a particular register of the nRF24L01device , 
			and return the byte .   
**********************************************************/  
uint8_t nRF24L01_ReadReg(uint8_t temp)
{
  uint8_t rx_buf;
  uint8_t rx_char = 'H';
  uint8_t regAddr = temp;

  /* Assert the Chip Select for nRF24L01 */
	CSN = 0;

  /* Send out Address of Register 
   * Discard the rx buffer 
   */
  csi20_transceive(&regAddr, 1, &rx_buf);
  
  /* Print H */
  //uart0_send(&rx_char, 1);

  regAddr = CMD_NOP;
  
  /* Send out NOP to read Data from Register */
  csi20_transceive(&regAddr, 1, &rx_buf);
  
  /* Print L */
  //rx_char = 'L';
  //uart0_send(&rx_char, 1);

  /* Deassert the Chip Select for nRF24L01 */
	CSN = 1;

	return(rx_buf);
}
/**********************************************************   
    
*  Function: nRF24L01_WriteReg(uint8_t dat)  
*  Parameters: uint8_t,uint8_t             
*  Returns: uint8_t  
*  Description:    
  			This function write one byte to a particular register of the SPIdevice , 
			and return the byte read  from SPI device during write,
			 according to SPI protocol.   
**********************************************************/  
uint8_t nRF24L01_WriteReg(uint8_t regAddr, uint8_t dat)
{
    uint8_t rx_buf;
    
    /* Assert the Chip Select for nRF24L01 */
    CSN = 0;
    
    /* Send out Address of Register 
     * Discard the rx buffer 
     */
    csi20_transceive(&regAddr, 1, &rx_buf);
    
    /* Data to be written */
    regAddr = dat;
    
    /* Send out Data to Write in Register */
    csi20_transceive(&regAddr, 1, &rx_buf);
    
    /* Deassert the Chip Select for nRF24L01 */
    CSN = 1;
    
    return(rx_buf);
}

/****************************************************************************************   
    
*  Function: nRF24L01_WriteBuffer(uint8_t dat)  
*  Parameters: uint8_t,uint8_t *,uint8_t            
*  Returns: uint8_t  
*  Description:    
  			This function write a block of  byte to particular register of the SPIdevice  
TODO: Loop is very inefficient, driver supports sending out array, but rx array needs to be 
allocation unnecessarily
****************************************************************************************/  
void nRF24L01_WriteBuffer(uint8_t cmd, uint8_t *payload, uint8_t bytes)
{
    uint8_t rx_buf;
    uint8_t dat;
    uint8_t i;
    
    /* Assert the Chip Select for nRF24L01 */
    CSN = 0;
    
    /* Send out Address of Register 
     * Discard the rx buffer 
     */
    csi20_transceive(&cmd, 1, &rx_buf);
    
    /* Send out Data to Write in Register */
    for(i=0; i<bytes; i++)
    {   
        dat = *payload;
        /* Write Register */
        csi20_transceive(&dat, 1, &rx_buf);
        payload++;
    }

    /* Deassert the Chip Select for nRF24L01 */
    CSN = 1;
    

	  //uint8_t i;
	  //CSN = 0;
	  //SPI_transceive(CMD);
	  //for(i=0;i<Bytes;i++)
	  //{
	  //   	SPI_transceive(PAYLOAD[i]);
	  //	//Serial_putUBYTEHex(PAYLOAD[i]);
	  //}
	  //CSN = 1;
}
///****************************************************************************************   
//    
//*  Function: nRF24L01_ReadBuffer(uint8_t dat)  
//*  Parameters: uint8_t,uint8_t *,uint8_t            
//*  Returns: uint8_t  
//*  Description:    
//  			This function write a block of  byte to particular register of the SPIdevice  
//		 
//****************************************************************************************/  
void nRF24L01_ReadBuffer(uint8_t cmd, uint8_t *payload, uint8_t bytes)
{				  
    uint8_t rx_buf;
    uint8_t i;
    uint8_t nop = CMD_NOP;
    
    /* Assert the Chip Select for nRF24L01 */
    CSN = 0;
    
    /* Send out Address of Register 
     * Discard the rx buffer 
     */
    csi20_transceive(&cmd, 1, &rx_buf);
    
    for(i=0; i<bytes; i++)
    {
        /* Read Register */
        csi20_transceive(&nop, 1, &rx_buf);
        *payload = rx_buf;
        payload++;
    }
    
    /* Deassert the Chip Select for nRF24L01 */
    CSN = 1;
}		
