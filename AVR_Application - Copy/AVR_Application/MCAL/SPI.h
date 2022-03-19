/*
 * SPI.h
 *
 * Created: 2/8/2022 4:15:09 PM
 *  Author: Ahmed Sedeek
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "StdTypes.h"
#include "MemMap.h"
#include "DIO_Interface.h"


typedef enum
{
	MASTER,
	SLAVE
}Master_Slave_type;

void SPI_Init(Master_Slave_type mode);

u8 SPI_SendReceive(u8 data);

void SPI_Send(u8 data);

u8 SPI_Receive(void);

u8 SPI_ReceivePeriodic(u8 *pdata);




#endif /* SPI_H_ */