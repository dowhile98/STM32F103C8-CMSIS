#include "stm32f10x.h"


void delayMs(uint32_t delay);
int main(void)
{

  RCC->APB2ENR |= ((uint32_t)0x0000000C);  //se habilita el reloj para el puerto A y B
	
  GPIOA->CRL = ((uint32_t)0x80022222);     //se establece como salida los pines PA0-PA4 y PA7 como entrada
	GPIOB->CRH = (uint32_t)0x00022222;			 //se establece como salida los pines PB8-PB13 
	
  while (1) {
		uint16_t readButton = 0x80;
		if(GPIOA->IDR & readButton){
			GPIOB->ODR|=0x1F00;
			GPIOA->ODR&=~(0x001F);
		}
		else{
			GPIOA->ODR|=0x001F;
			GPIOB->ODR&=~(0x1F00);
		}
  }
}
void delayMs(uint32_t delay){
	int i;
	for(;delay>0;delay--){
		for(i = 0;i<3195;i++);
	}
}
