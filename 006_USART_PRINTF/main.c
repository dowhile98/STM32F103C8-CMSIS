#include "stm32f10x.h"                  // Device header
#include <stdio.h>

void UART1_Init(void);
void delayMs(int delay);
int USART1_Write(int ch);
int USART1_Read(void);

int main(void){
	UART1_Init();
	
	printf("PROBANDO USART1 ->STM32F103C8\r\n");
	int n;
	while(1){
		if(USART1->SR & 0x20){
			scanf("%d",&n);
			printf("se recibio->%d\r\n",n);
		}
		printf("no se recibio nada\r\n");
		delayMs(500);
	}
}

void UART1_Init(void){
	RCC->APB2ENR = 1<<2;						//Bit 2 IOPAEN: I/O port A clock enable
	RCC->APB2ENR |=1<<14;						//Bit 14 SART1EN: USART1 clock enable
	
	//SE CONFIGURA LOS PINES PA9 Y PA10
	GPIOA->CRH = 0xA0;
	GPIOA->CRH |= 0x400;
	
	//se establece la velocidad en baudios
	USART1->BRR = 0x271;					//USART_BRR_DIV_Fraction
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

int USART1_Read(void){
	while(!(USART1->SR & 0x0020)){}//Bit 5 RXNE: Read data register not empty
	return USART1->DR;
}

void delayMs(int delay){
	int i;
	for(;delay>0;delay--){
		for(i = 0;i<3195;i++);
	}
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
