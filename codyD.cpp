//Cody Davis
#include "fonts.h"

void displayCD(int x, int y)
{
    Rect r;
    int advance = 0;
    r.bot = 20;
    r.left = x;
    r.center = y;
    
    ggprint8b(&r, advance, 0x00ffff44, "Cody Davis");
}
