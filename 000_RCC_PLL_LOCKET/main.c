#include "stm32f10x.h"                  // Device header

void delay_ms(int n);

int main(void){
	RCC->CR = 1<<16;
	while((RCC->CR & 1<<17)==0);
	RCC->CFGR = 1<<16;												// seleccione HSI / 2 para PLL
	RCC-> CFGR |= RCC_CFGR_PLLMULL4;								// mult por 9
	RCC-> CFGR =1<<24;
	while (!(RCC-> CR & (1 << 25)));								// espera hasta que esté bloqueado
	RCC->CFGR |= 0x2;
	RCC->APB2ENR = 0x4;
	GPIOA->CRL = 0x2;
	
	
	for(;;){
		GPIOA->ODR ^= 0x1;
		delay_ms(10000);
	}
}

void delay_ms(int n){
	SysTick->LOAD = 64000;
	SysTick->VAL = 0;
	SysTick->CTRL =0b101;
	
	for(int i= 0;i<n;i++){
		while((SysTick->CTRL & 0x10000)==0){
		}
	}
	SysTick->CTRL = 0x0;
}
