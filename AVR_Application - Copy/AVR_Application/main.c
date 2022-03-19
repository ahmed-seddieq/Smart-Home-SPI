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


/* ================ Functions Prototypes Section ================ */



/* ================ Main Function Section ================ */
int main(void)
{
	/* ==================== */
	Application_Init();
	/* ==================== */
	
	/*------- Variables -------*/
	u16 dataSend;
	u8 tempInt, dataRec;
	
	
	/*------- Functions -------*/
	LCD_GoToWriteString(FIRST_LINE, CELL_6, "Slave");
	LCD_GoToWriteString(SECOND_LINE, CELL_1, "Temp: ");
	
	
	SPI_Send(tempInt);
	
    while (1)
    {
		/*Displaying the temp on the Slave's LCD*/
		dataSend = LM35_GetTemp();
		tempInt = dataSend / 10;
		LCD_GoTo(SECOND_LINE, CELL_7);
		LCD_WriteNumber(tempInt);
		LCD_WriteString("    ");
		
		if (tempInt > 45)
		{
			DIO_WritePin(PINB0, HIGH); // Fire alarm
		}
		
		
		/*Periodically check if u as a slave received data by checking the flag*/
		if (SPI_ReceivePeriodic(&dataRec))
		{
			/*Sending the temp to Master*/
			SPI_Send(tempInt);	
		}
		
	}
	
}


/*================ Sub Program Section ================*/

void Application_Init(void)
{	
	/*=====MCAL Layer Init=====*/
	DIO_Init();
	ADC_Init(ADC_VCC, ADC_SCALER_64);
	SPI_Init(SLAVE);	
	
	
	/*=====Interrupts config=====*/
	sei();
	
	
	/*=====ECU Layer Init=====*/
	LCD_Init();
}






/*========================================================================
 *	  Revision Log
 *	  Date			   By				Ticket No.			Description
 	  --------------   --------------   --------------		--------------
      4Dec2021		   Ahmed Sedeek	    Ticket-1			Main File
  ========================================================================*/ 
