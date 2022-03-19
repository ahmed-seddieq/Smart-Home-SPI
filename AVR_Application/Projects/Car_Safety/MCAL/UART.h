/*
 * UART.h
 *
 * Created: 12/14/2021 8:59:39 AM
 *  Author: Ahmed Sedeek
 */ 


#ifndef UART_H_
#define UART_H_

#include "MemMap.h"
#include "StdTypes.h"
#include "Utils.h"

/* This Func Starts UART */
void UART_Init(void);
/* This Function Sends Byte Through UART */
void UART_Send(u8 data);
/* This Function Receive Byte Through UART*/
u8 UART_Receive(void);


u8 UART_Receive_PeriodicCheck(u8 *pdata);

#endif /* UART_H_ */