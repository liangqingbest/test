#include "SPI.h"

SPI_HandleTypeDef SPI_Handler;

uint8_t	SPI1_ReadWrite(uint8_t writedat);

void spi_init(void)
{
	SPI_Handler.Instance=SPI1;
	SPI_Handler.Init.Mode=SPI_MODE_MASTER;
	SPI_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_8;
	SPI_Handler.Init.CLKPhase=SPI_PHASE_1EDGE;
	SPI_Handler.Init.CLKPolarity=SPI_POLARITY_LOW;
	SPI_Handler.Init.Direction=SPI_DIRECTION_2LINES;
	SPI_Handler.Init.DataSize=SPI_DATASIZE_8BIT;
	SPI_Handler.Init.CRCPolynomial=SPI_CRCCALCULATION_DISABLE;
	SPI_Handler.Init.CRCCalculation=7;
	SPI_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;
	SPI_Handler.Init.TIMode=SPI_TIMODE_DISABLE;
	SPI_Handler.Init.NSS=SPI_NSS_SOFT;
	
	HAL_SPI_Init(&SPI_Handler);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
	GPIO_InitTypeDef GPIO_Init;
	if(hspi->Instance==SPI1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_SPI1_CLK_ENABLE();
		
		GPIO_Init.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_Init.Mode=GPIO_MODE_AF_PP;
		GPIO_Init.Pull=GPIO_PULLUP;
		GPIO_Init.Speed=GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOA,&GPIO_Init);
		
		GPIO_Init.Pin=GPIO_PIN_7;
		GPIO_Init.Mode=GPIO_MODE_OUTPUT_PP;
		GPIO_Init.Pull=GPIO_PULLUP;
		GPIO_Init.Speed=GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOB,&GPIO_Init);	

		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
	}
}

uint8_t	SPI1_ReadWrite(uint8_t writedat)
{
	uint8_t readdat;
	HAL_SPI_TransmitReceive(&SPI_Handler,&writedat,&readdat,1,1000);
	return readdat;
}

