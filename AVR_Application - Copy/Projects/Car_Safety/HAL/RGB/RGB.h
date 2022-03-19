/*
 * RGB.h
 *
 * Created: 12/29/2021 10:30:54 PM
 *  Author: Ahmed Sedeek
 */ 


#ifndef RGB_H_
#define RGB_H_

/*============== Include section ==============*/
#include <util/delay.h>
#include "Timers.h"
#include "StdTypes.h"
#include "ADC.h"

/*============== Macro Definitions section ==============*/
#define NUM_OF_COLORS	15
#define FIXED_COLORS	3

#define RED				0
#define GREEN			1
#define BLUE			2



/*============== Functions Prototypes ==============*/
void RGB_Init(void);
void RGB_SetColor(u8 color);/*The color must be less than or equal to NUM_OF_COLORS*/
void RGB_RoomShow(void);
void RGB_ControlLight(u8 duty);
void RGB_ControlLeds(u8 duty1, u8 duty2, u8 duty3);



#endif /* RGB_H_ */