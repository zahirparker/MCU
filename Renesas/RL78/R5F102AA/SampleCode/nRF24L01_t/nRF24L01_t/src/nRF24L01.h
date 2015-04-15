/************************************************************
		File Name:	nRF24L01.H
   
/***************************************************/
//#Define nRF24L01 interrupt flag's 
#define MAX_RT          0x10  // Max #of TX retrans interrupt 
#define TX_DS           0x20  // TX data sent interrupt 
#define RX_DR           0x40  // RX data received 
//****************************************************************// 
// SPI(nRF24L01) commands 
#define CMD_READ                0x00  // Define read command to register 
#define CMD_WRITE               0x20  // Define write command to register 
#define CMD_RD_RX_PLOAD         0x61  // Define RX payload register address 
#define CMD_WR_TX_PLOAD         0xA0  // Define TX payload register address 
#define CMD_FLUSH_TX            0xE1  // Define flush TX register command 
#define CMD_FLUSH_RX            0xE2  // Define flush RX register command 
#define CMD_REUSE_TX_PL         0xE3  // Define reuse TX payload register command 
#define CMD_NOP                 0xFF  // Define No Operation, might be used to read status register 
 
//***************************************************// 
// SPI(nRF24L01) registers(addresses) 
#define REG_CONFIG              0x00  // 'Config' register address 
#define REG_EN_AA               0x01  // 'Enable Auto Acknowledgment' register address 
#define REG_EN_RXADDR           0x02  // 'Enabled RX addresses' register address 
#define REG_SETUP_AW            0x03  // 'Setup address width' register address 
#define REG_SETUP_RETR          0x04  // 'Setup Auto. Retrans' register address 
#define REG_RF_CH               0x05  // 'RF channel' register address 
#define REG_RF_SETUP            0x06  // 'RF setup' register address 
#define REG_STATUS              0x07  // 'Status' register address 
#define REG_OBSERVE_TX          0x08  // 'Observe TX' register address 
#define REG_CD                  0x09  // 'Carrier Detect' register address 
#define REG_RX_ADDR_P0          0x0A  // 'RX address pipe0' register address 
#define REG_RX_ADDR_P1          0x0B  // 'RX address pipe1' register address 
#define REG_RX_ADDR_P2          0x0C  // 'RX address pipe2' register address 
#define REG_RX_ADDR_P3          0x0D  // 'RX address pipe3' register address 
#define REG_RX_ADDR_P4          0x0E  // 'RX address pipe4' register address 
#define REG_RX_ADDR_P5          0x0F  // 'RX address pipe5' register address 
#define REG_TX_ADDR             0x10  // 'TX address' register address 
#define REG_RX_PW_P0            0x11  // 'RX payload width, pipe0' register address 
#define REG_RX_PW_P1            0x12  // 'RX payload width, pipe1' register address 
#define REG_RX_PW_P2            0x13  // 'RX payload width, pipe2' register address 
#define REG_RX_PW_P3            0x14  // 'RX payload width, pipe3' register address 
#define REG_RX_PW_P4            0x15  // 'RX payload width, pipe4' register address 
#define REG_RX_PW_P5            0x16  // 'RX payload width, pipe5' register address 
#define REG_FIFO_STATUS         0x17  // 'FIFO Status Register' register address 


   
#define CONFIG_PWR_UP		    0x02
#define	CONFIG_PRIM_RX		  0x01
#define TX_PLOAD_WIDTH      1//32 		//32   5 byte payload
#define RX_PLOAD_WIDTH      1//32 		//32   5 byte payload
#define TX_ADR_LENGTH       5 
#define RX_AW               5 

//extern unsigned char TX_PAYLOAD[TX_PLOAD_WIDTH];// = {0x41,0x42,0x43,0x44,0x45};
//extern unsigned char PAYLOAD[RX_PLOAD_WIDTH];

 //Function Prototypes


void nRF24L01_Configure_TX(void);  		
void nRF24L01_Configure_RX(void) ;
void Transmit_Data();
void Receive_Data();
void nRF24L01_Set_as_TX();
void nRF24L01_Set_as_RX();  
void nRF24L01_WriteBuffer(unsigned char CMD,unsigned char [],unsigned char Bytes);
uint8_t nRF24L01_testSPI(void);
unsigned char nRF24L01_WriteReg(unsigned char reg, unsigned char dat);
void nRF24L01_ReadBuffer(unsigned char ,unsigned char *,unsigned char);//Read RX payload
unsigned char nRF24L01_ReadReg(unsigned char reg) ;
   


 


