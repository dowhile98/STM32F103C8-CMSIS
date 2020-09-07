#include "stm32f10x.h"

void delay_ms(int n);
void sysclk_Init();
int main(void){
	sysclk_Init();
	RCC->APB1ENR = RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL = 0x2A;
	
	//SE CONFIGURA EL TIMER
	TIM2->PSC = 8000-1;
	TIM2->ARR = 1000-1;
	TIM2->CCMR1 = 0x30;					//Bits 6:4 OC1M: Output compare 1 mode
	TIM2->CCER |=0x1;						//Bit 0 CC1E: Capture/Compare 1 output enable
	TIM2->CNT = 0;
	TIM4->CR1 = 1;							//Bit 0 CEN: Counter enable
	while(1){
		GPIOA->ODR ^= 0x2;
		delay_ms(1000);
	}
}

void sysclk_Init(){
	RCC->CR = 1<<16;						//activa el reloj externo
	while((RCC->CR & 1<<17)==0);//espera a que el reloj este listo
	/*RCC->CFGR = 1<<16;				//activa la entrada PLL
	RCC->CFGR |= 7<<18;					//Multiplica la entrada PLL por 9
	RCC->CFGR |= 4<<8;					//divide el reloj por 2 para APB1
	RCC->CR |=1<<24;						//se habilita el PLL
	while((RCC->CR & 1<<25)==0);*/
	RCC->CFGR = 0x1;						//Se selecciona el PLL como fuente de reloj
}
void delay_ms(int n){
	SysTick->LOAD = 8000-1;
	SysTick->VAL = 0;
	SysTick->CTRL =0b101;
	
	for(int i= 0;i<n;i++){
		while((SysTick->CTRL & 0x10000)==0){
		}
	}
	SysTick->CTRL = 0x0;
}