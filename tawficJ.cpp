#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <iostream>


void tjcredits(int x, int y) //, GLuint texid)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;


    ggprint8b(&r, 16, 0x00ffff44, "Tawfic Jobah");

    /*glPushMatrix();
    glTranslatef(x,y,0);
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    */

}
