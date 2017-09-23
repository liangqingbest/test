#ifndef __SPI_H
#define __SPI_H
#include "stm32f1xx.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_spi.h"

//SPI1初始化
void spi_init(void);
//SPI1读写一字节数据
unsigned char	SPI1_ReadWrite(unsigned char writedat);

#endif
