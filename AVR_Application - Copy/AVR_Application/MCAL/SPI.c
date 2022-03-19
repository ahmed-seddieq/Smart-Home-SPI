/*
 * SPI.c
 *
 * Created: 2/8/2022 4:14:51 PM
 *  Author: Ahmed Sedeek
 */ 


#include "SPI.h"

void SPI_Init(Master_Slave_type mode)
{
	if (mode == MASTER)
	{
		SET_BIT(SPCR, MSTR);
	}
	else
	{
		CLEAR_BIT(SPCR, MSTR);
	}
	
	//enable
	SET_BIT(SPCR, SPE);
}

u8 SPI_SendReceive(u8 data)
{
	SPDR = data;
	// W8 till u send the data to the slave
	while(!READ_BIT(SPSR, SPIF));
	return SPDR;
}

void SPI_Send(u8 data)
{
	SPDR = data;
}

u8 SPI_Receive(void)
{
	return SPDR;
}


u8 SPI_ReceivePeriodic(u8 *pdata)
{
	if (READ_BIT(SPSR, SPIF))
	{
		*pdata = SPDR;
		return 1 ;
	}
	return 0;
}