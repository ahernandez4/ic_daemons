//3350
//program: walk.cpp
//author:  Gordon Griesel
//date:    summer 2017 - 2018
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
//NOTES: WIP

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

using namespace std;

//external defines for functions
extern void drawDY_Credits(int x, int y);
extern void displayAlejandroH(int x, int y, GLuint);
extern void displayCD(int x, int y);
extern void tjcredits(int x, int y);//,GLuint texid);
extern int odinGetTime();
extern void passGlobalValues2Alex(int* min);//temporary fix
extern void odinPushTime(int time);
extern void displayOdinTime();
extern void drawMap(GLuint);
extern void loadMapFile();
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

//funcion prototypes
int deltaTime();
//

class Image {
    public:
        int width, height;
        unsigned char *data;
        ~Image() { delete [] data; }
        Image(const char *fname) {
            if (fname[0] == '\0')
                return;
            //printf("fname **%s**\n", fname);
            char name[40];
            strcpy(name, fname);
            int slen = strlen(name);
            name[slen-4] = '\0';
            //printf("name **%s**\n", name);
            char ppmname[80];
            sprintf(ppmname,"%s.ppm", name);
            //printf("ppmname **%s**\n", ppmname);
            char ts[100];
            //system("convert eball.jpg eball.ppm");
            sprintf(ts, "convert %s %s", fname, ppmname);
            system(ts);
            //sprintf(ts, "%s", name);
            FILE *fpi = fopen(ppmname, "r");
            if (fpi) {
                char line[200];
                fgets(line, 200, fpi);
                fgets(line, 200, fpi);
                while (line[0] == '#')
                    fgets(line, 200, fpi);
                sscanf(line, "%i %i", &width, &height);
                fgets(line, 200, fpi);
                //get pixel data
                int n = width * height * 3;			
                data = new unsigned char[n];			
                for (int i=0; i<n; i++)
                    data[i] = fgetc(fpi);
                fclose(fpi);
            } else {
                printf("ERROR opening image: %s\n",ppmname);
                exit(0);
            }
            unlink(ppmname);
        }
};

Image img[5] = {
    "images/shift.gif",
    "images/castlemap.gif",
    "images/tj.jpg",
    "images/fakeMario.png",
    "images/tilemap.png" };

struct PlayerOne{
    int x;
    int y;
    int moveSpeed;
} player;

//-----------------------------------------------------------------------------
//Setup timers
class Timers {
    public:
        double physicsRate;
        double oobillion;
        struct timespec timeStart, timeEnd, timeCurrent, time1, time2;
        struct timespec playTimeBegin;
        struct timespec playTimeEnd; 
        struct timespec walkTime;
        Timers() {
            physicsRate = 1.0 / 30.0;
            oobillion = 1.0 / 1e9;
        }
        double timeDiff(struct timespec *start, struct timespec *end) {
            return (double)(end->tv_sec - start->tv_sec ) +
                (double)(end->tv_nsec - start->tv_nsec) * oobillion;
        }
        void timeCopy(struct timespec *dest, struct timespec *source) {
            memcpy(dest, source, sizeof(struct timespec));
        }
        void recordTime(struct timespec *t) {
            clock_gettime(CLOCK_REALTIME, t);
        }
} timers;
//-----------------------------------------------------------------------------

class Global {
    public:
        int displayCredits;
        //int startTime;
        int displayTime;
        int done;
        int xres, yres;
        int walk;
        int walkFrame;
        double delay;
        char movebyte;
        //textures
        GLuint walkTexture;
        //added
        GLuint backgroundTexture;
        GLuint fakeMarioTexture;
        GLuint tilemapTexture;
        //playtime
        int minutesPlayed;
	int secondsCounter;
        Vec box[20];
        Global() {
            movebyte = 0;
            done=0;
            xres=800;
            yres=600;
            walk=0;
            walkFrame=0;
            delay = 0.1;
            for (int i=0; i<20; i++) {
                box[i][0] = rnd() * xres;
                box[i][1] = rnd() * (yres-220) + 220.0;
                box[i][2] = 0.0;
            }
        }
} g;

class X11_wrapper {
    private:
        Display *dpy;
        Window win;
    public:
        X11_wrapper() {
            GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
            XSetWindowAttributes swa;
            setupScreenRes(g.xres, g.yres);
            dpy = XOpenDisplay(NULL);
            if (dpy == NULL) {
                printf("\n\tcannot connect to X server\n\n");
                exit(EXIT_FAILURE);
            }
            Window root = DefaultRootWindow(dpy);
            XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
            if (vi == NULL) {
                printf("\n\tno appropriate visual found\n\n");
                exit(EXIT_FAILURE);
            } 
            Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
            swa.colormap = cmap;
            swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                StructureNotifyMask | SubstructureNotifyMask;
            win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
                    vi->depth, InputOutput, vi->visual,
                    CWColormap | CWEventMask, &swa);
            GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
            glXMakeCurrent(dpy, win, glc);
            setTitle();
        }
        ~X11_wrapper() {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        void setTitle() {
            //Set the window title bar.
            XMapWindow(dpy, win);
            XStoreName(dpy, win, "Walk Cycle");
        }
        void setupScreenRes(const int w, const int h) {
            g.xres = w;
            g.yres = h;
        }
        void reshapeWindow(int width, int height) {
            //window has been resized.
            setupScreenRes(width, height);
            //dont change viewport
            //glViewport(0, 0, (GLint)width, (GLint)height);
            glMatrixMode(GL_PROJECTION); glLoadIdentity();
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
            glOrtho(0, g.xres, 0, g.yres, -1, 1);
            setTitle();
        }
        void checkResize(XEvent *e) {
            //The ConfigureNotify is sent by the
            //server if the window is resized.
            if (e->type != ConfigureNotify)
                return;
            XConfigureEvent xce = e->xconfigure;
            if (xce.width != g.xres || xce.height != g.yres) {
                //Window size did change.
                reshapeWindow(xce.width, xce.height);
            }
        }
        bool getXPending() {
            return XPending(dpy);
        }
        XEvent getXNextEvent() {
            XEvent e;
            XNextEvent(dpy, &e);
            return e;
        }
        void swapBuffers() {
            glXSwapBuffers(dpy, win);
        }

} x11;

//function prototypes
void initOpengl(void);
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);


int main(void)
{
    initOpengl();
    init();
    int done = 0;
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.checkResize(&e);
            checkMouse(&e);
            done = checkKeys(&e);
        }
        physics();
        render();
        x11.swapBuffers();
    }
    odinPushTime(g.minutesPlayed);
    cleanup_fonts();
    return 0;
}

unsigned char *buildAlphaData(Image *img) //
{
    //add 4th component to RGB stream...
    int i;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    unsigned char a,b,c;
    //use the first pixel in the image as the transparent color.
    unsigned char t0 = *(data+0);
    unsigned char t1 = *(data+1);
    unsigned char t2 = *(data+2);
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        *(ptr+3) = 1;
        if (a==t0 && b==t1 && c==t2)
            *(ptr+3) = 0;
        //-----------------------------------------------
        ptr += 4;
        data += 3;
    }
    return newdata;
}

void initOpengl(void)
{
    //OpenGL initialization
    //glViewport(800, 600, 800*2, 600*2);
    glViewport(0, 0, 800, 600);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //
    //load the images file into a ppm structure.
    //
    int w = img[0].width;
    int h = img[0].height;
    //
    //create opengl texture elements
    glGenTextures(1, &g.walkTexture);
    glGenTextures(1, &g.backgroundTexture);
    glGenTextures(1, &g.fakeMarioTexture);
    //-------------------------------------------------------------------------
    //silhouette
    //this is similar to a sprite graphic
    //
    glBindTexture(GL_TEXTURE_2D, g.walkTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *walkData = buildAlphaData(&img[0]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    //free(walkData);
    //unlink("./images/walk.ppm");
    //----------------------------------------------------------------------
    //background
    glBindTexture(GL_TEXTURE_2D, g.backgroundTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    w = img[1].width;
    h = img[1].height;
    //maybe we dont need to build alpha
    //unsigned char *bgData = buildAlphaData(&img[1]); 
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, 
            GL_RGB, GL_UNSIGNED_BYTE, img[1].data);
    //free(bgData);//no need if we did not build alpha channel
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, g.tilemapTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    w = img[4].width;
    h = img[4].height;
    unsigned char *tileMapData = buildAlphaData(&img[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, tileMapData);
    //delete
    free(tileMapData);
    //----------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, g.fakeMarioTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    w = img[3].width;
    h = img[3].height;
    //
    //must build a new set of data...
    unsigned char *marioData = buildAlphaData(&img[3]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, marioData);
    //free(walkData);
    free(marioData);
}

void init() {
    player.x = 250;
    player.y = 80;
    player.moveSpeed = 3.75;
    //load mapfile
    loadMapFile();
    timers.recordTime(&timers.playTimeBegin);   
    g.minutesPlayed = odinGetTime();
    g.secondsCounter = g.minutesPlayed * 60;
    passGlobalValues2Alex(&g.minutesPlayed);//temporary fix
}


void playTime(int x, int y)
{

    Rect D;
    unsigned int c = 0x00ffff44;

    D.bot = y;
    D.left = x;
    D.center = 0;
    timers.recordTime(&timers.playTimeEnd);

    double timeSpan = timers.timeDiff(&timers.playTimeBegin, 
    &timers.playTimeEnd);
    timeSpan = timeSpan + g.secondsCounter;
    g.minutesPlayed = round(timeSpan/60);
    cout << "Time: " << timeSpan << endl;
    ggprint8b(&D, 0, c,  "Time: %i", (int)timeSpan);
}


void checkMouse(XEvent *e)
{
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;
    //
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button is down
        }
        if (e->xbutton.button==3) {
            //Right button is down
        }
    }
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
        //Mouse moved
        savex = e->xbutton.x;
        savey = e->xbutton.y;
    }
}

int checkKeys(XEvent *e)
{
    //keyboard input?
    g.movebyte = 0;
    timers.recordTime(&timers.time2);
    static int shift=0;
    if (e->type != KeyRelease && e->type != KeyPress)
        return 0;
    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    if (e->type == KeyRelease) {
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift = 0;
        return 0;
    }
    if (key == XK_Shift_L || key == XK_Shift_R) {
        shift=1;
        return 0;
    }
    (void)shift;
    switch (key) {
        case XK_w:
            g.movebyte = g.movebyte | 2;
            break;
        case XK_a:
            g.movebyte = g.movebyte | 4;
            break;
        case XK_s:
            g.movebyte = g.movebyte | 8;
            break;
        case XK_d:
            g.movebyte = g.movebyte | 16;
            break;
        case XK_j:
            break;
        case XK_t:
            g.displayTime ^= 1;
            break;
        case XK_Left:
            break;
        case XK_Right:
            break;
        case XK_Up:
            break;
        case XK_Down:
            break;
        case XK_equal:
            break;
        case XK_minus:
            break;
        case XK_Escape:
            return 1;
            break;
        case XK_c: 
            g.displayCredits ^=1;
	    
            break;
    }
    return 0;
}

Flt VecNormalize(Vec vec)
{
    Flt len, tlen;
    Flt xlen = vec[0];
    Flt ylen = vec[1];
    Flt zlen = vec[2];
    len = xlen*xlen + ylen*ylen + zlen*zlen;
    if (len == 0.0) {
        MakeVector(0.0,0.0,1.0,vec);
        return 1.0;
    }
    len = sqrt(len);
    tlen = 1.0 / len;
    vec[0] = xlen * tlen;
    vec[1] = ylen * tlen;
    vec[2] = zlen * tlen;
    return(len);
}

void physics(void)
{
    //glViewport(800, 600, 800*2, 600*2);
    int incx = 0;
    int incy = 0;
    if(g.movebyte == 0) {
        g.walk = 0;
    }
    else{
        g.walk = 1;
        if(g.movebyte & 2 ){
            incy++;
        }
        if(g.movebyte & 4 ){
            incx--;
        }
        if(g.movebyte & 8 ){
            incy--;
        }
        if(g.movebyte & 16){
            incx++;
        }
    }

    if (g.walk) {
        //man is walking...
        //when time is up, advance the frame.
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
        if (timeSpan > g.delay) {
            //advance
            ++g.walkFrame;
            if (g.walkFrame >= 16)
                g.walkFrame -= 16;
            timers.recordTime(&timers.walkTime);
        }
        for (int i=0; i<20; i++) {
            g.box[i][0] -= 2.0 * (0.05 / g.delay);
            if (g.box[i][0] < -10.0)
                g.box[i][0] += g.xres + 10.0;
        }
    }
    else{
        //man is standing
        //when time is up, advance the frame.
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.walkTime, 
                &timers.timeCurrent);
        if (timeSpan > g.delay*3) {
	    g.walkFrame = 0;
            timers.recordTime(&timers.walkTime);
        }
    }
    player.x += incx * player.moveSpeed;
    player.y += incy * player.moveSpeed;
    g.movebyte = 0;
}

void render(void)
{
    Rect r;
    //Clear the screen
    glColor3f(1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    float cx = g.xres/2.0;
    float cy = g.yres/2.0;
    //will use later
    float tx, ty;
    //my background
    //something about texture location from 0,0 to 1,1
    //int which part of the map
    //remove this
    /*
    glBindTexture(GL_TEXTURE_2D, g.backgroundTexture);
    glBegin(GL_QUADS);
    //0,1|0,0|1,0|1,1
    glTexCoord2f(0.0f,      1.0f);
    glVertex2i(0, 0);
    glTexCoord2f(0.0f,      0.0f);    
    glVertex2i(0, 600);
    glTexCoord2f(1.0f,      0.0f);    
    glVertex2i(800, 600);
    glTexCoord2f(1.0f,      1.0f);    
    glVertex2i(800, 0);
    glEnd();
    //to this
    */
    drawMap(g.tilemapTexture);
    glPushMatrix();
    //
    float h = 32;
    float w = h * 0.8;
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.walkTexture);
    //
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ix = g.walkFrame % 5;
    int iy = 0;
    if (g.walkFrame >= 8)
        iy = 1;
    //float tx = (float)ix / 7.0;
    //float ty = (float)iy / 2.0;
    tx = (float)ix / 7.0;
    ty = (float)iy / 2.0;
    glBegin(GL_QUADS);
    //must be glTex... followed by glVert..
    glTexCoord2f(tx,      ty+.5); 
    glVertex2i(cx-w +player.x, cy-h +player.y);
    glTexCoord2f(tx,      ty);    
    glVertex2i(cx-w +player.x, cy+h +player.y);
    glTexCoord2f(tx+.125, ty);    
    glVertex2i(cx+w +player.x, cy+h +player.y);
    glTexCoord2f(tx+.125, ty+.5); 
    glVertex2i(cx+w +player.x, cy-h +player.y);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    //
    unsigned int c = 0x00ffff44;
    r.bot = g.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, c, "w   move up");
    ggprint8b(&r, 16, c, "s   walk down");
    ggprint8b(&r, 16, c, "d   walk right");
    ggprint8b(&r, 16, c, "a   walk left");
    ggprint8b(&r, 16, c, "c   display credits");
    ggprint8b(&r, 16, c, "player local: %i,%i", player.x,player.y);
    

    //displaying time to screen from server
    //displayOdinTime();

    //this is for drawing names on screen for credits on "c" button press
    if(g.displayCredits) {
	    
        displayAlejandroH(350,332,g.fakeMarioTexture);
        drawDY_Credits(350, 300);
        tjcredits(350,316);
        displayCD(350, 348);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.1,0.1,0.1,0.1);
    }

    //this is for drawing the prompt for time and logic is for displaying
    //while time is not displayed and while not in credits screen
    if (!g.displayTime && !g.displayCredits) {
	ggprint8b(&r, 16, c, "press \"t\" to display time");
    }

    //Display Time elapsed during gameplay button = "t"
    if (g.displayTime) {
        playTime(15,484);
    }
}

int deltaTime()
{
    int time = (int) (timers.timeDiff(&timers.time1
                , &timers.time2) + 0.5);
    timers.recordTime(&timers.time1);
    return time;
}

