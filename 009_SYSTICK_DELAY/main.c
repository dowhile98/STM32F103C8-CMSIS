#include "stm32f10x.h"

void delay_ms(int n);
int main(void){
	RCC->APB2ENR = 0x4;
	GPIOA->CRL = 0x2;
	int i =10;
	while(1){
		GPIOA->ODR ^= 0x1;
		delay_ms(1+i);
		i+=5;
		if(i==200)
			i = 0;
	}
}

void delay_ms(int n){
	SysTick->LOAD = 72000;
	SysTick->VAL = 0;
	SysTick->CTRL =0b101;
	
	for(int i= 0;i<n;i++){
		while((SysTick->CTRL & 0x10000)==0){
		}
	}
	SysTick->CTRL = 0x0;
}
