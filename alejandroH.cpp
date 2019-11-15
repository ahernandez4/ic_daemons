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
#include <fstream> 
#include <string>
//temporary fix globals
#define MAP_TILE_ROWS 150
#define MAP_TILE_COLUMNS 250
int* minutesPlayedPtr;// = NULL;
//make sure we load the map before trying to use it
static int mapfileloaded = 0;
unsigned char maparray[MAP_TILE_ROWS][MAP_TILE_COLUMNS];
struct PlayerPtrs{
    int* x = NULL;
    int* y = NULL;
    int* speed = NULL;
};//
//temporary fix
static PlayerPtrs *playerptrs = NULL;
static int *playerptrsx = 0;
static int *playerptrsy = 0;
// This is my Friday code
// ~~~~~
// ~~~~~
//
class AlexGlobal{
    private:
    int *x,*y;
    static AlexGlobal * instance;
    AlexGlobal() {
        x = nullptr;
        y = nullptr;
    }
    AlexGlobal(AlexGlobal const& copy);
    AlexGlobal & operator = (AlexGlobal const& copy);
    public:
    static AlexGlobal *GetInstance(){
        if(!instance){
            instance = new AlexGlobal();
        }
        return instance;
    }
};
AlexGlobal* AlexGlobal::instance = 0;
AlexGlobal * ag = AlexGlobal::GetInstance();
//to here
void displayAlejandroH(int x, int y, GLuint atexture)
{
    if (minutesPlayedPtr == NULL) {
        *minutesPlayedPtr = 0;
    }

    //black screen
    glClearColor(0.1,0.1,0.1,0.1);
    glClear(GL_COLOR_BUFFER_BIT);
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

}
void passGlobalValues2Alex(int* min)
{
    minutesPlayedPtr = min;
}
void passPlayerPtrs2Alex(int* x, int* y, int*s)
{
    if(playerptrs == NULL)
        return;

    playerptrs->x = x;
    playerptrs->y = y;
    playerptrs->speed = s;
    playerptrsx = x;
    playerptrsy = y;
}
void initInternalAlexStuff()
{
    minutesPlayedPtr = NULL;
    playerptrs = new PlayerPtrs();
}
void deallocAlexStuff()
{
    delete playerptrs;
}
void drawMap(GLuint mapTexture)
{
    //which tile to draw 0.0 for magenta tile
    float tx = 0.40;
    if(mapfileloaded == 0)
        return;
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, mapTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(tx,      1.0f);
    glVertex2i(0, 0);
    glTexCoord2f(tx,      0.0f);
    glVertex2i(0, 600);
    glTexCoord2f(tx+0.1f,      0.0f);
    glVertex2i(800, 600);
    glTexCoord2f(tx+0.1f,      1.0f);
    glVertex2i(800, 0);
    glEnd();
    glPushMatrix();
    //as long as logic works out
    //it doesnt matter which direction we draw
    //drawing down-up and left-right
    for (int rows = MAP_TILE_ROWS -1; rows >= 0; rows--) {
        for (int cols = 0; cols < MAP_TILE_COLUMNS; cols++) {
            int ysmap = 149 - rows;
            char c = '0';
            c = maparray[rows][cols];
            //maybe switch to a switch(c) statement
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
            }
        }
    }
    glPopMatrix();

    //
}
void loadMapFile()
{
    //MAP_TILE_ROWS MAP_TILE_COLUMNS;
    mapfileloaded = 1; //we loaded the map
    std::string line;    
    std::ifstream mapifile;
    mapifile.open("map.tmx",std::ifstream::in);
    //while we havent reached end of file
    char character;
    int row = 0, col = 0;
    bool foundData = false;
    while (!mapifile.eof()) {
        //
        while (!foundData) {
            mapifile >> line;
            if(line.compare("<data")==0){
                foundData = true;
                //read the last token to discard
                mapifile >> line;
            }
        }
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
}

void moveMapFocus(int lateral, int vertical)
{
    int x = lateral;
    int y = vertical;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(800*x,800*(x+1),600*y,600*(y+1),-1,1);
}

void checkPlayerPos()
{
    static int prevx = 0;
    static int prevy = 0;
    //because *(playerptrs-x) gives me segfault
    int mapx = (int) *(playerptrsx) / 800;
    int mapy = (int) *(playerptrsy) / 600;
    if((prevx==mapx) && (prevy==mapy))
        return;
    moveMapFocus(mapx,mapy);
    prevx = mapx;
    prevy = mapy;
}
