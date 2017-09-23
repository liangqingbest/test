#ifndef _LED_H
#define _LED_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal_gpio.h"


#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_13
#define LED_CLK_ENABLE __HAL_RCC_GPIOC_CLK_ENABLE()

#define LED_ON HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_RESET)
#define LED_OFF HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_SET)
#define LED_TOGGLE HAL_GPIO_TogglePin(LED_PORT,LED_PIN)
	
	
void led_init(void);
void led_delay(uint32_t delayms);
void exit_init(void);
#endif
