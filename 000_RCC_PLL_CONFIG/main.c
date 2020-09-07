#include "stm32f10x.h"
void delay_ms(int n);
void RCC_Init();
void GPIO_Init();
int main(void){
	
	RCC_Init();
	GPIO_Init();
	
	while(1){
		GPIOA->ODR ^=0x1;
		delay_ms(200);
	}
}
void RCC_Init(){
	RCC->CR = RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY)==0){}
	RCC->CFGR = 1<<16; 										//Bit 16 PLLSRC: PLL entry clock source
	RCC->CFGR |=RCC_CFGR_PLLMULL9;
	RCC->CFGR |=RCC_CFGR_PPRE1_DIV2;			//Bits 10:8 PPRE1[2:0]: APB Low-speed prescaler (APB1)
	RCC->CR |=1<<24;											//Bit 24 PLLON: PLL enable
	while((RCC->CR & RCC_CR_PLLRDY)==0){}
	RCC->CFGR |=0x2;											//Bits 1:0 SW[1:0]: System clock Switch
}

void GPIO_Init(){
	RCC->APB2ENR = 0x4;										//Se habilita el reloj para el IOPORTA
	GPIOA->CRL = 0x2;
}
void delay_ms(int n){
	SysTick->LOAD = 72000;
	SysTick->VAL = 0;
	SysTick->CTRL = 0b101;
	
	for(int i = 0;i<n;i++){
		while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG)==0){}
	}
	SysTick->CTRL = 0x0;
}