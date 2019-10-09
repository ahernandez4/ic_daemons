/* Cody Davis
 * CMPS 3350
 * 
 * function: displayCD(int x, int y) 
 * description: meant to display name using ggprint8b
 *      x and y input is given to center the box in
 *      the desired location. see ggprint8b for more
 *      information on proper use.
 * 
 *  stages of debugging:
 *
 *  1)Identify - see what the problem is
 *
 *  2)Isolate - narrow the issue down to a single problem
 *
 *  3)fix - change the code to fix the problem
 *
 *  4)review - look back over the code and see if its fixe
 *
 *
*/

#include "fonts.h"

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
