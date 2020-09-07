/*este programa calcula la frecuencia de entrada.
*se usa el timer2 ->ch1. configurando el timer como captura de entrada.
*@PA0 entrada del canal 1
*la frecuencia medida de muestra por el puerto uart1 
*/
#include "stm32f10x.h"
#include <stdio.h>

/*PROTOTIPO DE FUNCIONES*/
void SystemClock_Config(void);
void UART1_Init(void);
void TIM2_Init(void);
void delayMs(int n);

/*fin*/

/*definicion de variables globales*/
uint32_t input_capture[2] = {0};
uint8_t count = 1;
double freq_ic = 0.0;
double diferencia;
/*fin*/

int main(void){
	//se configura el reloj del mcu
	SystemClock_Config();
	//se configura el usart1 
	UART1_Init();
	//se configura el timer2 para la captura de entrada
	TIM2_Init();
	printf("configuracion exitosa\r\n");
	while(1){
		printf("la frecuencia medida es->%.2f Hz\r\n",freq_ic);
		delayMs(100);
	}
}

/*DEFINICION DE FUNCIONES*/
void SystemClock_Config(void){
	RCC->CR = RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	RCC->CFGR = RCC_CFGR_SW_HSE;
}

void UART1_Init(void){
	RCC->APB2ENR |=RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
	
	/*pines PA9 y PA10 asigunados al usart1*/
	GPIOA->CRH = 0x888444A4;
	/*Se configura los parametros para el usart*/
	USART1->BRR = 0x341;											//96000Baudios a 8MHz
	USART1->CR1 = USART_CR1_TE;
	USART1->CR1 |= USART_CR1_UE;							//se activa el usart1
}
int USART1_Write(int ch){
	
	while(!(USART1->SR & 0x0080)){}//Bit 7 TXE: Transmit data register empty
	USART1->DR = ch & 0xFF;
	return ch;
}

/*funcion para inicializar el timer en moddo input capture*/

void TIM2_Init(void){
	//1.se activa el reloj TIM2
	RCC->APB1ENR = RCC_APB1ENR_TIM2EN;
	//se configura el periodo del conteo
	TIM2->ARR = 0xFFFF-1;
	TIM2->CCMR1 = TIM_CCMR1_CC1S_0;
	TIM2->CCER = TIM_CCER_CC1E;									//habilitacion de la captura de entrada
	TIM2->DIER = TIM_DIER_CC1IE;
	TIM2->CR1 = TIM_CR1_CEN;										//se habilita el conteo
	//se habilita la interrupcion
	NVIC_EnableIRQ(TIM2_IRQn);
}

//funcion que manena la interrupcion del timer2
void TIM2_IRQHandler(void){
	if(TIM2->SR & TIM_SR_CC1IF){
		if(count==1){
			input_capture[0]= TIM2->CCR1;
			count++;
		}
		else if(count==2){
			input_capture[1] = TIM2->CCR1;
			count = 1;
			if(input_capture[1]>input_capture[0]){
				diferencia = input_capture[1] - input_capture[0];
			}
			else{
				diferencia = 0xFFFF-input_capture[0]+1+input_capture[1];
			}
			freq_ic = 2*(8000000/diferencia);
		}
	}
	TIM2->SR = 0x0;
}

//funcion para generar retardo en milisegundos
void delayMs(int n){
	SysTick->LOAD = 8000-1;
	SysTick->VAL = 0;
	SysTick->CTRL =0b101;
	
	for(int i= 0;i<n;i++){
		while((SysTick->CTRL & 0x10000)==0){
		}
	}
	SysTick->CTRL = 0x0;
}
/*ESTA PARTE DEL CODIGO PERMITE EL MANEJO DE LA FUNCION PRINTF Y SCANF*/
struct __FILE{int handle;};

FILE __stdout ={0};

int fputc(int c,FILE *f)
{
	return USART1_Write(c);
}