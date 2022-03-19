/*
 * UART_Services.c
 *
 * Created: 1/7/2022 8:08:03 PM
 *  Author: Ahmed Sedeek
 */ 

/*=========================== Includes ===========================*/
#include "UART_Services.h"

/*=========================== Function Declarations ===========================*/
static void Tx_FuncCallBack(void);
static void Rx_FuncCallBack(void);

/*=========================== Global variables ===========================*/
static RxFlag = 0;
static TxFlag = 0;

static u8 *Tx_str;
/*Array of pointers contains the UART commands*/
static u8 *UART_arrPtrs [50]; 
static u8 PointersIter =  0;

u8 ECUdevice = 0;

extern volatile u8 recFlag;

/*The commands to check*/
u8 *commands[4] = {"LedOn", "LedOff", "LCD", "Servo"};

/*The functions to be executed according to the command*/
void (*FunPtr[4]) (u8 ecuNumber) = { LedOn, LedOff, LCD_num, Servo };
	


/*UART Stack*/

/*UART stack to store bytes in it*/
static volatile  UART_Stack[UART_STACK_SIZE];
/*Stack pointer to point to the empty index to store in it*/
static volatile s8 UART_sp = 0; 

/*UART Queue*/

/*UART stack to store bytes in it*/
static volatile u8 UART_Queue[UART_QUEUE_SIZE];

/*Queue pointers to point to the empty index to store in it*/
static volatile s8 InQueue = 0;
static volatile s8 DeQueue = 0;
static volatile s8 cicularFlag = 0;

/*=========================== Functions Definitions ===========================*/
/*Synch function (completes its functionality b4 returning)*/
void UART_SendString(u8 *str)
{
	for (u8 i = 0; str[i]; i++)
	{
		UART_Send(str[i]);
	}
}

/*Synch function (completes its functionality b4 returning)*/
void UART_ReceiveString(u8 *str)
{
	u8 i = 0;
	str[i] =  UART_Receive(); // Receive first, and then check
	for (i = 0; str[i] != 13; )
	{
		i++;
		str[i] = UART_Receive();
	}
	str[i] = 0;
}

/************** UART Async functions **************/
void UART_SendString_Async(u8 *str)
{
	//UART_Send(str[0]);
	if (TxFlag == 1)
	{
		TxFlag = 0;
		UART_TxC_SetCallBack(Tx_FuncCallBack);
		UART_TxInterruptEnable();
		UART_SendByteNonBlocking(str[0]);
		Tx_str = str; // give the received buffer to the ISR
	}
	
	
}

void UART_ReceiveString_Async(u8 *str)
{
	UART_RxC_SetCallBack(Rx_FuncCallBack);
	UART_RxInterruptEnable();
	UART_arrPtrs[PointersIter] = str; // Giving the string to the ISR
	
}

/*===================== Frame Services =====================*/
void UART_CommandFrame(u8 *command, u8 *ECU, u8 *ecuNum)
{
	u8 counter = 0, foundFlag = 0;
	for(u8 i = 0; command[i] != ' '; i++)
	{
		ECU[i] = command[i];
		counter++;
	}
	ECU[counter] = 0;
	for(u8 i = 0; i < 4 ; i++)
	{
		if(string_match_usingSize(command, commands[i], counter) == 1)
		{
			foundFlag = 1;
			ECUdevice = i;
			break;
		}
	}
	
	/*Right command*/
	if(foundFlag == 1)
	{
		u8 number = 0;
		u8 i;
		/*Save ECU Number*/
		for(i = (counter+1); command[i]; i++)
		{
			number = number*10 + (command[i] - '0');
		}
				
		*ecuNum = number;
		foundFlag = 0;
	}

}

u8 string_match_usingSize(u8* str1, u8* str2, u8 size)
{
	int iter;
	for(iter = 0; iter < size; iter++)
	{
		if(str1[iter] !=  str2[iter])
		{
			return 0;
		}
	}
	return 1;
}

u8 string_compare(u8* str1, u8* str2)
{
	u8 iter;
	for(iter = 0; str1[iter] || str2[iter]; iter++)
	{
		if(str1[iter] != str2[iter])
		{
			return 0;
		}
	}
	return 1;
}

void UART_CommandECU(u8 ecu)
{
	switch(ECUdevice)
	{
		case LED_ON:
			FunPtr[0](ecu);
		break;
		
		case LED_OFF:
			FunPtr[1](ecu);
		break;
		
		case LCD_COM:
			FunPtr[2](ecu);
		break;
		
		case SERVO:
			FunPtr[3](ecu);
		break;
		default: break;
	}
	
}

void LedOn(u8 led)
{
	if (1 == led)
	{
		DIO_WritePin(PINC0, HIGH);
	}
	else if (2 == led)
	{
		DIO_WritePin(PINC1, HIGH);
	}
	else if (3 == led)
	{
		DIO_WritePin(PINC2, HIGH);
	}
}

void LedOff(u8 led)
{
	if (1 == led)
	{
		DIO_WritePin(PINC0, LOW);
	}
	else if (2 == led)
	{
		DIO_WritePin(PINC1, LOW);
	}
	else if (3 == led)
	{
		DIO_WritePin(PINC2, LOW);
	}
}

void LCD_num(u8 LCDnum)
{
	LCD_GoToClear(SECOND_LINE, CELL_1, 16);
	LCD_GoTo(SECOND_LINE, CELL_1);
	LCD_WriteString("Number: ");
	LCD_GoTo(SECOND_LINE, CELL_8);
	LCD_WriteNumber(LCDnum);
}

void Servo(u8 angle)
{
	SERVO_Init();
	SERVO_SetAngle(angle);
}


/************** UART Tx and Rx CallBacks **************/
static void Tx_FuncCallBack(void)
{
	static u8 i = 1;
	if (Tx_str[i] != 0)
	{
		UART_SendByteNonBlocking(Tx_str[i]);
		i++;
	}
	else
	{
		i = 1;
		TxFlag = 1;
	}
	
	
}

static void Rx_FuncCallBack(void)
{
	/* as an ensure to start the new string from the beginning*/
	static u8 i, flag = 0; 
	if (flag == 0)
	{
		i = 0;
		flag = 1;
	}
	/*the interrupt says (I have a byte, come and put it in the string)*/
	UART_arrPtrs[PointersIter][i] = UART_ReadByteNonBlocking();
	/*If the byte is Enter, set a flag*/
	i++;
	
	if (13 == UART_arrPtrs[PointersIter][i-1])
	{
		UART_arrPtrs[PointersIter][i-1] = '\0';
		recFlag = 1;
		i = 0;
		PointersIter++;
	}
	
	
	
	
	//RxFlag = 1;
	
}



void UART_SendNumberU32(u32 num)
{
// 	UART_Send( (u8)  num);
// 	UART_Send( (u8) (num >> 8) );
// 	UART_Send( (u8) (num >> 16) );
// 	UART_Send( (u8) (num >> 24) );

	u8 *ptr = &num;
	UART_Send(ptr[0]);
	UART_Send(ptr[1]);
	UART_Send(ptr[2]);
	UART_Send(ptr[3]);
}

u32 UART_ReceiveNumberU32(void)
{
// 	u32 b1 = UART_Receive();
// 	u32 b2 = UART_Receive();
// 	u32 b3 = UART_Receive();
// 	u32 b4 = UART_Receive();
// 	
// 	u32 num = 0;
// 	num = b1 | b2 << 8 | b2 << 16 | b2 << 24;

	u32 num;
	u8 *ptr = &num;
	ptr[0] = UART_Receive();
	ptr[1] = UART_Receive();
	ptr[2] = UART_Receive();
	ptr[3] = UART_Receive();
	return num;
}

void UART_SendStringCheckSum(u8 *str)
{
	u8 i, lens = 0;
	u16 sum =0;
	/*count the length of the string*/
	for (lens = 0; str[lens]; lens++);
	UART_Send(lens);
	for (i=0; i<lens; i++)
	{
		UART_Send(str[i]);
		sum += str[i];
	}
	UART_Send((u8)sum);
	UART_Send((u8) (sum>>8) );
}

u8 UART_ReceiveStringCheckSum(u8 *str)
{
	u8 len, i, b1, b2;
	u16 sumCalc = 0, sumReceived = 0;
	len = UART_Receive();
	for (u8 i = 0; i < len; i++)
	{
		str[i] = UART_Receive();
		sumCalc += str[i];
	}
	b1 = UART_Receive();
	b2 = UART_Receive();
	sumReceived = (u16)b1 | (u16)(b2<<8);
	str[i] = 0;
	if (sumReceived == sumCalc)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/************** UART Stack And Queue **************/

UART_StackStatus UART_PushByte(u8 byte)
{
	UART_StackStatus status = UART_SCACK_DONE;
	/*If the stack is full*/
	if (UART_sp == UART_STACK_SIZE)
	{
		/*Stack is full*/
		status = UART_STACK_FULL;
	}
	else
	{
		UART_Stack[UART_sp] = byte;
		UART_sp++;
		status = UART_SCACK_DONE;
	}
	
	/*Return the status of the stack*/
	return status;
}

UART_StackStatus UART_PopByte(u8 *byte_ptr)
{
	UART_StackStatus status = UART_SCACK_DONE;
	/*If the stack is empty*/
	if (UART_sp == 0)
	{
		/*There's no data to be popped*/
		status = UART_STACK_EMPTY;
	}
	else
	{
		UART_sp--;
		/*Return the popped data*/
		*byte_ptr = UART_Stack[UART_sp] ;
		
		status = UART_SCACK_DONE;
	}
	
	/*Return the status of the stack*/
	return status;
}


UART_QueueStatus UART_InQueue(u8 byte)
{
	
	UART_QueueStatus status ;
	if (InQueue == UART_DeQueue && cicularFlag == 1)
	{
		/*Then the queue is full*/
		status = UART_QUEUE_FULL;
	}
	else
	{
		UART_Queue[InQueue] = byte;
		InQueue++;
		status = UART_QUEUE_DONE;
	}
	if (UART_QUEUE_SIZE == InQueue)
	{
		InQueue = 0;
		cicularFlag = 1;
	}
	return status;
}

UART_QueueStatus UART_DeQueue(u8 *byte_ptr)
{
	if (DeQueue == UART_QUEUE_SIZE)
	{
		DeQueue = 0;
		cicularFlag = 0;
	}
	if (InQueue == DeQueue && cicularFlag == 0)
	{
		/*Then the queue is empty*/
		return UART_QUEUE_EMPTY;
	}
	else
	{
		/*flag = 1 and in and out equals each other*/
		*byte_ptr = UART_Queue[DeQueue];
		DeQueue++;
		return UART_QUEUE_DONE;
	}
}