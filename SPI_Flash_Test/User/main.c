#include "stm32f10x.h"
#include "bsp_usart_dma.h"
#include "bsp_spi_flash.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"

 
 

/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     16773120
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

 
typedef enum 
{ 
	FAILED = 0, 
	PASSED = !FAILED
} TestStatus;     

/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "��л��ѡ��Ұ��stm32������\r\n";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

 


/*
 * ��������Buffercmp
 * ����  ���Ƚ������������е������Ƿ����
 * ����  ��-pBuffer1     src������ָ��
 *         -pBuffer2     dst������ָ��
 *         -BufferLength ����������
 * ���  ����
 * ����  ��-PASSED pBuffer1 ����   pBuffer2
 *         -FAILED pBuffer1 ��ͬ�� pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}
 
 
 
 
 /**
 *  1��
 *  2��ͬʱ���ڿɽ��յ��Դ������ַ�����һ��Ӣ���ַ�����ʾ��mini12864�ϡ�
 */
 

int main(void)
{  	
	 uint16_t i;
	 
	
	
	 LED_Init();
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 delay_init();
	
	
	/* ��ʼ��USART */
	 USART_Config(); 
	
	 printf("\r\n ����һ��16Mbyte����flash(W25Q128)ʵ�� \r\n");
	 
	
	

	
	
	
	/* 16M����flash W25Q128��ʼ�� */
	SPI_FLASH_Init();
	
	/* ��ȡ Flash Device ID */
	DeviceID = FLASH_ReadDeviceID();	
	delay_ms( 10 );
	
	
	/* ��ȡ SPI Flash ID */
	FlashID = FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* ���� SPI Flash ID */
	if (FlashID == sFLASH_ID)
	{	
		printf("\r\n ��⵽����flash W25Q128 !\r\n");
		
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		// �������4K����һ����������������С��λ������
		FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* �����ͻ�����������д��flash�� */
		// ����дһҳ��һҳ�Ĵ�СΪ256���ֽ�
		FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
		
		
		printf("\r\n д�������Ϊ��%s \r\t", Tx_Buffer);
		
		
		
		/* ���ո�д������ݶ������ŵ����ջ������� */
		FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);		
		
		printf("\r\n ����������Ϊ��%s \r\n", Rx_Buffer);
		/* ���д�������������������Ƿ���� */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{ 
			LED0=1;
			printf("\r\n 16M����flash(W25Q128)���Գɹ�!\n\r");
		}
		else
		{        
			LED1=1;
			printf("\r\n 16M����flash(W25Q128)����ʧ��!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else// if (FlashID == sFLASH_ID)
	{ 
		LED0=0;
		LED1=0;
		printf("\r\n ��ȡ���� W25Q64 ID!\n\r");
	}
	
	
	
	//����ͬʱ����LED
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{	
			ClearScreen();			
			WriteEn(0,0,USART_RX_BUF);
			USART_RX_STA=0;
			LED0=!LED0;
			LED1=!LED1;
		}
		
	}
}

//}
 
