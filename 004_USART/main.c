/*

APB2  BIT 14: USART1
USART1 conectado a los pines PA9(TX) y PA10(RX)
GPIOA bit 2:RCC_APB2ENR

*/

#include "stm32f10x.h"


void USART1_Init(void);
void USART1_write(int ch);
void delayMs(uint32_t delay);

int main(void){
	
	USART1_Init();
	while(1){
		USART1_write('H');
		USART1_write('i');
		USART1_write('\r');
		USART1_write('\n');
		delayMs(1000);
	}
}

void USART1_Init(void){
	//se activa el reloj del APB2
	RCC->APB2ENR |= 0x4004;     //bit 2 activa el IOPORTA, bit 14 activa el USART1
	
	//se configura el PA9 y PA10 en Alternate function
	uint32_t GPIOA_CRH = 0x00;
	GPIOA_CRH |= 0xA0;   			//0b1010    OUTPUT ALTERNATIVE FUNTION PP
	GPIOA_CRH |= 0x400;				//0b0100		FLOATING INPUT
	GPIOA->CRH = GPIOA_CRH;
	
	USART1->BRR = 0x1D4C;			//96000buadios a 72MHz  (APB2->72MHz)
														/*
															USARTDIV = Fck/(16*BUAD)
															->se convierte a hexadecimal la parte entera de la division
															->la parte fraccionaria se multiplica por 16 y se convierte a HEX
															->luego de une y se coloca en el registro BRRR
																USARTDIV = 72000000/(16*9600)
																				 = 468.75
																				 468=0x1D4
																					0.75*16 = 12 = 0xC
																USARTDIV = 0x1D4C
														*/
	USART1->CR1 = 0x2008;			//Bit 3 TE: Transmitter enable   UE: USART enable
}

void USART1_write(int ch){
	//espera mientras el buffer este vacio
	while(!(USART1->SR & 0x0080));
	USART1->DR = (ch & 0xFF);
}

void delayMs(uint32_t delay){
	int i;
	for(;delay>0;delay--){
		for(i = 0;i<3195;i++);
	}
}