#include "stm32f10x.h"


void delayMs(uint32_t delay);
uint16_t n;
int main(void)
{
	
  RCC->APB2ENR = ((uint32_t)0x00000004);
	
	GPIOA->CRL = ((uint32_t)0x00022222);
	GPIOA->BSRR =0x3;										//los bits 0 a 15 son de establecer
	delayMs(1000);											
	GPIOA->BSRR=3<<16;									//los bits 16 a 31 son de restablecer
	uint32_t time = 0;
  while (1) {
		n++;
		GPIOA->BSRR =0x1;
		delayMs(time+100);
		GPIOA->BSRR=1<<16;
		delayMs(time);
		GPIOA->BSRR=0x2;
		delayMs(time+100);
		GPIOA->BSRR=1<<17;
		delayMs(time);
		GPIOA->BSRR =0x3;										//los bits 0 a 15 son de establecer
		delayMs(time);											
		GPIOA->BSRR=3<<16;
		delayMs(time+1000);
		time+=10;
		if(time==50)
			time = 0;
  }
}
void delayMs(uint32_t delay){
	int i;
	for(;delay>0;delay--){
		for(i = 0;i<3195;i++);
	}
}