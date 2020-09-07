// RCC_CR para configurar el reloj

#include "stm32f10x.h"                  // Device header

void delay_ms(int n);
void sysclk_Init();
void GPIO_init();

int main(void){
	sysclk_Init();
	GPIO_init();
	
	while(1){
		GPIOA->ODR ^= 0x1;
		delay_ms(50);
	}
}

void sysclk_Init(){
	RCC->CR = 1<<16;											//Bit 16 HSEON: HSE clock enable
	while((RCC->CR & (1<<17))==0){				//Bit 17 HSERDY: External high-speed clock ready flag
	}																			//1: HSE oscillator ready
	RCC->CFGR = 0x1;											//Bits 1:0 SW[1:0]: System clock Switch
																				//01: HSE selected as system clock
}
void GPIO_init(){
	RCC->APB2ENR = 0x4;
	GPIOA->CRL = 0x2;
}
void delay_ms(int n){
	SysTick->LOAD = 8000;
	SysTick->VAL = 0;
	SysTick->CTRL = 0b101;
	
	for(int i = 0;i<n;i++){
		while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG)==0){}
	}
	SysTick->CTRL = 0x0;
}