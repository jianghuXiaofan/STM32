#include "bsp_spi1.h"
#include "bsp_cc1101.h"



static SPI_InitTypeDef SPI_InitStructure;
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ���ݳ���
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//ʱ�ӿ�������
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//�������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//Ԥ��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1,&SPI_InitStructure);
                  
	SPI_Cmd(SPI1,ENABLE);
	SPI1_ReadWriteByte(0XFF);
}

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
void SPI1_SetSpeed(uint8_t SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet;
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPI1_ReadWriteByte(u8 TxData)
{
	uint8_t retry = 0;
	SPI_I2S_SendData(SPI1,TxData);//ͨ�����跢��һ������
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET)//���ָ����SPI��־λ������񣻷��ͻ���ձ�־λ
	{                                
		retry++;
		if(retry>200) return 0;
	}
	retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET)//���ջ���Ϊ�ձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}
	return SPI_I2S_ReceiveData(SPI1);
}

