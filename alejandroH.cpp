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
#include <iostream>
#include <fstream> 
#include <string> //maybe remove
//temporary fix globals
#define MAP_TILE_ROWS 150
#define MAP_TILE_COLUMNS 250
int* minutesPlayedPtr;// = NULL;
//make sure we load the map before trying to use it
static int mapfileloaded = 0;
unsigned char maparray[MAP_TILE_ROWS][MAP_TILE_COLUMNS];
struct playerPtrs{
    int* x = NULL;
    int* y = NULL;
    int* speed = NULL;
} *playerptrs;
//
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
                "Your total played time: %d minutes",
                *minutesPlayedPtr);	
    }
    else {
        //less than 1 minute placeholder
        ggprint8b(&r, 32, 0x00ffff44,
                "Your total played time: less than 1 minute");	
    }
    //std::cout << *(playerptrs->x) << std::endl;

}
//temporary fix
void passGlobalValues2Alex(int* min)
{
    minutesPlayedPtr = min;
}
void passPlayerPtrs2Alex(int* x, int* y, int*s)
{
    if(playerptrs == NULL)
        return;
    (playerptrs->x) = x;
    (playerptrs->y) = y;
    (playerptrs->speed) = s;
}
void initInternalAlexStuff()
{
    minutesPlayedPtr = NULL;
    playerptrs = new playerPtrs();
}
void deallocAlexStuff()
{
    delete playerptrs;
}
void drawMap(GLuint)
{
    float tx = 0.0;
    //float ty = 0.0;
    //float 
    if(mapfileloaded == 0)
        return;
    //

    glColor3f(1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    //glColor4ub(255,255,255,255);
    //as long as logic works out
    //it doesnt matter which direction we draw

    glTexCoord2f(0.4f,      1.0f);
    glVertex2i(0, 0);
    glTexCoord2f(0.4f,      0.0f);
    glVertex2i(0, 600);
    glTexCoord2f(0.4f+0.1f,      0.0f);
    glVertex2i(800, 600);
    glTexCoord2f(0.4f+0.1f,      1.0f);
    glVertex2i(800, 0);


    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int rows = MAP_TILE_ROWS -1; rows >= 0; rows--) {
        for (int cols = 0; cols < MAP_TILE_COLUMNS; cols++) {
            int ysmap = 149 - rows;
            char c = '0';
            c = maparray[rows][cols];
            //maybe switch to a switch(c){ statement
            //
            if(c != '0') {
                int ix = ((int) c) - 49;
                //int iy = 0;
                tx = (float)ix / 10.0;
                //once we have multiple rows of tiles
                //ty = (float)iy / 2.0;
                glBegin(GL_QUADS);
                //must be glTex... followed by glVert..
                //
                glTexCoord2f(tx,      1.0f); 
                glVertex2i(32*(cols+0.5)-16, 32*(ysmap+0.5)-16);
                glTexCoord2f(tx,      0.0f);    
                glVertex2i(32*(cols+0.5)-16, 32*(ysmap+0.5)+16);
                glTexCoord2f(tx+.100, 0.0f);    
                glVertex2i(32*(cols+0.5)+16, 32*(ysmap+0.5)+16);
                glTexCoord2f(tx+.100, 1.0f); 
                glVertex2i(32*(cols+0.5)+16, 32*(ysmap+0.5)-16);
                glEnd();
                //glPopMatrix();
                //glDisable(GL_ALPHA_TEST);
            }
        }
        }
        glPopMatrix();

        //
    }
    void loadMapFile()
    {
        //MAP_TILE_ROWS][MAP_TILE_COLUMNS];

        mapfileloaded = 1; //we loaded the map
        //
        std::ifstream mapifile;
        mapifile.open("map.txt",std::ifstream::in);
        //while we havent reached end of file
        char character;
        int row = 0, col = 0;
        while (!mapifile.eof()) {
            //
            mapifile.get(character);
            //check character is within range
            if ((character < 58 && character > 47) || 
                    (character < 91 && character > 64) ||
                    (character < 123 && character > 96)) {
                if(col >=MAP_TILE_COLUMNS) {
                    col = 0;
                    row++;
                }
                maparray[row][col++] = character;
            }
        }
        mapifile.close();
        //for (int i = 0; i < 100; i++) {
        //    std::cout << maparray[148][i] << " " << std::endl;
        //}
    }

