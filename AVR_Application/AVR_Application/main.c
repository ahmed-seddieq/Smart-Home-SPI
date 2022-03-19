/*
 * @File		: AVR_Application.c
 * @Created     : 10/31/2021 1:30:27 PM
 * @Author      : Ahmed Sedeek
 * @GitHub      : https://github.com/Ahmed-Sedeek
 * @Linked In   : https://www.linkedin.com/in/ahmed-sedeek-/
 */ 


/*================================================================
 * @Project			: 
 * @Platform		: ATmega
 * @Peripheral		: ATmega32
  ================================================================*/ 


/* ================ Linking Section ================ */
#include "Application.h"
#define ABOVE_CRITICAL	1
#define UNDER_CRITICAL	2


/* ================ Global Variables Section ================ */
static volatile u8 dataRec;
u8 dataSend = 'a';
static volatile u8 getTempFlag = 0 ;
static volatile u8 exceedTempFlag = 0 ;

/* ================ Functions Prototypes Section ================ */
void ExtINT1_CallBack(void);
void ExtINT0_CallBack(void);

/* ================ Main Function Section ================ */
int main(void)
{
	/* ==================== */
	Application_Init();
	/* ==================== */
	
	/*------- Variables -------*/
	u8 criticalTemp = 45;
	u8 criticalFlag = UNDER_CRITICAL;
	
	
	/*------- Functions -------*/
	LCD_GoToWriteString(FIRST_LINE, CELL_6, "Master");
	LCD_GoToWriteString(SECOND_LINE, CELL_1, "Temp: ");
	
    while (1)
    {
		/*Sending dummy, Receiving temp via SPI*/
		dataRec = SPI_SendReceive(dataSend);
		
		/*Getting Temp*/
		if (1 ==  getTempFlag)
		{
			LCD_GoTo(SECOND_LINE, CELL_7);
			LCD_WriteNumber(dataRec);
			LCD_WriteString("    ");
			getTempFlag = 0;
		}
		
		/*Temp exceeded limit*/
		if (1 ==  exceedTempFlag)
		{
			criticalFlag = ABOVE_CRITICAL;
			LCD_GoTo(SECOND_LINE, CELL_7);
			LCD_WriteNumber(dataRec);
			LCD_WriteString("    ");	
// 			DIO_WritePin(PINB0, HIGH);
// 			_delay_ms(500);
// 			DIO_WritePin(PINB0, LOW);
// 			_delay_ms(500);
		}
		
		if (dataRec <= 45)
		{
			exceedTempFlag = 0;
		}
		else
		{
			exceedTempFlag = 1;
		}
		
	}
	
}


/*================ Sub Program Section ================*/

void Application_Init(void)
{	
	/*=====MCAL Layer Init=====*/
	DIO_Init();
	//ADC_Init(ADC_VCC, ADC_SCALER_64);
	SPI_Init(MASTER);	
	
	/*=====Interrupt config=====*/
	sei();
	EXI_Enable(EX_INT1);
	EXI_TriggerEdge(EX_INT1, FALLING_EDGE);
	EXI_SetCallBack(EX_INT1, ExtINT1_CallBack);
	
	EXI_Enable(EX_INT0);
	EXI_TriggerEdge(EX_INT0, RISING_EDGE);
	EXI_SetCallBack(EX_INT0, ExtINT0_CallBack);
	
	
	/*=====ECU Layer Init=====*/
	LCD_Init();
}

/*Getting Temp by order of the Master*/
void ExtINT1_CallBack(void)
{
	getTempFlag = 1;
}

/*Getting temp when it exceeds 45*/
void ExtINT0_CallBack(void)
{
	exceedTempFlag = 1;
}






/*========================================================================
 *	  Revision Log
 *	  Date			   By				Ticket No.			Description
 	  --------------   --------------   --------------		--------------
      4Dec2021		   Ahmed Sedeek	    Ticket-1			Main File
  ========================================================================*/ 
