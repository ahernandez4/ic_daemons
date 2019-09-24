//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <time.h>
//#include <math.h>
//#include <X11/Xlib.h>
//#include <X11/keysym.h>
//#include <GL/glx.h>
#include "fonts.h"
//#include <iostream>


void tjcredits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;


    ggprint8b(&r, 16, 0x00ffff44, "Tawfic Jobah");
     
}
