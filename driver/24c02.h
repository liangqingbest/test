#ifndef _24C02_H
#define _24C02_H

#define I2C_ADDR 0xa0
#define I2C_OWNADDR 0x0a
#define BUF_LEN 20
/***************根据不同开发板配置不同参数***********************************/
//智嵌物联网开发板STM32F407VET6
//#include "stm32f4xx.h"
//#include "stm32f4xx_hal_gpio.h"
//#include "stm32f4xx_hal_i2c.h"
//#define I2Cx I2C1
//#define I2C_PORT GPIOB
//#define I2C_SCL GPIO_PIN_7
//#define I2C_SDA GPIO_PIN_6

//#define I2C_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
//#define I2C_CLK_ENABLE() __HAL_RCC_I2C1_CLK_ENABLE()

//#define I2C_IRQ I2C1_EV_IRQn

//#define I2C_GPIO_AF	GPIO_Init.Alternate=GPIO_AF4_I2C1
//众想科技开发板STM32F103VET6
#include "stm32f1xx.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_i2c.h"

#define I2Cx I2C2
#define I2C_PORT GPIOB
#define I2C_SCL GPIO_PIN_10
#define I2C_SDA GPIO_PIN_11

#define I2C_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C_CLK_ENABLE() __HAL_RCC_I2C2_CLK_ENABLE()

#define I2Cx_IRQ I2C2_EV_IRQn

#define I2C_GPIO_AF

/*********************************************************/

void i2c_init(void);
HAL_StatusTypeDef i2c_read(uint8_t MemAddr,uint8_t *rxbuf,uint16_t buf_len);
HAL_StatusTypeDef i2c_write(uint8_t MemAddr,uint8_t *txbuf,uint16_t buf_len);
HAL_StatusTypeDef i2c_is_ready(uint32_t Trials);
void _24c02_main(void);
#endif
