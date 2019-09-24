//Cody Davis
#include "fonts.h"

void displayCD(int x, int y)
{
    Rect r;
    int advance = 48;
    r.bot = y;
    r.left = x;
    r.center = 0;
    
    ggprint8b(&r, advance, 0x00ffff44, "Cody Davis");
}
