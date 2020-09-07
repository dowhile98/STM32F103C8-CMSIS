#include "stm32f10x.h"                  // Device header
#include <string.h>

/*prototipo de funciones*/
void SystemClock_Config(void);
void SPI1_Init(void);
void SPI_Write(uint8_t ch);

/*fin*/
int main(void){
	SystemClock_Config();
	SPI1_Init();
	char *msg  = "hello word perro\r\n";
	uint8_t size = strlen(msg);
	SPI_Write('h');
	SPI_Write('o');
	SPI_Write('l');
	SPI_Write('a');
	SPI_Write('\r');
	SPI_Write('\n');
	while(1){
		
	}
}

/*definicion de funciones*/
void SystemClock_Config(void){
	RCC->CR = RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	RCC->CFGR = RCC_CFGR_SW_HSE;
}
void SPI1_Init(void){
	//1. se habilita el reloj para el spi y gpioa
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	//2.Se asocian los pines para el SPI1
	GPIOA->CRL = 0xB4B34444;
	GPIOA->BSRR = 0x10;
	//3.se configura el el SPI
	 SPI1->CR1 = 0x31C;    // Set baudrate, 8-bit data frame
   SPI1->CR2 = 0;
	 SPI1->CR1 |= 0x40;
}

void SPI_Write(uint8_t ch){
	while(!(SPI1->SR & SPI_SR_TXE));
	GPIOA->BSRR = 1<<4;
	SPI1->DR = ch;
	while(SPI1->SR & SPI_SR_BSY);
	GPIOA->BSRR = 1<<20;
	SPI1->SR = 0x0;
}