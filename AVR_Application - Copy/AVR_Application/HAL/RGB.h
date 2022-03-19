
#ifndef RGB_H_
#define RGB_H_

#include "TIMERS.h"
/* enum of colors which will be used ,that will ease the access to colors */
typedef enum{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	MAGENTA,
	CYAN,
	WHITE,
	ORANGE,
	PURBLE,
	LEMON,
	TEAL,
	VIOLET,
	COBALT,
	PINK
}Color_t;
/* values will be in  OCR0	OCR1A	OCR1B in sequence for each color */
#define array_of_colors		{\
	{255,0,0},\
	{0,255,0},\
	{0,0,255},\
	{255,255,0},\
	{255,0,255},\
	{0,255,255},\
	{255,255,255},\
	{255,39,0},\
	{255,0,39},\
	{20,255,0},\
	{0,255,39},\
	{20,0,255},\
	{0,39,255},\
	{158,39,255}};

	/*Array of names of colors,it will be used to display color on LCD */
	static char *colors_names[]={"RED","GREEN","BLUE","YELLOW","MAGENTA","CYAN","WHITE","ORANGE","PURBLE","LEMON","TEAL","VIOLET","COBALT","PINK"};

	void RGB_Init(void);
	void RGB_set_color(Color_t local_color);
	void RGB_ShowroomAsynch(Color_t local_color);

	#endif /* RGB_H_ */