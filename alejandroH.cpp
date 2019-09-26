/* Alejandro Hernandez
 * CMPS 3350
 * function displayAlejandroH(int x, int y)
 * description: displays name using ggprint8b at x,y location
 *
 */
#include "fonts.h"
void displayAlejandroH(int x, int y)
{
	Rect r;
	r.bot = y;
	r.left = x;
	r.center = 0;
	ggprint8b(&r, 32, 0x00ffff44,"Alejandro Hernandez");	
}
