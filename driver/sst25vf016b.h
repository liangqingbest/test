#ifndef _SST25VF016B_H
#define _SST25VF016B_H

/**********根据不同的开发板设置不同的参数******************************/
//智嵌物联网开发板STM32F4VET6
//#include "stm32f4xx.h"
//#include "stm32f4xx_hal_gpio.h"
//#include "stm32f4xx_hal_spi.h"
//#define SPIx SPI3
//#define SPI_PORT GPIOC
//#define SPI_CLK_PIN GPIO_PIN_10
//#define SPI_MOSI_PIN GPIO_PIN_11
//#define SPI_MISO_PIN GPIO_PIN_12

//#define SPI_CSS_PORT GPIOC
//#define SPI_CSS_PIN GPIO_PIN_2

//#define SPI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
//#define SPI_GPIO_CSS_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
//#define SPI_CLK_ENABLE() __HAL_RCC_SPI3_CLK_ENABLE()
//#define GPIO_INIT_AF GPIO_Init.Alternate=GPIO_AF6_SPI3

//众想科技stm32开发stm32f103vet6
#include "stm32f1xx.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_spi.h"

#define SPIx SPI1
#define SPI_PORT GPIOA
#define SPI_CLK_PIN GPIO_PIN_5
#define SPI_MOSI_PIN GPIO_PIN_6
#define SPI_MISO_PIN GPIO_PIN_7

#define SPI_CSS_PORT GPIOE
#define SPI_CSS_PIN GPIO_PIN_6

#define SPI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI_GPIO_CSS_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI_CLK_ENABLE() __HAL_RCC_SPI1_CLK_ENABLE()
#define GPIO_INIT_AF 
/*************************************************************/

#define	SEC_MAX_NUM     	511
#define SEC_SIZE		0x1000     

#define CMD_ERASE_ALL 0x60
#define CMD_ERASE_16K 0x20
#define CMD_ERASE_32K 0x52

#define CMD_WRITE_BUF 0x06
#define CMD_READ_BUF 0x0b 
#define CMD_READ_STATE_REG 0x05
#define CMD_WRITE_STATE_REG 0x01
#define CMD_STATE_WRITE_ABLE 0x50

#define CMD_JEDEC_ID_READ 0x9f
#define CMD_ID_READ 0x90

#define SPI_CS_LOW HAL_GPIO_WritePin(SPI_CSS_PORT,SPI_CSS_PIN,GPIO_PIN_RESET)
#define SPI_CS_HIGH HAL_GPIO_WritePin(SPI_CSS_PORT,SPI_CSS_PIN,GPIO_PIN_SET)

void spi_init(void);
void sst25vf016b_read(uint32_t addr,uint8_t* rbuf,uint32_t buf_len);
void sst25vf016b_write(uint32_t addr,uint8_t* wbuf,uint32_t buf_len);
uint8_t sst25f16_main(void);
#endif

