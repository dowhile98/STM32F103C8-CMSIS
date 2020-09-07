/*salida de comparacion del timer*/

#include "stm32f10x.h"
/*PROTOTIPO DE FUNCIOES*/
void SystemClock_Config(void);
void TIM2_Init(void);
void TIM2_IRQCallback(void);
/*fin*/
/*definicion de variables globales*/
uint32_t pulse_ch1 = 625;
uint32_t pulse_ch2 = 1250;
uint32_t pulse_ch3 = 500;
uint32_t pulse_ch4 = 1000;
uint32_t CCRx_value;
int main(void){
	SystemClock_Config();
	/*se inicializa el timer2*/
	TIM2_Init();
	while(1);
}

void SystemClock_Config(void){
	RCC->CR = RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	RCC->CFGR = RCC_CFGR_SW_HSE;
}
/*funcion que inicaliza los parametros el timer2*/
void TIM2_Init(void){
	//1. se habilita el reloj para time2 y el gpioa por los pines asociados a los canales
	RCC->APB1ENR = RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN|RCC_APB2ENR_AFIOEN;
	//2. Se asocia los pines para la comparacion de salida ch1-ch4
	GPIOA->CRL = 0x4444BBBB;					//se configura los pines PA0-PA3 como funcion alternativa push pull
	//3. se configura los parametros del timer2
	TIM2->PSC = 800-1;
	TIM2->ARR = 0xFFFF;
	TIM2->CCMR1 = 0x3030;
	TIM2->CCMR2 = 0x3030;
	TIM2->CCER = 0x1111;						//se activa la señal de salida para cada canal
	//3.se activa los banderas para las interrupciones
	TIM2->DIER = 0x1E;
	//4. se habilita las interrupciones
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn,2);
	//5. se precargan valores a los registro CCR1-CCR4
	TIM2->CCR1 = pulse_ch1;
	TIM2->CCR2 = pulse_ch2;
	TIM2->CCR3 = pulse_ch3;
	TIM2->CCR4 = pulse_ch4;
	//6. se habilita el conteo
	TIM2->CR1 = TIM_CR1_CEN;
}
//funcion que maneja la interrupcion del timer2
void TIM2_IRQHandler(void){
	TIM2_IRQCallback();
	TIM2->SR = 0x0;							//este programa tiene ciertos errores con algunos valores de los pulsos, tendrá que trabajar en una funcion que lo evite
}
void TIM2_IRQCallback(void){
	if(TIM2->SR & TIM_SR_CC1IF){
		CCRx_value = TIM2->CCR1;
		TIM2->CCR1 = CCRx_value + pulse_ch1;
	}
	if(TIM2->SR & TIM_SR_CC2IF){
		CCRx_value = TIM2->CCR2;
		TIM2->CCR2 = CCRx_value + pulse_ch2;
	}
	if(TIM2->SR & TIM_SR_CC3IF){
		CCRx_value = TIM2->CCR3;
		TIM2->CCR3 = CCRx_value + pulse_ch3;
	}
	if(TIM2->SR & TIM_SR_CC4IF){
		CCRx_value = TIM2->CCR4;
		TIM2->CCR4 = CCRx_value + pulse_ch4;
	}
}