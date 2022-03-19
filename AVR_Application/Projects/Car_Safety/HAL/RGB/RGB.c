/*
 * RGB.c
 *
 * Created: 12/29/2021 10:30:41 PM
 *  Author: Ahmed Sedeek
 */ 

/*============== Include section ==============*/
#include "RGB.h"

/*============== Globals section ==============*/
u8 RGB_Colors[NUM_OF_COLORS][FIXED_COLORS] = 
{
   /*R    G    B */
	{255, 0  , 0  },
	{0  , 255, 0  },
	{0  , 0  , 255},
	{150, 100, 60 },
	{60 , 120, 130},
	{150, 70 , 60 },
	{60 , 150, 160},
	{150, 180, 90 },
	{190, 50 , 180},
	{150, 90 , 90 },
	{149, 138, 175},
	{144, 174, 157},
	{125, 200, 144},
	{66 , 42 , 146},
	{99 , 57 , 94 }
};


/*
 * Each pin of the three RGB pins must be connected to (OC0 | OC1A | OC1B)
 */
void RGB_Init(void)
{
	TIMER0_Init(TIMER0_FASTPWM_MODE, TIMER0_SCALER_8);
	TIMER0_OC0Mode(OC0_INVERTING);
	TIMER1_ICR_SetTopTime(255);
	
	Timer1_Init(TIMER1_FASTPWM_ICR_TOP_MODE, TIMER1_SCALER_8);
	Timer1_OCRA1Mode(OCRA_INVERTING);
	Timer1_OCRB1Mode(OCRB_INVERTING);
}

void RGB_SetColor(u8 color)
{
	if (color <= NUM_OF_COLORS)
	{
		TIMER0_OC0_SetTime  (RGB_Colors[color][RED]);
		TIMER1_OCR1A_SetTime(RGB_Colors[color][GREEN]);
		TIMER1_OCR1B_SetTime(RGB_Colors[color][BLUE]);
	}
	else
	{
		return 0;
	}
}

void RGB_RoomShow(void)
{
	TIMER0_OC0_SetTime(0);
	TIMER1_OCR1A_SetTime(0);
	TIMER1_OCR1B_SetTime(0);
	for (u8 i = 1; i < 255; i++)
	{
		TIMER0_OC0_SetTime(i);
		_delay_ms(1);
		for (u8 j = 1; j < 255; j++)
		{
			TIMER1_OCR1A_SetTime(j);
			_delay_ms(1);
			for (u8 k = 1; k < 255; k++)
			{
				TIMER1_OCR1B_SetTime(k);
				_delay_ms(1);
			}
		}
	}
}

void RGB_ControlLight(u8 duty)
{
	TIMER0_OC0_SetTime(duty);
	TIMER1_OCR1A_SetTime(duty);
	TIMER1_OCR1B_SetTime(duty);
}

void RGB_ControlLeds(u8 duty1, u8 duty2, u8 duty3)
{
	TIMER0_OC0_SetTime(duty1);
	TIMER1_OCR1A_SetTime(duty2);
	TIMER1_OCR1B_SetTime(duty3);
}
