#include "main.h"
#include "st7735.h"
/*DEFINICION DE MACROS Y VARIABLES GLOBALES*/

/*FIN*/


int main(void){
	SystemClock_Config();
	SPI_InitConfig();
	ST7735_Init();
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
	//se activa el reloj
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	/*INICIALIZANDO SPI1 USANGO REGISTROS*/
	SPI1->CRCPR = 0x7;
	SPI1->CR1 = SPI_CR1_MSTR;
	SPI1->CR1 |= SPI_CR1_SSI|SPI_CR1_SSM;	//0x700;
	SPI1->CR1 |= SPI_CR1_BIDIMODE;
}

void HAL_Delay(uint32_t delay){
	SysTick->LOAD = 16000-1;
	SysTick->VAL = 0;
	SysTick->CTRL =0b101;
	for(int i= 0;i<delay;i++){
		while((SysTick->CTRL & 0x10000)==0){
		}
	}
	SysTick->CTRL = 0x0;
}
