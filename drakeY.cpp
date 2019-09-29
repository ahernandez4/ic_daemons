//Drake Young
//Group Name: segFault
//Instructor: Gordon Griesel
//3350
//date:    fall 2019
//
//Location for My code added to segFault 
//group project
//
//NOTES: WIP

#include "fonts.h"

// Funtion to draw my name for the credits.
void drawDY_Credits(int x, int y)
{
    Rect D;
    
    unsigned int c = 0x00ffff44;

    D.bot = x;   
    D.left = y;
    D.center = 200;

    ggprint8b(&D, 0, c, "Drake Young");
}


