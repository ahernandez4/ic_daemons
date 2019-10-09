/* Alejandro Hernandez
 * CMPS 3350
 * function displayAlejandroH(int x, int y)
 * description: displays name using ggprint8b at x,y location
 * 4 steps to debugging
 * 1: Reproduce the bug
 * 2: Locate the bad code
 * 3: Fix the code
 * 4: Test the fix. And also test the whole program. Make sure
 *    no new problems arrives
 */
#include <GL/glx.h>
#include "fonts.h"
void displayAlejandroH(int x, int y, GLuint atexture)
{
	int myx = x + 100;
    int myy = y + 10;
    Rect r;
	r.bot = y;
	r.left = myx;
	r.center = 0;
	ggprint8b(&r, 32, 0x00ffff44,"Alejandro Hernandez");	
    glColor3f(1.0,1.0,1.0);
    glEnable(GL_ALPHA_TEST);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, atexture);
    glBegin(GL_QUADS);
    //0,1|0,0|1,0|1,1
    glTexCoord2f(0.0f,      1.0f);
    glVertex2i(myx, myy);
    glTexCoord2f(0.0f,      0.0f);    
    glVertex2i(myx, myy+64);
    glTexCoord2f(1.0f,      0.0f);    
    glVertex2i(myx+64, myy+64);
    glTexCoord2f(1.0f,      1.0f);    
    glVertex2i(myx+64, myy);
    glEnd();
    glPopMatrix();

}
