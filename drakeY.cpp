//Drake Young
//Group Name: segFault
//Instructor: Gordon Griesel
//3350
//date:    fall 2019
//
//Description: Location for My code 
//added to segFault group project
//
//NOTES: WIP
//
//Four Steps of debugging
//1. Identify -Find the source of bug
//2. Isolate  -Seperate buggy function from rest of code 
//3. Fix      -Fix faulty code
//4. Test     -Test the fix to ensure no new bugs are created

#include "fonts.h"

// Funtion to draw my name for the credits.
void drawDY_Credits(int x, int y)
{
    Rect D;
    
    unsigned int c = 0x00ffff44;

    D.bot = y;   
    D.left = x;
    D.center = 0;

    ggprint8b(&D, 0, c, "Drake Young");
}


