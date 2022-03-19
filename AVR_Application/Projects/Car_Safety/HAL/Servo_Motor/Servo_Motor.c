/*
 * Servo_Motor.c
 *
 * Created: 12/21/2021 10:29:43 PM
 *  Author: Ahmed Sedeek
 */ 
#include "Servo_Motor.h"

void SERVO_SetAngle(u8 angle)
{
	u16 ServoAngle = ((u32) angle * 1000)/180 + 999;
	TIMER1_OCR1B_SetTime(ServoAngle);
}