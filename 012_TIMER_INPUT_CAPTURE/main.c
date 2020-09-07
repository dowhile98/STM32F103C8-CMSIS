#include "stm32f10x.h"                  // Device header
#include <stdio.h>
void RCC_init(void);
void TIM2_config(void);
void TIM3_config(void);

void UART1_Init(void);
int USART1_Write(int ch);
int USART1_Read(void);
void delayMs(int delay);
uint32_t timStamp = 0;

int main(void){
	RCC_init();
	TIM2_config();
	TIM3_config();
	UART1_Init();
	printf("bienvenido a los timer\r\n");
	while(1){
		while(!(TIM3->SR & 2));
		timStamp = TIM3->CCR1 ;
		printf("valor leido->%d\r\n",timStamp);
	}
}

void RCC_init(void){
	RCC->CR = 1<<16;										//activa el reloj externo(HSEON)
	while(!(RCC->CR & RCC_CR_HSERDY));	//espera a que el reloj este listo
	RCC->CFGR = 0x1;										//selecciona el reloj externo como fuente de reloj	
	RCC->APB2ENR = 1<<2;								//activa el reloj de puerto A
	RCC->APB1ENR = 0x1;									//activa el reloj para el tim2
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
}

void TIM2_config(void){
	//configuramos el pin A con funcion alternativa
	GPIOA->CRL = 0xA;									//se establece el pin PA0 como funcion alternativa
	TIM2->PSC = 8000-1;
	TIM2->ARR = 1000-1;
	TIM2->CCMR1 =0x330;								//Bits 9:8 CC2S[1:0]: Captura/camparacion activado
																			//Bits 6:4 OC1M: comparacion de salida modo 1
	TIM2->CCR1 = 0;										//Bits 15:0 CCR1[15:0]: Capture/Compare 1 value
	TIM2->CCER |= 0x1;								//Bit 0 CC1E: Capture/Compare 1 output enable
	TIM2->CNT = 0;										//Bits 15:0 CNT[15:0]: Counter value
	TIM2->CR1 = 0x1;									//Bit 0 CEN: habilita el conteo
}

void TIM3_config(void){
	GPIOA->CRL |= 0x0A000000;
		
	//configuramos el TIM3 ->input capture
	TIM3->PSC = 8000;									//se divide la frecuencuia por 8000		
	TIM3->CCMR1 = 0x41;								//Bits 1:0 CC1S: Capture/Compare 1 selection
																			//100: Force inactive level - OC1REF is forced low.
																			//Establece el canal 1 para capturar el borde
	TIM3->CCER = 1;										//Activa el modo captura
	TIM3->CR1 = 1;
}




void UART1_Init(void){
	RCC->APB2ENR |=1<<14;						//Bit 14 SART1EN: USART1 clock enable
	
	//SE CONFIGURA LOS PINES PA9 Y PA10
	GPIOA->CRH = 0xA0;
	GPIOA->CRH |= 0x400;
	
	//se establece la velocidad en baudios
	USART1->BRR = 0x45;					//USART_BRR_DIV_Fraction
	//Control register 1 (USART_CR1)
	USART1->CR1 = 1<<2;						//Bit 2 RE: Receiver enable
	USART1->CR1 |= 1<<3;					//Bit 3 TE: Transmitter enable
	USART1->CR1 |= 1<<13;					//Bit 13 UE: USART enable
}

int USART1_Write(int ch){
	
	while(!(USART1->SR & 0x0080)){}//Bit 7 TXE: Transmit data register empty
	USART1->DR = ch & 0xFF;
	return ch;
}

void delayMs(int delay){
	int i;
	for(;delay>0;delay--){
		for(i = 0;i<3195;i++);
	}
}

int USART1_Read(void){
	while(!(USART1->SR & 0x0020)){}//Bit 5 RXNE: Read data register not empty
	return USART1->DR;
}


struct __FILE{int handle;};
FILE __stdin = {0};
FILE __stdout ={1};
FILE __stderr = {2};
	
int fgetc(FILE *f){
	int c;
	c = USART1_Read();
	if(c == '\r'){
		USART1_Write(c);
		c = '\n';
	}
	USART1_Write(c);
	return c;
}

int fputc(int c,FILE *f)
{
	return USART1_Write(c);
}