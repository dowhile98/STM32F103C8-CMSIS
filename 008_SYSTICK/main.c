 #include "stm32f10x.h"
 

 int main(void){
	 RCC->APB2ENR = 0x4;
	 GPIOA->CRL = 0x2;
	 
	 //SysTick config
	 SysTick->LOAD = 8000000-1;
	 SysTick->VAL = 0;
	 SysTick->CTRL = 0b101;
	 while(1){
		 if(SysTick->CTRL & 0x10000)
			 GPIOA->ODR ^=0x1;
	 }
 }
 

