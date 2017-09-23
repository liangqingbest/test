#include "sst25vf016b.h"

SPI_HandleTypeDef SPI_Handler;

void spi_init(void)
{
	SPI_Handler.Instance=SPIx;
	SPI_Handler.Init.Mode=SPI_MODE_MASTER;
	SPI_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_64;
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
	if(hspi->Instance==SPIx)
	{
		SPI_GPIO_CLK_ENABLE();
		SPI_GPIO_CSS_CLK_ENABLE();
		SPI_CLK_ENABLE();
		
		GPIO_Init.Pin=SPI_CLK_PIN|SPI_MOSI_PIN|SPI_MISO_PIN;
		GPIO_Init.Mode=GPIO_MODE_AF_PP;
		GPIO_Init.Pull=GPIO_PULLUP;
		GPIO_Init.Speed=GPIO_SPEED_HIGH;
		GPIO_INIT_AF;
		HAL_GPIO_Init(SPI_PORT,&GPIO_Init);
		
		GPIO_Init.Pin=SPI_CSS_PIN;
		GPIO_Init.Mode=GPIO_MODE_OUTPUT_PP;
		GPIO_Init.Pull=GPIO_PULLUP;
		GPIO_Init.Speed=GPIO_SPEED_HIGH;
		HAL_GPIO_Init(SPI_CSS_PORT,&GPIO_Init);		
	}
}

uint8_t spi_write_read(uint8_t tdata)
{
	uint8_t rdata;
	HAL_SPI_TransmitReceive(&SPI_Handler,&tdata,&rdata,1,1000);
	return rdata;
}

void sst25vf016b_read(uint32_t addr,uint8_t* rbuf,uint32_t buf_len)
{
	uint32_t i;
	SPI_CS_LOW;
	spi_write_read(0x0b);
	spi_write_read((addr&0xffffff)>>16);
	spi_write_read((addr&0xffff)>>8);
	spi_write_read(addr&0xff);
	spi_write_read(0xff);
	for(i=0;i<buf_len;i++)
	{
		rbuf[i]=spi_write_read(0xff);
	}
	SPI_CS_HIGH;
}


void sst25vf016b_write(uint32_t addr,uint8_t* wbuf,uint32_t buf_len)
{
	uint8_t temp,statval;
	uint32_t i;
	SPI_CS_LOW;
	spi_write_read(0x05);
	temp=spi_write_read(0xff);
	SPI_CS_HIGH;
	//enable status reg write
	SPI_CS_LOW;
	spi_write_read(0x50);
	SPI_CS_HIGH;
	
	SPI_CS_LOW;
	spi_write_read(0x01);
	spi_write_read(0);
	SPI_CS_HIGH;
	
	for(i=0;i<buf_len;i++)
	{
		SPI_CS_LOW;
		spi_write_read(0x06);
		SPI_CS_HIGH;
		SPI_CS_LOW;
		spi_write_read(0x02);
		spi_write_read(((addr+i)&0xffffff)>>16);
		spi_write_read(((addr+i)&0xffff)>>8);
		spi_write_read((addr+i)&0xff);
		spi_write_read(wbuf[i]);
		SPI_CS_HIGH;
		
		do{
			SPI_CS_LOW;
			spi_write_read(0x05);
			statval=spi_write_read(0xff);
			SPI_CS_HIGH;
		}while(statval==0x03);
	}
	SPI_CS_LOW;
	spi_write_read(0x06);
	SPI_CS_HIGH;
	
	SPI_CS_LOW;
	spi_write_read(0x50);
	SPI_CS_HIGH;
	
	SPI_CS_LOW;
	spi_write_read(0x01);
	spi_write_read(temp);
	SPI_CS_HIGH;
}

void erase_sec(uint8_t cmd,uint32_t secaddr)
{
		uint8_t statval;
		SPI_CS_LOW;
		spi_write_read(CMD_WRITE_BUF);
		SPI_CS_HIGH;
	
		SPI_CS_LOW;
		if(cmd==CMD_ERASE_ALL)
		{
			spi_write_read(cmd);
		}
		else{
			spi_write_read(cmd);
			spi_write_read((secaddr&0xffffff)>>16);
			spi_write_read((secaddr&0xffff)>>8);
			spi_write_read(secaddr&0xff);			
		}
		SPI_CS_HIGH;
		
		do
		{
			SPI_CS_LOW;
			spi_write_read(CMD_READ_STATE_REG);
			statval=spi_write_read(0);
			SPI_CS_HIGH;
		}while(statval==0x03);
}


void sst25vf016b_erase(uint32_t sec1,uint32_t sec2)
{
	uint8_t temp1,temp2;
	uint32_t secsToEr,CurSec;

	SPI_CS_LOW;
	spi_write_read(CMD_READ_STATE_REG);
	temp1=spi_write_read(0);
	SPI_CS_HIGH;

	SPI_CS_LOW;
	spi_write_read(CMD_STATE_WRITE_ABLE);
	SPI_CS_HIGH;
	
	SPI_CS_LOW;
	spi_write_read(CMD_WRITE_STATE_REG);
	spi_write_read(0);
	SPI_CS_HIGH;
	
	SPI_CS_LOW;
	spi_write_read(CMD_WRITE_BUF);
	SPI_CS_HIGH;
	
	if(sec1>sec2)
	{
		temp2=sec1;
		sec1=sec2;
		sec2=temp2;
	}
	if(sec1==sec2)
	{
		erase_sec(CMD_ERASE_16K,SEC_SIZE*sec1);
		return;
	}
	
	if(sec1-sec2==SEC_MAX_NUM)
	{
		erase_sec(CMD_ERASE_ALL,0);
		return;	
	}
	secsToEr=sec2-sec1+1;
	
	while(secsToEr>=8)
	{
		erase_sec(CMD_ERASE_32K,SEC_SIZE*sec1);
		secsToEr-=8;
		CurSec+=8;
	}
	while(secsToEr>=1)
	{
		erase_sec(CMD_ERASE_16K,SEC_SIZE*CurSec);
		secsToEr-=1;
		CurSec+=1;		
	}
	
	SPI_CS_LOW;
	spi_write_read(CMD_WRITE_BUF);
	SPI_CS_HIGH;
	
	SPI_CS_LOW;
	spi_write_read(CMD_STATE_WRITE_ABLE);
	SPI_CS_HIGH;
	
	SPI_CS_LOW;
	spi_write_read(CMD_WRITE_STATE_REG);
	spi_write_read(temp1);
	SPI_CS_HIGH;
}



#define BUF_LEN 125
uint8_t spi_rbuf[BUF_LEN];
uint8_t spi_wbuf[BUF_LEN];
uint8_t sst25f16_main(void)
{
	uint8_t i=0;
	for(i=0;i<BUF_LEN;i++)
	{
		spi_wbuf[i]=3*i;
	}
	sst25vf016b_erase(0,BUF_LEN/SEC_SIZE+1);
	sst25vf016b_write(0,spi_wbuf,BUF_LEN);
	
	sst25vf016b_read(0,spi_rbuf,BUF_LEN);
	
	return 0;
}



