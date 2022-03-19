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


/* ================ Global Variables Section ================ */
u16 ultra1_Distance, ultra2_Distance, ultra3_Distance;
u8 flag = 0;

/* ================ Main Function Section ================ */
int main(void)
{
	/* Application Initialization */
	Application_Init(); 	
	
	LCD_WriteString("U1     U2     U3");
	
	
    while (1)
    {
		if (flag == 0)
		{
			ultra1_Distance = ULTRASONIC_GetDistance(PINC0);
			LCD_GoTo(SECOND_LINE, CELL_1);
			LCD_WriteNumber(ultra1_Distance);
			flag = 1;
		}
		
		
		if (flag == 1)
		{
			ultra1_Distance = ULTRASONIC_GetDistance(PINC1);
			LCD_GoTo(SECOND_LINE, CELL_7);
			LCD_WriteNumber(ultra1_Distance);
			flag = 2;
		}
		
		if (flag == 2)
		{
			ultra1_Distance = ULTRASONIC_GetDistance(PINC2);
			LCD_GoTo(SECOND_LINE, CELL_14);
			LCD_WriteNumber(ultra1_Distance);
			flag = 0;
		}
		
		
		
		
		
	}
	
}


/*================ Sub Program Section ================*/
void Application_Init(void)
{	
	/*=====Interrupt config=====*/
	sei();	
	
	
	/*=====MCAL Layer Init=====*/
	DIO_Init();
	//ADC_Init(ADC_VCC, ADC_SCALER_64);
	ULTRASONIC_Init();
	
	/*=====ECU Layer Init=====*/
	LCD_Init();
}



 

/*========================================================================
 *	  Revision Log
 *	  Date			   By				Ticket No.			Description
 	  --------------   --------------   --------------		--------------
      4Dec2021		   Ahmed Sedeek	    Ticket-1			Main File
  ========================================================================*/ 
