/*GENERADOR DE BASE DE TIEMPO*/
#include "stm32f10x.h"

/*PROTOTIPO DE FUNCIONES*/
void SystemClock_Config(void);							//funcion que configura el reloj del MCU
void GPIO_Init(void);												//funcion para inicializar pines GPIO
void TIM2_Init(void);

int main(void){
	/*inicializacin del reloj HSE->8MHz*/
	SystemClock_Config();
	/*inicializacion del pin GPIO en uso*/
	GPIO_Init();
	/*Se configura el timer*/
	TIM2_Init();
	
	while(1){
		while(!(TIM2->SR & TIM_SR_UIF));				//simula una funcion delay
		GPIOA->ODR ^=0x1;
		TIM2->SR = 0;
	}
}

void SystemClock_Config(void){
	RCC->CR = RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	RCC->CFGR = RCC_CFGR_SW_HSE;
}

void GPIO_Init(void){
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN;
	GPIOA->CRL = 0x2;													//se inicializa el pin PA0 como salida push pull
}

void TIM2_Init(void){
	RCC->APB1ENR = RCC_APB1ENR_TIM2EN;				//se activa el reloj para el timer2
	TIM2->PSC = 1999;			
	TIM2->ARR = 599;
	TIM2->CR1 = TIM_CR1_CEN;									//se activa el conteo
}
