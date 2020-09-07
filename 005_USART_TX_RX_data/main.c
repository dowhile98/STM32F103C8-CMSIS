


#include "stm32f10x.h"

//prototipo de funciones
void USART1_Init(void);
char USART1_Read(void);
void delayMs(int delay);
void LED_Play(int value);

int main(void){
	
	USART1_Init();
	RCC->APB2ENR |=RCC_APB2ENR_IOPBEN;
	GPIOA->CRL = 0x2;
	GPIOB->CRL = 0x2;
	char ch;
	while(1){
		if(USART1->SR & 0x20){
			ch = USART1_Read();
			LED_Play(ch);
		}
		GPIOB->ODR ^= 0x1;
		delayMs(400);
	}
}


void USART1_Init(void){
	//activamos el reloj;
	RCC->APB2ENR = 1<<2 ;					//IOPAEN: IO port A clock enable
	RCC->APB2ENR |= 1<<14;				//USART1EN: USART1 clock enable
	
	//configuramos los pines en la funcion alternativa
	GPIOA->CRH = 0xA0;						//Alternate function output Push-pull
	GPIOA->CRH = 0X400;						//Floating input (reset state)
	
	//se establece la velocidad en baudios
	USART1->BRR = 0x271;					//USART_BRR_DIV_Fraction
	//Control register 1 (USART_CR1)
	USART1->CR1 = 0x0004;					//RE: Receiver enable
	USART1->CR1 |= 0x2000;					//UE: USART enable
}

char USART1_Read(void){
	while(!(USART1->SR & 0x0020));//Bit 5 RXNE: Read data register not empty(EN ESPERA MIENTRAS ESTE VACIO EL REGISTRO)
	return USART1->DR;
}

void LED_Play(int value){
	value %= 16;
	for(;value>0;value--){
		GPIOA->BSRR = 0x1;
		delayMs(100);
		GPIOA->BSRR =1<<16;
		delayMs(100);
	}
	delayMs(400);
}

void delayMs(int delay){
	int i;
	for(;delay>0;delay--){
		for(i = 0;i<3195;i++);
	}
}
