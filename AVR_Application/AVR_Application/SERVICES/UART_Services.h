/*
 * UART_Services.h
 *
 * Created: 1/7/2022 8:07:42 PM
 *  Author: Ahmed Sedeek
 */ 


#ifndef UART_SERVICES_H_
#define UART_SERVICES_H_

#include "UART.h"
#include "StdTypes.h"
#include "LCD.h"
#include "DIO_Interface.h"

/*----------------- Macros -----------------*/
#define UART_STACK_SIZE	15
#define UART_QUEUE_SIZE	15

#define LED_ON	0
#define LED_OFF	1
#define LCD_COM	2
#define SERVO	3

/*----------------- Type Defined -----------------*/
typedef enum
{
	UART_STACK_EMPTY,
	UART_STACK_FULL,
	UART_SCACK_DONE
}UART_StackStatus;

typedef enum
{
	UART_QUEUE_EMPTY,
	UART_QUEUE_FULL,
	UART_QUEUE_DONE
}UART_QueueStatus;


/*-------------------- Functions Prototypes --------------------*/
void UART_SendString(u8 *str);
void UART_ReceiveString(u8 *str);

void UART_SendNumberU32(u32 num);
u32 UART_ReceiveNumberU32(void);

void UART_SendString_Async(u8 *str);
void UART_ReceiveString_Async(u8 *str);

void UART_SendStringCheckSum(u8 *str);
u8 UART_ReceiveStringCheckSum(u8 *str);



void UART_CommandFrame(u8 *command, u8 *ECU, u8 *ecuNum);
u8 string_match_usingSize(u8* str1, u8* str2, u8 size);
u8 string_compare(u8* str1, u8* str2);
void UART_CommandECU(u8 ecu);


/*ECUs Services*/
void LedOn(u8 led);
void LedOff(u8 led);
void LCD_num(u8 LCDnum);
void Servo(u8 angle);


/*If it returns STACK FULL , print stack full on the lcd or what ever u want in main*/
UART_StackStatus UART_PushByte(u8 byte);

/*This function returns:
 * - UART_StackStatus: The status of the stack
 * - byte_ptr		 : The address of the popped byte from the stack
*/
UART_StackStatus UART_PopByte(u8 *byte_ptr);

/*UART Queues*/

/*If it returns queue FULL , print stack full on the lcd or what ever u want in main*/
UART_QueueStatus UART_InQueue(u8 byte);

/*This function returns:
 * - UART_QueueStatus: The status of the queue
 * - byte_ptr		 : The address of the popped byte from the queue
*/
UART_QueueStatus UART_DeQueue(u8 *byte_ptr);

#endif /* UART_SERVICES_H_ */