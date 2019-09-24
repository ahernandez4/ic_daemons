//Alejandro Hernandez
#include "fonts.h"
void showAH(int x, int y)
{
	Rect r;
	r.bot = y;
	r.left = x;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ffff44,"Alejandro Hernandez");	
	//ggprint8b(&r, 16, 0x00ff0000,"Alejandro Hernandez");	
}
