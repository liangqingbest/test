#include "led.h"

void led_init(void)
{
	GPIO_InitTypeDef GPIO_Init;
	LED_CLK_ENABLE;
	
	GPIO_Init.Pin=LED_PIN;
	GPIO_Init.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull=GPIO_PULLUP;
	GPIO_Init.Speed=GPIO_SPEED_HIGH;
	
	HAL_GPIO_Init(LED_PORT,&GPIO_Init);
	
	HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_SET);
}

void exit_init(void)
{
	GPIO_InitTypeDef GPIO_Init;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_Init.Pin=GPIO_PIN_0;
	GPIO_Init.Mode=GPIO_MODE_IT_FALLING;
	GPIO_Init.Pull=GPIO_PULLUP;
	
	HAL_GPIO_Init(GPIOA,&GPIO_Init);
	
	HAL_NVIC_SetPriority(EXTI0_IRQn,3,2);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}


void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	led_delay(1000);
	if(GPIO_Pin==GPIO_PIN_0)
	{
		LED_TOGGLE;
	}
}

void led_delay(uint32_t delayms)
{
	while(delayms>0)
	{
		delayms--;
	}
}



