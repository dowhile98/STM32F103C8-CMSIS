/*GENERADOR DE BASE DE TIEMPO*/
#include "stm32f10x.h"

/*PROTOTIPO DE FUNCIONES*/
void SystemClock_Config(void);							//funcion que configura el reloj del MCU
void GPIO_Init(void);												//funcion para inicializar pines GPIO
void TIM2_Init(void);
void LED_Play(uint8_t state);

/*VARIABLES GLOBALES*/
uint8_t count = 0x1;

int main(void){
	/*inicializacin del reloj HSE->8MHz*/
	SystemClock_Config();
	/*inicializacion del pin GPIO en uso*/
	GPIO_Init();
	/*Se configura el timer*/
	TIM2_Init();															//actualizacion de evento cada 150ms
	
	while(1){
		LED_Play(count);
	}
}

void SystemClock_Config(void){
	RCC->CR = RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	RCC->CFGR = RCC_CFGR_SW_HSE;
}

void GPIO_Init(void){
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN;
	GPIOA->CRL = 0x44442222;									//se inicializa el pin PA0 como salida push pull
}

void TIM2_Init(void){
	RCC->APB1ENR = RCC_APB1ENR_TIM2EN;				//se activa el reloj para el timer2
	TIM2->PSC = 999;													//actualizacion de interrupcion cada 250ms
	TIM2->ARR = 799;
	TIM2->CR1 = TIM_CR1_CEN;									//se activa el conteo
	TIM2->DIER = TIM_DIER_UIE;								//se activa la bandera que generar la interrupcion
	NVIC_EnableIRQ(TIM2_IRQn);								//se activa la interrupcion del timer2
	
}
/*FUNCIO QUE GESTIONA LA INTERRUPCION DEL TIM2*/
void TIM2_IRQHandler(void){
	TIM2->SR = 0x0;
	count++;
}

void LED_Play(uint8_t state){
	if(state<5){
		switch(state){
			case 0: GPIOA->ODR = 0b0000; break;
			case 1: GPIOA->ODR = 0b0001; break;
			case 2: GPIOA->ODR = 0b0011; break;
			case 3: GPIOA->ODR = 0b0111; break;
			case 4: GPIOA->ODR = 0b1111; break;
		}
	}
	else{
		count = 0;
	}
}
