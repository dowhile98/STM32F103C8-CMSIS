#ifndef __MAIN_H
#define __MAIN_H
#include <stdint.h>

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_rcc.h"

/*DEFINICION DE PROTOTIPO DE FUNCIONES*/
void SystemClock_Config(void);
void GPIO_SPI_Init(void);
void SPI_InitConfig(void);
void HAL_Delay(uint32_t delay);

#endif
