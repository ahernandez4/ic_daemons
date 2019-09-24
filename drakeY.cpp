//Drake Young
#include "fonts.h"


void drawDY_Credits(int x, int y)
{
    Rect D;
    
    unsigned int c = 0x00ffff44;

    D.bot = y;   
    D.left = x;
    D.center = 0;

    ggprint8b(&D, 0, c, "Drake Young");
}


