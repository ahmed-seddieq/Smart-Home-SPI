/*
 * Ultrasonic.c
 *
 * Created: 12/25/2021 4:07:23 PM
 *  Author: Ahmed Sedeek
 */ 

#include "Ultrasonic.h"

/*=========== Global Variables ===========*/

static volatile u16 firstRising, firstFalling, ultraDistance, dutyTime;
static volatile u8 capFlag ;


/*=========== Functions Definitions ===========*/


static void ULTRASONIC_CaptureTimes(void)
{
	if (capFlag == 0)
	{
		firstRising = ICR1; // Capture the first rising edge time
		Timer1_InputCaptureEdge(FALLING);
		capFlag = 1;
		
	}
	else if (capFlag == 1)
	{
		firstFalling = ICR1; // Capture the first falling edge time
		Timer1_ICU_InterruptDisable();
		capFlag = 3;
	}
}


void ULTRASONIC_Init(void)
{
	Timer1_Init(TIMER1_NORMAL_MODE, TIMER1_SCALER_8);
	Timer1_ICU_SetCallBack(ULTRASONIC_CaptureTimes);
}

void ULTRASONIC_GenerateTrigger(DIO_Pin_type TriggerPin)
{
	DIO_WritePin(TriggerPin, HIGH);
	_delay_us(100);
	DIO_WritePin(TriggerPin, LOW);
}

u16 ULTRASONIC_GetDistance(DIO_Pin_type TriggerPin)
{
	
	TCNT1 = 0;
	Timer1_InputCaptureEdge(RISING);
	Timer1_ICU_InterruptEnable();
	capFlag = 0;
	ULTRASONIC_GenerateTrigger(TriggerPin);
	while(capFlag < 2) {} // W8 till u get the rising and falling edges times
	dutyTime = firstFalling - firstRising;
	ultraDistance = dutyTime / 58;
	
	capFlag = 0;
	
	return ultraDistance;
}


