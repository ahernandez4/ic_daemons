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
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include "GameScene.h"
#include "Enemy.h"
#include <vector>
#ifdef DEBUG_A
#include <iostream>
#endif
#define MAP_TILE_ROWS 150
#define MAP_TILE_COLUMNS 250
//some forward declaration
void moveMapFocus(int, int);
void checkPlayerPos();
void spawnEnemies(int,int);
void drawEnemies();
int* minutesPlayedPtr;// = NULL;
//make sure we load the map before trying to use it
static int mapfileloaded = 0;
unsigned int maparray[MAP_TILE_ROWS][MAP_TILE_COLUMNS];
int CONTAINS_ENEMIES[8][10] = {
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,
    0,0,0,1,0,1,0,0,0,0,
};
//externs
extern std::vector<Enemy> enemies;
extern bool collision(int,int);
extern GLuint texturearray[4];
struct PlayerPtrs{
    int* x = NULL;
    int* y = NULL;
    int* speed = NULL;
};//
//temporary fix
static PlayerPtrs *playerptrs = NULL;
static int *playerptrsx = 0;
static int *playerptrsy = 0;
//MyScene
class MyScene : public GameScene{
    int prev_playerx;
    int prev_playery;
    GLuint *gltextures;
    Rect *other;
    Rect *player;
    public:
    bool deleteSoon;
    MyScene(int *x, int *y, GLuint[]);
    void Draw();
};


// This is my Friday code
class AlexGlobal{
    private:
        static AlexGlobal * instance;
        AlexGlobal() {
            playerx = nullptr;
            playery = nullptr;
            mygs = nullptr;
        }
        AlexGlobal(AlexGlobal const& copy);
        AlexGlobal & operator = (AlexGlobal const& copy);
    public:
        int *playerx;
        int *playery;
        int mapcellx;
        int mapcelly;
        MyScene *mygs = nullptr;
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
//myscene function definitions
MyScene::MyScene(int *x, int*y,GLuint gltexture[]){
    this->prev_playerx = *x;
    this->prev_playery = *y;
    //this->playertexture = gltexture;
    this->gltextures = gltexture;
    *(ag->playerx) = 400;
    *(ag->playery) = 300;
#ifdef DEBUG_A
    std::cout << "myscene constructor(ag->player)" << std::endl;
    std::cout << *(ag->playerx) << std::endl;
    std::cout << *(ag->playery) << std::endl;
    std::cout << "myscene constructor(myscene->pre_player)" << std::endl;
    std::cout << (this->prev_playerx) << std::endl;
    std::cout << (this->prev_playery) << std::endl;
#endif
    moveMapFocus(0,0);
}
void MyScene::Draw(){
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2i(0, 0);
    glVertex2i(0, 600);
    glVertex2i(800, 600);
    glVertex2i(800, 0);
    glEnd();

    //glColor3f(1.0f,0.0f,0.0f);

    return;
}
//------------Enemy-------------
/*
private:
bool alive;
bool moving;
GLuint enemyTexture;

//Enemy(GLuint atexture);
int health;
int xpos;
int ypos;
void Draw();
void Spawn(int x, int y);
bool isMoving();
bool isAlive();
void updatePosition();
void hit(int h);

*/
void Enemy::Spawn(int x,int y){
    //return;
    this->xpos = x;
    this->ypos = y;
    this->health = 100;
    this->moving = false;
    this->alive = false;//true;
    return;
}
void Enemy::Draw(){
    //temp height and width
    float h = 32/2;
    float w = h*0.8;
#ifdef DEBUG_A
    std::cout << "my enemy x: " << this->xpos << std::endl;
    std::cout << "my enemy y: " << this->ypos << std::endl;
#endif
    glColor3f(0.8, 0.8, 0.6);
    glBegin(GL_QUADS);
    glVertex2i(this->xpos-w, this->ypos-h);
    glVertex2i(this->xpos-w, this->ypos+h);
    glVertex2i(this->xpos+w, this->ypos+h);
    glVertex2i(this->xpos+w, this->ypos-h);
    glEnd();

}


bool Enemy::isMoving(){
    return false;
}
bool Enemy::isAlive(){
    return this->alive;
}
void Enemy::updatePosition(){
    return;
}
void Enemy::hit(int h){
    this->health -= h;
    if(this->health <=0)
        this->alive = false;
    return;
}

//-------------------------------
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
    ag->playerx = x;
    ag->playery = y;
    ag->mapcelly = 0;
    ag->mapcellx = 0;
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
            int c = 0;
            c = maparray[rows][cols];
            //
            //if(c != '0') {
            int ix = c-1;
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
            //}
        }
    }
    glPopMatrix();

#ifdef DEBUG_A
    //test map array
    for(int i = 0; i < 10; i++)
    {
        std::cout << maparray[i][0] << std::endl;
    }
#endif    
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
    int tileint = 0;
    int row = 0;//, col = 0;
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
        mapifile >> line;
        for (unsigned int i = 0; i < line.length(); i++) {
            if(line[i] == ',')
                line[i] = ' ';
        }
#ifdef DEBUG_A        
        //std::cout << line << std::endl;
#endif      
        //check make sure we don't try to insert overrage of rows
        if (row < MAP_TILE_ROWS) {
            std::stringstream ss(line);
            for(int i = 0; i < MAP_TILE_COLUMNS; i++){
                ss >> tileint;
                maparray[row][i] = tileint;
            }
        }
        row++;
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
    spawnEnemies(x,7-y);
}

void checkPlayerPos()
{
    static int prevx = 0;
    static int prevy = 0;
    int mapx = (int) *(playerptrsx) / 800;
    int mapy = (int) *(playerptrsy) / 600;
    if(CONTAINS_ENEMIES[7-mapy][mapx]){
        drawEnemies();
    }
    if((prevx==mapx) && (prevy==mapy))
        return;
    moveMapFocus(mapx,mapy);
    prevx = mapx;
    prevy = mapy;
    ag->mapcellx = mapx;
    ag->mapcelly = mapy;
}
GameScene* createScene(GLuint atexture[]){
    return new MyScene(ag->playerx,ag->playery,atexture);
}
GameScene* checkscene(GameScene* scene){
    if (scene == nullptr)
        return nullptr;
    else if (scene->deleteSoon) {
        *(ag->playerx) = scene->prev_playerx;
        *(ag->playery) = scene->prev_playery;
        checkPlayerPos();
        delete scene;
        return nullptr;
    }
    else
        return scene;
}
void spawnEnemies(int mapposx, int mapposy){
    srand(time(NULL));
    enemies.clear();
    enemies.push_back(Enemy());
    enemies.push_back(Enemy());
    enemies.push_back(Enemy());
    enemies.push_back(Enemy());
    //only some areas spawn enemies
    if(CONTAINS_ENEMIES[mapposy][mapposx]){
        int offsetx = mapposx*800;
        int offsety = (7-mapposy)*600;
        for(unsigned int i = 0; i < enemies.size(); i++){
            //we dont need/want to spawn at edge
            int atx = (rand() % 700)+50;
            int aty = (rand() % 500)+50;
            enemies[i].Spawn(atx+offsetx, aty+offsety);
            //and we dont want to spawn on top of collidable objects
            while(collision(enemies[i].xpos,enemies[i].ypos)){
                atx = (rand() % 700)+50;
                aty = (rand() % 500)+50;
                enemies[i].Spawn(atx+offsetx, aty+offsety);
            }
        }
    }
    return;
}
void drawEnemies(){
    for (unsigned int i = 0; i < enemies.size(); i++) {
        if(enemies[i].isMoving()){
            enemies[i].updatePosition();
        }
        if(enemies[i].isAlive()){
            enemies[i].Draw();
        }
    }
    return;
}
