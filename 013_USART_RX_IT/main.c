/*ESTE PROGRAMA RECIBE NUMEROS Y DE ACUERDO AL NUMERO RECIBIDO 
*ENCIENDE LOS LEDS CONECTADOS A LOS PINES PA0-PA4
*EL NUMERO SE RECIBE MEDIANTE EL PUERTE USART1,
*HACIENDO USO DE LA INTERRUPCION*/

#include "stm32f10x.h"                  // Device header
#include <stdio.h>

//prototipo de funciones
void SystemClock_Config(void);
void UART1_Init(void);
void UART1_Write(int ch);
uint8_t UART1_Read(void);
void delayMs(int n);
void UART_Receive_IT(void);

/*VARIABLES GLOBALES*/
int data;

int main(){
	SystemClock_Config();								//se inicializa el reloj
	UART1_Init();												//se inicializa el puerto USART2
	GPIOA->CRL = 0x44442222;
	printf("configuracion exitosa\r\n");
	while(1){
		if(data ==1)
			GPIOA->ODR = 0x1;
		else if(data==2)
			GPIOA->ODR = 0x2;
		else if(data==3)
			GPIOA->ODR = 0x4;
		else if(data == 4)
			GPIOA->ODR = 0x8;
		else if(data == 5)
			GPIOA->ODR = 0xF;
		else
			GPIOA->ODR = 0x0;
	}
}


//funcion que configura la fuente de reloj del mcu
void SystemClock_Config(void){
	RCC->CR = RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY)==0){}
	RCC->CFGR = RCC_CFGR_PLLSRC_HSE; 										//se selecciona HSE como fuente para el PLL
	RCC->CFGR |=RCC_CFGR_PLLMULL4;											//se multiplica x el reloj del PLL
	RCC->CFGR |=RCC_CFGR_PPRE1_DIV2;										//se divide entre dos el reloj del (APB1)
	RCC->CR |= RCC_CR_PLLON;														//Bit 24 PLLON: PLL enable
	while((RCC->CR & RCC_CR_PLLRDY)==0){}
	RCC->CFGR |=0x2;																		//se selecciona la fuente de reloj del sistema
	
}
void UART1_Init(void){
	//1.se activa el reloj para el usart2 y el GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN|RCC_APB2ENR_USART1EN;
	//2.se habilita los pines RX y TX
	
	GPIOA->CRH = 0x4A4;
	
	//3. se configura el la velocidad de transferencia
	USART1->BRR = 0x271;
	//4. se configura los parametros del usart
	USART1->CR1 = USART_CR1_TE;							//Se habilita la transmision tx
	USART1->CR1 |= USART_CR1_RE;						//se habilita la recepcion rx
	USART1->CR1 |= USART_CR1_RXNEIE;				//se activa la bandera de la interrupcion para la recepcion de datos
	//4. se activa el puerto usart2
	USART1->CR1 |= USART_CR1_UE;
	//se activa las interrupciones para el USART1
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn,1);
}

/*MANEJADOR DE LA INTERRUPCION*/
void USART1_IRQHandler(void){
	if(USART1->SR & USART_SR_RXNE){
		UART_Receive_IT();
	}
}
//funcion que se llama cuando se realiza la interrupcion
void UART_Receive_IT(void){
	scanf("%d",&data);
	printf("retardo->%d\r\n",data);
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

void delayMs(int n){
	SysTick->LOAD = 72000-1;
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
