/*
D0->PA0....D7->PA7
RS->PA8
E->PA9
RW->PA10
*/
/******************INLUSION DE LIBRERIAS¨*********************/
#include "stm32f4xx.h"

//DEFINICION DE MACROS
#define LCD_PORT		GPIOA
#define D0_Pin			(1<<0)
#define D1_Pin			(1<<1)
#define D2_Pin			(1<<2)
#define D3_Pin			(1<<3)
#define D4_Pin			(1<<4)
#define D5_Pin			(1<<5)
#define D6_Pin			(1<<6)
#define D7_Pin			(1<<7)

#define RS_Pin			(1<<8)
#define E_Pin				(1<<9)


/*******DEFINICION DE PROTOTIPO DE FUNCIONES******************/

//funcion que se utilizará para inicializar los pines conectados a la lcd
void LCD_pinInit(void);
//funcion que se utilza para inicializar la lcd
void LCD_Init(void);
//funcion que se usa para enviar comandos a la lcd
void LCD_sendCmd(uint8_t cmd);
//funcion para enviar datos a lcd
void LCD_sendData(uint8_t data);
//funcion para enviar cadena de caracteres a la lcd
void LCD_writeString(char *str);
//funcion para mover el cursor
void LCD_setCursor(uint8_t row, uint8_t col);
//funcion que se usa para limpiar la lcd
void LCD_clear(void);
void LCD_delayMS(int delay);
int main(void){
	LCD_Init();
	
	while(1){
		int i;
		for(i = 0;i<16;i++){
			LCD_setCursor(0,i);
			LCD_sendData(0x53);
			LCD_delayMS(300);
			
		}
		for(i = 0;i<16;i++){
			LCD_setCursor(1,i);
			LCD_delayMS(300);
		}
	}
}


/*******DEFINICION  DE FUNCIONES******************/

//funcion que se utilizará para inicializar los pines conectados a la lcd
void LCD_pinInit(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//se habilita el reloj para el DATA PORT
	
	//DATA_PORT->MODER &=~(0xFFFF);//Se resetea el estado de los pines
	LCD_PORT->MODER |= 0x55555;//PA0-PA7 COMO SALIDA
	LCD_PORT->BSRR = RS_Pin;
	
}
void LCD_Init(void){
	LCD_pinInit();
	LCD_delayMS(60);//wait for 60ms
	LCD_sendCmd(0x30);
	LCD_delayMS(10);
	LCD_sendCmd(0x30);
	LCD_delayMS(1);
	LCD_sendCmd(0x30);//8bit data
	LCD_delayMS(1);
	
	LCD_sendCmd(0x38);//move cursor righ
	LCD_delayMS(1);
	LCD_sendCmd(0x01);//clear display
	LCD_delayMS(3);
	LCD_sendCmd(0x06);//clear display
	LCD_delayMS(1);
	LCD_sendCmd(0x0F);//clear display
	LCD_delayMS(1);
	LCD_sendCmd(0x40);//clear display
	LCD_delayMS(1);
	//enviar los comandos
}
//funcion que se usa para enviar comandos a la lcd
void LCD_sendCmd(uint8_t cmd){
	LCD_PORT->BSRR = RS_Pin<<16;//RS=0
	LCD_PORT->ODR = cmd;
	LCD_PORT->BSRR = E_Pin;
	LCD_delayMS(1);
	LCD_PORT->BSRR = E_Pin<<16;
}
//funcion para enviar datos a lcd
void LCD_sendData(uint8_t data){
	LCD_PORT->BSRR = RS_Pin;//RS=0
	LCD_PORT->ODR = data;
	LCD_PORT->BSRR = E_Pin;
	LCD_delayMS(0);
	LCD_PORT->BSRR = E_Pin<<16;
}
//funcion para enviar cadena de caracteres a la lcd
void LCD_writeString(char *str){
	while(*str){
		LCD_sendData(*str++);
	}
}
//funcion para mover el cursor
void LCD_setCursor(uint8_t row, uint8_t col){
	switch (row)
    {
			case 0:
				col |= 0x80;
        break;
      case 1:
        col |= 0xC0;
        break;
    }
		LCD_sendCmd(col);
}
//funcion que se usa para limpiar la lcd
void LCD_clear(void){
	LCD_sendCmd(0x01);
}
//funcion que genera retardos
void LCD_delayMS(int delay){
	int i;
	for(;delay>0;delay--){
		for(i = 0;i<3600;i++){
		}
	}		
}