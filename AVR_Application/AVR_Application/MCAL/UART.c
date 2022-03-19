/*
 * UART.c
 *
 * Created: 12/14/2021 8:58:42 AM
 *  Author: Ahmed Sedeek
 */ 
#include "UART.h"

static void (*UART_TxC_Fptr)	 (void)	= NULL_PTR;
static void (*UART_RxC_Fptr)	 (void)	= NULL_PTR;
static void (*UART_UDRE_Fptr)	 (void) = NULL_PTR;


u8 initFlag = 0;
/* This Func Starts UART */
void UART_Init(void)
{
	//Baudrate 9600
	UBRRL = 51;
	//normal speed
	CLEAR_BIT(UCSRA, U2X);
	
	/*Default frame 8 data 1 stop no parity*/
	
	//Enable Receiver and Transmitter peripherals
	SET_BIT(UCSRB, RXEN);
	SET_BIT(UCSRB, TXEN);
	initFlag = 1;
}


/* This Function Sends Byte Through UART */
void UART_Send(u8 data)
{
	
	if (initFlag == 1)
	{
		/* This Line Polls UDRE Flag to Check if Tx Register "UDR" Can Be Written*/
		while(!READ_BIT(UCSRA ,UDRE));
		
		/* This Line Assigns Data to UDR Register to be Sent Using UART */
		UDR = data;
	}
}


/* This Function Receive Byte Through UART (Blocking)*/
u8 UART_Receive(void)
{
	/*W8 till the receive is complete*/
	while (!READ_BIT(UCSRA, RXC));
	
	return UDR;
}

 /*(Non Blocking)*/
u8 UART_Receive_PeriodicCheck(u8 *pdata)
{
	/*If receive complete flag is set*/
	if (READ_BIT(UCSRA, RXC))
	{
		*pdata = UDR; // Return the data
		return 1; // Indication that the data arrived
	}
	return 0;// Indication that the data did not arrived
}

void UART_SendByteNonBlocking(u8 data)
{
	UDR = data;
}

u8 UART_ReadByteNonBlocking(void)
{
	return UDR;
}

/*=========================== UART CallBack Functions ===========================*/
void UART_TxC_SetCallBack(void (*LocalFptr)(void))
{
	UART_TxC_Fptr = LocalFptr;
}

void UART_RxC_SetCallBack(void (*LocalFptr)(void))
{
	UART_RxC_Fptr = LocalFptr;
}

void UART_UDRE_SetCallBack(void (*LocalFptr)(void))
{
	UART_UDRE_Fptr = LocalFptr;
}

/*=========================== UART Interrupts CFG ===========================*/
void UART_TxInterruptEnable(void)
{
	SET_BIT(UCSRB, TXCIE);
}

void UART_RxInterruptEnable(void)
{
	SET_BIT(UCSRB, RXCIE);
}

void UART_UDRE_InterruptEnable(void)
{
	SET_BIT(UCSRB, UDRIE);
}

/*=========================== UART ISRs ===========================*/

/* USART, Rx Complete */
ISR(UART_RX_vect)
{
	if (UART_RxC_Fptr != NULL_PTR)
	{
		UART_RxC_Fptr();
	}
}

/* USART, Tx Complete */
ISR(UART_TX_vect)
{
	if (UART_TxC_Fptr != NULL_PTR)
	{
		UART_TxC_Fptr();
	}
}

/* USART Data Register Empty */
ISR(UART_UDRE_vect)
{
	if (UART_UDRE_Fptr != NULL_PTR)
	{
		UART_UDRE_Fptr();
	}
}


