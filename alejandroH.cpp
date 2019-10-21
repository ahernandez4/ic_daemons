/* Alejandro Hernandez
 * CMPS 3350
 * function displayAlejandroH(int x, int y)
 * description: displays name using ggprint8b at x,y location
 * 4 steps to debugging
 * 1: Reproduce the bug
 * 2: Locate the bad code
 * 3: Fix the code
 * 4: Test the fix. And also test the whole program. Make sure
 *    no new problems arise
 */
#include <GL/glx.h>
#include "fonts.h"
//temporary fix globals
int* minutesPlayedPtr = NULL;
void displayAlejandroH(int x, int y, GLuint atexture)
{
    if (minutesPlayedPtr == NULL) {
        *minutesPlayedPtr = 0;
    }

    //black screen
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1,0.1,0.1,0.1);
	//int myx = x;// old values = x + 100;
    //int myy = y;//old valiues = y  + 10;
    //know values
    int resy = 600;
    int resx = 800-64;
    Rect r;
	r.bot = y;
	r.left = x;
	r.center = 0;
	ggprint8b(&r, 32, 0x00ffff44,"Alejandro Hernandez");	
    glColor3f(1.0,1.0,1.0);
    glEnable(GL_ALPHA_TEST);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, atexture);
    glBegin(GL_QUADS);
    //0,1|0,0|1,0|1,1
    glTexCoord2f(0.0f,      1.0f);
    glVertex2i(resx, resy);
    glTexCoord2f(0.0f,      0.0f);    
    glVertex2i(resx, resy+64);
    glTexCoord2f(1.0f,      0.0f);    
    glVertex2i(resx+64, resy+64);
    glTexCoord2f(1.0f,      1.0f);    
    glVertex2i(resx+64, resy);
    glEnd();
    glPopMatrix();
	r.bot = y-50;
	r.left = x;
    if(*minutesPlayedPtr > 0){
	    ggprint8b(&r, 32, 0x00ffff44,
        "Your total played time: %d minutes",*minutesPlayedPtr);	
    }
    else {
        //less than 1 minute placeholder
	    ggprint8b(&r, 32, 0x00ffff44,
        "Your total played time: less than 1 minute");	
    }

}
//temporary fix
void passGlobalValues2Alex(int* min)
{
    minutesPlayedPtr = min;
}
