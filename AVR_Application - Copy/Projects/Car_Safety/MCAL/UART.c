/*
 * UART.c
 *
 * Created: 12/14/2021 8:58:42 AM
 *  Author: Ahmed Sedeek
 */ 
#include "UART.h"


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
}


/* This Function Sends Byte Through UART */
void UART_Send(u8 data)
{
	/* This Line Polls UDRE Flag to Check if Tx Register "UDR" Can Be Written*/
	while(!READ_BIT(UCSRA ,UDRE));
	
	/* This Line Assigns Data to UDR Register to be Sent Using UART */
	UDR = data;
}


/* This Function Receive Byte Through UART*/
u8 UART_Receive(void)
{
	while (!READ_BIT(UCSRA, RXC));
	return UDR;
}

u8 UART_Receive_PeriodicCheck(u8 *pdata)
{
	if (READ_BIT(UCSRA, RXC))
	{
		*pdata = UDR;
		return 1;
	}
	return 0;
}

