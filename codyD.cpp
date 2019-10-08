/* Cody Davis
 * CMPS 3350
 * 
 * function: displayCD(int x, int y) 
 * description: meant to display name using ggprint8b
 *      x and y input is given to center the box in
 *      the desired location. see ggprint8b for more
 *      information on proper use.
 *
 *
*/

#include "fonts.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
extern int odinGetTime();

void displayCD(int x, int y)
{
    Rect r;
    int advance = 48;
    r.bot = y;
    r.left = x;
    r.center = 0;
    unsigned int color = 0x00ffff44; 
    
    ggprint8b(&r, advance, color, "Cody Davis");
}
/*void timePlayD(int x = 0, int y = 0)
{

    Rect r;
    int advance = 0;
    r.bot = y;
    r.left = x;
    r.center = 0;
    int time = 0;
    unsigned int color = 0x00ffff44;

    time = odinGetTime();
    std::string tmp;
    tmp = time;
    std::cout << tmp;
    //ggprint8b(&r, advance, color, (char)tmp);
    
    
}*/
