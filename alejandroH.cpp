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
    0,1,1,0,0,0,0,0,0,0,
    1,0,0,1,0,1,0,1,0,1,
    1,1,1,1,0,1,0,0,1,0,
    1,0,0,1,0,1,0,1,0,1,
    0,0,0,0,0,1,1,0,0,0,
    0,0,1,1,0,1,1,0,0,0,
};
//externs
extern std::vector<Enemy> enemies;
extern bool collision(int,int);
extern GLuint texturearray[7];
struct PlayerPtrs{
    int* x = NULL;
    int* y = NULL;
    int* speed = NULL;
};//
//temporary fix
static PlayerPtrs *playerptrs = NULL;
static int *playerptrsx = 0;
static int *playerptrsy = 0;

class Boss : public Enemy{
    public:
        void Draw();
};
//prototype
void spawnBoss(Boss*);
class MyScene : public GameScene{
    Rect *other;
    Rect *player;
    public:
    MyScene(int *x, int *y);
    void Draw();
};
class BossScene : public GameScene{
    public:
        Boss* boss;
        BossScene(int *x, int *y);
        void Draw();
};
// This is my Friday code
class AlexGlobal{
    private:
        static AlexGlobal * instance;
        AlexGlobal() {
            playerx = nullptr;
            playery = nullptr;
        }
        AlexGlobal(AlexGlobal const& copy);
        AlexGlobal & operator = (AlexGlobal const& copy);
    public:
        int *playerx;
        int *playery;
        int mapcellx;
        int mapcelly;
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
MyScene::MyScene(int *x, int*y){
    this->deleteSoon = false;
    this->prev_playerx = *x;
    this->prev_playery = (*y)-32;
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
    //here we draw the scene
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2i(0, 0);
    glVertex2i(0, 600);
    glVertex2i(800, 600);
    glVertex2i(800, 0);
    glEnd();

    //glColor3f(1.0f,0.0f,0.0f);
    if ((*(ag->playerx) > 350 && *(ag->playerx) < 450)  &&
            (*(ag->playery) > 144 && *(ag->playery) < 200)) {
        this->deleteSoon = true;
    }
    return;
}

BossScene::BossScene(int *x, int*y){
    this->boss = new Boss();
    spawnBoss(this->boss);
    this->deleteSoon = false;
    this->prev_playerx = *x;
    this->prev_playery = (*y)-32;
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
    checkPlayerPos();
    //CONTAINS_ENEMIES[7][0] = 1;
}
void BossScene::Draw(){
    //here we draw the scene
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2i(0, 0);
    glVertex2i(0, 600);
    glVertex2i(800, 600);
    glVertex2i(800, 0);
    glEnd();
//
    //glColor3f(1.0f,1.0f,1.0f);
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texturearray[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.40,      1.0f);
    glVertex2i(32, 32);
    glTexCoord2f(0.40,      0.0f);
    glVertex2i(32, 568);
    glTexCoord2f(0.40+0.1f,      0.0f);
    glVertex2i(768, 568);
    glTexCoord2f(0.40+0.1f,      1.0f);
    glVertex2i(768, 32);
    glEnd();
//
    //glColor3f(1.0f,0.0f,0.0f);
    if ((*(ag->playerx) > 350 && *(ag->playerx) < 450)  &&
            (*(ag->playery) > 144 && *(ag->playery) < 200)) {
        this->deleteSoon = true;
        //CONTAINS_ENEMIES[7][0] = 0;
        delete this->boss;
    }
    else{
        boss->updatePosition();
        this->boss->Draw();
    }
    return;
}

//------------Enemy-------------
void Enemy::Spawn(int x,int y){
    //return;
    this->xpos = x;
    this->ypos = y;
    this->health = 100;
    this->moving = false;
    this->alive = true;//false;//true;
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
    //glColor3f(0.8, 0.8, 0.6);
    glColor3f(1.0,1.0,1.0);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, texturearray[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,      1.0f);
    glVertex2i(this->xpos-w, this->ypos-h);
    glTexCoord2f(0.0f,      0.0f);
    glVertex2i(this->xpos-w, this->ypos+h);
    glTexCoord2f(1.0f,      0.0f);
    glVertex2i(this->xpos+w, this->ypos+h);
    glTexCoord2f(1.0f,      1.0f);
    glVertex2i(this->xpos+w, this->ypos-h);
    glEnd();
}

bool Enemy::isMoving(){
    return true;//false;
}
bool Enemy::isAlive(){
    return this->alive;
}
void Enemy::updatePosition(){
    if(this->xpos > *(ag->playerx))
            this->xpos = this->xpos - 1;
    else
            this->xpos = this->xpos + 1;

    if(this->ypos > *(ag->playery))
            this->ypos = this->ypos - 1;
    else
            this->ypos = this->ypos+1;
    return;
}
void Enemy::hit(int h){
    this->health -= h;
    if(this->health <=0)
        this->alive = false;
    return;
}
bool Enemy::checkAreaCollision(int x1, int x2, int y1, int y2){
    float h = 32/2;
    float w = h*0.8;
    if ((this->xpos - w > x1&&
                this->xpos + w < x1) ||
            (this->xpos - w < x2 &&
             this->xpos + w > x2)) {
        if ((this->ypos - h > y1&&
                    this->ypos + h < y1) ||
                (this->ypos - h < y2 &&
                 this->ypos + h > y2)){
            return true;
        }
    }
    else{
        return false;
    }
    return false;
}
//-------Boss-------------------
void Boss::Draw(){
    //temp height and width
    float h = 64/2;
    float w = h*0.8;
#ifdef DEBUG_A
    std::cout << "my enemy x: " << this->xpos << std::endl;
    std::cout << "my enemy y: " << this->ypos << std::endl;
#endif
    //glColor3f(0.8, 0.8, 0.6);
    glColor3f(1.0,1.0,1.0);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, texturearray[6]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,      1.0f);
    glVertex2i(this->xpos-w, this->ypos-h);
    glTexCoord2f(0.0f,      0.0f);
    glVertex2i(this->xpos-w, this->ypos+h);
    glTexCoord2f(1.0f,      0.0f);
    glVertex2i(this->xpos+w, this->ypos+h);
    glTexCoord2f(1.0f,      1.0f);
    glVertex2i(this->xpos+w, this->ypos-h);
    glEnd();
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
    int resy = 600-64;
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
    glVertex2i(0, 4800);
    glTexCoord2f(tx+0.1f,      0.0f);
    glVertex2i(20000, 4800);
    glTexCoord2f(tx+0.1f,      1.0f);
    glVertex2i(20000, 0);
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
        //std::cout << maparray[i][0] << std::endl;
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
GameScene* createScene(int which)
{
    if(which ==1)
        return new MyScene(ag->playerx,ag->playery);
    if(which == 2)        
        return new BossScene(ag->playerx,ag->playery);
    return nullptr;        
}
GameScene* checkscene(GameScene* scene)
{
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
void spawnEnemies(int mapposx, int mapposy)
{
    srand(time(NULL));
    enemies.clear();
    enemies.push_back(Boss());
    enemies.push_back(Boss());
    enemies.push_back(Boss());
    enemies.push_back(Boss());
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
void spawnBoss(Boss* b)
{
    srand(time(NULL));
    int atx = (rand() % 700)+50;
    int aty = (rand() % 500)+50;
    b->Spawn(atx, aty);
    while(collision(enemies[0].xpos,enemies[0].ypos)){
        atx = (rand() % 700)+50;
        aty = (rand() % 500)+50;
        b->Spawn(atx, aty);
    }
}
void drawEnemies()
{
    for (unsigned int i = 0; i < enemies.size(); i++) {
        if(enemies[i].isMoving()){
#ifdef MOBFOLLOWSU            
            enemies[i].updatePosition();
#endif            
        }
        if(enemies[i].isAlive()){
            enemies[i].Draw();
        }
    }
    return;
}

void drawSword(int frame, int dir)
{
    //return;
    int posx = *(ag->playerx);// + 32;
    int posy = *(ag->playery);// + 32;
    int size = 16;
    glColor3f(1.0,1.0,1.0);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, texturearray[5]);
    switch(dir){
        case 0:
            posy += 32;
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f,      1.0f);
            glVertex2i(posx-(size/(2-frame)), posy-(size/(2-frame)));
            glTexCoord2f(0.0f,      0.0f);
            glVertex2i(posx-(size/(2-frame)), posy+(size/(2-frame)));
            glTexCoord2f(1.0f,      0.0f);
            glVertex2i(posx+(size/(2-frame)), posy+(size/(2-frame)));
            glTexCoord2f(1.0f,      1.0f);
            glVertex2i(posx+(size/(2-frame)), posy-(size/(2-frame)));
            glEnd();
            break;
        case 1:
            posy -=32;
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f,      1.0f);
            glVertex2i(posx+(size/(2-frame)), posy+(size/(2-frame)));
            glTexCoord2f(0.0f,      0.0f);
            glVertex2i(posx+(size/(2-frame)), posy-(size/(2-frame)));
            glTexCoord2f(1.0f,      0.0f);
            glVertex2i(posx-(size/(2-frame)), posy-(size/(2-frame)));
            glTexCoord2f(1.0f,      1.0f);
            glVertex2i(posx-(size/(2-frame)), posy+(size/(2-frame)));
            glEnd();
            break;
        case 2:
            posx += 32;
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f,      1.0f);
            glVertex2i(posx-(size/(2-frame)), posy+(size/(2-frame)));
            glTexCoord2f(0.0f,      0.0f);
            glVertex2i(posx+(size/(2-frame)), posy+(size/(2-frame)));
            glTexCoord2f(1.0f,      0.0f);
            glVertex2i(posx+(size/(2-frame)), posy-(size/(2-frame)));
            glTexCoord2f(1.0f,      1.0f);
            glVertex2i(posx-(size/(2-frame)), posy-(size/(2-frame)));
            glEnd();
            break;
        case 3:
            posx -= 32;
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f,      1.0f);
            glVertex2i(posx+(size/(2-frame)), posy-(size/(2-frame)));
            glTexCoord2f(0.0f,      0.0f);
            glVertex2i(posx-(size/(2-frame)), posy-(size/(2-frame)));
            glTexCoord2f(1.0f,      0.0f);
            glVertex2i(posx-(size/(2-frame)), posy+(size/(2-frame)));
            glTexCoord2f(1.0f,      1.0f);
            glVertex2i(posx+(size/(2-frame)), posy+(size/(2-frame)));
            glEnd();
            break;
    }

}
void processAttack(int dir)
{
    int swordx = *(ag->playerx);
    int swordy = *(ag->playery);
    switch(dir){
        case 0:
            swordy += 32;
            break;
        case 1:
            swordy -= 32;
            break;
        case 2:
            swordx += 32;
            break;
        case 3:
            swordx -= 32;
            break;
    }
    for (unsigned int i = 0; i < enemies.size(); i++) {
        if (enemies[i].isAlive() && 
                enemies[i].checkAreaCollision(swordx-16,swordx+16,
                    swordy-16,swordy+16)) {
            enemies[i].hit(100);
        }
#ifdef DEBUG_A
        std::cout << "we hit an ememies" << std::endl;
#endif
    }
    return;
}
