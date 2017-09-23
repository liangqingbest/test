#include "24c02.h"

I2C_HandleTypeDef I2C_Handler; 

void i2c_delay(uint32_t us)
{
	while(us>0)
	{
		us--;
	}
}


void i2c_init(void)
{
	I2C_Handler.Instance=I2Cx;
	I2C_Handler.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
	I2C_Handler.Init.ClockSpeed=100000;
	I2C_Handler.Init.DualAddressMode=I2C_DUALADDRESS_DISABLE;
	I2C_Handler.Init.DutyCycle=I2C_DUTYCYCLE_2;
	I2C_Handler.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
	I2C_Handler.Init.NoStretchMode=I2C_NOSTRETCH_DISABLE;
	I2C_Handler.Init.OwnAddress1=I2C_OWNADDR;
	I2C_Handler.Init.OwnAddress2=0;
	
	HAL_I2C_Init(&I2C_Handler);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
	GPIO_InitTypeDef GPIO_Init;
	
	I2C_GPIO_CLK_ENABLE();
	I2C_CLK_ENABLE();
	
	GPIO_Init.Pin=I2C_SCL|I2C_SDA;
	GPIO_Init.Mode=GPIO_MODE_AF_OD;
	GPIO_Init.Pull=GPIO_PULLUP;
	GPIO_Init.Speed=GPIO_SPEED_HIGH;
	I2C_GPIO_AF;
	HAL_GPIO_Init(I2C_PORT,&GPIO_Init);
	
	HAL_NVIC_SetPriority(I2Cx_IRQ,3,1);
	HAL_NVIC_EnableIRQ(I2Cx_IRQ);
}


HAL_StatusTypeDef i2c_read(uint8_t MemAddr,uint8_t *rxbuf,uint16_t buf_len)
{
	uint16_t i;
	HAL_StatusTypeDef status;
	for(i=0;i<buf_len;i++)
	{
		MemAddr+=i;
		status=HAL_I2C_Mem_Read(&I2C_Handler,I2C_ADDR,MemAddr,I2C_MEMADD_SIZE_8BIT,&rxbuf[i],1,3000);
		if(status!=HAL_OK)
		{
			return status;
		}
		i2c_delay(10000);
	}
	return HAL_OK;
}

HAL_StatusTypeDef i2c_write(uint8_t MemAddr,uint8_t *txbuf,uint16_t buf_len)
{
	uint16_t i;
	HAL_StatusTypeDef status;
	for(i=0;i<buf_len;i++)
	{
		MemAddr+=i;
		status=HAL_I2C_Mem_Write(&I2C_Handler,I2C_ADDR,MemAddr,I2C_MEMADD_SIZE_8BIT,&txbuf[i],1,3000);
		if(status!=HAL_OK)
		{
			return status;
		}
		i2c_delay(10000);
	}
	return HAL_OK;
}


HAL_StatusTypeDef i2c_is_ready(uint32_t Trials)
{
	return HAL_I2C_IsDeviceReady(&I2C_Handler,I2C_ADDR,Trials,3000);
}

uint8_t i2c_rxbuf[BUF_LEN];
uint8_t i2c_txbuf[BUF_LEN];

void _24c02_main(void)
{
	uint8_t i;
	for(i=0;i<BUF_LEN;i++)
	{
		i2c_txbuf[i]=i;
	}
//	if(i2c_is_ready(100)!=HAL_OK)
//	{
////		Error_Handler();
//	}
//	else
//	{
		if(i2c_write(0,i2c_txbuf,BUF_LEN)!=HAL_OK)
		{
	//		Error_Handler();
		}
		else
		{
			if(i2c_read(0,i2c_rxbuf,BUF_LEN)!=HAL_OK)
			{
		//		Error_Handler();
			}
		}
//	}
}

