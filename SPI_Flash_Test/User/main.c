#include "stm32f10x.h"
#include "bsp_usart_dma.h"
#include "bsp_spi_flash.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"

 
 

/* 获取缓冲区的长度 */
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

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\n";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

 


/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
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
 *  1、
 *  2、同时串口可接收电脑串口助手发来的一串英文字符，显示在mini12864上。
 */
 

int main(void)
{  	
	 uint16_t i;
	 
	
	
	 LED_Init();
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 delay_init();
	
	
	/* 初始化USART */
	 USART_Config(); 
	
	 printf("\r\n 这是一个16Mbyte串行flash(W25Q128)实验 \r\n");
	 
	
	

	
	
	
	/* 16M串行flash W25Q128初始化 */
	SPI_FLASH_Init();
	
	/* 获取 Flash Device ID */
	DeviceID = FLASH_ReadDeviceID();	
	delay_ms( 10 );
	
	
	/* 获取 SPI Flash ID */
	FlashID = FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID)
	{	
		printf("\r\n 检测到串行flash W25Q128 !\r\n");
		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		// 这里擦除4K，即一个扇区，擦除的最小单位是扇区
		FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		// 这里写一页，一页的大小为256个字节
		FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
		
		
		printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);
		
		
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);		
		
		printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{ 
			LED0=1;
			printf("\r\n 16M串行flash(W25Q128)测试成功!\n\r");
		}
		else
		{        
			LED1=1;
			printf("\r\n 16M串行flash(W25Q128)测试失败!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else// if (FlashID == sFLASH_ID)
	{ 
		LED0=0;
		LED1=0;
		printf("\r\n 获取不到 W25Q64 ID!\n\r");
	}
	
	
	
	//例如同时控制LED
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
 
