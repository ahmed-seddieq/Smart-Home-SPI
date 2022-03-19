/*
 * Ultrasonic.h
 *
 * Created: 12/25/2021 4:07:37 PM
 *  Author: Ahmed Sedeek
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

/*================= Include Section =================*/
#include "DIO_Interface.h"
#include "Timers.h"
#include "StdTypes.h"
#include <util/delay.h>
#include "LCD.h"

/*================= Macro Definitions Section =================*/
#define ULTRASONIC_1	0x00
#define ULTRASONIC_2	0x01
#define ULTRASONIC_3	0x02
#define ULTRASONIC_4	0x03




/*================= Function Prototype Section =================*/
void ULTRASONIC_Init(void);
void ULTRASONIC_GenerateTrigger(DIO_Pin_type TriggerPin);
u16 ULTRASONIC_GetDistance(DIO_Pin_type TriggerPin);

#endif /* ULTRASONIC_H_ */