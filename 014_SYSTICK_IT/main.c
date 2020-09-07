#include "stm32f10x.h"                  // Device header

/*PROTOTIPO DE FUNCIONES*/
void SystemClock_Config(void);
void SysTick_Setup(uint32_t load_value);
void GPIO_Init(void);

int main(void){
	SystemClock_Config();
	SysTick_Setup(8000000*0.05);					//SystemClk_Frec * Periodo
	GPIO_Init();
	
	while(1){
	}
}

void SystemClock_Config(void){
	RCC->CR = RCC_CR_HSEON;											//Bit 16 HSEON: HSE clock enable
	while((RCC->CR & RCC_CR_HSERDY)==0);				//se espeara hasta el HSE este listo
													
	RCC->CFGR = RCC_CFGR_SW_HSE;								//Se selecciona el HSE como fuente de reloj del sytema
}

void SysTick_Setup(uint32_t load_value){
	SysTick->LOAD = load_value-1;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x7;
	NVIC_EnableIRQ(SysTick_IRQn);					//se activa la interrupcion
}
void GPIO_Init(void){
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN;		//Se activa el reloj para el GPIOA
	GPIOA->CRL = 0x2;
}
void SysTick_Handler(void){
	GPIOA->ODR ^= 0x1;										//se cambia el estado del pin
}
