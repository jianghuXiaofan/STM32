#include "bsp_cc1101.h"
#include "bsp_spi1.h"


//10, 7, 5, 0, -5, -10, -15, -20, dbm output power, 0x12 == -30dbm
uint8_t PaTabel[] = { 0xc0, 0xC8, 0x84, 0x60, 0x68, 0x34, 0x1D, 0x0E};

// Sync word qualifier mode = 30/32 sync word bits detected 
// CRC autoflush = false 
// Channel spacing = 199.951172 
// Data format = Normal mode 
// Data rate = 2.00224 
// RX filter BW = 58.035714 
// PA ramping = false 
// Preamble count = 4 
// Whitening = false 
// Address config = No address check 
// Carrier frequency = 400.199890 
// Device address = 0 
// TX power = 10 
// Manchester enable = false 
// CRC enable = true 
// Deviation = 5.157471 
// Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Packet length = 255 
// Modulation format = GFSK 
// Base frequency = 399.999939 
// Modulated = true 
// Channel number = 1 
// PA table 
#define PA_TABLE {0xc2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,}

const uint8_t CC1101InitData[22][2]= 
{
  {CC1101_IOCFG0,      0x06},
  {CC1101_FIFOTHR,     0x47},
  {CC1101_PKTCTRL0,    0x05},
  {CC1101_CHANNR,      0x01},
  {CC1101_FSCTRL1,     0x06},
  {CC1101_FREQ2,       0x0F},
  {CC1101_FREQ1,       0x62},
  {CC1101_FREQ0,       0x76},
  {CC1101_MDMCFG4,     0xF6},
  {CC1101_MDMCFG3,     0x43},
  {CC1101_MDMCFG2,     0x13},
  {CC1101_DEVIATN,     0x15},
  {CC1101_MCSM0,       0x18},
  {CC1101_FOCCFG,      0x16},
  {CC1101_WORCTRL,     0xFB},
  {CC1101_FSCAL3,      0xE9},
  {CC1101_FSCAL2,      0x2A},
  {CC1101_FSCAL1,      0x00},
  {CC1101_FSCAL0,      0x1F},
  {CC1101_TEST2,       0x81},
  {CC1101_TEST1,       0x35},
  {CC1101_MCSM1,       0x3B},
};

extern uint8_t SPI1_ReadWriteByte(uint8_t TxData);

/*Initialize the CC1101, User can modify it*/
void CC1101Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	CC_CSN_Clk|CC_IRQ_Clk|CC_GDO2_Clk, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = CC_CSN_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CC_CSN_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CC_IRQ_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CC_IRQ_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CC_GDO2_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CC_GDO2_Port, &GPIO_InitStructure);
	CC_CSN_H();//片选拉高
	SPI1_Init();
	//下面是cc1101的初始化
	volatile uint8_t i, j;

    CC1101Reset();    
    
    for( i = 0; i < 22; i++ )
    {
        CC1101WriteReg( CC1101InitData[i][0], CC1101InitData[i][1] );
    }
    CC1101SetAddress( 0x05, BROAD_0AND255 );
    CC1101SetSYNC( 0x8799 );
    CC1101WriteReg(CC1101_MDMCFG1,0x72); //Modem Configuration

    CC1101WriteMultiReg(CC1101_PATABLE, PaTabel, 8 );

    i = CC1101ReadStatus( CC1101_PARTNUM );//for test, must be 0x80
    i = CC1101ReadStatus( CC1101_VERSION );//for test, refer to the datasheet
}

/*read a byte from the specified register*/
/*
================================================================================
Function : CC1101ReadReg( )
    read a byte from the specified register
INPUT    : addr, The address of the register
OUTPUT   : the byte read from the rigister
================================================================================
*/
uint8_t CC1101ReadReg(uint8_t addr)
{
	uint8_t i;
	CC_CSN_L();
	SPI1_ReadWriteByte(addr | READ_SINGLE);
	i = SPI1_ReadWriteByte(0xFF);
	CC_CSN_H();
	return i;
}
/*
================================================================================
Function : CC1101ReadMultiReg( )
    Read some bytes from the rigisters continously
INPUT    : addr, The address of the register
           buff, The buffer stores the data
           size, How many bytes should be read
OUTPUT   : None
================================================================================
*/
void CC1101ReadMultiReg( uint8_t addr, uint8_t *buff, uint8_t size )
{
    uint8_t i, j;
    CC_CSN_L();
    SPI1_ReadWriteByte( addr | READ_BURST);
    for( i = 0; i < size; i ++ )
    {
        for( j = 0; j < 20; j ++ );
        *( buff + i ) = SPI1_ReadWriteByte( 0xFF );
    }
    CC_CSN_H();
}


/*
================================================================================
Function : CC1101ReadStatus( )
    Read a status register
INPUT    : addr, The address of the register
OUTPUT   : the value read from the status register
================================================================================
*/
uint8_t CC1101ReadStatus(uint8_t addr)
{
	uint8_t i;
    CC_CSN_L();
    SPI1_ReadWriteByte( addr | READ_BURST);
    i = SPI1_ReadWriteByte( 0xFF );
    CC_CSN_H();
    return i;
}

/*
================================================================================
Function : CC1101SetTRMode( )
    Set the device as TX mode or RX mode
INPUT    : mode selection
OUTPUT   : None
================================================================================
*/
void CC1101SetTRMode(TRMODE mode)
{
	if(mode == TX_MODE)
	{
        CC1101WriteReg(CC1101_IOCFG0,0x46);
        CC1101WriteCmd( CC1101_STX );
    }
    else if( mode == RX_MODE )
    {
        CC1101WriteReg(CC1101_IOCFG0,0x46);
        CC1101WriteCmd( CC1101_SRX );
    }
}

/*
================================================================================
Function : CC1101WriteReg( )
    Write a byte to the specified register
INPUT    : addr, The address of the register
           value, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteReg(uint8_t addr, uint8_t value)
{
    CC_CSN_L();
    SPI1_ReadWriteByte( addr );
    SPI1_ReadWriteByte( value );
    CC_CSN_H();
}

/*
================================================================================
Function : CC1101WriteMultiReg( )
    Write some bytes to the specified register
INPUT    : addr, The address of the register
           buff, a buffer stores the values
           size, How many byte should be written
OUTPUT   : None
================================================================================
*/
void CC1101WriteMultiReg( uint8_t addr, uint8_t *buff, uint8_t size )
{
    uint8_t i;
    CC_CSN_L();
    SPI1_ReadWriteByte( addr | WRITE_BURST );
    for( i = 0; i < size; i ++ )
    {
        SPI1_ReadWriteByte( *( buff + i ) );
    }
    CC_CSN_H();
}
/*
================================================================================
Function : CC1101WriteCmd( )
    Write a command byte to the device
INPUT    : command, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteCmd( uint8_t command )
{
    CC_CSN_L();
    SPI1_ReadWriteByte( command );
    CC_CSN_H();
}

/*
================================================================================
Function : CC1101Reset( )
    Reset the CC1101 device
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101Reset( void )
{
    uint8_t x;

    CC_CSN_H();
    CC_CSN_L();
    CC_CSN_H();
    for( x = 0; x < 100; x ++ );
    CC1101WriteCmd( CC1101_SRES );
}

/*
================================================================================
Function : CC1101SetIdle( )
    Set the CC1101 into IDLE mode
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101SetIdle( void )
{
    CC1101WriteCmd(CC1101_SIDLE);
}
/*
================================================================================
Function : CC1101ClrTXBuff( )
    Flush the TX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrTXBuff( void )
{
    CC1101SetIdle();//MUST BE IDLE MODE
    CC1101WriteCmd( CC1101_SFTX );
}
/*
================================================================================
Function : CC1101ClrRXBuff( )
    Flush the RX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrRXBuff( void )
{
    CC1101SetIdle();//MUST BE IDLE MODE
    CC1101WriteCmd( CC1101_SFRX );
}
/*
================================================================================
Function : CC1101SendPacket( )
    Send a packet
INPUT    : txbuffer, The buffer stores data to be sent
           size, How many bytes should be sent
           mode, Broadcast or address check packet
OUTPUT   : None
================================================================================
*/
void CC1101SendPacket( uint8_t *txbuffer, uint8_t size, TX_DATA_MODE mode )
{
    uint8_t address;
    if( mode == BROADCAST )             { address = 0; }
    else if( mode == ADDRESS_CHECK )    { address = CC1101ReadReg( CC1101_ADDR ); }

    CC1101ClrTXBuff( );
    
    if( ( CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03 ) != 0 )
    {
        CC1101WriteReg( CC1101_TXFIFO, size + 1 );
        CC1101WriteReg( CC1101_TXFIFO, address );
    }
    else
    {
        CC1101WriteReg( CC1101_TXFIFO, size );
    }

    CC1101WriteMultiReg( CC1101_TXFIFO, txbuffer, size );
    CC1101SetTRMode( TX_MODE );
    while( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_2 ) != 0 );
    while( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_2 ) == 0 );

    CC1101ClrTXBuff( );
}

/*
================================================================================
Function : CC1101GetRXCnt( )
    Get received count of CC1101
INPUT    : None
OUTPUT   : How many bytes hae been received
================================================================================
*/
uint8_t CC1101GetRXCnt( void )
{
    return ( CC1101ReadStatus( CC1101_RXBYTES )  & BYTES_IN_RXFIFO );
}
/*
================================================================================
Function : CC1101SetAddress( )
    Set the address and address mode of the CC1101
INPUT    : address, The address byte
           AddressMode, the address check mode
OUTPUT   : None
================================================================================
*/
void CC1101SetAddress( uint8_t address, ADDR_MODE AddressMode)
{
    uint8_t btmp = CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03;
    CC1101WriteReg(CC1101_ADDR, address);
    if     ( AddressMode == BROAD_ALL )     {}
    else if( AddressMode == BROAD_NO  )     { btmp |= 0x01; }
    else if( AddressMode == BROAD_0   )     { btmp |= 0x02; }
    else if( AddressMode == BROAD_0AND255 ) { btmp |= 0x03; }   
}
/*
================================================================================
Function : CC1101SetSYNC( )
    Set the SYNC bytes of the CC1101
INPUT    : sync, 16bit sync 
OUTPUT   : None
================================================================================
*/
void CC1101SetSYNC( uint16_t sync )
{
    CC1101WriteReg(CC1101_SYNC1, 0xFF & ( sync>>8 ) );
    CC1101WriteReg(CC1101_SYNC0, 0xFF & sync ); 
}
/*
================================================================================
Function : CC1101RecPacket( )
    Receive a packet
INPUT    : rxBuffer, A buffer store the received data
OUTPUT   : 1:received count, 0:no data
================================================================================
*/
uint8_t CC1101RecPacket( uint8_t *rxBuffer )
{
    uint8_t status[2];
    uint8_t pktLen;
    uint16_t x , j = 0;

    if ( CC1101GetRXCnt( ) != 0 )
    {
        pktLen = CC1101ReadReg(CC1101_RXFIFO);           // Read length byte
        if( ( CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03 ) != 0 )
        {
            x = CC1101ReadReg(CC1101_RXFIFO);
        }
        if( pktLen == 0 )           { return 0; }
        else                        { pktLen --; }
        CC1101ReadMultiReg(CC1101_RXFIFO, rxBuffer, pktLen); // Pull data
        CC1101ReadMultiReg(CC1101_RXFIFO, status, 2);   // Read  status bytes

        CC1101ClrRXBuff( );

        if( status[1] & CRC_OK ) 
{   return pktLen; }
        else                     
{   return 0; }
    }
    else   {  return 0; }                               // Error
}


/*
================================================================================
Function : CC1101Init( )
    Initialize the CC1101, User can modify it
INPUT    : None
OUTPUT   : None
================================================================================
*/
//void CC1101Init( void )
//{
//    volatile uint8_t i, j;

//    CC1101Reset( );    
//    
//    for( i = 0; i < 22; i++ )
//    {
//        CC1101WriteReg( CC1101InitData[i][0], CC1101InitData[i][1] );
//    }
//    CC1101SetAddress( 0x05, BROAD_0AND255 );
//    CC1101SetSYNC( 0x8799 );
//    CC1101WriteReg(CC1101_MDMCFG1,   0x72); //Modem Configuration

//    CC1101WriteMultiReg(CC1101_PATABLE, PaTabel, 8 );

//    i = CC1101ReadStatus( CC1101_PARTNUM );//for test, must be 0x80
//    i = CC1101ReadStatus( CC1101_VERSION );//for test, refer to the datasheet
//}
