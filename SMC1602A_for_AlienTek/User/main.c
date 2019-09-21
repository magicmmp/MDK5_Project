#include "usart.h"
#include "key.h"
#include "delay.h"
#include "sys.h"
#include "smc1602A.h"


int main()
{	
	
	uint8_t line[16];	
	int i;
	int N=0;

	delay_init();
	GPIO_Init_1602();
	initial_1602();
	for(i=16;i<40;i++)
		writeByteToRAM_1602(ROW_1,i,'A'+i-16);
	for(i=16;i<40;i++)
		writeByteToRAM_1602(ROW_2,i,'a'+i-16);		
	
	while(1)
	{		
		for(i=0;i<16;i++)
			line[i]=i+'!';			
		showOn_1602(ROW_1,line);
		
		
		for(i=0;i<16;i++)
			line[i]=i+'!'+16;	
		
		if(N<40)		
			line[15]=readByteFromRAM_1602(ROW_1,N);
		else
			line[15]=readByteFromRAM_1602(ROW_2,N-40);
			
		showOn_1602(ROW_2,line);
		
		delay_ms(500);		
	
		N=(N+1)%80;
	}

}



