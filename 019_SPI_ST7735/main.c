#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_rcc.h"

/*DEFINICION DE MACROS Y VARIABLES GLOBALES*/

/*FIN*/
/*DEFINICION DE PROTOTIPO DE FUNCIONES*/
void SystemClock_Config(void);
void GPIO_SPI_Init(void);
void SPI_InitConfig(void);
/*FIN*/

int main(void){
	SystemClock_Config();
	SPI_InitConfig();
	while(1){
		
	}
}
void SystemClock_Config(void){
	RCC->CR = RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	RCC->CR |= RCC_CR_PLLON;
	RCC-> CFGR = RCC_CFGR_PLLSRC_HSE; 
	RCC->CFGR |= RCC_CFGR_PLLMULL2;
	while(!(RCC->CR & RCC_CR_PLLRDY));
	RCC->CFGR |= RCC_CFGR_SW_PLL;
}	
void GPIO_SPI_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_Spi = {0};
	
	GPIO_Spi.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Spi.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Spi.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Spi);
}

void SPI_InitConfig(void){
	SPI_InitTypeDef SPI_struct = {0};
	
	//se activa el reloj
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	//se configura los parametros del spi
	SPI_struct.SPI_Mode = SPI_Mode_Master;
	SPI_struct.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_struct.SPI_DataSize = SPI_DataSize_8b;
	SPI_struct.SPI_CPOL = SPI_CPOL_Low;
	SPI_struct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_struct.SPI_NSS = SPI_NSS_Soft;
	SPI_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_struct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_struct.SPI_CRCPolynomial = 10;
	//se resetea el spi1
	SPI_I2S_DeInit(SPI1);
	//se inicializa los parametros del spi1
	SPI_Init(SPI1,&SPI_struct);
	//se inicializa el spi
	SPI_Cmd(SPI1,ENABLE);
}