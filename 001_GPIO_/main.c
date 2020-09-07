#include "stm32f10x_rcc.h"
#include "stm32f10x.h"


void delayMs(uint32_t delay);
int main(void)
{
  uint32_t CRL_TEMP = 0x00;

  RCC->APB2ENR |= ((uint32_t)0x00000004);

  CRL_TEMP |= ((uint32_t)0x00000002);
  CRL_TEMP |= ((uint32_t)0x00000020);
  GPIOA->CRL = CRL_TEMP;
 
  while (1) {
		GPIOA->ODR |= ((uint32_t)0x3);
		delayMs(1000);
		GPIOA->ODR &= ~((uint32_t)0x3);
		delayMs(500);
		
    /*uint16_t readButton = (uint16_t)0x0002;
    if (GPIOA->IDR & readButton) 
    {
      if (GPIOA->IDR & readButton)
      {
        GPIOA->BSRR |= ((uint32_t)0x0001);
        while (GPIOA->IDR & readButton);
      }
    }
    GPIOA->BRR |= ((uint32_t)0x0001);*/
  }
}
void delayMs(uint32_t delay){
	int i;
	for(;delay>0;delay--){
		for(i = 0;i<3195;i++);
	}
}
