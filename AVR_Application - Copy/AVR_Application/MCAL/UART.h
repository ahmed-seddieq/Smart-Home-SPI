/*
 * UART.h
 *
 * Created: 12/14/2021 8:59:39 AM
 *  Author: Ahmed Sedeek
 */ 


#ifndef UART_H_
#define UART_H_

/*=========================== Includes ===========================*/

#include "MemMap.h"
#include "StdTypes.h"
#include "Utils.h"

/*=========================== Functions Prototypes ===========================*/

/* This Func Starts UART */
void UART_Init(void);
/* This Function Sends Byte Through UART */
void UART_Send(u8 data);
/* This Function Receive Byte Through UART*/
u8 UART_Receive(void);
/*Periodic check*/
u8 UART_Receive_PeriodicCheck(u8 *pdata);

void UART_SendByteNonBlocking(u8 data);
u8 UART_ReadByteNonBlocking(void);



/*=========================== UART CallBack Functions ===========================*/
void UART_TxC_SetCallBack(void (*LocalFptr)(void));
void UART_RxC_SetCallBack(void (*LocalFptr)(void));
void UART_UDRE_SetCallBack(void (*LocalFptr)(void));


/*=========================== UART Interrupts ===========================*/
void UART_TxInterruptEnable(void);
void UART_RxInterruptEnable(void);
void UART_UDRE_InterruptEnable(void);


#endif /* UART_H_ */